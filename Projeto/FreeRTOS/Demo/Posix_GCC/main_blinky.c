/*
 * FreeRTOS V202012.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/******************************************************************************
 * NOTE 1: The FreeRTOS demo threads will not be running continuously, so
 * do not expect to get real time behaviour from the FreeRTOS Linux port, or
 * this demo application.  Also, the timing information in the FreeRTOS+Trace
 * logs have no meaningful units.  See the documentation page for the Linux
 * port for further information:
 * https://freertos.org/FreeRTOS-simulator-for-Linux.html
 *
 * NOTE 2:  This project provides two demo applications.  A simple blinky style
 * project, and a more comprehensive test and demo application.  The
 * mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting in main.c is used to select
 * between the two.  See the notes on using mainCREATE_SIMPLE_BLINKY_DEMO_ONLY
 * in main.c.  This file implements the simply blinky version.  Console output
 * is used in place of the normal LED toggling.
 *
 * NOTE 3:  This file only contains the source code that is specific to the
 * basic demo.  Generic functions, such FreeRTOS hook functions, are defined
 * in main.c.
 ******************************************************************************
 *
 * main_blinky() creates one queue, one software timer, and two tasks.  It then
 * starts the scheduler.
 *
 * The Queue Send Task:
 * The queue send task is implemented by the prvQueueSendTask() function in
 * this file.  It uses vTaskDelayUntil() to create a periodic task that sends
 * the value 100 to the queue every 200 milliseconds (please read the notes
 * above regarding the accuracy of timing under Linux).
 *
 * The Queue Send Software Timer:
 * The timer is an auto-reload timer with a period of two seconds.  The timer's
 * callback function writes the value 200 to the queue.  The callback function
 * is implemented by prvQueueSendTimerCallback() within this file.
 *
 * The Queue Receive Task:
 * The queue receive task is implemented by the prvQueueReceiveTask() function
 * in this file.  prvQueueReceiveTask() waits for data to arrive on the queue.
 * When data is received, the task checks the value of the data, then outputs a
 * message to indicate if the data came from the queue send task or the queue
 * send software timer.
 *
 * Expected Behaviour:
 * - The queue send task writes to the queue every 200ms, so every 200ms the
 *   queue receive task will output a message indicating that data was received
 *   on the queue from the queue send task.
 * - The queue send software timer has a period of two seconds, and is reset
 *   each time a key is pressed.  So if two seconds expire without a key being
 *   pressed then the queue receive task will output a message indicating that
 *   data was received on the queue from the queue send software timer.
 *
 * NOTE:  Console input and output relies on Linux system calls, which can
 * interfere with the execution of the FreeRTOS Linux port. This demo only
 * uses Linux system call occasionally. Heavier use of Linux system calls
 * may crash the port.
 */

#include <stdio.h>
#include <pthread.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Local includes. */
#include "console.h"

/* Priorities at which the tasks are created. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define	mainQUEUE_SEND_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue.  The times are converted from
milliseconds to ticks using the pdMS_TO_TICKS() macro. */
#define mainTASK_SEND_FREQUENCY_MS			pdMS_TO_TICKS( 200UL )
#define mainTIMER_SEND_FREQUENCY_MS			pdMS_TO_TICKS( 2000UL )

/* The number of items the queue can hold at once. */
#define mainQUEUE_LENGTH					( 2 )

/* The values sent to the queue receive task from the queue send task and the
queue send software timer respectively. */
#define mainVALUE_SENT_FROM_TASK			( 100UL )
#define mainVALUE_SENT_FROM_TIMER			( 200UL )

/*-----------------------------------------------------------*/

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvQueueReceiveTask( void *pvParameters );
static void prvQueueSendTask( void *pvParameters );

/*
 * The callback function executed when the software timer expires.
 */
static void prvQueueSendTimerCallback( TimerHandle_t xTimerHandle );

/*-----------------------------------------------------------*/

/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;

/* A software timer that is started from the tick hook. */
static TimerHandle_t xTimer = NULL;

/*-----------------------------------------------------------*/

/*** SEE THE COMMENTS AT THE TOP OF THIS FILE ***/
#define mainDELAY_LOOP_COUNT   ( 0xfffff )

/* The task functions. */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );
void vTaskFunction( void *pvParameters);	

//#include "xparameters.h"

#include "scheduler.h"

static void vTestTask(void *pvParam){
    char* cParam = pvParam;
	int x = 0;
	scanf("%d",&x);
        if(('a' == *cParam) || (x == 1)){
            printf("Hello %c\r\n", *cParam);
        }else{
            printf("Imario\r\n");
        }
}

//TaskHandle_t xHandle = NULL;
//char cParam = 'Imario';

int main_blinky( void )
{
	/*vSchedulerInit();

    vSchedulerPeriodicTaskCreate(vTestTask, "Teste",configMINIMAL_STACK_SIZE, &cParam, 1, &xHandle, pdMS_TO_TICKS(50),pdMS_TO_TICKS(500),
    pdMS_TO_TICKS(100), pdMS_TO_TICKS(500) );

    vSchedulerStart();*/
}



/*void main_blinky( void )
{
static const char *text1 = "Task 1 is running\r\n";
static const char *text2 = "Task 2 is running\r\n";
xTaskCreate(vTaskFunction, "Task1", 1000, (void*) text1, 1, NULL);
xTaskCreate(vTaskFunction, "Task2", 1000, (void*) text2, 1, NULL);
    
vTaskStartScheduler();
while(1);

return 0;
}*/
/*-----------------------------------------------------------*/
void vTaskFunction( void *pvParameters ){
const char *pcTaskName;
volatile uint32_t ul;/* volatile to ensure ul is not optimized away. */

	pcTaskName = (char *) pvParameters;
    /* As per most tasks, this task is implemented in an infinite loop. */
    while(1){
        /* Print out the name of this task. */
        printf(pcTaskName);

        /* Delay for a period. */
        for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ ){
        /* This loop is just a very crude delay implementation.  
        There isnothing to do in here.  
        Later exampleswill replace this crudeloop with a proper delay/sleep function. */
        }
    }
}
void vTask1( void *pvParameters ){
//const char *pcTaskName = "Task 1 is running\r\n";
volatile uint32_t ul;/* volatile to ensure ul is not optimized away. */
xTaskCreate(vTask2, "Task2", 1000, NULL, 1, NULL);
    /* As per most tasks, this task is implemented in an infinite loop. */
    while(1){
        /* Print out the name of this task. */
        printf("Task 1 is running\r\n");

        /* Delay for a period. */
        for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ ){
        /* This loop is just a very crude delay implementation.  
        There isnothing to do in here.  
        Later exampleswill replace this crudeloop with a proper delay/sleep function. */
        }
    }
}
void vTask2( void *pvParameters ){
//const char *pcTaskName = "Task 2 is running\r\n";
volatile uint32_t ul;/* volatile to ensure ul is not optimized away. */

    /* As per most tasks, this task is implemented in an infinite loop. */
    while(1){
        /* Print out the name of this task. */
        printf("Task 2 is running\r\n");
        /* Delay for a period. */
        for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ ){
        /* This loop is just a very crude delay implementation.  
        There isnothing to do in here.  
        Later exampleswill replace this crudeloop with a proper delay/sleep function. */
        }
    }
}

static void prvQueueSendTask( void *pvParameters )
{
TickType_t xNextWakeTime;
const TickType_t xBlockTime = mainTASK_SEND_FREQUENCY_MS;
const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TASK;

	/* Prevent the compiler warning about the unused parameter. */
	( void ) pvParameters;

	/* Initialise xNextWakeTime - this only needs to be done once. */
	xNextWakeTime = xTaskGetTickCount();

	for( ;; )
	{
		/* Place this task in the blocked state until it is time to run again.
		The block time is specified in ticks, pdMS_TO_TICKS() was used to
		convert a time specified in milliseconds into a time specified in ticks.
		While in the Blocked state this task will not consume any CPU time. */
		vTaskDelayUntil( &xNextWakeTime, xBlockTime );

		/* Send to the queue - causing the queue receive task to unblock and
		write to the console.  0 is used as the block time so the send operation
		will not block - it shouldn't need to block as the queue should always
		have at least one space at this point in the code. */
		xQueueSend( xQueue, &ulValueToSend, 0U );
	}
}
/*-----------------------------------------------------------*/

static void prvQueueSendTimerCallback( TimerHandle_t xTimerHandle )
{
const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TIMER;

	/* This is the software timer callback function.  The software timer has a
	period of two seconds and is reset each time a key is pressed.  This
	callback function will execute if the timer expires, which will only happen
	if a key is not pressed for two seconds. */

	/* Avoid compiler warnings resulting from the unused parameter. */
	( void ) xTimerHandle;

	/* Send to the queue - causing the queue receive task to unblock and
	write out a message.  This function is called from the timer/daemon task, so
	must not block.  Hence the block time is set to 0. */
	xQueueSend( xQueue, &ulValueToSend, 0U );
}
/*-----------------------------------------------------------*/

static void prvQueueReceiveTask( void *pvParameters )
{
uint32_t ulReceivedValue;

	/* Prevent the compiler warning about the unused parameter. */
	( void ) pvParameters;

	for( ;; )
	{
		/* Wait until something arrives in the queue - this task will block
		indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
		FreeRTOSConfig.h.  It will not use any CPU time while it is in the
		Blocked state. */
		xQueueReceive( xQueue, &ulReceivedValue, portMAX_DELAY );

		/* To get here something must have been received from the queue, but
		is it an expected value?  Normally calling printf() from a task is not
		a good idea.  Here there is lots of stack space and only one task is
		using console IO so it is ok.  However, note the comments at the top of
		this file about the risks of making Linux system calls (such as
		console output) from a FreeRTOS task. */
		if( ulReceivedValue == mainVALUE_SENT_FROM_TASK )
		{
			console_print( "Message received from task\n" );
		}
		else if( ulReceivedValue == mainVALUE_SENT_FROM_TIMER )
		{
			console_print( "Message received from software timer\n" );
		}
		else
		{
			console_print( "Unexpected message\n" );
		}
	}
}
/*-----------------------------------------------------------*/
