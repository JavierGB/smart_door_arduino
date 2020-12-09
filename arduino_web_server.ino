/*
  Digital Trush Hackathon 2020
  Javier Gonzalez Benito
  javi.gb.mad@gmail.com
*/

#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 100);
EthernetServer server(80);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

String text = "";
void setup()
{
  initWebServer();
  initLCD();
}

void loop()
{
  EthernetClient client = server.available();
  if (client) {
    Serial.println("New request");
    
    if (client.connected() && client.available()) {
      boolean valid = false;
      String request = readRequestFromClient(client);

      String userName = getRequestParam(request, "userName");
      String validPassport = getRequestParam(request, "validPassport");

      if (validPassport == "true") {
        text = "Wellcome " + userName + "!";
        valid = true;
      }

      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: application/json");
      client.println();

      client.println("{");
      client.println("\"text\": "); client.println("\"" + text + "\"");
      client.println("}");


      delay(100);
      client.stop();

      if (valid) {
        lcd.setCursor(0, 1);
        lcd.print(text);
      } else {
        accessDenied();
      }
      refreshLCD();
      String text = "";
    }


  }
}

String getRequestParam(String request, String code) {
  code.concat('=');
  int codeInitPos = request.indexOf(code) + code.length();

  String subRequest = request.substring(codeInitPos);
  int codeLastPos = subRequest.indexOf("&");

  if (codeLastPos == -1) {
    codeLastPos = subRequest.indexOf(" HTTP/1.1");
  }
  return subRequest.substring(0, codeLastPos);

}

String readRequestFromClient(EthernetClient client) {
  String request = "";
  char c = ' ';
  while (c != '\n') {
    c = client.read();
    request.concat(c);
  }
  return request;
}

void initWebServer() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void initLCD() {
  lcd.begin(16, 2);
  lcd.print(" Digital Trust");
}

void accessDenied() {
  printError();
  cleanLCD();
  printError();
  cleanLCD();
  printError();
  cleanLCD();
  printError();
}

void refreshLCD() {
  delay(10000);
  lcd.setCursor(0, 1);
  lcd.print("                   ");
}

void printError() {
  lcd.setCursor(0, 1);
  text = "Access Denied!";
  lcd.print(text);
  delay(800);
}
void cleanLCD() {
  lcd.setCursor(0, 1);
  text = "                    ";
  lcd.print(text);
  delay(500);
}
