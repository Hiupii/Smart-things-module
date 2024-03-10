#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include<stdio.h>

#include <Servo.h>

#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

const byte ROWS = 4; //4 hàng
const byte COLS = 3; //3 cột

String arr[8] = {};
String password[8] = {"1", "2", "3", "4", "5", "6", "7", "8"};

int place = 0;
int input = 6;

bool value = true;

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void servo_func();

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

LiquidCrystal_I2C lcd(0x27,16,2);  // Khai báo địa chỉ của LCD gồm 16 cột, 2 dòng

byte rowPins[ROWS] = {7, 2, 3, 5}; //nối hàng của keypad
byte colPins[COLS] = {6, 8, 4};    //nối cột keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  lcd.init();// Khởi động màn hình lCD                 
  lcd.backlight();// Bật đèn nền LCD
  lcd.setCursor(0,0); //Đưa con trỏ vị trí tới hàng 1 cột 1, nếu là (1,0) thì 
  lcd.print("pass: ");
  lcd.setCursor(0,1);
  lcd.print("status: ");
  Serial.begin(9600); //khởi tạo serial
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  myservo.attach(1);  // attaches the servo on pin 9 to the servo object
}

void keypad_func(){
    //Kiểm tra nút được nhấn
    char key = keypad.getKey();

    //in nút ra màn hình
    if (key != NO_KEY){
      lcd.setCursor(input,0);
      lcd.print(key);
      // Serial.println(key);
      arr[place] = key;
      place++;
      input++;
    }

    if(place == 8)
    {
      for(int i = 0;i<8;i++)
      {
        if(arr[i] != password[i])
        {
          value = false;
          break;
        }
      }
      if(value == true)
      {
        lcd.setCursor(0,1);
        lcd.print("    Welcome!    ");
        servo_func();
      }
      else
      {
        place = 0;
        value = true;
        lcd.setCursor(8,1);
        lcd.print("Wrong!");
        input = 6;
        lcd.setCursor(input,0);
        lcd.print("        ");
        delay(1000);
        lcd.setCursor(8,1);
        lcd.print("      ");
      }
    }
}

void servo_func() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void rfid_func() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
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
    Serial.println("Authorized access");
    lcd.setCursor(0,1);
    lcd.print("    Welcome!    ");
    servo_func();
    Serial.println();
    delay(3000);
  }
 
 else   {
    Serial.println(" Access denied");
    delay(3000);
  }
} 


void loop()
{
  keypad_func();
  rfid_func();
  // servo_func();
}
