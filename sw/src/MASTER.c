#include "../inc/tm4c123gh6pm.h"
#include "../inc/Timer0A.h"
#include "../inc/Timer2A.h"
#include "../inc/Timer5A.h"
#include "Switches.h"
#include "MASTER.h"
#include "../inc/ST7735.h"


uint16_t curr_time[3];
uint16_t set_time[3];
uint8_t page;
uint8_t ringAlarm;

#define HH 0
#define MM 1
#define SS 2

#define MAIN_CLOCK 0
#define SOUND_SELECT 1
#define ALARM_SELECT 2
#define ALARM_RINGING 3

#define RED 0
#define BLUE 1
#define YELLOW 2
#define GREEN 3

void buttonHandler(){
	ST7735_SetCursor(3, 3);
	ST7735_OutUDec(69);
}

void setAlarmTime(){
	// if they press respective button for H,M --> will change the alarm time they want to set
	if(GPIO_PORTC_DATA_R == 1<<BLUE){ //
		while(GPIO_PORTC_DATA_R == 1<<BLUE){} // wait for button release
		set_time[HH]++;
		if (set_time[HH] >= 24) set_time[HH] %= 24; // cycle HH select
	}
	if(GPIO_PORTC_DATA_R == 1<<YELLOW){ // 
		while(GPIO_PORTC_DATA_R == 1<<YELLOW){} // wait for button release
		set_time[MM]++;
		if (set_time[MM] >= 60) set_time[MM] %= 60; // cycle HH select
	}
}

void TickTock_ISR(){
	// tick tock from 00:00:00 to 24:00:00
	curr_time[SS]++;
	if (curr_time[SS] == 60) {
		curr_time[SS] = 0;
		curr_time[MM]++;
		if (curr_time[MM] == 60) {
			curr_time[MM] = 0;
			curr_time[HH]++;
			if (curr_time[HH] == 24) {
				curr_time[HH] = 0;
			}
		}
	}
	
	// run curr time against set time
	if (curr_time[HH] == set_time[HH] && curr_time[MM] == set_time[MM] && curr_time[SS] == set_time[SS]) {
		ringAlarm = 1;
		page = ALARM_RINGING;
	}
	
	ST7735_SetCursor(0, 0);
	ST7735_OutUDec(curr_time[SS]);
}

void TickTock_INIT(){
	// set default time to 00:00:00
	curr_time[HH] = 0; curr_time[MM] = 0; curr_time[SS] = 0;

	// Dedicate Timer5A to one second time increments "tick tock"
	Timer5A_Init(&TickTock_ISR, 80000000, 3); // 80 MHz / 80,000,000 = 1 Hz = 1 second increments
	Switch_Init(&buttonHandler);
	
	// Dedicate button controls
	// -- PAGES -- 
	// *main clock*
	// - red: toggle between Military (24-hr) and Standard (12-hr AM/PM) time
	// - green: easter egg sprite @-@
	
	// *sound select*
	// - red: abandon changes
	// - blue: cycle upwards
	// - yellow: cycle downwards
	// - green: save changes & return to main
	
	// *alarm time select* ("back home" , "HH", "MM", "SS", "set alarm")
	// - red: cycle left
	// - blue: cycle HH upward from 00
	// - yellow: cycle MM upward from 00
	// - green: cycle right
	
	// *alarm ringing*
	// - red: go back to main
	// - blue: go back to main
	// - yellow: go back to main
	// - green: go back to main
	//Switch_Init( &buttonHandler );	
	

}