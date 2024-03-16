#include <Servo.h>

#define BUTTON_PIN 6  // Chân GPIO được kết nối với nút nhấn
#define SERVO_PIN 13  // Chân GPIO được kết nối với servo motor

Servo servo;

int buttonState = 0;
int angle = 0;
bool clockwise = true;

void setup() {
  servo.attach(SERVO_PIN);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // Kiểm tra trạng thái của nút nhấn
  if (digitalRead(BUTTON_PIN) == LOW) {
    // Đảo chiều quay của servo
    // Thực hiện xoay servo
    if (clockwise) {
      clockwise = !clockwise;
      servo.write(0);  // Quay theo chiều kim đồng hồ
    } else {
      clockwise = !clockwise;
      servo.write(180);  // Quay ngược chiều kim đồng hồ
    }
    // Chờ servo hoàn thành quay
    delay(1000);
  }

  delay(50);  // Delay để giảm độ nhạy của nút nhấn
}
