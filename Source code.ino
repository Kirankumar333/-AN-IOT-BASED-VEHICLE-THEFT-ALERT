#include <SoftwareSerial.h>

SoftwareSerial mySerial(9,10);
int pirPin = 2;  // Replace with the actual pin your PIR sensor is connected to
bool motionDetected = false;

void makeCall() {
  mySerial.println("ATD+919480020516;");  // Replace +123456789 with the phone number you want to call
  delay(1000);
}

void sendSMS() {
  mySerial.println("AT+CMGF=1");    // Sets the GSM Module in Text Mode
  delay(1000);
  mySerial.println("AT+CMGS=\"+919480020516\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("ALERT MESSAGE: MOTION DETECTED IN YOUR VEHICLE");  // The SMS text you want to send
  delay(1000);
  mySerial.println((char)26);  // ASCII code of CTRL+Z for signaling the end of SMS to the module
  delay(1000);
}

void setup() {
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  pinMode(pirPin, INPUT);  // Set PIR pin as input
  delay(1000);
}

void loop() {
  if (digitalRead(pirPin) == HIGH && !motionDetected) {
    // Motion detected and alert not sent yet, send SMS and make a call
    makeCall();
    sendSMS();
    motionDetected = true;  // Set flag to true to indicate alert sent
   
    delay(50000);  // Delay to avoid sending multiple SMS/calls for the same motion
  }

  if (Serial.available() > 0) {
    char command = Serial.read();
    switch (command) {
      case 'r':
        mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
        delay(1000);
        break;
    }
  }

  if (mySerial.available() > 0) {
    Serial.write(mySerial.read());
  }
}
