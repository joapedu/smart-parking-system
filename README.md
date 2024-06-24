<h1 align = "center"> Smart Park System ESP32 </h1><br>

<h2> &#128269; About the project </h2><br>

<p>Intelligent parking system connected to firebase and managed by Android Application. The system has access card reading, parking availability, alarm system and real time monitoring.</p><br>

<h2> &#128302; Technologies Used </h2><br>

<p align="center">
  <a href="https://skillicons.dev">
    <img src="https://skillicons.dev/icons?i=arduino,kotlin,firebase" />
  </a>
</p>

<br><h2> &#128161; How it Works? </h2>

<br><p align="center">
  <img src="https://github.com/Brevex/Smart-Parking-System-ESP32/blob/076c03dcfa8072db26ae9f7a94119233ed02e8fe/circuit/circuit.png" alt="circuit">
</p><br>

<ul>
  <li>Reads the card and checks if it has a valid ID in the system. If authorized, a green LED will light up and a gate represented by a servo motor will open. If the card is declined 3 times, an alarm will sound and a red LED will flash.</li>
  <li>An LCD display will inform the driver whether there are available spaces or not. If there are none, the parking lot will be inaccessible.</li>
  <li>The parking space will be monitored by an infrared sensor</li>
  <li>All parking operations will be monitored through an Android application, which will inform: available parking spaces, ID of the last card used, ID of the last card declined, number of times the alarm was activated and system connection status.</li>
</ul>

<br><h2> &#128295; Circuit Assembly </h2>

<br><div align="center">

  <h3>Using ESP32 devkit</h3>   
  
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
