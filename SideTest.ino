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
unsigned long counter = 0;
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
bool CANDONE;

bool SCREEN2DONE=false;
bool SCREEN1=false;
bool IsScreenTaken=false;
bool SCREEN1DONE=false;
unsigned long screen1;
unsigned long delayStart;

unsigned long screen2;
bool SCREEN2;


unsigned long screen3;
bool SCREEN3;
bool SCREEN3DONE=false;

bool SCREEN4; 
bool SCREEN4DONE=false;
unsigned long screen4;

bool SCREEN5;
bool SCREEN5DONE=false;
unsigned long screen5;

bool SCREEN6;
bool SCREEN6DONE=false;
unsigned long screen6;

bool SCREEN7;
bool SCREEN7DONE=false;
unsigned long screen7;

bool SCREEN8;
bool SCREEN8DONE=false;
unsigned long screen8;

bool SCREEN9;
bool SCREEN9DONE=false;
unsigned long screen9;

bool SCREEN10;
bool SCREEN10DONE=false;
unsigned long screen10;

bool SCREEN11;
bool SCREEN11DONE=false;
unsigned long screen11;

bool SCREEN12;
bool SCREEN12DONE=false;
unsigned long screen12;

bool SCREEN13;
bool SCREEN13DONE=false;
unsigned long screen13;

bool SCREEN14;
bool SCREEN14DONE=false;
unsigned long screen14;

bool SCREEN15;
bool SCREEN15DONE=false;
unsigned long screen15;

bool SCREEN16;
bool SCREEN16DONE=false;
unsigned long screen16;


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


  delayStart=millis();

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
//Display screen once if all is ok if not all is ok reset screen
    if (AOK && DOK) {
      if(!IsScreenTaken && !SCREEN1 &&  !SCREEN1DONE){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ANALOG OK");
      lcd.setCursor(0, 1);
      lcd.print("DIGITAL OK");
      IsScreenTaken=true;
      screen1=millis();
      SCREEN1=true;
      }else if(millis()>=screen1+3000 && SCREEN1 && IsScreenTaken){
        lcd.clear();
        SCREEN1=false;
        IsScreenTaken=false;
        SCREEN1DONE=true;
      }
      //delay(2000);
     // can = true;
      if(!IsScreenTaken && !SCREEN2 && !SCREEN2DONE){
      lcd.clear();
      lcd.print("STARTING CAN");
      SCREEN2=true;
      IsScreenTaken=true;
      screen2=millis();
      }else if(millis()>=screen2+2000 && IsScreenTaken && SCREEN2){
        SCREEN2=false;
        IsScreenTaken=false;
        SCREEN2DONE=true;
        can=true;
        lcd.clear();



      }
      //delay(2000);
    }else{
      //lcd.clear();
      lcd.setCursor(0, 0);
      if (AOK == false) {
        if(!IsScreenTaken && !SCREEN3 && !SCREEN3DONE){
          lcd.clear();
        lcd.print("ANALOG NOT OK");
        screen3=millis();
        SCREEN3=true;
        IsScreenTaken=true;
        //delay(2000);
       // delay(500);
        }else if(millis()>=screen3+2000 && IsScreenTaken && SCREEN3){
            SCREEN3=false;
            IsScreenTaken=false;
            SCREEN3DONE=true;
            lcd.clear();


        }

        
        
        if (OK[0]) {
          if(!IsScreenTaken && !SCREEN4 && !SCREEN4DONE){  
          lcd.print("A0 ");    
          lcd.print(" OK");
          SCREEN4=true;
          IsScreenTaken=true;
          screen4=millis();
          
          //delay(1000);
          //delay(1000);
          }else if (millis()>=screen4+2000 && IsScreenTaken && SCREEN4){
            SCREEN4=false;
            IsScreenTaken=false;
            SCREEN4DONE=true;
            lcd.clear();
            

          }
        } else {

          if(!IsScreenTaken && !SCREEN5 && !SCREEN5DONE){
          SCREEN5=true;
          IsScreenTaken=true;
          lcd.print("A0 ");  
          lcd.print(" NOT OK");
          screen5=millis();
          lcd.setCursor(0, 1);
          lcd.print("A0 Value: ");
          lcd.setCursor(11, 1);
          lcd.print(voltage[0]);
          }else if (millis()>=screen5+2000 && IsScreenTaken && SCREEN5){
            SCREEN5=false;
            IsScreenTaken=false;
            SCREEN5DONE=true;
            lcd.clear();  

          }
      
        }
       // lcd.clear();
      
        if (OK[1]) {

          
          if(!IsScreenTaken && !SCREEN6 && !SCREEN6DONE){
            SCREEN6=true;
            IsScreenTaken=true;
            lcd.clear();
             screen6=millis();
             lcd.setCursor(0, 0); 
            lcd.print("A1 ");
          lcd.print(" OK");
         
          }else if(millis()>=screen6+2000 && IsScreenTaken && SCREEN6){
            SCREEN6=false;
            IsScreenTaken=false;
            SCREEN6DONE=true;
            lcd.clear();

          }
        } else {
          if(!IsScreenTaken && !SCREEN7 && !SCREEN7DONE){
            SCREEN7=true;
            IsScreenTaken=true;
            lcd.clear();
            screen7=millis();
            lcd.print("A1 ");
          lcd.print(" NOT OK");
          lcd.setCursor(0, 1);
         lcd.print("A1 Value: ");
         lcd.setCursor(11, 1);
          lcd.print(voltage[0]);
          
          
          }else if(millis()>=screen7+2000 && IsScreenTaken && SCREEN7){
            SCREEN7=false;
            IsScreenTaken=false;
            SCREEN7DONE=true;  
            lcd.clear();

          }
          
          
          
        }
        
        if (OK[2]) {
          if(!IsScreenTaken && !SCREEN8 && !SCREEN8DONE){
            SCREEN8=true;
            IsScreenTaken=true;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("A2 ");
          lcd.print(" OK");
          screen8=millis();
          
          }else if(millis()>=screen8+2000 && IsScreenTaken && SCREEN8){
            SCREEN8=false;
            IsScreenTaken=false;
            SCREEN8DONE=true;
            lcd.clear();

          }
        } else {
          if(!IsScreenTaken && !SCREEN9 && !SCREEN9DONE){
            screen9=millis();
            SCREEN9=true;
            IsScreenTaken=true;
            lcd.setCursor(0, 0);
            lcd.print("A2 ");
          lcd.print(" NOT OK");
          lcd.setCursor(0, 1);
          lcd.print("A2 Value: ");
          lcd.setCursor(11, 1);
          lcd.print(voltage[0]);
          
          }else if(millis()>=screen9+2000 && IsScreenTaken && SCREEN9){
            SCREEN9=false;
            IsScreenTaken=false;
            SCREEN9DONE=true;
            lcd.clear();

          }
          
        }
       
     
        
        if (OK[3]) {
          if(!IsScreenTaken && !SCREEN10 && !SCREEN10DONE){
            lcd.clear();
            screen10=millis();
            SCREEN10=true;
            IsScreenTaken=true;
            lcd.setCursor(0, 0);
            lcd.print("A3 ");
            lcd.print(" OK");
            
          }else if (millis()>=screen10+2000 && IsScreenTaken && SCREEN10){
            SCREEN10=false;
            IsScreenTaken=false;
            SCREEN10DONE=true;
            lcd.clear();

          }
        } else {
          if(!IsScreenTaken && !SCREEN11 && !SCREEN11DONE){
            lcd.clear();
            screen11=millis();
            SCREEN11=true;
            IsScreenTaken=true;
          lcd.setCursor(0, 0);
          lcd.print("A3 ");
          lcd.print(" NOT OK");
          lcd.setCursor(0, 1);
          lcd.print("A3 Value: ");
          lcd.setCursor(11, 1);
          lcd.print(voltage[0]);
          }else if(millis()>=screen11+2000 && IsScreenTaken && SCREEN11){
            SCREEN11=false;
            IsScreenTaken=false;
            SCREEN11DONE=true;
            lcd.clear();

          }

          
        }
        

      } else {
       
      }
      if (!DOK) {
        if(!IsScreenTaken && !SCREEN12 && !SCREEN12DONE){
        lcd.clear();
        lcd.print("DIGITAL NOT OK");
        screen12=millis();
        SCREEN12=true;
        IsScreenTaken=true;
        
        
        }else if(millis()>=screen12+2000 && IsScreenTaken && SCREEN12){
          SCREEN12=false;
          IsScreenTaken=false;
          SCREEN12DONE=true;
          lcd.clear();

        }
       
        if (OK1[0]) {
          if(!IsScreenTaken && !SCREEN13 && !SCREEN13DONE){
            lcd.clear();
            screen13=millis();
            SCREEN13=true;
            IsScreenTaken=true;
            lcd.print("D0: ");
            lcd.setCursor(5, 0);
          lcd.print(" OK");

          }else if(millis()>=screen13+2000 && IsScreenTaken && SCREEN13){
            SCREEN13=false;
            IsScreenTaken=false;
            SCREEN13DONE=true;
            lcd.clear();


          }
        } else {
          if(!IsScreenTaken && !SCREEN14 && !SCREEN14DONE){
            lcd.clear();
            screen14=millis();
            SCREEN14=true;
            IsScreenTaken=true;
               lcd.print("D0: ");
            lcd.setCursor(5, 0);
          lcd.print(" NOT OK");
           lcd.setCursor(0, 1);
           lcd.print("VALUE: LOW");
        
          }else if(millis()>=screen14+2000 && IsScreenTaken && SCREEN14){
            SCREEN14=false;
            IsScreenTaken=false;
            SCREEN14DONE=true;
            lcd.clear();

          }
          
        }
        
        if (OK1[1]) {
          if(!IsScreenTaken && !SCREEN15 && !SCREEN15DONE){
            screen15=millis();
            SCREEN15=true;
            IsScreenTaken=true;
            lcd.clear();
        lcd.print("D1 ");
        lcd.setCursor(3, 0);
          lcd.print(" OK");
         
          }else if(millis()>=screen15+2000 && IsScreenTaken && SCREEN15){
            SCREEN15=false;
            IsScreenTaken=false;
            SCREEN15DONE=true;
            lcd.clear();

          }
        } else {
          if(!IsScreenTaken && !SCREEN16 && !SCREEN16DONE){
            screen16=millis();
            SCREEN16=true;
            IsScreenTaken=true;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("D1: ");
            lcd.setCursor(5, 0);
          lcd.print(" NOT OK");
         lcd.setCursor(0, 1);
          lcd.print("D1 Value: LOW");
          
          }else if(millis()>=screen16+2000 && IsScreenTaken && SCREEN16){
            SCREEN16=false;
            IsScreenTaken=false;
            SCREEN16DONE=true;
            lcd.clear();

          }
         
        }
      } else {
       // lcd.clear();
        //delay(500);
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


    if (!AOK || !DOK) {
      Serial.println(CANDONE);
      if(CANDONE==false){
        CANDONE=true;
        SCREEN3DONE=false;
        SCREEN4DONE=false;
        SCREEN5DONE=false;
        SCREEN6DONE=false;
        SCREEN7DONE=false;
        SCREEN8DONE=false;
        SCREEN9DONE=false;
        SCREEN10DONE=false;
        SCREEN11DONE=false;
        SCREEN12DONE=false;
        SCREEN13DONE=false;
        SCREEN14DONE=false;
        SCREEN15DONE=false;
        SCREEN16DONE=false;
        IsScreenTaken=false;
      }
      if(IsScreenTaken==false){
      
      SCREEN3DONE=false;
      SCREEN4DONE=false;
      SCREEN5DONE=false;
      SCREEN6DONE=false;
      SCREEN7DONE=false;
      SCREEN8DONE=false;
      SCREEN9DONE=false;
      SCREEN10DONE=false;
      SCREEN11DONE=false;
      SCREEN12DONE=false;
      SCREEN13DONE=false;
      SCREEN14DONE=false;
      SCREEN15DONE=false;
      SCREEN16DONE=false;
      IsScreenTaken=false;

      }
      
      can = false;
     // lcd.clear();
    }else {
      if(CANDONE==true){
        CANDONE=false;

      }
      CANDONE=false;
      if(!can){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ALL OKAY");
      lcd.setCursor(0, 1);
      lcd.print("STARTING CAN");
      delay(2500);
      can = true;}
      }
  }



//Serial.println("LOOP OVER");

}  // loop over