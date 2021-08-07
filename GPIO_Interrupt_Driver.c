#include "GPIO_Driver.h"


volatile uint32_t* PORT_IS_R[]={&GPIO_PORTA_IS_R,&GPIO_PORTB_IS_R,&GPIO_PORTC_IS_R,      //Array of AMSEL Registers for each port
                              &GPIO_PORTD_IS_R,&GPIO_PORTE_IS_R,&GPIO_PORTF_IS_R};

volatile uint32_t* PORT_IBE_R[]={&GPIO_PORTA_IBE_R,&GPIO_PORTB_IBE_R,&GPIO_PORTC_IBE_R,      //Array of AMSEL Registers for each port
                              &GPIO_PORTD_IBE_R,&GPIO_PORTE_IBE_R,&GPIO_PORTF_IBE_R};

volatile uint32_t* PORT_IEV_R[]={&GPIO_PORTA_IEV_R,&GPIO_PORTB_IEV_R,&GPIO_PORTC_IEV_R,      //Array of AMSEL Registers for each port
                              &GPIO_PORTD_IEV_R,&GPIO_PORTE_IEV_R,&GPIO_PORTF_IEV_R};


volatile uint32_t* PORT_ICR_R[]={&GPIO_PORTA_ICR_R,&GPIO_PORTB_ICR_R,&GPIO_PORTC_ICR_R,      //Array of AMSEL Registers for each port
                              &GPIO_PORTD_ICR_R,&GPIO_PORTE_ICR_R,&GPIO_PORTF_ICR_R};

volatile uint32_t* PORT_IM_R[]={&GPIO_PORTA_IM_R,&GPIO_PORTB_IM_R,&GPIO_PORTC_IM_R,      //Array of AMSEL Registers for each port
                              &GPIO_PORTD_IM_R,&GPIO_PORTE_IM_R,&GPIO_PORTF_IM_R};

volatile uint32_t* PORT_MIS_R[]={&GPIO_PORTA_MIS_R,&GPIO_PORTB_MIS_R,&GPIO_PORTC_MIS_R,      //Array of AMSEL Registers for each port
                              &GPIO_PORTD_MIS_R,&GPIO_PORTE_MIS_R,&GPIO_PORTF_MIS_R};

void PIN_Edge_Level_Trigger(char port_name,int pin,char type[15])
  {
         if(strcmp(type , "Edge_Sensitive")==0)
      {
        *PORT_IS_R[port_name-65] &= ~(1UL << pin);                                                    //To Disable AMSEL
      }
         else if (strcmp(type , "Level_Sensitive")==0)
      {
        *PORT_IS_R[port_name-65] |= 1UL << pin;                                                       //To Enable AMSEL
      }
  }
void PIN_Both_Edge_Trigger(char port_name,int pin,char type[10])
  {
      if(strcmp(type , "False")==0)
      {
        *PORT_IBE_R[port_name-65] &= ~(1UL << pin);                                                    //To Disable AMSEL
      }
         else if (strcmp(type , "True")==0)
      {
        *PORT_IBE_R[port_name-65] |= 1UL << pin;                                                       //To Enable AMSEL
      }
  }
void PIN_Rising_Falling_Trigger(char port_name,int pin,char type[10])
  {
      if(strcmp(type , "Falling")==0)
      {
        *PORT_IEV_R[port_name-65] &= ~(1UL << pin);                                                    //To Disable AMSEL
      }
         else if (strcmp(type , "Rising")==0)
      {
        *PORT_IEV_R[port_name-65] |= 1UL << pin;                                                       //To Enable AMSEL
      }
  }


void PIN_Clear_Interrupt(char port_name,int pin)
  {
      *PORT_ICR_R[port_name-65] |= 1UL << pin;
  }

void PIN_Unmask(char port_name,int pin)
  {
    *PORT_IM_R[port_name-65] |= 1UL << pin;
  }

void Interrupt_Priority(volatile uint32_t * priority_R,int bits,int priority)
  {
    *priority_R = priority << bits ;
  }
void Interrupt_Enable(volatile uint32_t * Interrupt_Enable_R,int bits)
  {
    *Interrupt_Enable_R |= 1UL << bits;
  }

bool PIN_Interrupt_Flag(char port_name,int pin)
  {
   return (*PORT_MIS_R[port_name-65] & 1UL << pin);
  }
	