// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
#include <stdint.h>
uint8_t num = 85, cont = 10;
int n = 0;
unsigned long lastUp = 0;

#define IO_LOOP_DELAY 200

void setup() {
  Wire.begin(20);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event
  pinMode(A0, INPUT);

  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
}

void loop() {
  if (millis() > (lastUp + IO_LOOP_DELAY)) {
    lastUp = millis();
    n = analogRead(A0);
    Serial.println(n);

  }
  if (Serial1.available()) {
    Serial.println(Serial1.read());
  }
  if (Serial2.available()) {
    Serial.println(Serial2.read());
  }
}

void requestEvent() {
  num = (uint8_t)(n >> 2);
  Wire.write(num); //
  Serial.print("Respondiendo->");
  Serial.println(num);
}
void receiveEvent(int howMany) {
  if (1 == Wire.available()) {
    num = Wire.read();    // receive byte as an integer
    //if (num == 15) {
    //  bandera = true; No implementado
    //}
  }
  else {
    while (1 < Wire.available()) { // loop through all but the last
      char x = Wire.read(); // receive byte as a character
    }
    num = Wire.read();    // receive byte as an integer
  }
  Serial.print("Recibiendo: ");
  Serial.println(num);
}
