#include<lpc17xx.h>
#define r1 (1<<19)	   // naming GPIO pins
#define o1 (1<<14)
#define g1 (1<<20)
#define r2 (1<<21)
#define o2 (1<<15)
#define g2 (1<<22)
#define r3 (1<<23)
#define o3 (1<<16)
#define g3 (1<<24)
#define r4 (1<<25)
#define o4 (1<<17)
#define g4 (1<<26)

void delay(unsigned int);	// declaration of delay function

void Timer0_Init(void)		// TimerO Initialization 
{
 LPC_TIM0->PR = 3;			// Value in Prescalar Register

 LPC_TIM0->MR0 = 3000000;	// MR0 Interrupt value				
 LPC_TIM0->MR1 = 6000000;	// MR1 Interrupt value
 LPC_TIM0->MR2 = 7000000;	// MR2 Interrupt value
 LPC_TIM0->MR3 = 10000000;	// MR3 Interrupt value

 LPC_TIM0->MCR = (1<<0)|(1<<3)|(1<<6)|(1<<9);	  	// Match Control Register to generate interrupts at all four MRs
 LPC_TIM0->TCR = (1<<1);							// Timer Control Register to reset the counter
 LPC_TIM0->TCR = (1<<0);							// TCR for start the counter
}

void Timer1_Init(void)
{
 LPC_TIM1->PR = 3;

 LPC_TIM1->MR0 = 11000000;
 LPC_TIM1->MR1 = 14000000;
 LPC_TIM1->MR2 = 15000000;
 LPC_TIM1->MR3 = 18000000;

 LPC_TIM1->MCR = (1<<0)|(1<<3)|(1<<6)|(1<<9)|(1<<10);
 LPC_TIM1->TCR = (1<<1);
 LPC_TIM1->TCR = (1<<0);
}

void Timer0_IRQHandler(void)
{
  if(LPC_TIM0->IR & 0x0F) {
        LPC_TIM0->IR = 0x0F;  // Clear all match flags
    }

 if(LPC_TIM0->IR & (1<<0))		// Checking for Interrupt at MR0		
  {
   LPC_GPIO1->FIOSET = g1 | r2 | r3 | r4;
   LPC_GPIO1->FIOCLR = o1 | g4;
   delay(200);
   LPC_TIM0->IR = (1<<0);		// Writing 1 to the 0th bit clears the pin. So it clears MR0
  }
  if(LPC_TIM0->IR & (1<<1))		// Checking for Interrupt at MR1
  {
   LPC_GPIO1->FIOSET = o2 | r3 | r4 | g1;
   LPC_GPIO1->FIOCLR = r2;
   delay(200);
   LPC_TIM0->IR = (1<<1);		 // Writing 1 to the 1st bit clears the pin. So it clears MR1
  }
  if(LPC_TIM0->IR & (1<<2))		 // Checking for Interrupt at MR2
  {
   LPC_GPIO1->FIOSET = g2 | r1 | r3 | r4;
   LPC_GPIO1->FIOCLR = o2 | g1;
   delay(200);
   LPC_TIM0->IR = (1<<2);		// Writing 1 to the 2nd bit clears the pin. So it clears MR2
  }
  if(LPC_TIM0->IR & (1<<3))		 // Checking for Interrupt at MR3
  {
   LPC_GPIO1->FIOSET = o3 | r1 | r4 | g2;
   LPC_GPIO1->FIOCLR = r3;
   delay(200);
   LPC_TIM0->IR = (1<<3);		// Writing 1 to the 3rd bit clears the pin. So it clears MR3
  }
}

void Timer1_IRQHandler(void)
{
 if(LPC_TIM1->IR & 0x0F) {
        LPC_TIM1->IR = 0x0F;  // Clear all match flags
    }

 LPC_GPIO1->FIOCLR = (1<<10);
 if(LPC_TIM1->IR & (1<<0))		
  {
   LPC_GPIO1->FIOSET = g3 | r1 | r2 | r4;
   LPC_GPIO1->FIOCLR = o3 | g2;
   delay(200);
   LPC_TIM1->IR = (1<<0);
  }
  if(LPC_TIM1->IR & (1<<1))		
  {
   LPC_GPIO1->FIOSET = o4 |  r1 | r2 | g3;
   LPC_GPIO1->FIOCLR = r4;
   delay(200);
   LPC_TIM1->IR = (1<<1);
  }
  if(LPC_TIM1->IR & (1<<2))		
  {
   LPC_GPIO1->FIOSET = g4 | r1 | r2 | r3;
   LPC_GPIO1->FIOCLR = o4 | g3;
   delay(200);
   LPC_TIM1->IR = (1<<2);
  }
  if(LPC_TIM1->IR & (1<<3))		
  {
   LPC_GPIO1->FIOSET = o1 | r2 | r3 | g4;
   LPC_GPIO1->FIOCLR = r1;
   delay(200);
   LPC_TIM1->IR = (1<<3);
   LPC_TIM0->TC = 0;
  }
}

int main()
{
 LPC_GPIO1->FIODIR |= (0xff<<19) | (0x0f<<14);		// configuring 12 pins as output pins 
 LPC_GPIO1->FIOCLR = (0xff<<19) | (0X0f<<14);		// clearing 12 pins			 
 //timers
 LPC_SC->PCONP |= (0x03<<1);	  					// Enabling Power configuration 
 LPC_SC->PCLKSEL0 |= (1<<2) | (1<<4); 				// Sets bits 2 and 4 for clock of Timers 0 and 1.

 NVIC_EnableIRQ(TIMER0_IRQn); // Enable interrupts
 NVIC_EnableIRQ(TIMER1_IRQn);

 Timer0_Init();
 Timer1_Init();

 while(1);
}		

void delay(unsigned int k)
{
 unsigned int i,j;
 for(i=0;i<k;i++)
 {
  for(j=0;j<3000;j++);
 }
}
