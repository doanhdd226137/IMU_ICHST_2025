#include <WiFi.h>
#include <esp_now.h>


// Địa chỉ MAC của 4 Slave
uint8_t slaveMacAddress1[] = {0xF8, 0xB3, 0xB7, 0x51, 0xB9, 0x94};
uint8_t slaveMacAddress2[] = {0x08, 0xF9, 0xE0, 0x88, 0x5D, 0x14};
uint8_t slaveMacAddress3[] = {0xF8, 0xB3, 0xB7, 0x52, 0x7B, 0xDC};
uint8_t slaveMacAddress4[] = {0xF8, 0xB3, 0xB7, 0x50, 0xDE, 0xC8};

struct SensorData {
  float yaw;
  float roll;
  float pitch;
  unsigned long pico_timestamp;
  uint8_t sender_id;
};

struct TriggerMessage {
  uint8_t trigger_value;
};


void onDataReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  if (len == sizeof(SensorData)) {
    SensorData *sensorData = (SensorData *)data;
    Serial.printf("%d,%lu,%.2f,%.2f,%.2f\n",
                  sensorData->sender_id, sensorData->pico_timestamp,
                  sensorData->yaw, sensorData->roll, sensorData->pitch);
  } else {
    Serial.println("⚠️ Dữ liệu không hợp lệ.");
  }
}


void addPeer(const uint8_t *macAddr) {
  if (!esp_now_is_peer_exist(macAddr)) {
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, macAddr, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) == ESP_OK) {
      Serial.print("✅ Đã thêm Slave: ");
      for (int i = 0; i < 6; i++) {
        Serial.print(macAddr[i], HEX);
        if (i < 5) Serial.print(":");
      }
      Serial.println();
    } else {
      Serial.println("❌ Không thể thêm Slave.");
    }
  }
}


void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ ESP-NOW khởi tạo thất bại.");
    return;
  }
  esp_now_register_recv_cb(onDataReceive);
  addPeer(slaveMacAddress1);
  addPeer(slaveMacAddress2);
  addPeer(slaveMacAddress3);
  addPeer(slaveMacAddress4);
}


void loop() {
  static unsigned long lastTrigger = 0;
  if (millis() - lastTrigger >= 50) {
    lastTrigger = millis();
    TriggerMessage trigger = {0x01};
    esp_now_send(slaveMacAddress1, (uint8_t *)&trigger, sizeof(trigger));
    esp_now_send(slaveMacAddress2, (uint8_t *)&trigger, sizeof(trigger));
    esp_now_send(slaveMacAddress3, (uint8_t *)&trigger, sizeof(trigger));
    esp_now_send(slaveMacAddress4, (uint8_t *)&trigger, sizeof(trigger));
  }
}
