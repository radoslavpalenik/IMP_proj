/**
 * @author Radoslav Páleník <xpalen05@stud.fit.vutbr.cz>
 * @date 2020-12-20
 * @name IMP Projekt 2020 - Zakrmovacia RC loď
 */
#include <SPI.h>  
#include "RF24.h"
#include <Servo.h>
#include <SparkFun_TB6612.h>


#define AIN1 2
#define AIN2 4
#define PWMA 3


#define BIN1 8
#define BIN2 7
#define PWMB 6

#define STBY 1


const int motDirection = 1;


Motor motRight = Motor(AIN1, AIN2, PWMA, motDirection, STBY);
Motor motLeft = Motor(BIN1, BIN2, PWMB, motDirection, STBY);

Servo baitThrower;

RF24 myRadio (9, 10);
struct package
{
  int id;
  int xValue;
  int yValue;
  bool throwBait;
};

byte addresses[][6] = {"0"}; 


bool delivered = false;

bool thrown = false;

typedef struct package Package;
Package data;

//Inicializácia spojenia
void startRadio(){
    myRadio.begin(); 
    myRadio.setChannel(115); 
    myRadio.setPALevel(RF24_PA_MAX);
    myRadio.setDataRate( RF24_250KBPS ) ; 
    myRadio.openReadingPipe(1, addresses[0]);
    myRadio.startListening();
  }

void setup() 
{
 

  Serial.begin(115200);
  delay(1000);

  startRadio();
}


void loop()  
{
 

  if ( myRadio.available()) 
  {
    while (myRadio.available())
    {
       myRadio.read( &data, sizeof(data) );

        //Vysypanie zakrmovacej zmesi
        if(data.throwBait && !thrown){
          
          thrown = true;
          delivered = true;
          
          baitThrower.attach(5);
          baitThrower.write(145);
          delay(1050);
    
          baitThrower.write(87);
          delay(1050);
    
          baitThrower.detach();
    
          }
          
      //Regulácia motorov
      if(data.yValue > 610){
        
        if(data.xValue <= 278){
          
            motLeft.brake();
            motRight.drive(240,250); 
                 
          }else if(data.xValue >= 778){
            
            motRight.brake();
            motLeft.drive(240,250);
            
          }else{
           
                forward(motRight, motLeft, 220);
                delay(150);
                
          }
        
        
        }else{
           brake(motRight, motLeft);
           
        }

    //Re-inicializácia spojenia
      if(delivered){
        delivered = false;
        startRadio();
        }
      //Dovolenie viacnásobného vyklopenia zásobníku
       if(thrown && data.throwBait == false){
          thrown = false;
        }
    }
/* // Výpis prijatých dát
    Serial.print("\n-------\n");
    Serial.print(data.id);
    Serial.println(data.xValue);
    Serial.println(data.yValue);
    Serial.println(data.throwBait);
  */


  }

}
