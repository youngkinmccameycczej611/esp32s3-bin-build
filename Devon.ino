#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>

/* ========== WiFi 配置 ========== */
const char* ssid     = "iQOOZ9Turbo+";
const char* password = "http://huidev.cn";

/* ========== OTA 固件地址 ========== */
const char* firmware_url = "http://huidev.cn/hota.bin";

/* ========== 状态 LED ========== */
#define LED_PIN 48
#define LED_BRIGHT 20

void ledOn(uint8_t v) {
  ledcWrite(0, v);
}

void ledOff() {
  ledcWrite(0, 0);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n[ESP32-S3 OTA Boot]");

  ledcSetup(0, 5000, 8);
  ledcAttachPin(LED_PIN, 0);
  ledOn(LED_BRIGHT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");

  uint8_t retry = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    ledOn((retry++ % 2) ? LED_BRIGHT : 0);

    if (retry > 30) {
      Serial.println("\nWiFi Failed");
      ledOn(255);
      return;
    }
  }

  Serial.println("\nWiFi OK");
  ledOn(LED_BRIGHT);

  Serial.println("Start OTA...");
  WiFiClient client;
  httpUpdate.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

  t_httpUpdate_return ret =
    httpUpdate.update(client, firmware_url);

  if (ret == HTTP_UPDATE_OK) {
    Serial.println("OTA Success");
    delay(1000);
    ESP.restart();
  } else {
    Serial.printf("OTA Failed (%d): %s\n",
      httpUpdate.getLastError(),
      httpUpdate.getLastErrorString().c_str()
    );
    ledOn(255);
  }
}

void loop() {
  delay(10000);
}
