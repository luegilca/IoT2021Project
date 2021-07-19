#include <SoftwareSerial.h>
#include <Servo.h>

// Constants
const int rxPin     = 10; // RX pin of MH10 Module
const int txPin     = 11; // TX pin of HM10 Module
const int pingPin   = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin   = 6; // Echo Pin of Ultrasonic Sensor
const int servoPin  = 9;   // Pushbutton Pin
const int btnPin    = 4;   // Pushbutton Pin
const int ledPin    = 13;
const int baudRate  = 9600;
const int threshold = 8; // Centimeters to trigger ultrasonic sensor
// Variables
int val     = 0; // variable to read the value from the analog pin
int value   = 0;     // variable for reading the pin status
bool state  = true; //Machine is off
// Instances
Servo myservo; // Create object to control a servo motor
SoftwareSerial moduloBtLE(rxPin, txPin); // Create object to communicate with Bluetooth module

void setup() {
  // Serial USB
  Serial.begin(baudRate);
  // Serial Bluetooth module
  moduloBtLE.begin(baudRate);
  while (!Serial) {
    // Wait for connection    
  }
  Serial.println("\n Connection ready.\n");

  // Connecting to module
  delay(1000);
  moduloBtLE.write("AT+VERSION");
  delay(1000);  
  
  //Setting up pins
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(btnPin, INPUT);  
  //digitalWrite(ledPin, HIGH); 
  
  // Attaches the Servo Motor signal line to pin 9
  myservo.attach(servoPin); 
}

void loop() {
//  Serial.println(digitalRead(btnPin));
//  if (digitalRead(btnPin) == HIGH) {
//    state = !state;
//  }
//
//  if (state == true) { // Machine is on
//    digitalWrite(ledPin, HIGH);  // turn LED ON

    // Sends to serial port what it comes from bluetooth module
    if (moduloBtLE.available()) {
      Serial.write(moduloBtLE.read());
    }
    // Sends to bluetooth module what it comes from serial port
    if (Serial.available()) {
      moduloBtLE.write(Serial.read());
    }
    // Setup of ultrasonic sensor
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pingPin, LOW);
    
    // reads the value of the potentiometer (value between 0 and 1023)
    // scale it to use it with the servo (value between 0 and 180)
    int angle = 0;
    long duration, cm;
    
  
    duration = pulseIn(echoPin, HIGH);
    cm = microsecondsToCentimeters(duration);
  
    if (cm < threshold) //hand is placed near sensor
    {
      delay(10);
  
      for (angle = 0; angle < 180; angle += 1) { // command to move from 0 degrees to 180 degrees pressing sanitizer
        myservo.write(angle);                 //command to rotate the servo to the specified angle
        //Serial.println(angle);
      }
      delay(10);
      for (angle = 180; angle >= 1; angle -= 10) { // command to move from 180 degrees to 0 degrees, removing the pressure in sanitizer bottle
        myservo.write(angle);              //command to rotate the servo to the specified angle
        //Serial.println(angle);<
      }
      moduloBtLE.write("device_id:1\n");
      Serial.print("ENVIANDO SEÑAL");
      delay(2000);
    }
    else {
      myservo.write(0);                 //command to rotate the servo to the specified angle
    }
//  }
//  else{
//    digitalWrite(ledPin, LOW);  // turn LED OFF
//  }   
//  delay(0);
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
