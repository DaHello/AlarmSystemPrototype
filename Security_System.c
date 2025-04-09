/**********************************************************************
* Filename    : Security_System.c
* Description : Uses an infrared motion sensor and detects thermal motion nearby, and alerts through a buzzer. LEDs, and an LCD display. 
* Assignment  : Final Project
* Author      : Robert Kane
* Course	  : CSC210-401
* Reference   : www.freenove.com
* Date        : 2024/04/20
* Version     : 1.5
**********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "I2CLCD1602.h"         // include all header files and methods from I2CLCD1602.c

#define sensorPin   0          // define infrared Sensor pin
#define ledRedPin   24          // define red LED located at GPIO16
#define ledGreenPin 28          // define green LED located at GPIO19
#define buzzerPin   25          // define active buzzer located at GPIO26

void main(void)
{
    printf("Program is starting ... \n"
           "ctrl+c to exit program.\n");

	wiringPiSetup();                // initialize wiringPi

    pinMode(ledRedPin, OUTPUT);     // Set led pins to output to send signals
	pinMode(ledGreenPin, OUTPUT);
    pinMode(sensorPin, INPUT);      // set sensorPin to Input to recieve signals IF NO WORK
    pinMode(buzzerPin, OUTPUT);   	// set pin for buzzer to Output

    if(setupLCD() == -1) {   	// if the setup of the LCD fails exit the program
        exit(1);
    };

    pullUpDnControl(ledRedPin, PUD_UP);       // pull led pins up to HIGH level
    pullUpDnControl(ledGreenPin, PUD_UP);
	while(1){   // infinite loop, until ctrl+C is entered
		if (digitalRead(sensorPin) == HIGH){                       // Sensor tripped
			digitalWrite(ledGreenPin, LOW);                                                         // Toggle off green led
			digitalWrite(ledRedPin, HIGH);                                                          // Toggle on red led
			digitalWrite(buzzerPin, HIGH);   														// Toggle on buzzer

			clearDisplay();      // clear the display of leftover pixels
			displayWarning();    // display warning
                                                                 // Toggle the buzzer on
			printf("WARNING! Motion detected; buzzer on; led red >>>\n");		                // Output information to terminal
		}
		else {							                          // Sensor not tripped 
			digitalWrite(ledRedPin, LOW);                                                           // Toggle off red led
			digitalWrite(ledGreenPin, HIGH);                                                        // Toggle on Green led
			digitalWrite(buzzerPin, LOW); 															// Toggle off buzzer

         		clearDisplay();     // clear the LCD display
		        displayAllClear();     // display "All clear."

			printf("All clear, no thermal motion detected; buzzer off; led green <<<\n");		// Output information to terminal
		}
	}
}
