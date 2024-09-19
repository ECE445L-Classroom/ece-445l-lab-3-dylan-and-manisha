#include "../inc/ST7735.h"
#include "../inc/tm4c123gh6pm.h"
#include "Switches.h"

void (*HandlerC)(void);

// PC3-0 ARE INITIALIZED AS INPUT POS. LOGIC SWITCHES
// 0x0F
void Switch_Init( void(*RED_Handler)() , void(*BLUE_Handler)(), void(*GREEN_Handler)(), void(*YELLOW_Handler)()){ 
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000008;  // 1) activate clock for Port D
  delay = SYSCTL_RCGC2_R;        // allow time for clock to settle
  GPIO_PORTD_DIR_R &= ~0x0F;     // 2) set direction register
  GPIO_PORTD_AFSEL_R &= ~0x0F;   // 3) regular port function
  GPIO_PORTD_DEN_R |= 0x0F;      // 4) enable digital port
}


void GPIOPortC_Handler(void){
    GPIO_PORTC_ICR_R = 0xF0;   // clear flag
    (*HandlerC)(); // execute user function
}