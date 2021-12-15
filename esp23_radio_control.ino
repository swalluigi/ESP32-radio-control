/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/

/*Remote controll for quadcopter using esp32
 *  Ian Sbar
 *   
 *  Dofs used
 *  Up/Down                     Chan3   left stick ver    servoPin3 10
 *  Right/Left                  Chan1   Right Stick Horz  servoPin1 8
 *  Forward/Backward            Chan2   Right Stick vert  servoPin2 9 
 *  Clockwise/counterclockwise  Chan4   Left Stick horz   servoPin4 11
 *  
 *  May wish to use 1250 min and 1750 max to start
 *  */
 

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WiFiAP.h>
#include <ESP32_Servo.h>

#define servoPin1 25
#define servoPin2 33
#define servoPin3 32
#define servoPin4 35
// Set these to your desired credentials.
const char *ssid = "ohno";
const char *password = "pls_work";


WiFiServer server(80);

int chan1Lv1 = 1500;
int chan2Lv1 = 1500;
int chan3Lv1 = 1500;
int chan4Lv1 = 1500;

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;

void setup() {
  
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");

  myservo1.attach(servoPin1,1000,2000);  
  myservo2.attach(servoPin2,1000,2000);
  myservo3.attach(servoPin3,1000,2000);
  myservo4.attach(servoPin4,1000,2000);

  myservo1.writeMicroseconds(chan1Lv1);
  myservo2.writeMicroseconds(chan2Lv1);
  myservo3.writeMicroseconds(chan3Lv1);
  myservo4.writeMicroseconds(chan4Lv1);

  Serial.print("chan1 level");
  Serial.println(chan1Lv1);
  Serial.print("chan2 level");
  Serial.println(chan2Lv1);
  Serial.print("chan3 level");
  Serial.println(chan3Lv1);
  Serial.print("chan4 level");
  Serial.println(chan4Lv1);
  
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("welcome to Ian's quad control page <br>");
            client.print("<button><a href=\"/F\">forward</a> <button><br>");
            client.print("<button><a href=\"/B\">backward </a> <button><br>");
            client.print("<button><a href=\"/U\">up</a> <button><br>");
            client.print("<button><a href=\"/D\">down</a> <button><br>");
            client.print("<button><a href=\"/R\">right</a> <button><br>");
            client.print("<button><a href=\"/L\">left</a> <button><br>");
            client.print("<button><a href=\"/CL\">clockwise</a> <button><br>");
            client.print("<button><a href=\"/CCL\">counterclock</a> <button><br>");

            client.print("<button><a href=\"/S\">hover</a> <button><br>");
            
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        


        
        if (currentLine.endsWith("GET /F")) { //tells the drone to move forward
            chan2Lv1=chan2Lv1+100;
            if(chan2Lv1>1750){
              chan2Lv1=1750;      
            }
            
             myservo1.writeMicroseconds(chan1Lv1);
             myservo2.writeMicroseconds(chan2Lv1);
             myservo3.writeMicroseconds(chan3Lv1);
             myservo4.writeMicroseconds(chan4Lv1);

             Serial.print("chan1 level ");
             Serial.println(chan1Lv1);
             Serial.print("chan2 level");
             Serial.println(chan2Lv1);
             Serial.print("chan3 level");
             Serial.println(chan3Lv1);
             Serial.print("chan4 level");
             Serial.println(chan4Lv1);
               
        }
        if (currentLine.endsWith("GET /B")) {//tells the drone to move backward
            chan2Lv1=chan2Lv1-100;
            if(chan2Lv1>1750){
              chan2Lv1=1750;      
            }
            
             myservo1.writeMicroseconds(chan1Lv1);
             myservo2.writeMicroseconds(chan2Lv1);
             myservo3.writeMicroseconds(chan3Lv1);
             myservo4.writeMicroseconds(chan4Lv1);

             Serial.print("chan1 level");
             Serial.println(chan1Lv1);
             Serial.print("chan2 level");
             Serial.println(chan2Lv1);
             Serial.print("chan3 level");
             Serial.println(chan3Lv1);
             Serial.print("chan4 level");
             Serial.println(chan4Lv1);                      // GET /L turns the LED off
        }
         if (currentLine.endsWith("GET /U")) {//tells the drone to move up
            chan3Lv1=chan3Lv1+100;
            if(chan3Lv1>1750){
              chan3Lv1=1750;      
            }
            
             myservo1.writeMicroseconds(chan1Lv1);
             myservo2.writeMicroseconds(chan2Lv1);
             myservo3.writeMicroseconds(chan3Lv1);
             myservo4.writeMicroseconds(chan4Lv1);

             Serial.print("chan1 level");
             Serial.println(chan1Lv1);
             Serial.print("chan2 level");
             Serial.println(chan2Lv1);
             Serial.print("chan3 level");
             Serial.println(chan3Lv1);
             Serial.print("chan4 level");
             Serial.println(chan4Lv1);
      }
       if (currentLine.endsWith("GET /D")) { //tells the drone to move down
            chan3Lv1=chan3Lv1-100;
            if(chan3Lv1>1750){
              chan3Lv1=1750;      
            }
            
             myservo1.writeMicroseconds(chan1Lv1);
             myservo2.writeMicroseconds(chan2Lv1);
             myservo3.writeMicroseconds(chan3Lv1);
             myservo4.writeMicroseconds(chan4Lv1);

             Serial.print("chan1 level");
             Serial.println(chan1Lv1);
             Serial.print("chan2 level");
             Serial.println(chan2Lv1);
             Serial.print("chan3 level");
             Serial.println(chan3Lv1);
             Serial.print("chan4 level");
             Serial.println(chan4Lv1);       
    }
     if (currentLine.endsWith("GET /R")) { //tells the drone to move to the right
            chan1Lv1=chan1Lv1+100;
            if(chan1Lv1>1750){
              chan1Lv1=1750;      
            }
            
             myservo1.writeMicroseconds(chan1Lv1);
             myservo2.writeMicroseconds(chan2Lv1);
             myservo3.writeMicroseconds(chan3Lv1);
             myservo4.writeMicroseconds(chan4Lv1);

             Serial.print("chan1 level");
             Serial.println(chan1Lv1);
             Serial.print("chan2 level");
             Serial.println(chan2Lv1);
             Serial.print("chan3 level");
             Serial.println(chan3Lv1);
             Serial.print("chan4 level");
             Serial.println(chan4Lv1);        
        }
        if (currentLine.endsWith("GET /L")) { //tells the drone to move to the left
            chan1Lv1=chan1Lv1-100;
            if(chan1Lv1>1750){
              chan1Lv1=1750;      
            }
            
             myservo1.writeMicroseconds(chan1Lv1);
             myservo2.writeMicroseconds(chan2Lv1);
             myservo3.writeMicroseconds(chan3Lv1);
             myservo4.writeMicroseconds(chan4Lv1);

             Serial.print("chan1 level");
             Serial.println(chan1Lv1);
             Serial.print("chan2 level");
             Serial.println(chan2Lv1);
             Serial.print("chan3 level");
             Serial.println(chan3Lv1);
             Serial.print("chan4 level");
             Serial.println(chan4Lv1);        
        }
        if (currentLine.endsWith("GET /CL")) { //tells the drone to rotate clockwise
            chan4Lv1=chan4Lv1+100;
            if(chan4Lv1>1750){
              chan4Lv1=1750;      
            }
            
             myservo1.writeMicroseconds(chan1Lv1);
             myservo2.writeMicroseconds(chan2Lv1);
             myservo3.writeMicroseconds(chan3Lv1);
             myservo4.writeMicroseconds(chan4Lv1);

             Serial.print("chan1 level");
             Serial.println(chan1Lv1);
             Serial.print("chan2 level");
             Serial.println(chan2Lv1);
             Serial.print("chan3 level");
             Serial.println(chan3Lv1);
             Serial.print("chan4 level");
             Serial.println(chan4Lv1);         
        }
        if (currentLine.endsWith("GET /CCL")) {//tells the drone to rotate counterclockwise
            chan4Lv1=chan4Lv1-100;
            if(chan4Lv1>1750){
              chan4Lv1=1750;      
            }
            
             myservo1.writeMicroseconds(chan1Lv1);
             myservo2.writeMicroseconds(chan2Lv1);
             myservo3.writeMicroseconds(chan3Lv1);
             myservo4.writeMicroseconds(chan4Lv1);

             Serial.print("chan1 level");
             Serial.println(chan1Lv1);
             Serial.print("chan2 level");
             Serial.println(chan2Lv1);
             Serial.print("chan3 level");
             Serial.println(chan3Lv1);
             Serial.print("chan4 level");
             Serial.println(chan4Lv1);         
        }
       if (currentLine.endsWith("GET /S")) { //tells the drone to hover in place
            chan3Lv1=1500;
            chan4Lv1=1500;
            chan2Lv1=1500;
            chan1Lv1=1500;
            
            
            
             myservo1.writeMicroseconds(chan1Lv1);
             myservo2.writeMicroseconds(chan2Lv1);
             myservo3.writeMicroseconds(chan3Lv1);
             myservo4.writeMicroseconds(chan4Lv1);

             Serial.print("chan1 level");
             Serial.println(chan1Lv1);
             Serial.print("chan2 level");
             Serial.println(chan2Lv1);
             Serial.print("chan3 level");
             Serial.println(chan3Lv1);
             Serial.print("chan4 level");
             Serial.println(chan4Lv1);;          
       }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
