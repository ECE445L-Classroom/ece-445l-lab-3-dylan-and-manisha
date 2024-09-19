// Key.c
// This software configures the off-board piano keys
// Lab 6 requires a minimum of 4 keys, but you could have more
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 1/2/23  
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// **************Key_Init*********************
// Initialize piano key inputs on PA5-2 or PE3-0
// Input: none 
// Output: none
void Key_Init(void){ 
  // used in Lab 6 
	volatile uint32_t delay;
	//initialize piano key inputs
	//initialize clock for PORT A
	SYSCTL_RCGCGPIO_R |= 0x01;
	delay = 10;
	//initialize DIR for PORT A
	GPIO_PORTA_DIR_R &= ~0x3C;
	
	//initialize DEN for PORT A
	GPIO_PORTA_DEN_R |= 0x3C;
	
	//initialize PDR (internal resistors) for PORT A (positive logic switches)
	//GPIO_PORTA_PDR_R |= 0x3C;
}
// **************Key_In*********************
// Input from piano key inputs on PA5-2 or PE3-0
// Input: none 
// Output: 0 to 15 depending on keys
//   0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2, 0x08 is just Key3
uint32_t Key_In(void){ 
  // write this
	uint32_t key;
	
	//read and mask input keys
	//get the value from the PORTA DATA register
	//mask the value (only have bits 5-2--which hold our input value)
	key = GPIO_PORTA_DATA_R & 0x3C;
	
	//shift the value to the right by 2
	//we want the value ot be in bits (3-0)
	key = key >> 2;
	
	//return the value
  return key;
}

//------------LaunchPad_Init------------
// Initialize Switch input and LED output
// Input: none
// Output: none
void LaunchPad_Init(void){
// implement if needed
}


//------------LaunchPad_Input------------
// Input from Switches, 
// Convert hardware negative logic to software positive logic 
// Input: none
// Output: 0x00 none
//         0x01 SW2 pressed (from PF4)
//         0x02 SW1 pressed (from PF1)
//         0x03 both SW1 and SW2 pressed
uint8_t LaunchPad_Input(void){
// implement if needed
  return 0; // replace   
}
//------------LaunchPad__Output------------
// Output to LaunchPad LEDs 
// Positive logic hardware and positive logic software
// Input: 0 off, bit0=red,bit1=blue,bit2=green
// Output: none
void LaunchPad_Output(uint8_t data){  // write three outputs bits of PORTF
// implement if needed

}

