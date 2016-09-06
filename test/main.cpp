#include "joltikit/joltikit.h"
#include "LSM9DS0.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

using namespace etk;


constexpr uint16 STACK_SIZE = 2000;
StaticTask_t sensor_task_buffer;
StackType_t sensor_stack[STACK_SIZE];

void sensor_task(void* param)
{
	while(true)
	{
		Serial1.print("Hello world!\r\n");
		etk::sleep_ms(100);
	}
}


int main(void)
{
    clock_setup();
    
    Serial1.begin(57600);

	
	TaskHandle_t xHandle = NULL;
    auto xReturned = xTaskCreateStatic(
                    sensor_task,
                    "sensor_task",
                    STACK_SIZE,
                    nullptr,
                    5,
                    sensor_stack,
                    &sensor_task_buffer);
	
	vTaskStartScheduler();
}



