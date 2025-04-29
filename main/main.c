#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>
#include <driver/spi_master.h>
#include "esp_driver.h"


  


void app_main(void)
{

    ST7735_Init();

    printf("Init program \n");

    
    while(1){
     loop();
        printf("loop\n");
        vTaskDelay(1000/ portTICK_PERIOD_MS);        
    }
}