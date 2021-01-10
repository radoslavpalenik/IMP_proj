/**
 * @author Radoslav Páleník <xpalen05@stud.fit.vutbr.cz>
 * @date 2020-12-20
 * @name IMP Projekt 2020 - Zakrmovacia RC loď
 */

#include <SPI.h>  
#include "RF24.h"

#define joyX A3 // Os X
#define joyY A4 // Os Y

const int SW_pin = 2; // Vypustenie návnad


RF24 myRadio (9, 10);

bool pressed = false;

byte addresses[][6] = {"0"};

struct package
{
  int id=1;
  int xValue;
  int yValue;
  bool throwBait;
};


typedef struct package Package;
Package data;

void setup()
{
  
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(115200);
  delay(1000);
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openWritingPipe( addresses[0]);
  delay(1000);
}

void loop()
{
  //Naplnenie package
  data.id = data.id + 1;
  data.xValue = analogRead (joyX);
  data.yValue = analogRead(joyY);
  
  if(digitalRead(SW_pin) == LOW){
    pressed = true;
    }
  if(pressed){
      data.throwBait = true;   
    }else{
      data.throwBait = false;
      }
      
  myRadio.write(&data, sizeof(data)); 

  
  //Posielanie dát každých 350 ms
  delay(350);

}
