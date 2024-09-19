// General imports.
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Speaker2.h"

// Device specific imports.
#include <inc/LED.h>
#include <inc/Timer3A.h>
#include <inc/PWM.h>
#include <inc/Unified_Port_Init.h>
#include <inc/tm4c123gh6pm.h>

static void Sound_Step(void) {
	GPIO_PORTB_DATA_R ^= 0x1;
}

void Sound_Init(void) {
  SYSCTL_RCGCGPIO_R |= 0x02;            // 2) activate port B
  volatile int delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTB_AMSEL_R &= ~0x02;          // disable analog functionality on PB1
  GPIO_PORTB_DEN_R |= 0x02;             // enable digital I/O on PB1
  GPIO_PORTB_DIR_R |= 0x02;
  Timer3A_Init(&Sound_Step, 0xFFFFFFFF, 5); 
}

void Sound_Start(uint32_t freq) {
    // 1sec / freq / 2, ( /2 since we need to toggle twice in a cycle for a square wave )
    Timer3A_Start((80000000 / freq) >> 2); 
}

void Sound_Stop(void) {
    Timer3A_Stop();
}

void Timer3A_Start(uint32_t period){
  TIMER3_CTL_R |= TIMER_CTL_TAEN;       // 10) enable timer4A
	TIMER3_TAILR_R = period-1;    // 4) reload value
}
