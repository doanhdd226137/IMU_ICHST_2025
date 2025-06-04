#include <AccelStepper.h>

// Motor 1
#define STEP_PIN1 4
#define DIR_PIN1 5
#define ENA_PIN1 6  // Enable motor 1

// Motor 2
#define STEP_PIN2 10
#define DIR_PIN2 9
#define ENA_PIN2 8  // Enable motor 2

// Motor 3
#define STEP_PIN3 11
#define DIR_PIN3 12
#define ENA_PIN3 13 // Enable motor 3

// Bước và tính toán góc
const int stepsPerRev = 200;       // 1.8° stepper motor
const int microstep = 8;           // nếu dùng vi bước 1/8
const float degreesPerStep = 360.0 / (stepsPerRev * microstep);
const int stepsFor90Deg = (int)(90.0 / degreesPerStep); // Số bước để quay 90°

AccelStepper motor1(AccelStepper::DRIVER, STEP_PIN1, DIR_PIN1);
AccelStepper motor2(AccelStepper::DRIVER, STEP_PIN2, DIR_PIN2);
AccelStepper motor3(AccelStepper::DRIVER, STEP_PIN3, DIR_PIN3);

// Trạng thái chuyển động
bool motor1GoingTo90 = true;
bool motor2GoingToMinus90 = true;
bool motor3GoingTo90 = true;

void setup() {
  Serial.begin(115200);

  // Chân ENA
  pinMode(ENA_PIN1, OUTPUT);
  pinMode(ENA_PIN2, OUTPUT);
  pinMode(ENA_PIN3, OUTPUT);

  // Bật motor
  digitalWrite(ENA_PIN1, LOW);  // LOW = bật với TP6600
  digitalWrite(ENA_PIN2, LOW);
  digitalWrite(ENA_PIN3, LOW);

  // Đảo chiều quay cho cả 3 động cơ nếu cần
  motor1.setPinsInverted(true, false, false);
  motor2.setPinsInverted(true, false, false);
  motor3.setPinsInverted(true, false, false);

  // Tốc độ & gia tốc
  motor1.setMaxSpeed(4000);
  motor1.setAcceleration(3000);

  motor2.setMaxSpeed(4000);
  motor2.setAcceleration(3000);

  motor3.setMaxSpeed(4000);
  motor3.setAcceleration(3000);

  // Motor 1 quay tới 90°, Motor 2 quay tới -90°, Motor 3 quay tới 90°
  motor1.moveTo(stepsFor90Deg);
  motor2.moveTo(stepsFor90Deg);
  motor3.moveTo(stepsFor90Deg);
}

void loop() {
  motor1.run();
  motor2.run();
  motor3.run();

  // Khi cả ba động cơ đã tới vị trí đích
  if (motor1.distanceToGo() == 0 && motor2.distanceToGo() == 0 && motor3.distanceToGo() == 0) {
    delay(1000);  // Chờ 1 giây giữa các pha

    if (motor1GoingTo90) {
      // Quay tất cả động cơ về 0°
      motor1.moveTo(0);
      motor2.moveTo(0);
      motor3.moveTo(0);
      Serial.println("→ Motor 1, 2, 3 quay về 0 độ");
    } else {
      // Quay Motor 1 tới 90°, Motor 2 tới -90°, Motor 3 tới 90°
      motor1.moveTo(stepsFor90Deg);
      motor2.moveTo(stepsFor90Deg);
      motor3.moveTo(stepsFor90Deg);
      Serial.println("→ Motor 1 tới 90°, Motor 2 tới -90°, Motor 3 tới 90°");
    }

    // Đổi trạng thái quay
    motor1GoingTo90 = !motor1GoingTo90;
    motor2GoingToMinus90 = !motor2GoingToMinus90;
    motor3GoingTo90 = !motor3GoingTo90;
  }
}
ENCODER
#include <Arduino.h>
#include <util/atomic.h>


// Encoder 3 (CPR = 4 * 400 = 1600)
#define ENC3_A 2
#define ENC3_B 3
volatile long count3 = 0;
float cpr3 = 4.0 * 400;
volatile bool prevA3 = LOW;
volatile bool prevB3 = LOW;


// Encoder 4 (CPR = 4 * 600 = 2400)
#define ENC4_A 18
#define ENC4_B 19
volatile long count4 = 0;
float cpr4 = 4.0 * 600;
volatile bool prevA4 = LOW;
volatile bool prevB4 = LOW;


// Encoder 2 (CPR = 4 * 400 = 1600)
#define ENC2_A 20
#define ENC2_B 21
volatile long count2 = 0;
float cpr2 = 4.0 * 600;
volatile bool prevA2 = LOW;
volatile bool prevB2 = LOW;


float qFb2 = 0;
float qFb3 = 0;
float qFb4 = 0;


float prevT = 0;
float currT = 0;


// ==== Encoder 3 ISR ====
void readEncoder3() {
  int inc = 0;
  bool currA = digitalRead(ENC3_A);
  bool currB = digitalRead(ENC3_B);


  if (prevA3 == LOW && currA == HIGH) inc = (currB == LOW) ? -1 : 1;
  if (prevA3 == HIGH && currA == LOW) inc = (currB == LOW) ? 1 : -1;
  if (prevB3 == LOW && currB == HIGH) inc = (currA == LOW) ? 1 : -1;
  if (prevB3 == HIGH && currB == LOW) inc = (currA == LOW) ? -1 : 1;


  count3 += inc;
  prevA3 = currA;
  prevB3 = currB;
}


// ==== Encoder 4 ISR ====
void readEncoder4() {
  int inc = 0;
  bool currA = digitalRead(ENC4_A);
  bool currB = digitalRead(ENC4_B);


  if (prevA4 == LOW && currA == HIGH) inc = (currB == LOW) ? -1 : 1;
  if (prevA4 == HIGH && currA == LOW) inc = (currB == LOW) ? 1 : -1;
  if (prevB4 == LOW && currB == HIGH) inc = (currA == LOW) ? 1 : -1;
  if (prevB4 == HIGH && currB == LOW) inc = (currA == LOW) ? -1 : 1;


  count4 += inc;
  prevA4 = currA;
  prevB4 = currB;
}


// ==== Encoder 2 ISR ====
void readEncoder2() {
  int inc = 0;
  bool currA = digitalRead(ENC2_A);
  bool currB = digitalRead(ENC2_B);

  
  if (prevA2 == LOW && currA == HIGH) inc = (currB == LOW) ? -1 : 1;
  if (prevA2 == HIGH && currA == LOW) inc = (currB == LOW) ? 1 : -1;
  if (prevB2 == LOW && currB == HIGH) inc = (currA == LOW) ? 1 : -1;
  if (prevB2 == HIGH && currB == LOW) inc = (currA == LOW) ? -1 : 1;


  count2 += inc;
  prevA2 = currA;
  prevB2 = currB;
}

// ==== Tính vị trí từ xung ====
void updatePosition() {
  long countSnapshot2 = 0, countSnapshot3 = 0, countSnapshot4 = 0;

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    countSnapshot2 = count2;
    countSnapshot3 = count3;
    countSnapshot4 = count4;
  }

  qFb2 = countSnapshot2 * 360.0 / cpr2;
  qFb3 = countSnapshot3 * 360.0 / cpr3;
  qFb4 = countSnapshot4 * 360.0 / cpr4;
}

void setup() {
  Serial.begin(115200);

  // Encoder 3
  pinMode(ENC3_A, INPUT_PULLUP);
  pinMode(ENC3_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC3_A), readEncoder3, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC3_B), readEncoder3, CHANGE);

  // Encoder 4
  pinMode(ENC4_A, INPUT_PULLUP);
  pinMode(ENC4_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC4_A), readEncoder4, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC4_B), readEncoder4, CHANGE);

  // Encoder 2
  pinMode(ENC2_A, INPUT_PULLUP);
  pinMode(ENC2_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC2_A), readEncoder2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC2_B), readEncoder2, CHANGE);
}

void loop() {
  currT = millis();
  updatePosition();
  Serial.print("Encoder2: ");
  Serial.print(qFb2, 2);
  Serial.print("° | Encoder3: ");
  Serial.print(qFb3, 2);
  Serial.print("° | Encoder4: ");
  Serial.print(qFb4, 2);
  Serial.print("° | Time: ");
  Serial.println(currT);
  delay(40);
}




