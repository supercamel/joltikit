#include "FreeRTOS.h"
#include "task.h"
#include "portable.h"
#include <etk/etk.h>


void etk_tick()
{
	etk::tick();
}

extern "C" {

void vPortSVCHandler( void );
void xPortPendSVHandler( void );
void xPortSysTickHandler( void );

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
	static StaticTask_t xIdleTaskTCB;
	static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
	static StaticTask_t xTimerTaskTCB;
	static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

void sv_call_handler(void)
{
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    	vPortSVCHandler();
}


void pend_sv_handler(void)
{
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
		xPortPendSVHandler();
}

void vPortSetupTimerInterrupt( void )
{

}


void sys_tick_handler(void)
{
	static int tick_count = 0;
    etk_tick();
    if((tick_count++) == 100)
    {
    	tick_count = 0;
		if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
		{
		    xPortSysTickHandler();
		}
    }
} 

}



