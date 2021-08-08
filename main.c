#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "GPIO_Driver.h"
#include "Interrupt_Driver.h"
#include "Debug_Driver.h"

//Example of using Semaphore, GPIO_Driver, Interrupt_Driver, and Debug_Driver

/* This Project uses the GPIO_Driver and Interrupt_Driver libraries which provide
   definitions for different functions that help to intialize Tiva c  peripherals.
   Debug_Driver helps to control each Led on port F Depending on the color.
   To use this driver you will also need to use tm4c123gh6pm to map 
   registers correctly.
   this example create two tasks which are blue and red.
   Blue task is Continuous Task repeats itself evey 1 sec.
   Red Task waits Binary Semaphore to take action and the semaphore is depending 
   on GPIO interrupt from PORTF.
   
   Used pins
   ===========
   Pin F0 as digital input
   Pin F1 as digital output
   History
   =======
   2021/August/08  - First release (Mosad)
*/

SemaphoreHandle_t Semaphore;																		//Create Semaphore
void Initialization(void);																			//Initialization for GPIO interrupt

void vPeriodicTask1(void *pvParameters);												//First Task Function
void vPeriodicTask2(void *pvParameters);												//Second Task Function

int main()
{	
	Initialization();																							//Initialize both interrupt and switch
	
	Semaphore=xSemaphoreCreateBinary();														//Create Binary Semaphore
	
	xTaskCreate(vPeriodicTask1, "Blue_Led", 256, NULL, 1, NULL);	//Create First Task
	xTaskCreate(vPeriodicTask2, "Red_Led", 256, NULL, 1, NULL);		//Create Second Task

	vTaskStartScheduler();																				//Start Scheduler to handle both Tasks

	for (;;);
}

void vPeriodicTask1(void *pvParameters)
{
	static bool toggle = 1;
	const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;
	for (;;) 
			{
				Debug_Led("Blue",toggle);																	//toggle Blue Led
				toggle = !toggle;
				vTaskDelay( xDelay );
			}
}

void vPeriodicTask2(void *pvParameters)
{
	static bool toggle = 1;
	for (;;) 
			{		
				xSemaphoreTake(Semaphore,portMAX_DELAY);									//See if we can obtain the semaphore.  If the semaphore is not available wait 10 ticks to see if it becomes free.
				Debug_Led("Red",toggle);																	//Toggle Red Led
				toggle = !toggle;
			}
}

void GPIOF_Handler (void)
{
BaseType_t contextRequst;																					//variable that will be updated by the semaphore API to indicate a context switch is needed so ISR call schedule 
contextRequst=pdFALSE;																						//initialize it to fals which means no context required now 
xSemaphoreGiveFromISR(Semaphore,&contextRequst);									//give semaphore by incrementing it to indicate ISR occured 
portYIELD_FROM_ISR(contextRequst);																//If higher priority task is unblocked due to giving the semaphore then do a context switch by calling the scheduler
}

void Initialization(void)
{
	CLK_Enable('F');																								//Enable Port F
	GPIO_PORT_UNLOCK('F');																					//Unlock Port F
	PIN_ANALOG_DIGITAL('F',4,"Digital");														//Pin F4 as Digital
	PIN_FUNCTION_SELECT('F',4,"General I/O");												//Pin F4 as General Input/output
	PIN_INPUT_OUTPUT('F',4,"Input");																//Pin F4 as Input
	PIN_PULLUP_RESISTOR	('F',4,"Pullup");														//Pin F4 as Pullup
	PIN_Edge_Level_Trigger('F',4,"Level_Sensitive");								//Switch as Level Sensititve
	PIN_Unmask('F',4);																							//Unmask  Pin F4
	NVIC_SetPriority(30,5);																					//set Priority for PORTF
	NVIC_EnableIRQ(30);																							//Enable Interrupt																			
}

