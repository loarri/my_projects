/*
* Title: Christmas with STM ToF
* Author: Lorenzo Arrigoni
* Ver. 1.0
* Date: December 2022
* Mail: laelectronics@libero.it
* More info: https://loarri.wordpress.com/its-christmas-time-with-stm-tof/
* Video: https://youtu.be/OHwQfEFHlAs
*
*---------------------------------------------------------------------
*  This software uses the STM ToF features (X-NUCLEO-53L1A1) with an STM32 NUCLEO 
*  F401RE board to recognize the obstacle in front of it. When the event happens 
*  the mp3 shield (DFPlayer, driven by an I2C protocol) is commanded to randomly play 
*  a song among those present on the sd card and triggers the relay to drive the led 
*  strip on the Christmas tree.  
*---------------------------------------------------------------------
*
* * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
 * THE OWNER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
*/


/* Includes ------------------------------------------------------------------*/
#include <Arduino.h>
#include <Wire.h>
#include <vl53l1x_x_nucleo_53l1a1_class.h>
#include <stmpe1600_class.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "config.h"



#define DEV_I2C Wire


#define interruptPin A2

// Components.
STMPE1600DigiOut xshutdown_top(&DEV_I2C, GPIO_15, (0x42 * 2));
STMPE1600DigiOut xshutdown_left(&DEV_I2C, GPIO_14, (0x43 * 2));
STMPE1600DigiOut xshutdown_right(&DEV_I2C, GPIO_15, (0x43 * 2));
VL53L1X_X_NUCLEO_53L1A1 sensor_vl53l1x_top(&DEV_I2C, &xshutdown_top);
VL53L1X_X_NUCLEO_53L1A1 sensor_vl53l1x_left(&DEV_I2C, &xshutdown_left);
VL53L1X_X_NUCLEO_53L1A1 sensor_vl53l1x_right(&DEV_I2C, &xshutdown_right);



// RX, TX
//mySoftwareSerial(D2,D8)
HardwareSerial mySoftwareSerial(PA_12,PC_6); // RX on pin 2 (unused), TX on pin 3 (to S1).D2, D8,-->PA_12, PC_6
DFRobotDFPlayerMini myDFPlayer;


volatile int interruptCount=0;
volatile int count=0;
bool soglia_superata = false;

void measure()
{
   interruptCount=1;
}

void setup()
{
   VL53L1X_Error status;

   pinMode(interruptPin, INPUT_PULLUP);
   attachInterrupt(interruptPin, measure, RISING);

   // Initialize serial for output.
   Serial.begin(115200);
   Serial.println("Starting...");

////
  mySoftwareSerial.begin(9600);
  
  pinMode(LED_BUILTIN, OUTPUT);
  LED_OFF;

  pinMode(RELE,OUTPUT); 
  RELE_OFF;
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  
  Serial.println(F("DFPlayer Mini online."));
  Serial.println(F("set volune.."));
  myDFPlayer.volume(VOLUME);  //Set volume value. From 0 to 30

  Serial.printf("\nStart program (solgia=%d) ...\n",SOGLIA);

  ////

   // Initialize I2C bus.
   DEV_I2C.begin();

   // Configure VL53L1X top component.
   sensor_vl53l1x_top.begin();

   // Switch off VL53L1X top component.
   sensor_vl53l1x_top.VL53L1X_Off();

   // Configure (if present) VL53L1X left component.
   sensor_vl53l1x_left.begin();

   // Switch off (if present) VL53L1X left component.
   sensor_vl53l1x_left.VL53L1X_Off();

   // Configure (if present) VL53L1X right component.
   sensor_vl53l1x_right.begin();

   // Switch off (if present) VL53L1X right component.
   sensor_vl53l1x_right.VL53L1X_Off();

   // Initialize VL53L1X top component.
   status = sensor_vl53l1x_top.InitSensor(0x10);
   if(status)
   {
      Serial.println("Init sensor_vl53l1xx_top failed...");
   }

   //Change timing budget to the minimum consented by the long range mode (20ms)
   status = sensor_vl53l1x_top.VL53L1X_SetTimingBudgetInMs(20);
   if( status )
   {
      Serial.println("SetMeasurementTimingBudgetMicroSeconds top sensor failed");
   }
   status = sensor_vl53l1x_top.VL53L1X_SetInterMeasurementInMs(20);
   if( status )
   {
      Serial.println("SetInterMeasurementPeriodMilliSeconds top sensor failed");
   }

   //Start measurement
   sensor_vl53l1x_top.VL53L1X_StartRanging();
}

void loop()
{
   if (interruptCount)
   {
      uint16_t distance;
      int status;

      interruptCount=0;
      // Led blinking.
      digitalWrite(13, HIGH);

      //read distance
      status = sensor_vl53l1x_top.VL53L1X_GetDistance(&distance);
      if( status )
      {
         Serial.println("GetDistance top sensor failed");
      }

      //restart sensor
      status = sensor_vl53l1x_top.VL53L1X_ClearInterrupt();
      if( status )
      {
         Serial.println("Restart top sensor failed");
      }

      // Output data.
      char report[64];
      snprintf(report, sizeof(report), "| Distance top [mm]: %d |", distance);
      Serial.println(report);
      digitalWrite(13, LOW);

     if (distance < SOGLIA) {
       soglia_superata = true;
       //static unsigned long timer = millis();
       Serial.println("Soglia superata!");
       LED_ON;
       RELE_ON;
       /* -- mp3 --*/
       myDFPlayer.randomAll();
     }

     if (soglia_superata) {
       while(count <= CONTEGGIO) {
         count ++;
         Serial.println(count);
       }
       if (count>CONTEGGIO) 
       {
         Serial.println("stop mp3");
         myDFPlayer.stop();
         RELE_OFF;
         LED_OFF;
         soglia_superata=false;
         count=0;
       }
     }
   }
}
