#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <mcp2515.h>

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args) write(args);
#else
#define printByte(args) print(args, BYTE);
#endif
//Define pins
#define APIN_4V0 A0
#define APIN_33V0 A1
#define APIN_5V0 A2
#define APIN_12V0 A3
#define DPIN_33V0 7
#define DPIN_4V0 6
#define LED_NFC 5
#define LED_ZERO 3

LiquidCrystal_I2C lcd(0x27, 20, 4);
struct can_frame canMsg;
MCP2515 mcp2515(10);
uint8_t heart[8] = { 0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0 };
float Bvoltage[4] = { 4.63, 3.35, 2.52, 1.11 };
int pinSwitch = 0;
uint32_t count = 0;
int sensorValue[4];
bool can = false;
int counter = 0;
bool AOK = false;
bool DOK = false;
bool firstClear = false;
uint16_t T1;
uint16_t T2;
uint16_t HAL;
bool ZERO;
float voltage[4];
bool OK[4];
bool OK1[2];
const float napaka = 0.2;
char tipka = 'X';
int pinVal = 0;
bool flag = false;
bool NFC;

bool checkPIN(int pin) {
  if (digitalRead(pin) == HIGH) return true;
  else return false;
}



void setup() {
  //PIN SETUP
  pinMode(DPIN_33V0, INPUT);
  pinMode(DPIN_4V0, INPUT);
  pinMode(LED_NFC, OUTPUT);
  pinMode(LED_ZERO, OUTPUT);
  pinMode(pinSwitch, INPUT);
  Serial.begin(115200);
  //LCD INIT
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.command(0xA6);
  //CAN SETUP
  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
}

void loop() {

  counter++;
  if (!can) {
    if (checkPIN(DPIN_33V0)) {
      OK1[0] = true;
      DOK = true;
    } else {
      DOK = false;
    }
    if (checkPIN(DPIN_4V0)) {
      OK1[1] = true;
      DOK = true;
    } else {
      DOK = false;
    }
    sensorValue[0] = analogRead(APIN_4V0);
    sensorValue[1] = analogRead(APIN_33V0);
    sensorValue[2] = analogRead(APIN_5V0);
    sensorValue[3] = analogRead(APIN_12V0);
    for (int x = 0; x < 4; x++) {
      voltage[x] = sensorValue[x] * (5.0 / 1023.0);
    }
    if (voltage[0] < Bvoltage[0] + napaka && voltage[0] > Bvoltage[0] - napaka) {
      OK[0] = true;
      AOK = true;
    } else AOK = false;
    if (voltage[1] < Bvoltage[1] + napaka && voltage[1] > Bvoltage[1] - napaka) {
      OK[1] = true;
      AOK = true;
    } else AOK = false;
    if (voltage[2] < Bvoltage[2] + napaka && voltage[2] > Bvoltage[2] - napaka) {
      OK[2] = true;
      AOK = true;
    } else AOK = false;
    if (voltage[3] < Bvoltage[3] + napaka && voltage[3] > Bvoltage[3] - napaka) {
      OK[3] = true;
      AOK = true;
    } else AOK = false;

    if (AOK && DOK) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ANALOG OK");
      lcd.setCursor(0, 1);
      lcd.print("DIGITAL OK");
      delay(2000);
      can = true;
      lcd.clear();
      lcd.print("STARTING CAN");
      delay(2000);
    }else{
      lcd.clear();
      lcd.setCursor(0, 0);
      if (AOK == false) {
        lcd.print("ANALOG NOT OK");
        delay(2000);
        delay(500);
        lcd.print("A0 ");
        if (OK[0]) {
          lcd.print(" OK");
          delay(1000);
          delay(1000);
        } else {
          lcd.print(" NOT OK");
          delay(1000);
          delay(1000);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("A0 Value: ");
          lcd.setCursor(11, 0);
          lcd.print(voltage[0]);
          delay(1000);
          delay(1000);
        }
        lcd.clear();
        lcd.print("A1 ");
        if (OK[1]) {
          lcd.print(" OK");
          delay(1000);
        } else {
          lcd.print(" NOT OK");
          delay(1000);
          delay(200);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("A1 Value: ");
          lcd.setCursor(11, 0);
          lcd.print(voltage[0]);
          delay(1000);
        }
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("A2 ");
        if (OK[2]) {
          lcd.print(" OK");
          delay(1000);
        } else {
          lcd.print(" NOT OK");
          delay(1000);
          delay(1000);
          delay(200);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("A2 Value: ");
          lcd.setCursor(11, 0);
          lcd.print(voltage[0]);
          delay(1000);
          delay(1000);
        }
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("A3 ");
        if (OK[3]) {
          lcd.print(" OK");
          delay(1000);
        } else {
          lcd.print(" NOT OK");
          delay(1000);
          delay(1000);
          delay(200);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("A3 Value: ");
          lcd.setCursor(11, 0);
          lcd.print(voltage[0]);
          delay(1000);
          delay(1000);
        }
        delay(200);

      } else {
        delay(400);
        delay(1000);
      }
      if (!DOK) {
        lcd.clear();
        lcd.print("DIGITAL NOT OK");
        delay(2000);
        lcd.clear();
        lcd.print("D0 ");
        lcd.setCursor(3, 0);
        if (OK1[0]) {
          lcd.print(" OK");
          delay(1000);
          delay(1000);
        } else {
          lcd.print(" NOT OK");
          delay(1000);
          delay(1000);
          delay(200);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("D0 Value: ");
          lcd.setCursor(11, 0);
          lcd.print("LOW");
          delay(1000);
        }
        lcd.clear();
        lcd.print("D1 ");
        lcd.setCursor(3, 0);
        if (OK1[1]) {
          lcd.print(" OK");
          delay(2000);
        } else {
          lcd.print(" NOT OK");
          delay(200);
          delay(1500);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("D1 Value: ");
          lcd.setCursor(11, 0);
          lcd.print("LOW");
          delay(1000);
          delay(1000);
        }
      } else {
        lcd.clear();
        delay(500);
      }
    }
  } else {  //starting CAN read
    if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
      Serial.print(canMsg.can_id, HEX);  // print ID
      Serial.print(" ");
      Serial.print(canMsg.can_dlc, HEX);  // print DLC
      Serial.print(" ");
      for (int i = 0; i < canMsg.can_dlc; i++) {  // print the data
        Serial.print(canMsg.data[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
      if (canMsg.can_id == 0x13) {
        T1 = (canMsg.data[1] << 8) | canMsg.data[0];
        T2 = (canMsg.data[3] << 8) | canMsg.data[2];
      }
      if (canMsg.can_id == 0x11) {
        HAL = (canMsg.data[1] << 8) | canMsg.data[0];
        if (canMsg.data[2] == 0x01) {
          ZERO = true;
        } else ZERO = false;
      }
      if (canMsg.can_id == 0x12) {
        if (canMsg.data[2] == 0x01) {
          tipka = '4';
        } else if (canMsg.data[3] = 0x01) {
          tipka = '3';
        }
      }
      if (canMsg.can_id == 0x14) {
        digitalWrite(LED_NFC, HIGH);
        delay(600);
        digitalWrite(LED_NFC, LOW);
      }
    }

    if (firstClear == false) {
      lcd.clear();
      firstClear = true;
    }
    if (counter % 500 == 0) lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TIPKA:");
    lcd.setCursor(6, 0);
    lcd.print(tipka);
    lcd.setCursor(9, 0);
    lcd.print("T1:");
    lcd.print(T1);
    lcd.setCursor(0, 1);
    lcd.print("HAL:");
    lcd.setCursor(4, 1);
    if (counter % 2 == 0) lcd.print(HAL);
    lcd.setCursor(9, 1);
    lcd.print("T2:");
    lcd.print(T2);
    if (ZERO) {
      digitalWrite(LED_ZERO, HIGH);
    } else digitalWrite(LED_ZERO, LOW);
  }

  if (counter % 100 == 0) {
    if (digitalRead(DPIN_33V0) == HIGH) {
      OK1[0] = true;
      //DOK = true;
    } else {
      DOK = false;
    }
    if (digitalRead(DPIN_4V0) == HIGH) {
      OK1[1] = true;
      //DOK = true;
    } else {
      DOK = false;
    }

    sensorValue[0] = analogRead(APIN_4V0);
    sensorValue[1] = analogRead(APIN_33V0);
    sensorValue[2] = analogRead(APIN_5V0);
    sensorValue[3] = analogRead(APIN_12V0);
    for (int x = 0; x < 4; x++) {

      voltage[x] = sensorValue[x] * (5.0 / 1023.0);
    }

    if (voltage[0] < Bvoltage[0] + napaka && voltage[0] > Bvoltage[0] - napaka) {
      OK[0] = true;
      //AOK = true;
    } else AOK = false;
    if (voltage[1] < Bvoltage[1] + napaka && voltage[1] > Bvoltage[1] - napaka) {
      OK[1] = true;
      //AOK = true;
    } else AOK = false;
    if (voltage[2] < Bvoltage[2] + napaka && voltage[2] > Bvoltage[2] - napaka) {
      OK[2] = true;
      //AOK = true;


    } else AOK = false;
    if (voltage[3] < Bvoltage[3] + napaka && voltage[3] > Bvoltage[3] - napaka) {
      OK[3] = true;
     // AOK = true;
    } else AOK = false;


    if (!AOK || !DOK) {
      can = false;
    }
  }





}  // loop over