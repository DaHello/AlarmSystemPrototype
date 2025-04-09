/**********************************************************************
* Filename    : I2CLCD1602.c
* Description : Use the LCD display data
* Assignment  : Final Project
* Reference   : www.freenove.com 
* Author      : Robert Kane
* Course      : CSC210-401
* modification: 2024/04/21
**********************************************************************/
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pcf8574.h>
#include <lcd.h>
#include "I2CLCD1602.h"

int pcf8574_address = 0x27;        // PCF8574T:0x27, PCF8574AT:0x3F
#define BASE    64                 // BASE any number above 64
//Define the output pins of the PCF8574, which are directly connected to the LCD1602 pin.
#define RS      BASE+0
#define RW      BASE+1
#define EN      BASE+2
#define LED     BASE+3
#define D4      BASE+4
#define D5      BASE+5
#define D6      BASE+6
#define D7      BASE+7

// ADDED BY ME
int lcdhd;  // used to handle LCD
void displayWarning()   // displays the message "Warning." to notify motion detected
{
    lcdPosition(lcdhd,0,0);    				            // set the LCD cursor position to (0,0)
    lcdPrintf(lcdhd,"Warning.");                        // display message
    delay(100);                                         // wait a bit
}

// ADDED BY ME
void clearDisplay()   // clear display after sensor no longer senses thermal
{
    lcdPosition(lcdhd,0,1);                             // set the LCD cursor position to (0,1)
    lcdClear(lcdhd);                                    // clear the LCD messsage
}

// ADDED BY ME
void displayAllClear()   // displays the message "All clear." when motion is not detected
{
    lcdPosition(lcdhd,0,0);    				            // set the LCD cursor position to (0,0)
    lcdPrintf(lcdhd,"All clear.");                      // display message
    delay(100);
}

int detectI2C(int addr)      // detect I2C device at a given address
{
    int _fd = wiringPiI2CSetup (addr);                              // setup I2C device at address
    if (_fd < 0){		
        printf("Error address : 0x%x \n",addr);                     // error with given address
        return 0 ;
    } 
    else{	
        if(wiringPiI2CWrite(_fd,0) < 0){
            printf("Not found device in address 0x%x \n",addr);     // device not found
            return 0;
        }
        else{
            printf("Found device in address 0x%x \n",addr);         // device found
            return 1 ;
        }
    }
}

//FUNCTION ADDED BY ME (in place of main)
int setupLCD(void)    // setup the LCD screen
{
    if(detectI2C(0x27)){                // if the address matches '0x27' assign it to address var
        pcf8574_address = 0x27;
    }else if(detectI2C(0x3F)){          // if the address is 0x3F assign it to address var
        pcf8574_address = 0x3F;
    }else{                              // if no correct address was obtained from I2C return -1
        printf("No correct I2C address found, \n"
        "Please use command 'i2cdetect -y 1' to check the I2C address! \n"
        "Program Exit. \n");
        return -1;
    }
    pcf8574Setup(BASE,pcf8574_address);                 // initialize PCF8574
    for(int i=0; i<8; i++){
        pinMode(BASE+i,OUTPUT);                         // set PCF8574 port to output mode
    }
    digitalWrite(LED,HIGH);                             // turn on LCD backlight
    digitalWrite(RW,LOW);                               // allow writing to LCD
	lcdhd = lcdInit(2,16,4,RS,EN,D4,D5,D6,D7,0,0,0,0);  // initialize LCD and return “handle” used to handle LCD
    if(lcdhd == -1){                                    // if initialize failed
        printf("lcdInit failed !");
        return 1;
    }

    return 0;
}