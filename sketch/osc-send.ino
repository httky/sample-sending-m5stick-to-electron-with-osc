#include <M5StickCPlus.h>
#include <WiFi.h>
#include <OSCMessage.h>
#include <WiFiUdp.h>

const char *ssid = "ssid"; // wifiのssid
const char *password = "password"; // wifiのパスワード
IPAddress destinationIP(0, 0, 0, 0); // 送信先のIPアドレス
const unsigned int destinationPort = 10000; // 送信先のポート
float gyroX, gyroY, gyroZ;

WiFiUDP Udp;

void sendOSC(const char *route, int message) {
  OSCMessage msg(route);
  msg.add(message); // 任意のデータを追加
  Udp.beginPacket(destinationIP, destinationPort);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
}

void sendGyroOSC(const char *route, float x, float y, float z) {
  OSCMessage msg(route);
  msg.add(x);
  msg.add(y);
  msg.add(z);
  Udp.beginPacket(destinationIP, destinationPort);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
}


void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.setRotation(1);
  int x = M5.IMU.Init(); //return 0 is ok, return -1 is unknow
  Serial.println(x);

  M5.Lcd.fillScreen(BLACK); // 画面を黒でクリアする

  // M5.Lcd.setCursor(0, 0); // テキストの表示位置を設定する
  // M5.Lcd.setTextColor(WHITE); // テキストの色を白に設定する
  // M5.Lcd.setTextSize(2); // テキストのサイズを2倍に設定する
  // M5.Lcd.println("Hello, M5Stick!"); // テキストを表示する

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi...");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  // M5.Lcd.println("Hello, M5Stick!!!"); // テキストを表示する

  Udp.begin(WiFi.localIP(), destinationPort);
}

void loop() {
  // ボタンイベントを反映したい場合はupdateが必要
  M5.update();

  M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
  // M5.Lcd.setCursor(0, 30);
  // M5.Lcd.printf("X:%7.2f/nY:%7.2f/nZ:%7.2f ", gyroX, gyroY, gyroZ);

  if (M5.BtnA.isPressed()) {
    sendOSC("/m5stick/pressed", 1);
  } else {
    sendOSC("/m5stick/pressed", 0);
  }

  sendGyroOSC("/m5stick/gyro", gyroX, gyroY, gyroZ);

  delay(200);
}
