#include <WiFi.h>
#include <Wire.h>
#include <MFRC522.h>
#include <ESP32Servo.h> 
#include <FirebaseESP32.h>
#include <LiquidCrystal_I2C.h>

#include "addons/RTDBHelper.h"
#include "addons/TokenHelper.h"
#include "../include/credentials.h"

#define GREEN_LED 7
#define RED_LED 8
#define BUZZER 36
#define SENSOR_FC51 17
#define SERVO_PIN 13
#define RST_PIN 15
#define SS_PIN 4

LiquidCrystal_I2C lcd(0x27, 20, 4);
MFRC522 rfid(SS_PIN, RST_PIN);
Servo servoMotor;

FirebaseData   fbdo;
FirebaseAuth   auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool          signupOK           = false;
const char*   validIDs[]         = {""};
const int     maxInvalidAttempts = 3;

int    emptyParkingSpaces  = 1;
int    invalidAttempts     = 0;
int    alarmActvations     = 0;
String lastID              = "";
String lastInvalidID       = "";

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
}

void connectFirebase()
{
  config.host        = DATABASE_URL;
  config.api_key     = API_KEY;
  auth.user.email    = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  if (Firebase.ready()) 
  {
    Serial.println("Connected to Firebase");
  } 
  else { Serial.println("Firebase not coneccted"); }
}

void sendDataToFirebase()
{
  if (Firebase.ready()) 
  {
    if (!Firebase.setInt(fbdo, "/vagasDisponiveis", emptyParkingSpaces)) 
    {
      Serial.print("Erro ao enviar emptyParkingSpaces: ");
      Serial.println(fbdo.errorReason().c_str());
    }

    if (!Firebase.setString(fbdo, "/ultimoID", lastID.c_str())) 
    {
      Serial.print("Erro ao enviar lastID: ");
      Serial.println(fbdo.errorReason().c_str());
    }

    if (!Firebase.setString(fbdo, "/ultimoIDAlarme", lastInvalidID.c_str())) 
    {
      Serial.print("Erro ao enviar lastInvalidID: ");
      Serial.println(fbdo.errorReason().c_str());
    }

    if (!Firebase.setInt(fbdo, "/alarmAtivacoes", alarmActvations)) 
    {
      Serial.print("Erro ao enviar alarmActvations: ");
      Serial.println(fbdo.errorReason().c_str());
    }

    bool isWiFiConnected = WiFi.status() == WL_CONNECTED;
    if (!Firebase.setBool(fbdo, "/wifiConectado", isWiFiConnected)) 
    {
      Serial.print("Erro ao enviar isWiFiConnected: ");
      Serial.println(fbdo.errorReason().c_str());
    }
  } 
  else { Serial.println("Firebase não está pronto"); }
}

void updateParkingLotStatus() 
{
  lcd.setCursor(0, 1);

  if (emptyParkingSpaces > 0) 
  {
    lcd.print("VAGAS DISPONIVEIS: ");
    lcd.print(emptyParkingSpaces);
  } 
  else { lcd.print("VAGAS INDISPONIVEIS"); }
}

String readRFIDCard() 
{
  String content = "";

  for (byte i = 0; i < rfid.uid.size; i++) 
  {
    content += String(rfid.uid.uidByte[i], HEX);
  }
  content.toUpperCase();
  return content;
}

void accessFree(String id) 
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ACESSO LIBERADO");
  lcd.setCursor(0, 1);
  lcd.print("ID DO CARTAO: ");
  lcd.print(id);

  digitalWrite(GREEN_LED, HIGH);
  servoMotor.write(90); 
  delay(2000); 
  servoMotor.write(0); 
  digitalWrite(GREEN_LED, LOW);

  emptyParkingSpaces--;
  updateParkingLotStatus();
  lastID= id;
  sendDataToFirebase();
}

void accessDenied() 
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ACESSO NEGADO");

  digitalWrite(RED_LED, HIGH);
  delay(500);
  digitalWrite(RED_LED, LOW);
  delay(500);
  digitalWrite(RED_LED, HIGH);
  delay(500);
  digitalWrite(RED_LED, LOW);

  invalidAttempts++;

  if (invalidAttempts >= maxInvalidAttempts) 
  {
    tone(BUZZER, 1000, 2000); 
    lcd.setCursor(0, 1);
    lcd.print("TENTATIVAS EXCEDIDAS");

    alarmActvations++;
    lastInvalidID = lastID;
    sendDataToFirebase();
  }
}

void cardValidate(String id) 
{
  bool isValid = false;

  for (int i = 0; i < sizeof(validIDs) / sizeof(validIDs[0]); i++) 
  {
    if (id == validIDs[i]) 
    {
      isValid = true;
      break;
    }
  }

  if (isValid) { accessFree(id); } else { accessDenied(); }
}

void parkingLotMonitor() 
{
  int sensorState = digitalRead(SENSOR_FC51);

  if (sensorState == LOW && emptyParkingSpaces < 1) 
  {
    emptyParkingSpaces++;
    updateParkingLotStatus();
    sendDataToFirebase();
  }
}

void setup() 
{
  Serial.begin(115200);
  
  connectWifi();
  connectFirebase();

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
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

  updateParkingLotStatus();
}

void loop() 
{
  parkingLotMonitor();

  if (millis() - sendDataPrevMillis > 10000) 
  {
    sendDataPrevMillis = millis();
    sendDataToFirebase();
  }

  if (emptyParkingSpaces > 0) 
  {
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) 
    {
      String cardID = readRFIDCard();
      cardValidate(cardID);
      rfid.PICC_HaltA();
    }
  }
}
