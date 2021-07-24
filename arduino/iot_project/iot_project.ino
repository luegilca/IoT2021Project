/*
  Creado: 
      * Luis Ernesto Gil Castellanos (luegilca@unal.edu.co)
      * Cristian Orjuela (ccorjuelav@unal.edu.co)
  Internet of Things 2021-I
*/
#include <SoftwareSerial.h>
#include <Servo.h>

// Constants
const int rxPin         = 10;   // RX pin of MH10 Module (white)
const int txPin         = 11;   // TX pin of HM10 Module (gray)
const int pingPin       = 7;    // Trigger Pin of Ultrasonic Sensor
const int echoPin       = 6;    // Echo Pin of Ultrasonic Sensor
const int servoPin      = 9;    // Servo motor signal pin
const int btnPin        = 4;    // Pushbutton Pin
const int ledPin        = 13;   // Led pin
const int baudRate      = 9600; // Default baud rate
const int releaseDelay  = 2000; // Amount of time (in ms) to detect a hand near the sensor
const int waitDelay     = 300;  // Amount of time (in ms) to wait until servo gets back to initial state
const int readDelay     = 200;  // Amount of time (in ms) to read pushbutton signal
const int pressDelay    = 5;    // Amount of time (in ms) to write signal to push bottle
const int unpressDelay  = 5;    // Amount of time (in ms) to write signal to unpush bottle
const int threshold     = 8;    // Centimeters to trigger ultrasonic sensor

// Variables
int state     = false;  // Machine is off
int angle     = 0;      // Rotation angle for servo motor    
long duration, cm = 0;  // Pulse duration of ultrasonic sensor and calculated distance

// Instances
Servo myservo; // Create object to control a servo motor
SoftwareSerial moduloBtLE(rxPin, txPin); // Create object to communicate with Bluetooth module

// Methods
long microsecondsToCentimeters(long microseconds) {
  return microseconds / 27 / 2;
}

void setup() {
  // Serial USB
  Serial.begin(baudRate);
  // Serial Bluetooth module
  moduloBtLE.begin(baudRate);
  while (!Serial) {
    // Wait for connection    
  }
  while (!moduloBtLE) {
    // Wait for connection    
  }
  Serial.println("\n Connection ready.\n");

  // Connecting to module
  delay(1000); 
  
  //Setting up pins
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(btnPin, INPUT);  
  digitalWrite(ledPin, LOW); 
  
  // Attaches the Servo Motor signal line to pin 9
  myservo.attach(servoPin); 
  myservo.write(0);
}

void loop() {
  if (digitalRead(btnPin) == HIGH) {
    state = !state;
    Serial.print("Machine is: ");
    Serial.println(state ? "ON" : "OFF");    
  }
  
  if (state == true) { 
    // Machine is on
    digitalWrite(ledPin, HIGH);  // turn LED ON
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
    // Reads data from sensor
    duration = pulseIn(echoPin, HIGH);
    cm = microsecondsToCentimeters(duration);
  
    if (cm < threshold)
    {
      for (int angle = 0; angle <= 180; angle++) {
        myservo.write(angle);
        delay(pressDelay);
      }
      
      delay(waitDelay);
      
      for (int angle = 180; angle > 0; angle--) { 
        myservo.write(angle);
        delay(unpressDelay);
      }
      // Sending usage info to bluetooth module
      moduloBtLE.write("device_id:1\n");
      Serial.println("Device used!");
      delay(releaseDelay);
    }
    else {
      Serial.print("Distance: ");
      Serial.println(cm);
    }
  }
  else{
    digitalWrite(ledPin, LOW);  // turn LED OFF
  }   
  delay(readDelay);
}
