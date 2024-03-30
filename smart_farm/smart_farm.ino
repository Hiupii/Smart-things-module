#include <Servo.h>

#include <ArduinoJson.h>
#include<SoftwareSerial.h>
SoftwareSerial mySerial(10,11);

#include <Key.h>
#include <Keypad.h>

#include <DHT.h>
#include <DHT_U.h>


#define DHT_Data 2
#define Temp_Led 3
#define DHT_Fan 16

DHT dht(DHT_Data, DHT22);

#define PIR_Data 4
#define PIR_Led 5
#define PIR_Button 6
#define PIR_Light 7
int light_value = 0;
unsigned long timeDelay = 0;

Servo Door_Servo;
bool Door_Pos = true;


#define MQ_Data A0
Servo Window_Servo;
#define MQ_Fan 18
int MQ_Led = 17;
bool Window_Pos = true;

unsigned long period = 1000;

void setup() 
{
  dht.begin();
  pinMode(DHT_Fan, OUTPUT);
  pinMode(Temp_Led, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
  while(!Serial) delay(1);

  pinMode(PIR_Data, INPUT_PULLUP);
  pinMode(PIR_Led, OUTPUT);
  pinMode(PIR_Button, INPUT_PULLUP);
  pinMode(PIR_Light, OUTPUT);

  Door_Servo.attach(14);

  pinMode(MQ_Data, INPUT);
  Window_Servo.attach(15);
  pinMode(MQ_Fan, OUTPUT);
  pinMode(MQ_Led, OUTPUT);
}

void getDHT(float &temp, float &hum)
{
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  //Serial.println(temp);
  if(temp>=35)
    digitalWrite(DHT_Fan, HIGH);
  else
    digitalWrite(DHT_Fan, LOW);
  if(temp<35)
    digitalWrite(Temp_Led, HIGH);
  else
    digitalWrite(Temp_Led, LOW);
}

void getPIR()
{
  if(digitalRead(PIR_Data)==HIGH) 
  {
    digitalWrite(PIR_Led, LOW);
  } 
  else 
  {
    digitalWrite(PIR_Led, HIGH);
  }

  if(digitalRead(PIR_Button)==LOW) 
  {
    if(millis() - timeDelay > 50)
    {
      if
      (digitalRead(PIR_Button)==LOW)
      {
        digitalWrite(PIR_Light, !digitalRead(PIR_Light));
      }
      timeDelay = millis();
    }
  }
}

/*void getRFID()
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "F3 BB C4 36") //change here the UID of the card/cards that you want to give access
  {
    lcd.setCursor(0,0);
    lcd.print("ID: 1");
    lcd.setCursor(0,1);
    lcd.print("OK");
    getServoTrue(Door_Pos, Door_Servo);
  }
  else   {
    lcd.setCursor(0,0);
    lcd.print("ID: Unknown");
    lcd.setCursor(0,1);
    lcd.print("Deny");
  }
}*/

void getMQ(float &value)
{
  value = analogRead(MQ_Data);
  if(value >= 500)
  {
    getServoTrue(Window_Pos, Window_Servo);
    getServoTrue(Door_Pos, Door_Servo);
    digitalWrite(MQ_Fan, HIGH);
    blinkLed(period, MQ_Led);
  }
  else
  {
    getServoFalse(Door_Pos, Door_Servo);
    getServoFalse(Window_Pos, Window_Servo);
    digitalWrite(MQ_Fan, LOW);
    digitalWrite(MQ_Led, LOW);
  }
}

void blinkLed(unsigned long &period, int &ledPin)
{
  unsigned long currentMillis;
  if (millis() - currentMillis >= period)  //test whether the period has elapsed
  {
    digitalWrite(ledPin, !digitalRead(ledPin));  //if so, change the state of the LED.  Uses a neat trick to change the state
    currentMillis = millis();  //IMPORTANT to save the start time of the current LED state.
  }
}

void getServoTrue(bool &value, Servo &servo)
{
  value = !value;
  servo.write(0);  
  delay(500);
}

void getServoFalse(bool &value, Servo &servo)
{
  value = !value;
  servo.write(180);   
  delay(500);
}

DynamicJsonDocument doc(1024);

void loop() {
  // put your main code here, to run repeatedly:
  float t, h, g;
  getDHT(t, h);
  getPIR();
  // getRFID();
  getMQ(g);
  doc["temperature"] = t;
  doc["humidity"] = h;
  doc["gas"] = g;
  
  serializeJson(doc, mySerial);
  serializeJson(doc, Serial);
  mySerial.println();
  Serial.println();
  Serial.println(digitalRead(6));
  Serial.println(digitalRead(7));
}
