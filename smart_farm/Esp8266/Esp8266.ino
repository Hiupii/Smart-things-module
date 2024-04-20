#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

// Khai báo các chân RX và TX cho espsoftwareserial
#define RX_PIN 4  // D2
#define TX_PIN 5  // D1

// Tạo một đối tượng espsoftwareserial với tốc độ baud là 9600
SoftwareSerial mySerial(RX_PIN, TX_PIN);
DynamicJsonDocument doc(1024);

//----Thay đổi thành thông tin của bạn---------------
const char* ssid = "Samsung Măng Cụt";       //Wifi connect
const char* password = "12345678";  //Password

const char* mqtt_server = "182b64cd4fa04f0b91aedae6db156158.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_username = "ltnhung";  //User
const char* mqtt_password = "Ltnhung1";  //Password

WiFiClientSecure espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//------------Connect to MQTT Broker-----------------------------
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientID = "ESP8266Client-";
    clientID += String(random(0xffff), HEX);
    if (client.connect(clientID.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("client/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

//-----------Call back Method for Receiving MQTT massage and Switch LED---------
void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage += (char)payload[i];
  Serial.println("Massage arived [" + String(topic) + "]" + incommingMessage);

  DynamicJsonDocument doc(100);
  DeserializationError error = deserializeJson(doc, incommingMessage);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  serializeJson(doc, mySerial);
  mySerial.println();
  // JsonObject obj = doc.as<JsonObject>();
  // if (obj.containsKey("out1")) {
  //   boolean p = obj["out1"];
  //   digitalWrite(out[0], p);
  //   Serial.println("out1: " + String(p));
  // }
  // if (obj.containsKey("out2")) {
  //   boolean p = obj["out2"];
  //   digitalWrite(out[1], p);
  //   Serial.println("out2: " + String(p));
  // }
  // if (obj.containsKey("out3")) {
  //   int p = obj["out3"];
  //   analogWrite(out[2], p);
  //   Serial.println("out3: " + String(p));
  // }
}

//-----Method for Publishing MQTT Messages---------
void publishMessage(const char* topic, String payload, boolean retained) {
  if (client.publish(topic, payload.c_str(), true))
    Serial.println("Message published [" + String(topic) + "]: " + payload);
}

void setup() {
  // Khởi tạo cổng nối tiếp phần cứng với tốc độ baud là 115200
  Serial.begin(115200);

  // Khởi tạo cổng nối tiếp phần mềm với tốc độ baud là 9600
  mySerial.begin(9600);

  // In ra thông báo khởi động
  Serial.println("ESP8266 UART Communication Example");

  setup_wifi();
  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

unsigned long timeUpdata = millis();

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  //read DHT11

  if (mySerial.available()) {
    String data = mySerial.readStringUntil('\n');
    DeserializationError error = deserializeJson(doc, data);
    char mqtt_message[128];

    serializeJson(doc, Serial);
    serializeJson(doc, mqtt_message);
    publishMessage("sensor", mqtt_message, true);

  }
}
