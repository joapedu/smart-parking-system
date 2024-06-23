#include <WiFi.h>
#include <Wire.h>
#include <Servo.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include "../include/credentials.h"

#define LED_VERDE 7
#define LED_VERMELHO 8
#define BUZZER 36
#define SENSOR_FC51 17
#define SERVO_PIN 13

#define RST_PIN 15
#define SS_PIN 4

LiquidCrystal_I2C lcd(0x27, 20, 4);
MFRC522 rfid(SS_PIN, RST_PIN);
Servo servoMotor;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

const char* validIDs[] = {""};
const int maxInvalidAttempts = 3;

int vagasDisponiveis = 1;
int invalidAttempts = 0;
String ultimoID = "";
String ultimoIDAlarme = "";

void connectWifi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void connectFirebase()
{
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("SignUp OK");
    signupOK = true;
  }
  else { Serial.printf("%s\n", config.signer.signupError.message.c_str()); }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void sendDataToFirebase() 
{
  if (millis() - sendDataPrevMillis > 5000) 
  { 
    if (Firebase.ready()) 
    {
      Firebase.RTDB.setInt(&fbdo, "/vagasDisponiveis", vagasDisponiveis);
      Firebase.RTDB.setString(&fbdo, "/ultimoID", ultimoID);
      Firebase.RTDB.setBool(&fbdo, "/wifiConectado", WiFi.status() == WL_CONNECTED);
      Firebase.RTDB.setInt(&fbdo, "/alarmAtivacoes", invalidAttempts);
      Firebase.RTDB.setString(&fbdo, "/ultimoIDAlarme", ultimoIDAlarme);

      sendDataPrevMillis = millis(); 
    } 
    else { Serial.println(fbdo.errorReason().c_str()); }
  }
}

void atualizaStatusVagas() 
{
  lcd.setCursor(0, 1);

  if (vagasDisponiveis > 0) 
  {
    lcd.print("VAGAS DISPONIVEIS: ");
    lcd.print(vagasDisponiveis);
  } 
  else { lcd.print("VAGAS INDISPONIVEIS"); }
}

String leCartaoRFID() 
{
  String conteudo = "";

  for (byte i = 0; i < rfid.uid.size; i++) 
  {
    conteudo += String(rfid.uid.uidByte[i], HEX);
  }

  conteudo.toUpperCase();
  return conteudo;
}

void acessoLiberado(String id) 
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ACESSO LIBERADO");
  lcd.setCursor(0, 1);
  lcd.print("ID DO CARTAO: ");
  lcd.print(id);

  digitalWrite(LED_VERDE, HIGH);
  servoMotor.write(90); 

  delay(2000); 

  servoMotor.write(0); 
  digitalWrite(LED_VERDE, LOW);

  vagasDisponiveis--;
  atualizaStatusVagas();

  ultimoID = id;
  sendDataToFirebase();
}

void acessoNegado() 
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ACESSO NEGADO");

  digitalWrite(LED_VERMELHO, HIGH);
  delay(500);
  digitalWrite(LED_VERMELHO, LOW);
  delay(500);
  digitalWrite(LED_VERMELHO, HIGH);
  delay(500);
  digitalWrite(LED_VERMELHO, LOW);

  invalidAttempts++;

  if (invalidAttempts >= maxInvalidAttempts) 
  {
    tone(BUZZER, 1000, 2000); 
    lcd.setCursor(0, 1);
    lcd.print("TENTATIVAS EXCEDIDAS");
    
    ultimoIDAlarme = ultimoID;
    sendDataToFirebase();
  }
}

void validaCartao(String id) 
{
  bool valido = false;

  for (int i = 0; i < sizeof(validIDs) / sizeof(validIDs[0]); i++) 
  {
    if (id == validIDs[i]) 
    {
      valido = true;
      break;
    }
  }

  if (valido) { acessoLiberado(id); } else { acessoNegado(); }
}

void monitorarVagas() 
{
  int estadoSensor = digitalRead(SENSOR_FC51);

  if (estadoSensor == LOW && vagasDisponiveis < 1) 
  {
    vagasDisponiveis++;
    atualizaStatusVagas();
    sendDataToFirebase();
  }
}

void setup() 
{
  Serial.begin(115200);
  
  connectWifi();
  connectFirebase();

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(SENSOR_FC51, INPUT);
  servoMotor.attach(SERVO_PIN);

  lcd.begin(0x27, 20, 4);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("SISTEMA DE ESTACIONAMENTO");

  SPI.begin();
  rfid.PCD_Init();
  servoMotor.write(0);

  atualizaStatusVagas();
}

void loop() 
{
  monitorarVagas();
  sendDataToFirebase();

  if (vagasDisponiveis > 0) 
  {
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) 
    {
      String idCartao = leCartaoRFID();
      validaCartao(idCartao);
      rfid.PICC_HaltA();
    }
  }

  delay(500);
}
