#define GPIO_PORTD_DATA_R       (*((volatile unsigned long *)0x400073FC))
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *)0x40007420))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
#define PD5                     (*((volatile unsigned long *)0x40007080))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOD      0x00000008  // port D Clock Gating Control


// PD5-0 ARE INITIALIZED AS INPUT POS. LOGIC SWITCHES
void Switch_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000008;  // 1) activate clock for Port D
  delay = SYSCTL_RCGC2_R;        // allow time for clock to settle
  GPIO_PORTD_DIR_R &= ~0x2F;     // 2) set direction register
  GPIO_PORTD_AFSEL_R &= ~0x2F;   // 3) regular port function
  GPIO_PORTD_DEN_R |= 0x2F;      // 4) enable digital port
}