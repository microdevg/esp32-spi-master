#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>
#include "lvgl.h"
#include "esp_driver.h"
#include "gui_simple.h"

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define BYTES_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565)) 
#define BUFF_SIZE (128 * 128 * BYTES_PER_PIXEL)

lv_display_t* display1;

uint32_t getTime(){
    uint32_t ret =  (xTaskGetTickCount() );
    return ret;
}





  void my_flush_cb_with_DMA(lv_display_t * display, const lv_area_t * area, uint8_t * px_map)
  {
      uint16_t *buf16 = (uint16_t *)px_map;  // Convertir el buffer a 16 bits para facilitar el acceso a cada píxel.
      uint32_t pixel_count = (area->y2 - area->y1 + 1) * (area->x2 - area->x1 + 1);
      for (uint32_t i = 0; i < pixel_count; i++) {
          uint16_t color = buf16[i];
          uint16_t inverted_color = ((color & 0xFF00) >> 8) | ((color & 0x00FF) << 8);
          buf16[i] = inverted_color;  // Actualizar el valor del píxel con el color invertido
      }
      ST7735_DrawImage(0,0,128,128,buf16);
      lv_display_flush_ready(display);
  }



  void display_task(void *pvParameter){
    ST7735_Init();
    void *buf_1 = spi_bus_dma_memory_alloc(SPI_ST7735, BUFF_SIZE, 0);
    assert(buf_1);
    void *buf_2 = spi_bus_dma_memory_alloc(SPI_ST7735, BUFF_SIZE, 0);
    assert(buf_2);
    lv_init();
    lv_tick_set_cb(getTime);
    display1 = lv_display_create(128,128);
    lv_display_set_buffers(display1, buf_1, buf_2, BUFF_SIZE,LV_DISPLAY_RENDER_MODE_FULL);
    lv_display_set_flush_cb(display1, my_flush_cb_with_DMA);
    create_ui();
    printf("Init program \n");
    uint32_t time_till_next_ms = 0;
      while(1){ 
          time_till_next_ms = lv_timer_handler();
          time_till_next_ms = MAX(time_till_next_ms, (1000 / CONFIG_FREERTOS_HZ));
          vTaskDelay(time_till_next_ms/portTICK_PERIOD_MS);
      }
  }

void app_main(void)
{
  printf("init main\n");
  xTaskCreate(display_task, "display_task", 4096, NULL, 5, NULL);
  int counter = 0;
    while(1){
      printf("counter:%d\n",counter++);
      vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
