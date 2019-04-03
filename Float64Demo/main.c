/*
 * Float64Demo.c
 *
 * Created: 20.03.2018 18:32:07
 * Author : chaos
 */ 

//#include <avr/io.h>
#include <stdio.h>

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
#include "avr_f64.h"												//Library Include


extern void vApplicationIdleHook( void );
void float64TestTask(void *pvParameters);

void vApplicationIdleHook( void )
{	
	
}

int main(void)
{
    resetReason_t reason = getResetReason();

	vInitClock();
	vInitDisplay();	
	//Starten des float64TestTask. Braucht relativ viel Stack-Speicher, da die Library im Stack abgelegt wird. (Library ist nicht auf FreeRTOS ausgelegt)
	xTaskCreate( float64TestTask, (const char *) "float64tsk", configMINIMAL_STACK_SIZE+300, NULL, 1, NULL); 
	vTaskStartScheduler();
	return 0;
}

void float64TestTask(void *pvParameters) {
	(void) pvParameters;
	float64_t testvar1 = f_sd(2);									//Erstellen einer Double-Variable, Initialisiert mit dem Wert 2
	float64_t testvar2 = f_sd(3);
	char resultstring1[20];
	char resultstring2[20];
	for(;;) {
		testvar1 = f_mult(testvar1, f_sd(0.9999));					//Multiplikation zweier Double Variablen
		testvar2 = f_div(testvar2, f_sd(1.000001));					//Division zweier Double Variablen
		char* tempResultString = f_to_string(testvar1, 16, 16);		//Verwandeln einer Double-Variable in einen String
		sprintf(resultstring1, "1: %s", tempResultString);			//Einsetzen des Strings in einen anderen String
		tempResultString = f_to_string(testvar2, 16, 16);
		sprintf(resultstring2, "2: %s", tempResultString);
		vDisplayClear();											//Löschen des ganzen Displays
		vDisplayWriteStringAtPos(0,0,"Float64 Test");				//Ausgabe auf das Display
		vDisplayWriteStringAtPos(1,0,"%s", resultstring1);
		vDisplayWriteStringAtPos(2,0,"%s", resultstring2);
		vDisplayWriteStringAtPos(3,0,"2 as float: %f", f_ds(testvar2));
		vTaskDelay(400 / portTICK_RATE_MS);
	}
}
