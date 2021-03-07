// Adafruit IO Publish & Subscribe Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
#include <stdint.h>
#include <stdio.h>

/************************ Example Starts Here *******************************/
// Variables de control de datos
uint8_t count = 85, rlec = 0, glec = 0;
bool rec = false;
uint8_t rxdata = 0;
uint8_t txdata = 0;

// Track time of last published messages and limit feed->save events to once
// every IO_LOOP_DELAY milliseconds.
//
// Because this sketch is publishing AND subscribing, we can't use a long
// delay() function call in the main loop since that would prevent io.run()
// from being called often enough to receive all incoming messages.
//
// Instead, we can use the millis() function to get the current time in
// milliseconds and avoid publishing until IO_LOOP_DELAY milliseconds have
// passed.
#define IO_LOOP_DELAY 3000
unsigned long lastUpdate = 0;

// set up the 'counter' feed
AdafruitIO_Feed *counter = io.feed("counter");
AdafruitIO_Feed *LuzRoja = io.feed("LuzRoja");
AdafruitIO_Feed *LuzVerde = io.feed("LuzVerde");

void setup() {

  // start the serial connection
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  LuzRoja->onMessage(handleMessageR);
  LuzVerde->onMessage(handleMessageG);

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  LuzRoja->get();
  LuzVerde->get();
}

void loop() {
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(32, OUTPUT);

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  if (millis() > (lastUpdate + IO_LOOP_DELAY)) {
    // save count to the 'counter' feed on Adafruit IO
    //Serial.print("sending -> ");
    //Serial.print(count);
    counter->save(rxdata);
    // increment the count by 1
    // after publishing, store the current time
    lastUpdate = millis();
    //Serial.print("* * * ");
    //Serial.print(rlec);
    //Serial.print(" -- ");
    //Serial.print(glec);
    if (rec) {
      //  Serial.println(" - TRANSMITTING");
      Serial.println(txdata);
    }
    else {
      //  Serial.println("");
    }
    rec = false;
  }
  if (Serial2.available()) {
    rxdata = (uint8_t)Serial2.read();
    Serial.print("Serial2data: ");
    Serial.println(rxdata);
    Serial2.write(txdata); // ENVIANDO A LA VEZ LOS DATOS AL PIC
    if (rxdata & 128) {
      digitalWrite(13, HIGH);
    } else {
      digitalWrite(13, LOW);
    }
    if (rxdata & 64) {
      digitalWrite(12, HIGH);
    } else {
      digitalWrite(12, LOW);
    }
    if (rxdata & 32) {
      digitalWrite(14, HIGH);
    } else {
      digitalWrite(14, LOW);
    }
    if (rxdata & 16) {
      digitalWrite(27, HIGH);
    } else {
      digitalWrite(27, LOW);
    }
    if (rxdata & 8) {
      digitalWrite(26, HIGH);
    } else {
      digitalWrite(26, LOW);
    }
    if (rxdata & 4) {
      digitalWrite(25, HIGH);
    } else {
      digitalWrite(25, LOW);
    }
    if (rxdata & 2) {
      digitalWrite(33, HIGH);
    } else {
      digitalWrite(33, LOW);
    }
    if (rxdata & 1) {
      digitalWrite(32, HIGH);
    } else {
      digitalWrite(32, LOW);
    }
  }

}

// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleMessageR(AdafruitIO_Data *data) {
  //Serial.print("Red received <- ");
  if (data->toPinLevel() == HIGH) {
    //Serial.println("HIGH");
    rlec = 1;
  }
  else {
    //Serial.println("LOW");
    rlec = 0;
  }
  txdata = rlec + glec*2;
  rec = true;
}

void handleMessageG(AdafruitIO_Data *data) {
  //Serial.print("Green received <- ");
  if (data->toPinLevel() == HIGH) {
    //Serial.println("HIGH");
    glec = 1;
  }
  else {
    //Serial.println("LOW");
    glec = 0;
  }
  txdata = rlec + glec*2 ;
  rec = true;
}
