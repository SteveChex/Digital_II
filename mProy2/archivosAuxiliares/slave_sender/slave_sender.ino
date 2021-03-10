// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
#include <stdint.h>
uint8_t num = 85, n = 0;
unsigned long lastUp = 0;

#define IO_LOOP_DELAY 1000

void setup() {
  Wire.begin(20);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event
  pinMode(5, OUTPUT);

  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
}

void loop() {
  if (millis() > (lastUp + IO_LOOP_DELAY)) {
    lastUp = millis();
    n++;
    if (n == 3) {
      n = 0;
    }
  }
  if (Serial1.available()) {
    Serial.println(Serial1.read());
  }
  if (Serial2.available()) {
    Serial.println(Serial2.read());
  }
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  Wire.write(num); //
  if (digitalRead(5)) {
    digitalWrite(5, HIGH);
  }
  else {
    digitalWrite(5, LOW);
  }
  // as expected by master
  // Serial.print("Respondiendo");
  // Serial.println(num);
}
void receiveEvent(int howMany) {
  if (1 == Wire.available()) {
    num = (uint8_t)Wire.read();    // receive byte as an integer
  }
  else {
    num = (uint8_t)Wire.read();    // receive byte as an integer
    while (1 < Wire.available()) { // loop through all but the last
      char x = Wire.read(); // receive byte as a character
    }
  }
  //Serial.print("Recibiendo");
  //Serial.println(num);
}
