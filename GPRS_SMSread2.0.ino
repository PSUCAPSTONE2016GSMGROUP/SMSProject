#include <GPRS_Shield_Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <string.h>

#define PIN_TX    2
#define PIN_RX    3
#define BAUDRATE  9600
#define MESSAGE  "Invaild"
#define MESSAGE2  "Unauthorized"

#define MESSAGE_LENGTH 160
char message[MESSAGE_LENGTH];
int messageIndex = 0;

// create variables to hold data
char phone[16];
char datetime[24];

GPRS gprsTest(PIN_TX,PIN_RX,BAUDRATE); //RX,TX,PWR,BaudRate

void setup() {
  // begin com at BAUDRATE of 9600 
  Serial.begin(9600);
  
  // keep trying to initialize until initialization is successful
  while(!gprsTest.init()) {
      Serial.print("init error\r\n");
      delay(1000);
  }
  delay(3000);  
  Serial.println("Init Success, please send SMS message to me!");
  
  // initialize digital pin A0, and A3 as outputs.
  pinMode(A0, OUTPUT);
  pinMode(A3, OUTPUT);
}

void loop() {
  delay(100);
   messageIndex = gprsTest.isSMSunread();
   if (messageIndex > 0) { //At least, there is one UNREAD SMS
      gprsTest.readSMS(messageIndex, message, MESSAGE_LENGTH, phone, datetime);           
      //In order not to full SIM Memory, is better to delete it
      gprsTest.deleteSMS(messageIndex);
      
     // check if the message was sent by an authorized phone number 
      if (strcmp (phone,"5038101652") == 0 || strcmp (phone,"9713403666") == 0 || strcmp (phone,"6199226759") == 0 || strcmp (phone,"5039603745") == 0) {
        // apply appropriate action based on the message 
        if(String(message).equalsIgnoreCase("Unlock")){
          digitalWrite(A0, HIGH); // turn the LED on by making the voltage high
          digitalWrite(A3, HIGH);
          gprsTest.sendSMS(phone,message); 
        } 
        else if(String(message).equalsIgnoreCase("Lock")){
          digitalWrite(A0, LOW); // turn the LED off by making the voltage LOW
          digitalWrite(A3, LOW); 
          gprsTest.sendSMS(phone,message); 
        } 
        else {
          char charBuf[50];
          gprsTest.sendSMS(phone,MESSAGE); 
        }
      }
      else {
          gprsTest.sendSMS(phone,MESSAGE2); // send unauthorized user to the unauthorized sender 
      }
      
      Serial.print("From number: ");
        Serial.println(phone);  
        Serial.print("Datetime: ");
        Serial.println(datetime);        
        Serial.print("Recieved Message: ");
        Serial.println(message);
   }
}
