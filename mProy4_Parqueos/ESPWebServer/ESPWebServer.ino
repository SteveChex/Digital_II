/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server
  Basándose en los ejemplos de:
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <WebServer.h>
//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password
const char* ssid = "GUAIFAI";  // Enter your SSID here
const char* password = "apollo1234";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

uint8_t display1[7] = {2, 15, 4, 21, 19, 5, 18};
uint8_t numbers[5][7] = {
  {1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 0, 0, 0, 0},
  {1, 1, 0, 1, 1, 0, 1},
  {1, 1, 1, 1, 0, 0, 1},
  {0, 1, 1, 0, 0, 1, 1}
};

bool LED1status = LOW;
uint8_t state1 = LOW, state2 = LOW, state3 = LOW, state4 = LOW;
uint8_t dataHandler = 0, lastHandler = 0, numSel = 0;

//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  // CONFIGURACION DE RED
  Serial.begin(115200);
  Serial.println("Conectando a ");
  Serial.println(ssid);

  // COMUNICACION SERIAL LA TIVA
  Serial2.begin(115200, SERIAL_8N1, 16, 17);

  // ASIGNANDO SALIDAS
  for (int i = 0; i < 7; i++) {
    pinMode(display1[i], OUTPUT);
  }

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", handle_OnConnect); // Directamente desde e.g. 192.168.0.8
  server.on("/site1", handle_sites);

  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {
  server.handleClient();
  for (int  i = 0; i < 7; i++) {
    digitalWrite(display1[i], 0);
  }
  for (int  i = 0; i < 7; i++) {
    digitalWrite(display1[i], numbers[numSel][i]);
  }

  if (Serial2.available() > 0) {
    uint8_t tempData;
    tempData = Serial2.read();
    if (tempData > 40) {
      dataHandler = tempData;
      Serial.println(dataHandler);
      dataHandler = getUartNumber(dataHandler); // traduciendo dato ascii
    }
  }
  parkingData(dataHandler, &state1, &state2, &state3, &state4); // obteniendo estados de los parqueos

}
//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(state1, state2, state3, state4));
}
//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
String SendHTML(uint8_t par1, uint8_t par2, uint8_t par3, uint8_t par4) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\" http-equiv=\"refresh\" content=\"10\">\n";
  ptr += "<title>Parqueo ESP32</title>\n";
  ptr += "<style>html { font-family: Helvetica; color: black display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 25px; background-color: #EFF0D1; color: black;} h1 {color: black; margin: 10px auto 20px auto;} h3 {color: black ;margin-bottom: 20px; margin-top: 10px;}\n";
  ptr += ".button {display: block;width: 100px;background-color: #FFFFFF;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 2px;}\n";
  ptr += ".button-on {background-color: #54FD00;}\n";
  ptr += ".button-on:active {background-color: #47D600;}\n";
  ptr += ".button-off {background-color: #FF6400;}\n";
  ptr += ".button-off:active {background-color: #C54D00;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 5px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Parqueo-Matic</h1>\n";
  ptr += "<h3>Disponibilidad de parqueos</h3>\n";

  ptr += "<span id=\"sites\">";
  ptr += "<p>Parqueo 1</p><a class=\"button\"> - - </a>\n";
  ptr += "<p>Parqueo 2</p><a class=\"button\"> - - </a>\n";
  ptr += "<p>Parqueo 3</p><a class=\"button\"> - - </a>\n";
  ptr += "<p>Parqueo 4</p><a class=\"button\"> - - </a>\n";
  ptr += "</span>";

  ptr += "<script> setInterval(function(){getSite1();}, 1500);"; // Call a function repetatively with 2 Second interval
  ptr += "function getSite1(){";
  ptr += "var xhttp = new XMLHttpRequest();";
  ptr += "xhttp.onreadystatechange = function(){";
  ptr += "if (this.readyState == 4 && this.status == 200){";
  ptr += "document.getElementById(\"sites\").innerHTML = this.responseText;}";
  ptr += "};";
  ptr += "xhttp.open(\"GET\", \"site1\", true);";
  ptr += "xhttp.send();";
  ptr += "}";

  ptr += "</script>";
  ptr += "</html>\n";
  return ptr;
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
//************************************************************************************************
// Traductor UART
//************************************************************************************************
uint8_t getUartNumber(uint8_t rxdata) {
  switch (rxdata) { // Convirtiendo el numero ascii recibido en un dato numerico
    case 48:
      return 0;
    case 49:
      return 1;
    case 50:
      return 2;
    case 51:
      return 3;
    case 52:
      return 4;
    case 53:
      return 5;
    case 54:
      return 6;
    case 55:
      return 7;
    case 56:
      return 8;
    case 57:
      return 9;
    case 65:
      return 10;
    case 66:
      return 11;
    case 67:
      return 12;
    case 68:
      return 13;
    case 69:
      return 14;
    case 70:
      return 15;
  }
}
//************************************************************************************************
// Control de Parqueos
//************************************************************************************************
void parkingData(uint8_t rxdata, uint8_t* p1, uint8_t* p2, uint8_t* p3, uint8_t* p4) {
  numSel = 0;
  if ((rxdata & 1) == 0) { // Revisando primer parqueo
    *p1 = HIGH;
  }
  else {
    *p1 = LOW;
    numSel++;
  }

  if ((rxdata & 2) == 0) { // Revisando segundo parqueo
    *p2 = HIGH;
  }
  else {
    *p2 = LOW;
    numSel++;
  }

  if ((rxdata & 4) == 0) { // Revisando tercer parqueo
    *p3 = HIGH;
  }
  else {
    *p3 = LOW;
    numSel++;
  }

  if ((rxdata & 8) == 0) { // Revisando cuarto parqueo
    *p4 = HIGH;
  }
  else {
    *p4 = LOW;
    numSel++;
  }
}
//************************************************************************************************
// Respuesta a solicitud de Parqueos
//************************************************************************************************
void handle_sites() {
  String value = String("<p>Parqueo 1</p>");
  if (state1) {
    value += String("<a class=\"button button-off\">Tuki</a>");
  } else {
    value += String("<a class=\"button button-on\">Tukin't</a>");
  }
  value += String("<p>Parqueo 2</p>");
  if (state2) {
    value += String("<a class=\"button button-off\">Tuki</a>");
  } else {
    value += String("<a class=\"button button-on\">Tukin't</a>");
  }
  value += String("<p>Parqueo 3</p>");
  if (state3) {
    value += String("<a class=\"button button-off\">Tuki</a>");
  } else {
    value += String("<a class=\"button button-on\">Tukin't</a>");
  }
  value += String("<p>Parqueo 4</p>");
  if (state4) {
    value += String("<a class=\"button button-off\">Tuki</a>");
  } else {
    value += String("<a class=\"button button-on\">Tukin't</a>");
  }
  server.send(200, "text/plane", value); //Send ADC value only to client ajax request
}
