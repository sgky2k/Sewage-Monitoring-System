String myAPIkey = "3Z0HC0VKJ6A573N1";  
#include <SoftwareSerial.h>
#include <DHT.h>;
SoftwareSerial gprsSerial(2, 3); // Rx,  Tx
#include "SPI.h"
int level = 0;
int flag=0;
#define DHTTYPE DHT11
#define DHTPIN  A0
long writingTimer = 20; 
long startTime = 0;
long waitTime = 0;
DHT dht(DHTPIN, DHTTYPE,11);
long duration; 
int distance;
float humidity, temp_f;  
int a = 0;
int b = 0;
int d=0;
int c = 0;
void setup()
{
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(4, INPUT);
  Serial.begin(9600);
  gprsSerial.begin(9600);               // the GPRS baud rate   
  dht.begin();
  delay(1000);
}
void loop()
{
  temp_f = dht.readTemperature();
  humidity = dht.readHumidity();
  a=analogRead(A2);
  b=map(analogRead(A3), 0, 1023, 0, 100);
  c=map(analogRead(A4), 0, 1023, 0, 100);
  level= (digitalRead(4));
  
  Serial.println(temp_f);
  Serial.println(humidity);
  Serial.println(a);
  Serial.println(b);
  Serial.println(c);
  Serial.println(level);  
  if (((a> 50)||(b>90)||(c>90)||(level==1))&&(flag==0)) {
      gprsSerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  gprsSerial.println("AT+CMGS=\"+917448860412\"\r"); // Replace x with mobile number
  delay(1000);
  gprsSerial.println("I am SMS from GSM Module");// The SMS text you want to send
  delay(100);
   gprsSerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println("hlo");
    delay(8000);
    flag=1;
  }  
  else if(((a< 50)&&(b<90)&&(c<90)&&(level!=1))&&(flag==1)){
    flag=0;
  }
      waitTime = millis()-startTime;   
  if (waitTime > (writingTimer*1000)) 
  {
    api();
    startTime = millis();   
  }
}

 void send_message(String message)
{
  gprsSerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  gprsSerial.println("AT+CMGS=\"+917448860412\"\r"); // Replace x with mobile number
  delay(1000);
  gprsSerial.println("I am SMS from GSM Module");// The SMS text you want to send
  delay(100);
  gprsSerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println("hlo");
  Serial.println(message);
}
void api(){   
  if (gprsSerial.available())
    Serial.write(gprsSerial.read());
 
  gprsSerial.println("AT");
  delay(1000);
 
  gprsSerial.println("AT+CPIN?");
  delay(1000);
 
  gprsSerial.println("AT+CREG?");
  delay(1000);
 
  gprsSerial.println("AT+CGATT?");
  delay(1000);
 
  gprsSerial.println("AT+CIPSHUT");
  delay(1000);
 
  gprsSerial.println("AT+CIPSTATUS");
  delay(2000);
 
  gprsSerial.println("AT+CIPMUX=0");
  delay(2000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CSTT=\"airtelgprs.com\"");//start task and setting the APN,
  delay(1000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIICR");//bring up wireless connection
  delay(3000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIFSR");//get local IP adress
  delay(2000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSPRT=0");
  delay(3000);
 
  ShowSerialData();
  
  gprsSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();
  
  String str="GET https://api.thingspeak.com/update?api_key=1CXFFIFPDZ0KGOAG&field1=" + String(temp_f) +"&field2="+String(humidity)+"&field3="+String(a)+"&field4="+String(b)+"&field5="+String(c)+"&field6="+String(level); 
  Serial.println(str);
  gprsSerial.println(str);//begin send data to remote server
  
  delay(4000);
  ShowSerialData();
 
  gprsSerial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet 
  gprsSerial.println();
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
} 
void ShowSerialData()
{
  while(gprsSerial.available()!=0)
  Serial.write(gprsSerial.read());
  delay(5000); 
  

}
