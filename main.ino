#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <Servo.h>

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

const char* validIDs[] = {"1234567890", "0987654321", "1122334455"};
const int maxInvalidAttempts = 3;

int vagasDisponiveis = 1;
int invalidAttempts = 0;

void setup() 
{
  Serial.begin(115200);

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(SENSOR_FC51, INPUT);
  servoMotor.attach(SERVO_PIN);

  lcd.begin();
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

  delay(500);
}

void atualizaStatusVagas() 
{
  lcd.setCursor(0, 1);

  if (vagasDisponiveis > 0) 
	{
    lcd.print("VAGAS DISPONIVEIS: ");
    lcd.print(vagasDisponiveis);
  } 
	else 
	{
    lcd.print("VAGAS INDISPONIVEIS");
  }
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

  if (valido) 
	{
    acessoLiberado(id);
  } 
	else 
	{
    acessoNegado();
  }
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
