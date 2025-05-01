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
    loop();
    char buffer[20]={0};
    int counter =0;
    while(1){
        sprintf(buffer,"Counter:%d",counter);
        printf("loop\n");
        ST7735_WriteString(0,20,buffer,Font_11x18 , ST7735_WHITE, ST7735_RED);

        vTaskDelay(1000/ portTICK_PERIOD_MS);        
        counter++;
    }
}