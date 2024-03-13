#include <ArduinoJson.h>

#include <Key.h>
#include <Keypad.h>

#include <DHT.h>
#include <DHT_U.h>



#define DHT_Out 2
#define Temp_Led 3
#define DHT_Fan 16

DHT dht(DHT_Out, DHT11);

#define PIR_Out 4
#define PIR_Led 5
#define PIR_Button 6
#define PIR_Light 19
int light_value = 0;

#define Door_Servo 14

#define MQ_Out A0
#define Window_Servo 15
#define Fan_Out 18
#define MQ_Led 17

void setup() {
  dht.begin();
  pinMode(DHT_Fan, OUTPUT);
  pinMode(Temp_Led, OUTPUT);
  Serial.begin(9600);

  pinMode(PIR_Out, INPUT_PULLUP);
  pinMode(PIR_Led, OUTPUT);
  pinMode(PIR_Button, INPUT_PULLUP);
  pinMode(PIR_Light, OUTPUT);
}

void getDHT(float &temp, float &hum)
{
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  Serial.println(temp);
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
  if(digitalRead(PIR_Out)==HIGH) {
    digitalWrite(PIR_Led, LOW);
  } else {
    digitalWrite(PIR_Led, HIGH);
  }

  if(digitalRead(PIR_Button)==LOW) {
    delay(20);
    if(digitalRead(PIR_Button)==LOW) {
      //Serial.println("Pressed");
      if(light_value == 0)
      {
        digitalWrite(PIR_Light, HIGH);
        light_value = 1;
      }
      else {
        digitalWrite(PIR_Light, LOW);
        light_value = 0;
      }
    } 
  }
  while(digitalRead(PIR_Button)==LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  float t, h;
  getDHT(t, h);
  getPIR();
}
