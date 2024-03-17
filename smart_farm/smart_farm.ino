#include <Servo.h>

#include <ArduinoJson.h>

#include <Key.h>
#include <Keypad.h>

#include <DHT.h>
#include <DHT_U.h>


#define DHT_Data 2
#define Temp_Led 3
#define DHT_Fan 16

DHT dht(DHT_Data, DHT11);

#define PIR_Data 4
#define PIR_Led 5
#define PIR_Button 6
#define PIR_Light 19
int light_value = 0;

Servo Door_Servo;
bool Door_Pos = true;

#define MQ_Data A0
Servo Window_Servo;
#define MQ_Fan 18
#define MQ_Led 17
bool Window_Pos = true;

void setup() 
{
  dht.begin();
  pinMode(DHT_Fan, OUTPUT);
  pinMode(Temp_Led, OUTPUT);
  Serial.begin(9600);

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
  if(temp>=30)
    digitalWrite(DHT_Fan, HIGH);
  else
    digitalWrite(DHT_Fan, LOW);
  if(temp<=25)
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
    delay(20);
    if(digitalRead(PIR_Button)==LOW) 
    {
      //Serial.println("Pressed");
      if(light_value == 0)
      {
        digitalWrite(PIR_Light, HIGH);
        light_value = 1;
      }
      else 
      {
        digitalWrite(PIR_Light, LOW);
        light_value = 0;
      }
    } 
  }
  while(digitalRead(PIR_Button)==LOW);
}

void getMQ(float &value)
{
  value = analogRead(MQ_Data);
  if(value >= 500)
  {
    getServoTrue(Window_Pos, Window_Servo);
    getServoTrue(Door_Pos, Door_Servo);
    digitalWrite(MQ_Fan, HIGH);
    digitalWrite(MQ_Led, HIGH);
  }
  else
  {
    getServoFalse(Door_Pos, Door_Servo);
    getServoFalse(Window_Pos, Window_Servo);
    digitalWrite(MQ_Fan, LOW);
    digitalWrite(MQ_Led, LOW);
  }
}

void getServoTrue(bool &value, Servo &servo)
{
  value = !value;
  servo.write(0);  
  delay(1000);
}

void getServoFalse(bool &value, Servo &servo)
{
  value = !value;
  servo.write(180);   
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  float t, h, g;
  getDHT(t, h);
  getPIR();
  getMQ(g);
  Serial.println(g);
}
