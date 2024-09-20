#include "../inc/ST7735.h"
#include "../inc/tm4c123gh6pm.h"
#include "Switches.h"

void (*ButtonHandler)(void);

// PC7-4 ARE INITIALIZED AS INPUT POS. LOGIC SWITCHES
// 0xF0
void Switch_Init( void(*Handler)(void) ){ 
	volatile unsigned long delay;
  SYSCTL_RCGCGPIO_R |= 0x00000004;  // 1) activate clock for Port C
  delay = SYSCTL_RCGC2_R;        // allow time for clock to settle
  GPIO_PORTC_DIR_R &= ~0xF0;     // 2) set direction register
  //GPIO_PORTC_AFSEL_R &= ~0xF0;   // 3) regular port function
  GPIO_PORTC_DEN_R |= 0xF0;      // 4) enable digital port
	
	GPIO_PORTC_IS_R &= ~0xF0;	// make PE0-3 edge sensitive
	GPIO_PORTC_IBE_R &= ~0xF0;	// not both edges
	GPIO_PORTC_IEV_R &= ~0xF0;	// falling edge triggered
	//GPIO_PORTC_PUR_R |= 0xF0;	// enable pull up resistors
	GPIO_PORTC_ICR_R = 0xF0;	// clear flag 0-3
	GPIO_PORTC_IM_R |= 0xF0;	// arm interrupt on PE0-3
	NVIC_PRI0_R = (NVIC_PRI0_R&0xFF00FFFF)|0x00A00000;   // priority 6
	NVIC_EN0_R |= 4;	// enable NVIC
	ButtonHandler = Handler;
}


void GPIOPortC_Handler(void){
	ST7735_SetCursor(0, 0);
	ST7735_OutUDec(69);
	
	GPIO_PORTC_ICR_R = 0xF0;   // clear flag
	(*ButtonHandler)(); // execute user function
	
	
	ST7735_SetCursor(2, 2);
	ST7735_OutUDec(88);
}