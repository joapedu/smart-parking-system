<h1 align = "center"> Smart Parking System ESP32 </h1><br>

<h2> &#128269; About the project </h2>

<p>Intelligent parking system connected to firebase and managed by Android Application. The system has access card reading, parking availability, alarm system and real time monitoring.</p><br>

<h2> &#128302; Technologies Used </h2><br>

<p align="center">
  <a href="https://skillicons.dev">
    <img src="https://skillicons.dev/icons?i=arduino,kotlin,firebase" />
  </a>
</p>

<br><h2> &#128161; How it Works? </h2>

<br><p align="center">
  <img src="https://github.com/Brevex/Smart-Parking-System-ESP32/blob/efe7cafe0db70c078ed22324fe00f2e397b1a88d/circuit/circuit.png" alt="circuit">
</p><br>

<ul>
  <li>Reads the card and checks if it has a valid ID in the system. If authorized, a green LED will light up and a gate represented by a servo motor will open. If the card is declined 3 times, an alarm will sound and a red LED will flash.</li>
  <li>An LCD display will inform the driver whether there are available spaces or not. If there are none, the parking lot will be inaccessible.</li>
  <li>The parking space will be monitored by an infrared sensor</li>
  <li>All parking operations will be monitored through an Android application, which will inform: available parking spaces, ID of the last card used, ID of the last card declined, number of times the alarm was activated and system connection status.</li>
</ul>

<br><h2> &#128293; Firebase setup </h2>

<p>Within the /include directory you must define the value of these variables in the "credentials.h" file to match the credentials of your realtime database.</p>
<p><b>WARNING:</b> THIS IS A DEMO PROJECT. NEVER INCLUDE CREDENTIALS IN THE SOURCE CODE OF YOUR PRODUCTION PROJECT</p>

````c++
#ifndef CREDENTIALS_H
#define CREDENTIALS_H

const char* WIFI_SSID     = "";
const char* WIFI_PASSWORD = "";

const char* API_KEY       = "";
const char* DATABASE_URL  = "";

const char* USER_EMAIL    = "";
const char* USER_PASSWORD = "";

#endif
````
<p>Your Firebase directories must match the ones declared in this function. If you want to change them, you will also 
need to change the name of the directories in this function and in the FirebaseRepository.kt file (responsible for collecting Firebase data in the app)</p>

````c++
void sendDataToFirebase() 
{
    if (Firebase.ready()) 
    {
        FirebaseJson json;
        json.set("/vagasDisponiveis", emptyParkingSpaces);
        json.set("/ultimoID", lastID.c_str());
        json.set("/tentativasInvalidas", invalidAttempts);
        json.set("/ativacoesAlarme", alarmActvations);
        json.set("/ultimoIDInvalido", lastInvalidID.c_str());
        json.set("/wifiConectado", getWifiStatus());
        Firebase.updateNode(fbdo, "/", json);
    }
}
````

<br><h2> &#128295; Circuit Assembly </h2>

<br><div align="center">

  <h3>Using ESP­-WROOM­-32</h3>   
  
  | Compenent               | Component Pin | ESP32 Pin |
  |:-----------------------:|:-------------:|:---------:|
  | fc-51 (Infrared Sensor) | out           | GPIO17    |
  | fc-51 (Infrared Sensor) | VCC           | 5V        |
  | RFID-RC522              | SDA           | GPIO4     |
  | RFID-RC522              | SCK           | GPIO18    |
  | RFID-RC522              | MOSI          | GPIO23    |
  | RFID-RC522              | MISO          | GPIO19    |
  | RFID-RC522              | RST           | GPIO15    |
  | RFID-RC522              | VCC           | 3.3V      |
  | LCD 20x4 (I2C)          | SDA           | GPIO21    |
  | LCD 20x4 (I2C)          | SCL           | GPIO22    |
  | LCD 20x4 (I2C)          | VCC           | 5V        |
  | Servo                   | SCL           | GPIO13    |
  | Servo                   | VCC           | 5V        |
  | Buzzer                  | pin2          | GPIO36    |
  | LED (Green)             | anode         | GPIO7     |
  | LED (Red)               | anode         | GPIO8     |
  
</div>

<br><h3 align = "center"> - By <a href = "https://www.linkedin.com/in/breno-barbosa-de-oliveira-810866275/" target = "_blank">Breno</a> - </h3>
