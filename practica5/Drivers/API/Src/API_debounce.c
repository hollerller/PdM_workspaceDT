/*
 * API_debounce.c
 *
 *  Created on: Nov 12, 2023
 *      Author: hollerller
 */

#include <stdio.h>
#include "API_debounce.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "API_uart.h"

// Private declarations

typedef enum{
BUTTON_UP,
BUTTON_FALLING,
BUTTON_DOWN,
BUTTON_RAISING,
} debounceState_t;		// Define a variable with the FMS states

static const uint8_t DEBOUNCETIME = 40; // Debounce delay constant

delay_t debounceDelay;		// Create a variable type delay_t
tick_t initialDelay = DEBOUNCETIME;  // Set the initial time of the delay (40ms)

static debounceState_t debounceState; // Variable for the FSM state
static bool_t edgeDetected;	// variable to check if the edge was detected

void buttonPressed(); // Function to execute when button is pressed
void buttonReleased(); // Function to execute when button is released

void debounceFSM_init(){			// Initialize the FMS
	debounceState = BUTTON_UP;		// Assume that the initial state of the button is UP
	delayInit(&debounceDelay, initialDelay);	// Initialize the delay
	edgeDetected = false; 	// Initialize the edgeDetected variable
}


void debounceFSM_update(){

	uint8_t buttonState = HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin); // Read the button state (pressed or released)

	switch (debounceState){

	case BUTTON_UP:		// State when button is released

		if (!buttonState) {	// Check if the button is pressed (the button state is 1 when button is up)
			debounceState = BUTTON_FALLING;	// Next state for falling edge
			delayInit(&debounceDelay, 40);	// Initialize the delay for debounce
		}
	break;

	case BUTTON_FALLING:	// State for falling edge

		if (delayRead(&debounceDelay)){	// Check if the delay has finished
			if (!buttonState) {			// If button is still pressed after debounce
				debounceState = BUTTON_DOWN;	// Move to the button down state
				buttonPressed();	// Execute button pressed functino
			} else {
				debounceState = BUTTON_UP;	// Go back to the button up state
			}
		}

		break;

	case BUTTON_DOWN:	// State when button is pressed

		if (buttonState) {	// Check if the button is released
			debounceState = BUTTON_RAISING;	//	Move to the next state for rising edge
			delayInit(&debounceDelay, 40);	// Initialize the delay for debounce
		}

		break;


	case BUTTON_RAISING:	// State for the rising edge

		if (delayRead(&debounceDelay)){	// Check if the delay has finished
			if (buttonState) {	// If button is still released after debounce
				debounceState = BUTTON_UP;	// Move back to the button up state
				buttonReleased();	// Execute the button release function
			} else {
				debounceState = BUTTON_DOWN;	// Return to the button down state
			}
		}

		break;

	default:

		debounceState = BUTTON_UP;
		edgeDetected = false;
		break;

	}
}


void buttonPressed(){

	uint8_t edgeMessage[] = "Button pressed\r\n";

	edgeDetected = true; // An edge was detected
	uartSendString(edgeMessage);
}


void buttonReleased(){
	uint8_t edgeMessage[] = "Button released\r\n";

	uartSendString(edgeMessage);
}

bool_t readKey(){

	if (edgeDetected){	// If an edge is detected
		edgeDetected = false;	// Turns the variable value to false
		return true;	// Return true (Key Pressed)
	} else
		return false;	// Key not pressed

}
