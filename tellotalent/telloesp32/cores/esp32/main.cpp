#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "Arduino.h"

TaskHandle_t loopTaskHandle = NULL;

#if CONFIG_AUTOSTART_ARDUINO

bool loopTaskWDTEnabled;

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

void loopTask(void *pvParameters)
{
    setup();
    for(;;) {
        if(loopTaskWDTEnabled){
            esp_task_wdt_reset();
        }
        loop();
        yield();
    }
}

extern "C" void app_main()
{
    loopTaskWDTEnabled = false;
    initArduino();
    xTaskCreateUniversal(loopTask, "loopTask", 8192, NULL, 1, &loopTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);
}

#endif
