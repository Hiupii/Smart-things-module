#include <Servo.h>

#define BUTTON_PIN 6  
#define SERVO_PIN 13  

Servo myservo;

int buttonState = 0;
int angle = 0;
bool clockwise = true;

void setup() 
{
  myservo.attach(SERVO_PIN);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() 
{
  buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == LOW) 
  {
    getServo(clockwise, myservo);
  }
}

void getServo(bool &value, Servo &servo)
{
  if (value) 
  {
    value = !value;
    servo.write(0);
  } else 
  {
    value = !value;
    servo.write(180);
  } 
  delay(1000);
}