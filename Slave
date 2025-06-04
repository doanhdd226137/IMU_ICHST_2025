#include <WiFi.h>
#include <esp_now.h>
#include <deque>  
#define RX_PIN 16
#define TX_PIN 17

uint8_t masterMacAddress[] = { 0x8C, 0x4F, 0x00, 0x3C, 0x78, 0xC8 };

struct TriggerMessage {
  uint8_t trigger_value;
};

struct SensorData {
  float yaw;
  float roll;
  float pitch;
  unsigned long pico_timestamp;
  uint8_t sender_id;
};

std::deque<SensorData> dataQueue;
const size_t MAX_QUEUE_SIZE = 50;

bool triggered = false;

void onDataReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  if (len == sizeof(TriggerMessage)) {
    TriggerMessage *trigger = (TriggerMessage *)data;
    if (trigger->trigger_value == 0x01) {
      Serial.println("✅ Nhận trigger từ master.");
      triggered = true;
    }
  } else {
    Serial.println("⚠️ Trigger không hợp lệ hoặc sai struct.");
  }
}


void sendDataFromQueue() {
  while (!dataQueue.empty()) {
    SensorData sensorData = dataQueue.front();
    esp_err_t result = esp_now_send(masterMacAddress, (uint8_t *)&sensorData, sizeof(sensorData));
    if (result == ESP_OK) {
      Serial.println("📤 Data sent to master:");
      Serial.print("Timestamp: ");
      Serial.print(sensorData.pico_timestamp);
      Serial.print(", Yaw: ");
      Serial.print(sensorData.yaw);
      Serial.print(", Roll: ");
      Serial.print(sensorData.roll);
      Serial.print(", Pitch: ");
      Serial.println(sensorData.pitch);
      dataQueue.pop_front();
    } else {
      Serial.println("❌ Failed to send data to master.");
      break;
    }
  }
}


void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ ESP-NOW init thất bại.");
    return;
  }

  esp_now_register_recv_cb(onDataReceive);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, masterMacAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (!esp_now_is_peer_exist(masterMacAddress)) {
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("❌ Không thể thêm master vào peer list.");
    }
  }
  Serial.println("🚀 ESP32 Slave sẵn sàng.");
}


void loop() {

  if (Serial1.available()) {
    String data = Serial1.readStringUntil('\n');
    Serial.println("🔹 Dữ liệu nhận được từ Pico: " + data);


    float yaw, roll, pitch;
    unsigned long pico_timestamp;
    int n = sscanf(data.c_str(), "%f,%f,%f,%lu", &yaw, &roll, &pitch, &pico_timestamp);


    if (n == 4) {
      SensorData sensorData;
      sensorData.yaw = yaw;
      sensorData.roll = roll;
      sensorData.pitch = pitch;
      sensorData.pico_timestamp = pico_timestamp;  
      
      sensorData.sender_id = 3;  //Thay id slave vao day

      if (dataQueue.size() >= MAX_QUEUE_SIZE) {
        dataQueue.pop_front();
        Serial.println("⚠️ Hàng đợi đầy, xóa bỏ dữ liệu cũ nhất.");
      }
        dataQueue.push_back(sensorData);
    } else {
      Serial.println("⚠️ Dữ liệu không hợp lệ.");
    }
  }


  if (triggered) {
    triggered = false;
    sendDataFromQueue();
  }
}
