// This is a demo by Jiankai.li

#include "TM1636.h"
#include "pitches.h"

//--Declare a TM1636 Class object that control the 4-digit display--//
TM1636 tm1636(7,8);
#define DEBUG 1

enum status{
  Standby     = 0,           /* Standby Waiting for press button */
  Button1Stop = 1,
  Button2Stop = 2,
  Button3Stop = 3,
  Failed      = 4,           /* play the result                  */
  Success     = 5,
};
typedef enum status SystemStatus;
SystemStatus WorkingStatus;


#define BTN_1   11
#define BTN_2   10
#define BTN_3   9

//-------pin definition of LEDs---------------//
#define LED4  5
#define LED3  4
#define LED2  3
#define LED1  2

#define RELAY      A5
#define FeedBack   A4
#define RELAYOpen  255
#define RELAYClose 0

/* sensorValue = analogRead(A1); */

#define ALARM_BUZZER 6

#define TimeInterval1 500                       // 0.1s
#define TimeInterval2 250                       // 0.1s
#define TimeInterval3 100                       // 0.1s
int8_t displayNum[4]; 
unsigned long StartTime  = 0;   
void setup()
{
  /*Set the default brightness is BRIGHT_TYPICAL and clear the display*/
#if DEBUG
  Serial.begin(9600);
#endif
  init_pins();
  tm1636.init();
//  SuccessMusicPlay(ALARM_BUZZER);
  displayNum[0] = 0;
  displayNum[1] = 0;
  displayNum[2] = 0;
  displayNum[3] = INDEX_BLANK;
  tm1636.display(displayNum);
  // disp[0] = 1;
  // disp[1] = 1;
  // disp[2] = 1;  
  // tm1636.display(disp);
  // Serial.println("Start count");
  // delay(5000);
   // Serial.println("End count");
  StartTime = millis();
}
void loop()
{
  switch(WorkingStatus) {
  case Standby:                           
    if(WaitingButton1() == true) {
      
      WorkingStatus = Button1Stop;
      StartTime = millis();
    } else {
      if( (millis() - StartTime) > TimeInterval1) {
        for(uint8_t i = 0;i<3 ; i++) {
          if (++displayNum[i] == 10) {
            displayNum[i] = 0;
          } else {
          }
        }
        tm1636.display(displayNum);
        StartTime = millis();
      } else {
      }
      tm1636.display(displayNum);  
    }
    
    break;
  case Button1Stop:
    if(WaitingButton2() == true) {
      WorkingStatus = Button2Stop;
      StartTime = millis();
    } else {
      if( (millis() - StartTime) > TimeInterval2) {
        for(uint8_t i = 1;i<3 ; i++) {
          if (++displayNum[i] == 10) {
            displayNum[i] = 0;
          } else {
          }
        }
        tm1636.display(displayNum);
        StartTime = millis();
      } else {
        
      }
      tm1636.display(displayNum);  
    }
    break;
  case Button2Stop:
  
    if(WaitingButton3() == true) {
      
      WorkingStatus = Button3Stop;
      StartTime = millis();
    } else {
      if( (millis() - StartTime) > TimeInterval3) {
        for(uint8_t i = 2;i<3 ; i++) {
          if (++displayNum[i] == 10) {
            displayNum[i] = 0;
          } else {
          }
        }
        tm1636.display(displayNum);
        StartTime = millis();
      } else {
      }
      tm1636.display(displayNum);  
    }
    break;
  case Button3Stop:
    if((displayNum[0] == displayNum[1]) && (displayNum[1] == displayNum[2])) {
      WorkingStatus = Success;
    } else {
      WorkingStatus = Failed;
    }
    break;
  case Failed:
    FailedMusicPlay(ALARM_BUZZER);
    displayNum[0] = 0;
    displayNum[1] = 0;
    displayNum[2] = 0;
    WorkingStatus = Standby;
#ifdef DEBUG
      Serial.println("Failed");
#endif
    break;
  case Success:
    SuccessMusicPlay(ALARM_BUZZER);
    turnOnAll();
    playSex();    
    analogWrite(RELAY,RELAYOpen);
    delay(1000);
//    while(1) {
//    Serial.println("FeedBack");
//    Serial.println(analogRead(FeedBack));
//    delay(50);
//    }
    while(true){
      uint16_t temp = analogRead(FeedBack);
#ifdef DEBUG
      Serial.println("FeedBack");
      Serial.println(analogRead(temp));
#endif
      if(temp <= 400) {
        break;
      } else {
      }
    }
    displayNum[0] = 0;
    displayNum[1] = 0;
    displayNum[2] = 0;
    analogWrite(RELAY,RELAYClose);
    turnOffAll();
    WorkingStatus = Standby;
#ifdef DEBUG
      Serial.println("Success");
#endif
    break;
  default:
    break;
  }

}

//-------------------pinmode of btn------------------------------------
void init_pins()
{
  /*K1 and K2 are set to be input pins*/
  pinMode(BTN_1, INPUT);
  digitalWrite(BTN_1, HIGH);
  pinMode(BTN_2, INPUT);
  digitalWrite(BTN_2, HIGH);
  pinMode(BTN_3, INPUT);
  digitalWrite(BTN_3, HIGH);
  /*The buzzer is set to be output pin*/
  pinMode(ALARM_BUZZER, OUTPUT);
  digitalWrite(ALARM_BUZZER, LOW);

  pinMode(LED4, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED1, OUTPUT);
  turnOffAll();//Turn off all the LEDs first.
  pinMode(RELAY,OUTPUT);
}

/*Scan keys and get to know which one is pressed*/
/*Return: if no key is pressed, return -1.*/



//----------------------which btn is pressed------------------------------

uint8_t WaitingButton1(void) 
{
  uint8_t ButtonFlag = false;
  if(digitalRead(BTN_1) == LOW) {
    delay(20);
    while (digitalRead(BTN_1) == LOW)//validate a press that outlasts 20ms
    {
      digitalWrite(ALARM_BUZZER, HIGH);
      ButtonFlag = true;
#ifdef DEBUG
      Serial.println("BTN_1 is pressed");
#endif
    }
  } else {
  }
  digitalWrite(ALARM_BUZZER, LOW);
  return ButtonFlag;
    
  
}


uint8_t WaitingButton2(void) 
{
  uint8_t ButtonFlag = false;
  if(digitalRead(BTN_2) == LOW) {
    delay(20);
    while (digitalRead(BTN_2) == LOW)//validate a press that outlasts 20ms
    {
      digitalWrite(ALARM_BUZZER, HIGH);
      ButtonFlag = true;
#ifdef DEBUG
      Serial.println("BTN_2 is pressed");
#endif
    }
  } else {
    
  }
  digitalWrite(ALARM_BUZZER, LOW);
  return ButtonFlag;
}



uint8_t WaitingButton3(void) 
{
  uint8_t ButtonFlag = false;
  if(digitalRead(BTN_3) == LOW) {
    delay(20);
    while (digitalRead(BTN_3) == LOW)//validate a press that outlasts 20ms
    {
      digitalWrite(ALARM_BUZZER, HIGH);
      ButtonFlag = true;
#ifdef DEBUG
      Serial.println("BTN_3 is pressed");
#endif
    }
  } else {
  }
  digitalWrite(ALARM_BUZZER, LOW);
  return ButtonFlag;
}


/*
* If you want to use this function, don't forget add the pitches.h head file 
for example;
#include "pitches.h"
*/
void SuccessMusicPlay(uint8_t buzzerpin) 
{
  uint8_t flag = 0;
  int melody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
  };
  int noteDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4
  };
  for(int i = 0;i<2; i++) {
    for (int thisNote = 0; thisNote < 8; thisNote++) {
      if(flag == 0) {
        turnOnAll();
        flag = 1;
      } else {
        flag = 0;
        turnOffAll();
      }
    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerpin, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(buzzerpin);
   }
  }
}

void FailedMusicPlay(uint8_t buzzerpin) 
{
  for(int i = 0;i<3; i++) {
      digitalWrite(buzzerpin, HIGH);   // turn the LED on (HIGH is the voltage level)
      turnOn(LED1);
      turnOn(LED2);
      turnOn(LED3);
      turnOn(LED4);
      delay(300);              // wait for a second
      digitalWrite(buzzerpin, LOW);    // turn the LED off by making the voltage LOW
      turnOff(LED1);
      turnOff(LED2);
      turnOff(LED3);
      turnOff(LED4);
      delay(300);              // wait for a second
  }
}

void playSex()
{
  int duration = 30;
  uint8_t shifter = 0x01;
  for (int tone = 2; tone < 3000; tone += 30){
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
      digitalWrite(ALARM_BUZZER, HIGH);
      delayMicroseconds(tone);
      digitalWrite(ALARM_BUZZER, LOW);
      delayMicroseconds(tone);
    }
  }

}


//LED display after shot

/*Turn on one LED*/
inline void turnOn(int pinLED)
{
  digitalWrite(pinLED, HIGH);
}
/*Turn Off one LED*/
inline void turnOff(int pinLED)
{
  digitalWrite(pinLED, LOW);
}

void runLED(byte speed)
{
  if((speed > 0)&&(speed < 11))//If the value of speed is valid?
  {
    uint8_t shifter = 0x01;
    for(uint8_t i = 0;i < 4;i ++)
    {
      if(shifter&0x01)//if the lowest significant bit that represents LED1 is HIGH? 
        turnOn(LED1);//yes, turn on LED1.
      else      
        turnOff(LED1);//no, turn off LED1.
      if(shifter&0x02) 
        turnOn(LED2);
      else      
        turnOff(LED2);
      if(shifter&0x04) 
        turnOn(LED3);
      else      
        turnOff(LED3);
      if(shifter&0x08)
        turnOn(LED4);
      else      
        turnOff(LED4);
      shifter <<= 1;//left shift 1 bit, light up the next LED and put out the current one.
      delay(500/speed);
    }
    turnOffAll();//Turn off all LEDs after running the LEDs
  }
}
/*Turn off all the LEDs*/
inline void turnOffAll()
{
  turnOff(LED4);
  turnOff(LED3);
  turnOff(LED2);
  turnOff(LED1);
}
inline void turnOnAll()
{
      turnOn(LED1);
      turnOn(LED2);
      turnOn(LED3);
      turnOn(LED4);
}










