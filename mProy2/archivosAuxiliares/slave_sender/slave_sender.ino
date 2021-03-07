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

#define IO_LOOP_DELAY 2000

void setup() {
  Wire.begin(20);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event
  pinMode(5, OUTPUT);

  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  if (millis() > (lastUp + IO_LOOP_DELAY)) {
    Serial.println("");
    switch (n) {
      case 0:
        Serial1.write(85);
        break;
      case 1:
        Serial1.write(15);
        break;
      case 2:
        Serial1.write(1);
        break;
    }
    lastUp = millis();
    n++;
    if (n == 3) {
      n = 0;
    }
  }
  if (Serial1.available()) {
    Serial.println(Serial1.read());
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
}
void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char x = Wire.read(); // receive byte as a character
  }
  num = (uint8_t)Wire.read();    // receive byte as an integer
  Serial.println("--");
  Serial.println(num);
  Serial.println("--");
}
