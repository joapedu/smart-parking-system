#include <WiFi.h>
#include <Wire.h>
#include <Servo.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define API_KEY "AIzaSyA7bFyxiaC90NKdbZTL148DIRQtPnkx3IM"
#define DATABASE_URL "https://parking-system-b3bc7-default-rtdb.firebaseio.com/"

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

const char* validIDs[] = {"", "", ""};
const int maxInvalidAttempts = 3;

int vagasDisponiveis = 1;
int invalidAttempts = 0;

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

void firebaseData()
{
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) 
  {
    sendDataPrevMillis = millis();

    String path = "/parking_system/status";
    
    FirebaseJson json;
    json.set("vagasDisponiveis", vagasDisponiveis);
    json.set("invalidAttempts", invalidAttempts);
    FirebaseJsonArray validIDsArray;

    for (int i = 0; i < sizeof(validIDs) / sizeof(validIDs[0]); i++) 
    {
      validIDsArray.add(validIDs[i]);
    }
    json.set("validIDs", validIDsArray);

    if (Firebase.RTDB.setJSON(&fbdo, path.c_str(), &json)) 
    {
      Serial.println("Dados enviados com sucesso!");
    } 
    else 
    {
      Serial.print("Falha ao enviar dados: ");
      Serial.println(fbdo.errorReason().c_str());
    }
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

  String path = "/parking_system/logs";
  FirebaseJson log;
  log.set("id", id);
  log.set("status", "acesso liberado");
  log.set("timestamp", millis());
  Firebase.RTDB.pushJSON(&fbdo, path.c_str(), &log);
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
  }

  String path = "/parking_system/logs";
  FirebaseJson log;
  log.set("status", "acesso negado");
  log.set("timestamp", millis());
  Firebase.RTDB.pushJSON(&fbdo, path.c_str(), &log);
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

  if (vagasDisponiveis > 0) 
  {
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) 
    {
      String idCartao = leCartaoRFID();
      validaCartao(idCartao);
      rfid.PICC_HaltA();
    }
  }

  firebaseData();
  delay(500);
}
