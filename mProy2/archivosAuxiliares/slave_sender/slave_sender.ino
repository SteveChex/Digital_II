// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
#include <stdint.h>
uint8_t num = 85;

void setup() {
  Wire.begin(20);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event
  pinMode(5, OUTPUT);
}

void loop() {
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  Wire.write(num); //
  if (digitalRead(5)){
    digitalWrite(5, HIGH);
    }
    else {
      digitalWrite(5, LOW);
      }
  // as expected by master
}
void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char x = Wire.read(); // receive byte as a character
  }
  num = (uint8_t)Wire.read();    // receive byte as an integer
}
