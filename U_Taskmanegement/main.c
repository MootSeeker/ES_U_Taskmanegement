/*
 * U_Taskmanegement.c
 *
 * Created: 20.03.2018 18:32:07
 * Author : chaos
 */ 

//#include <avr/io.h>
#include "avr_compiler.h"
#include "pmic_driver.h"
#include "TC_driver.h"
#include "clksys_driver.h"
#include "sleepConfig.h"
#include "port_driver.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "stack_macros.h"

#include "mem_check.h"

#include "init.h"
#include "utils.h"
#include "errorHandler.h"
#include "NHD0420Driver.h"

#include "ButtonHandler.h"


extern void vApplicationIdleHook( void );
//void vLedBlink(void *pvParameters);
void vButtonTask(void *pvParameters);

// Led blinks tasks which contain each two leds
void vLedPairOne( void *pvParameters ); 
void vLedPairTwo( void *pvParameters ); 
void vLedPairThree( void *pvParameters ); 
void vLedPairFour( void *pvParameters ); 

//TaskHandle_t ledTask;

// Create Taskhandles for all tasks
#define TASK_STATE_MAX ( 5 )
typedef enum
{
	handleLed1 = 0, 
	handleLed2 = 1, 
	handleLed3 = 2, 
	handleLed4 = 3,	
	
}enTaskHandle;

typedef struct 
{
	TaskHandle_t handle; 	
	
}st_task_state_t;

st_task_state_t task_state[ TASK_STATE_MAX ]; 


void vApplicationIdleHook( void )
{	
	
}

int main(void)
{
	BaseType_t task_status;
	
    resetReason_t reason = getResetReason();

	vInitClock();
	vInitDisplay();
				 
	task_status = xTaskCreate( vButtonTask,				// Funktions Name
							(const char *) "btTask",	// Task Name
							configMINIMAL_STACK_SIZE,	// Stack gr�sse
							NULL,						// �bergabe Wert
							3,							// Prio
							NULL);						// Handle
	configASSERT( task_status == pdPASS );				// Pr�fen ob der Task korrekt erstellt wurde
	
	task_status = xTaskCreate( vLedPairOne,
							(const char *) "ldTask1",
							configMINIMAL_STACK_SIZE,
							NULL,
							1,
							&task_state[handleLed1].handle);
	configASSERT( task_status == pdPASS ); 
	
	task_status = xTaskCreate( vLedPairTwo,
							(const char *) "ldTask2",
							configMINIMAL_STACK_SIZE,
							NULL,
							1,
							&task_state[handleLed2].handle);
	configASSERT( task_status == pdPASS ); 
	
	task_status = xTaskCreate( vLedPairThree,
							(const char *) "ldTask3",
							configMINIMAL_STACK_SIZE,
							NULL,
							1,
							&task_state[handleLed3].handle);
	configASSERT( task_status == pdPASS ); 
	
	task_status = xTaskCreate( vLedPairFour,
							(const char *) "ldTask4",
							configMINIMAL_STACK_SIZE,
							NULL,
							1,
							&task_state[handleLed4].handle);
	configASSERT( task_status == pdPASS ); 
	
	vDisplayClear();
	vDisplayWriteStringAtPos(0,0,"FreeRTOS 10.0.1");
	vDisplayWriteStringAtPos(1,0,"EDUBoard 1.0");
	vDisplayWriteStringAtPos(2,0,"Template");
	vDisplayWriteStringAtPos(3,0,"ResetReason: %d", reason);
	vTaskStartScheduler( );
	return 0;
}


void vButtonTask(void *pvParameters) 
{
	initButtons();
	vTaskDelay(3000);
	vDisplayClear();
	vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
	vDisplayWriteStringAtPos(1,0, "LastPress: None");
	vDisplayWriteStringAtPos(2,0, "Type: Not Pressed");
	
	for(;;) 
	{
		updateButtons();
		
		if(getButtonPress(BUTTON1) == SHORT_PRESSED) 
		{
			vDisplayClear();
			vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
			vDisplayWriteStringAtPos(1,0, "LastPress: Button1");
			vDisplayWriteStringAtPos(2,0, "Type: Short");
			vTaskResume( task_state[handleLed1].handle ); 
		}
		if(getButtonPress(BUTTON2) == SHORT_PRESSED) 
		{
			vDisplayClear();
			vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
			vDisplayWriteStringAtPos(1,0, "LastPress: Button2");
			vDisplayWriteStringAtPos(2,0, "Type: Short");
			vTaskResume( task_state[handleLed2].handle ); 
		}
		if(getButtonPress(BUTTON3) == SHORT_PRESSED) 
		{
			vDisplayClear();
			vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
			vDisplayWriteStringAtPos(1,0, "LastPress: Button3");
			vDisplayWriteStringAtPos(2,0, "Type: Short");
			vTaskResume( task_state[handleLed3].handle ); 
		}
		if(getButtonPress(BUTTON4) == SHORT_PRESSED) 
		{
			vDisplayClear();
			vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
			vDisplayWriteStringAtPos(1,0, "LastPress: Button4");
			vDisplayWriteStringAtPos(2,0, "Type: Short");
			vTaskResume( task_state[handleLed4].handle ); 
		}
		if(getButtonPress(BUTTON1) == LONG_PRESSED) 
		{
			vDisplayClear();
			vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
			vDisplayWriteStringAtPos(1,0, "LastPress: Button1");
			vDisplayWriteStringAtPos(2,0, "Type: Long");
		}
		if(getButtonPress(BUTTON2) == LONG_PRESSED) 
		{
			vDisplayClear();
			vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
			vDisplayWriteStringAtPos(1,0, "LastPress: Button2");
			vDisplayWriteStringAtPos(2,0, "Type: Long");
		}
		if(getButtonPress(BUTTON3) == LONG_PRESSED) 
		{
			vDisplayClear();
			vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
			vDisplayWriteStringAtPos(1,0, "LastPress: Button3");
			vDisplayWriteStringAtPos(2,0, "Type: Long");
		}
		if(getButtonPress(BUTTON4) == LONG_PRESSED) 
		{
			vDisplayClear();
			vDisplayWriteStringAtPos(0,0, "Button-DemoProgram");
			vDisplayWriteStringAtPos(1,0, "LastPress: Button4");
			vDisplayWriteStringAtPos(2,0, "Type: Long");
		}
		vTaskDelay((1000/BUTTON_UPDATE_FREQUENCY_HZ)/portTICK_RATE_MS);
	}
}
	
// Led blinks tasks which contain each two leds
void vLedPairOne( void *pvParameters )
{
	/* Parameters not used in this task. */
	( void ) pvParameters;
	
	PORTF.DIRSET = (1 << 1) | (1 << 0); /*LED2 & LED2*/
	PORTF.OUTCLR = 0x03;
	
	for(;;)
	{
		vTaskSuspend( NULL );	//Suspend it self
		PORTF.OUTTGL = 0x03;				
		vTaskDelay(100 / portTICK_RATE_MS);  
	}
}

void vLedPairTwo( void *pvParameters )
{
	/* Parameters not used in this task. */
	( void ) pvParameters;
		
	PORTF.DIRSET = (1 << 3) | (1 << 2) ; /*LED4 & LED3*/
	PORTF.OUTCLR = 0x0C;
		
	for(;;)
	{
		vTaskSuspend( NULL );	//Suspend it self 
		PORTF.OUTTGL = 0x0C;
		vTaskDelay(100 / portTICK_RATE_MS);	
	}
}

void vLedPairThree( void *pvParameters )
{
	/* Parameters not used in this task. */
	( void ) pvParameters;
		
	PORTE.DIRSET = (1 << 1) | (1 << 0); /*LED6 & LED5*/
	PORTE.OUTCLR = 0x03;
	
	for(;;)
	{
		vTaskSuspend( NULL );	//Suspend it self
		PORTE.OUTTGL = 0x03;						
		vTaskDelay(100 / portTICK_RATE_MS);	  
	}
}

void vLedPairFour( void *pvParameters )
{
	/* Parameters not used in this task. */
	( void ) pvParameters;
		
	PORTE.DIRSET = (1 << 3) | (1 << 2) ; /*LED8 & LED7*/
	PORTE.OUTCLR = 0x0C;
	
	for(;;)
	{
		vTaskSuspend( NULL );	//Suspend it self 
		PORTE.OUTTGL = 0x0C;
		vTaskDelay(100 / portTICK_RATE_MS); 
	}
}