/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Local includes. */
#include "console.h"

#define    BLINKY_DEMO       0
#define    FULL_DEMO         1

#define mainSELECTED_APPLICATION BLINKY_DEMO

/* This demo uses heap_3.c (the libc provided malloc() and free()). */

/*-----------------------------------------------------------*/
extern void main_blinky( void );
extern void main_full( void );
extern void main_tcp_echo_client_tasks( void );
static void traceOnEnter( void );
/*
 * Only the comprehensive demo uses application hook (callback) functions.  See
 * http://www.freertos.org/a00016.html for more information.
 */
void vFullDemoTickHookFunction( void );
void vFullDemoIdleFunction( void );

/*
 * Prototypes for the standard FreeRTOS application hook (callback) functions
 * implemented within this file.  See http://www.freertos.org/a00016.html .
 */
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask,
                                    char *pcTaskName );
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize );
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize );

/*
 * Writes trace data to a disk file when the trace recording is stopped.
 * This function will simply overwrite any trace files that already exist.
 */
static void prvSaveTraceFile( void );

/*-----------------------------------------------------------*/

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
use a callback function to optionally provide the memory required by the idle
and timer tasks.  This is the stack that will be used by the timer task.  It is
declared here, as a global, so it can be checked by a test that is implemented
in a different file. */
StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

/* Notes if the trace is running or not. */
static BaseType_t xTraceRunning = pdTRUE;

/*--------------- Inicio do projeto --------------------------------------------*/
#define CTRL_ALG_LOAD           0x1
#define HTTP_REQUEST_PROC_LOAD  0xf
#define RS232_CHAR_PROC_LOAD    0x1
volatile TickType_t ulIdleCycleCount = 0UL;

/*-------------- Inclusão da biblioteca do ESFree ---------------*/
#include "scheduler.h"
#include <stdlib.h>
#include <string.h>

/*------------ Definição das funções ----------------------*/
void LEDTask(void *pvParameters);
void vToggleLED(char led[]);
void SendData(void *pvParameters);
void KeypadScanningTask(void *pvParameters);
void ReceiveData(void *pvParameters);
void PlantControlTask( void *pvParameters );
void createAperiodcJob(void *pvParam);
static void WebServerTask( void *pvParameters );
static void RS232Task( void *pvParameters );
/*-----------------------------------------------------------*/
/*--------------- Handles -----------------------------------*/
TaskHandle_t xHandle = NULL;
TaskHandle_t xHandle2 = NULL;
TaskHandle_t xHandle3 = NULL;
TaskHandle_t xHandle4 = NULL;
TaskHandle_t xHandle5 = NULL;
/*-----------------------------------------------------------*/

/*-------------------- Queues --------------*/
QueueHandle_t xDataQueue = NULL;
/*------------------------------------------*/

int main( void )
{
   /* Do not include trace code when performing a code coverage analysis. */
    #if ( projCOVERAGE_TEST != 1 )
    {
        /* Initialise the trace recorder.  Use of the trace recorder is optional.
        See http://www.FreeRTOS.org/trace for more information. */
       vTraceEnable( TRC_START );

        /* Start the trace recording - the recording is written to a file if
        configASSERT() is called. */
        uiTraceStart();
    }
    #endif
    /*-------------------- Criação da fila ----------------*/
    xDataQueue = xQueueCreate(2,sizeof(int));
    /*----------------------------------------------------*/
    //uiTraceStart();
    vSchedulerInit();

    
    vSchedulerPeriodicTaskCreate(PlantControlTask, "receivedata",configMINIMAL_STACK_SIZE, NULL, 2, &xHandle,
    /*Fase*/ pdMS_TO_TICKS(50),
    /*Periodo*/pdMS_TO_TICKS(15),
    /*WCET*/pdMS_TO_TICKS(700), 
    /*Deadline*/pdMS_TO_TICKS(1000) );
 
    vSchedulerPeriodicTaskCreate(SendData, "sendata",configMINIMAL_STACK_SIZE, NULL, 0, &xHandle2,
    /*Fase*/ pdMS_TO_TICKS(50),/*Periodo*/pdMS_TO_TICKS(15),
    /*WCET*/pdMS_TO_TICKS(300),/*Deadline*/ pdMS_TO_TICKS(500) );

    vSchedulerPeriodicTaskCreate(KeypadScanningTask, "keypad",configMINIMAL_STACK_SIZE, NULL, 1, &xHandle3
    ,/*Fase*/ pdMS_TO_TICKS(50),/*Periodo*/ pdMS_TO_TICKS(15),
    /*WCET*/pdMS_TO_TICKS(100),/*Deadline*/ pdMS_TO_TICKS(1000) );

    vSchedulerPeriodicTaskCreate(LEDTask, "ledtask",configMINIMAL_STACK_SIZE, NULL, 0, &xHandle4
    ,/*Fase*/ pdMS_TO_TICKS(50),/*Periodo*/pdMS_TO_TICKS(1000),
    /*WCET*/pdMS_TO_TICKS(100),/*Deadline*/ pdMS_TO_TICKS(500) );

    vSchedulerPeriodicTaskCreate(createAperiodcJob, "Aperiodic",configMINIMAL_STACK_SIZE, NULL, 0, &xHandle5
    ,/*Fase*/ pdMS_TO_TICKS(50),/*Periodo*/pdMS_TO_TICKS(1000),
    /*WCET*/pdMS_TO_TICKS(100),/*Deadline*/ pdMS_TO_TICKS(200) );

    vSchedulerStart();
    return 0;
}
/*-------------------------------------------------------------------*/
/*--------------- Implementações das funções do projeto ------------*/


#define DELAY_PERIOD 1000

void PlantControlTask( void *pvParameters ){
    int data_sensor_1;
    int data_sensor_2;
    if(xQueueReceive(xDataQueue,&data_sensor_1,pdMS_TO_TICKS(10)) == pdTRUE){
            if(xQueueReceive(xDataQueue,&data_sensor_2,pdMS_TO_TICKS(10)) == pdTRUE){
              for(int i = 0; i < CTRL_ALG_LOAD; i++){
              }
            }else{

            }
    }else{

    }
}

void SendData(void *pvParameters){
    int data_sensor_1 = rand() % 100;
    int data_sensor_2 = rand() % 100;
    if(data_sensor_1 <= 90){
        xQueueSend(xDataQueue,&data_sensor_1,portMAX_DELAY);
    }
    if(data_sensor_2 <= 90){
        xQueueSend(xDataQueue,&data_sensor_2,portMAX_DELAY);
    }
}

void KeypadScanningTask(void *pvParameters){
    TickType_t xLastWakeTime;
    char k;
    xLastWakeTime = xTaskGetTickCount();
    
    if(KeyPressed(&k) == pdTRUE){
        vTaskDelayUntil( &xLastWakeTime,pdMS_TO_TICKS(50) );
        UpdateDisplay(k);
    }

}
void UpdateDisplay(char Key ){
    printf("Tecla pressionada: %c.\n", Key);
    printf("------------------------\n");
}
int  KeyPressed(char *k){
    char key[12];
    key[0] = '>';
    key[1] = '<';
    int num = 48;
    for(int i = 2; i < 12;i++){
        key[i] = num;
        num++;
    }
    if((rand() % 100) > 98){
        *k = key[rand() % 12];
        return pdTRUE;
    }else{
        return pdFALSE;
    }
}
void LEDTask(void *pvParameters){
        if(xGetUsageCPU() > 85)
        {   

            vToggleLED("Vermelho");
        }
        else
        {   
            printf("Uso da CPU %d\n", xGetUsageCPU());
            vToggleLED("Verde");
        }
    ulTaskClearIdleRunTimeCounter();     
}

static void WebServerTask(void *pvParam){
  printf("HTTP!\n");
  printf("------------------------\n");
    for(int i = 0; i < HTTP_REQUEST_PROC_LOAD; i++){

    }
}
static void RS232Task(void *pvParam){
    printf("PDA Acessado!\n");
    printf("------------------------\n");
    for(int i = 0; i < RS232_CHAR_PROC_LOAD; i++){

    }
}
void createAperiodcJob(void *pvParam){
    srand(times(NULL));
    if((rand() % 100) > 70){
        vSchedulerAperiodicJobCreate( RS232Task, "RS232", NULL, pdMS_TO_TICKS( 100 ) );
    }
    if((rand() % 100) > 85){
        vSchedulerAperiodicJobCreate( WebServerTask, "WebServer", NULL, pdMS_TO_TICKS( 100 ) );
    }   

}

void vToggleLED(char led[]){
    printf("LED %s ON!\n", led);
    vTaskDelay(pdMS_TO_TICKS(25));
    printf("LED %s OFF!\n", led);
    vTaskDelay(pdMS_TO_TICKS(25));
    printf("------------------------\n"); 
}

int xGetUsageCPU(){
    //((1000 - time_idle)/1000)*100)
    return (100ul - (ulTaskGetIdleRunTimeCounter()/10));
}

void vApplicationMallocFailedHook( void )
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c, heap_2.c or heap_4.c is being used, then the
    size of the    heap available to pvPortMalloc() is defined by
    configTOTAL_HEAP_SIZE in FreeRTOSConfig.h, and the xPortGetFreeHeapSize()
    API function can be used to query the size of free heap space that remains
    (although it does not provide information on how the remaining heap might be
    fragmented).  See http://www.freertos.org/a00111.html for more
    information. */
    vAssertCalled( __FILE__, __LINE__ );
}
/*-----------------------------------------------------------*/


void vApplicationIdleHook( void )
{
    ulIdleCycleCount++;
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
    to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
    task.  It is essential that code added to this hook function never attempts
    to block in any way (for example, call xQueueReceive() with a block time
    specified, or call vTaskDelay()).  If application tasks make use of the
    vTaskDelete() API function to delete themselves then it is also important
    that vApplicationIdleHook() is permitted to return to its calling function,
    because it is the responsibility of the idle task to clean up memory
    allocated by the kernel to any task that has since deleted itself. */

    /*
    usleep(15000);
    traceOnEnter();*/

    #if ( mainSELECTED_APPLICATION == FULL_DEMO )
    {
        /* Call the idle task processing used by the full demo.  The simple
        blinky demo does not use the idle task hook. */
        vFullDemoIdleFunction();
    }
    #endif
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask,
                                    char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected.  This function is
    provided as an example only as stack overflow checking does not function
    when running the FreeRTOS POSIX port. */
    vAssertCalled( __FILE__, __LINE__ );
}
/*-----------------------------------------------------------*/


void traceOnEnter()
{
    int ret;
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    ret = select(1, &fds, NULL, NULL, &tv);
    if ( ret > 0 )
    {
    if( xTraceRunning == pdTRUE )
    {
        prvSaveTraceFile();
    }
    /* clear the buffer */
    char buffer[200];
    read(1, &buffer, 200);
    }
}

void vLoggingPrintf( const char *pcFormat,
                     ... )
{
va_list arg;

    va_start( arg, pcFormat );
    vprintf( pcFormat, arg );
    va_end( arg );
}
/*-----------------------------------------------------------*/

void vApplicationDaemonTaskStartupHook( void )
{
    /* This function will be called once only, when the daemon task starts to
    execute    (sometimes called the timer task).  This is useful if the
    application includes initialisation code that would benefit from executing
    after the scheduler has been started. */
}
/*-----------------------------------------------------------*/

void vAssertCalled( const char * const pcFileName,
                    unsigned long ulLine )
{
static BaseType_t xPrinted = pdFALSE;
volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;

    /* Called if an assertion passed to configASSERT() fails.  See
    http://www.freertos.org/a00110.html#configASSERT for more information. */

    /* Parameters are not used. */
    ( void ) ulLine;
    ( void ) pcFileName;


    taskENTER_CRITICAL();
    {
        /* Stop the trace recording. */
        if( xPrinted == pdFALSE )
        {
            xPrinted = pdTRUE;

            if( xTraceRunning == pdTRUE )
            {
                prvSaveTraceFile();
            }
        }

        /* You can step out of this function to debug the assertion by using
        the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
        value. */
        while( ulSetToNonZeroInDebuggerToContinue == 0 )
        {
            __asm volatile ( "NOP" );
            __asm volatile ( "NOP" );
        }
    }
    taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/

static void prvSaveTraceFile( void )
{
    /* Tracing is not used when code coverage analysis is being performed. */
    #if ( projCOVERAGE_TEST != 1 )
    {
    FILE * pxOutputFile;

        vTraceStop();

        pxOutputFile = fopen( "Trace.dump", "wb" );

        if( pxOutputFile != NULL )
        {
            fwrite( RecorderDataPtr, sizeof( RecorderDataType ), 1, pxOutputFile );
            fclose( pxOutputFile );
            printf( "\r\nTrace output saved to Trace.dump\r\n" );
        }
        else
        {
            printf( "\r\nFailed to create trace dump file\r\n" );
        }
    }
    #endif /* if ( projCOVERAGE_TEST != 1 ) */
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

