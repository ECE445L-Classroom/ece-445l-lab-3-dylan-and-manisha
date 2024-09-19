// UART1.c
// Runs on LM4F120/TM4C123
// Use UART1 to implement bidirectional data transfer to and from 
// another microcontroller in Lab 9.  This time, interrupts and FIFOs
// are used.
// Daniel Valvano
// 1/2/2023

/* Lab solution, Do not post
 http://users.ece.utexas.edu/~valvano/
*/

// U1Rx (VCP receive) connected to PC4
// U1Tx (VCP transmit) connected to PC5
#include <stdint.h>
//#include "Fifo.h"
//#include "UART1.h"
#include "../inc/tm4c123gh6pm.h"
#define UART_CTL_TXE 0x00000100 // UART Transmit FIFO enable
#define UART_CTL_RXE 0x00000200 // UART Receive FIFO enable

int32_t RxCounter = 0;
// Initialize UART1
// Baud rate is 2000 bits/sec
// baud rate is the total number of bits transmitted per second
void UART1_Init(void){
  // write this
	volatile int delay = 0;
	// activate UART1
	SYSCTL_RCGCUART_R |= 0x00000002;
	delay++;
	delay++;
	// activate port c
	SYSCTL_RCGCGPIO_R |= 0x00000004;
	delay++;
	delay++;
	// disable UART
	UART1_CTL_R &= ~0x00000001;
	// IBRD = int(80000000/(16*2000)) = int(2500)
	UART1_IBRD_R = 2500;
	// FBRD = round(0*64) = 0
	UART1_FBRD_R = 0;
	// 8 bit, no parity bits, one stop, FIFOs
	UART1_LCRH_R = 0x0070;
	// enable UART
	UART1_CTL_R = 0x301; //changed this
	// enable alternate function on PC5, PC4
	GPIO_PORTC_AFSEL_R |= 0x30;
	// configure PC5, PC4 as UART1
	GPIO_PORTC_DEN_R |= 0x30;
	GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF) + 0x00220000;
	//disable analog on PC5, PC4
	GPIO_PORTC_AMSEL_R &= ~0x30;

	//reciever
	//RTRIS (bit 6) reciever time out
	//idle input on PC4 for 16 bit-times
	// UART1_RIS_R |= 0x40;
	//IM register contains the ARM bits
	//Bit 6 is a trigger flag for reciever timeout
	UART1_IM_R |= 0x40;
	//enable interrupt 6 in NVIC
	//bit 6 is UART1; write 1 to enable, writing 0 has no effect
	NVIC_EN0_R |= 0x40;
	//set priority (21-23) to 0
	NVIC_PRI1_R &= ~0xE00000;
	//recieve FIFO goes from 7 to 8 characters (1/2 full)
	//UART1_IFLS_R &= ~0x28;
	//UART1_IFLS_R |= 0x10;
	
	Fifo_Init();
	
	UART1_CTL_R |= (UART_CTL_UARTEN|UART_CTL_TXE|UART_CTL_RXE); // enable UART	
}

//------------UART1_InChar------------
// Wait for new input, 
// Use your FIFO, do not access UART1 hardware
// then return ASCII code
// Input: none
// Output: char read from UART
char UART1_InChar(void){
  // write this
	while((UART1_FR_R&0x0010) != 0){}
	return((char) (Fifo_Get()));
}
//------------UART1_InMessage------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until > is typed
//    or until max length of the string is reached.
// Input: pointer to empty buffer of 8 characters
// Output: Null terminated string
void UART1_InMessage(char *bufPt){
  // write this if you need it
}

//------------UART1_OutChar------------
// Output 8-bit to serial port
// Do not use interrupts or FIFO, use busy wait
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART1_OutChar(char data){
  // write this
	// wait for TXFF in UART1_FR_R to be 0
	while((UART1_FR_R&0x0020) != 0){}
  // write a byte to UART1_DR_R
  UART1_DR_R = data; 
}
#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
// RTRIS UART1 receiver has timed out
// Put data into your FIFO
void UART1_Handler(void){
	char letter;
  // write this
	//0) toggle a heartbeat (change from 0 to 1, or from 1 to 0), 
	GPIO_PORTF_DATA_R ^= 0x40;
	//1) toggle a heartbeat (change from 0 to 1, or from 1 to 0), 
	//2) as long as the RXFE bit in the UART1_FR_R is zero (should be exactly 8 bytes, but could be more or less)
	//   -Read a byte from UART1_DR_R 
	//   -Put the byte into your software FIFO 
	while((UART1_FR_R&0x0010) == 0){
		letter  = UART1_DR_R;
		Fifo_Put(letter);
	}
	//Fifo_Put(UART1_InChar());
	//3) Increment a RxCounter, used as debugging monitor of the number of UART messages received
	RxCounter++;
	//4) acknowledge the interrupt by clearing the flag which requested the interrupt 
  UART1_ICR_R = 0x40;   // this clears bit 6 (RTRIS) in the RIS register
	// 5) toggle a heartbeat (change from 0 to 1, or from 1 to 0), 
}