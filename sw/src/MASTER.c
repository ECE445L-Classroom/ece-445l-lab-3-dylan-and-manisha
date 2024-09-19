#include "./inc/tm4c123gh6pm.h"
#include "../inc/Timer0A.h"
#include "../inc/Timer2A.h"
#include "../inc/Timer5A.h"
#include "MASTER.h"

uint16_t curr_time[3];
uint16_t set_time[3];
uint8_t page;
uint8_t ringAlarm;

#define HH 0
#define MM 1
#define SS 2

#define HOME 0
#define SET_ALARM 1
#define WAKE_UP 2

void setAlarmTime(){
	if (page == SET_ALARM) {
		// if they press respective button for H,M,S --> will change the alarm time they want to set
		if(GPIO_PORTC_DATA_R == 1<<HH){ 
			while(GPIO_PORTC_DATA_R == 1<<HH){} // wait for button release
			set_time[HH]++;	
		}
		if(GPIO_PORTC_DATA_R == 1<<MM){
			while(GPIO_PORTC_DATA_R == 1<<MM){} // wait for button release
			set_time[MM]++;
		}
		if(GPIO_PORTC_DATA_R == 1<<SS){
			while(GPIO_PORTC_DATA_R == 1<<SS){} // wait for button release
			set_time[SS]++;
		}
	}
}

void TickTock_ISR(){
	// tick tock from 00:00:00 to 24:00:00
	curr_time[SS]++;
	if (curr_time[SS] == 60) {
		curr_time[SS] %= 60;
		curr_time[MM]++;
		if (curr_time[MM] == 60) {
			curr_time[MM] %= 60;
			curr_time[HH]++;
			if (curr_time[HH] == 24) {
				curr_time[HH] %= 24;
			}
		}
	}
	
	// run curr time against set time
	if (curr_time[HH] == set_time[HH] && curr_time[MM] == set_time[MM] && curr_time[SS] == set_time[SS]) {
		ringAlarm = 1;
		page = WAKE_UP;
	}
}

void TickTock_INIT(){
	// set default time to 00:00:00
	curr_time[HH] = 0; curr_time[MM] = 0; curr_time[SS] = 0;

	// Dedicate Timer5A to one second time increments "tick tock"
	Timer5A_Init(&TickTock_ISR, 80000000, 3); // 80 MHz / 80,000,000 = 1 Hz = 1 second increments

}