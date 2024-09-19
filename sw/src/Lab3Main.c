// Lab3Main.c
// Runs on LM4F120/TM4C123
// Uses ST7735.c to display the clock
// patterns to the LCD.
//    16-bit color, 128 wide by 160 high LCD

// Specify your hardware connections, feel free to change
// PD0 is squarewave output to speaker
// PE0 is mode select
// PE1 is left
// PE2 is right 
// PE3 is up
// PE4 is down
// if alarm is sounding, any button will quiet the alarm

#include <stdio.h>
#include <stdint.h>
#include "../inc/ST7735.h"
#include "../inc/PLL.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/Timer0A.h"
#include "../inc/Timer5A.h"
#include "Lab3.h"
#include "MASTER.h"

// ---------- Prototypes   -------------------------
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode

// --- GLOBAL VARIABLES --- //
int hour; // 0 - 24
int minute; // 0 - 60
int second; // 0 - 60
int start; 
int pause;
int reset; 

void UserTask(void){
  GPIO_PORTB_DATA_R ^= 0x02;
}

int main(void){
  DisableInterrupts();
  PLL_Init(Bus80MHz); // bus clock at 80 MHz
	ST7735_InitR(INITR_REDTAB); // start up LCD with black screen
  
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x02;
	delay = SYSCTL_RCGCGPIO_R;
	GPIO_PORTB_DIR_R |= 0x02;
	GPIO_PORTB_DEN_R |= 0x02;
	
	//Timer0A_Init(&UserTask, 181818, 1); // task, period, priority --> 440Hz --> 80MHz / 181818
	TickTock_INIT();
  EnableInterrupts();
	
	
  while(1){		
		WaitForInterrupt();
	}
	
}