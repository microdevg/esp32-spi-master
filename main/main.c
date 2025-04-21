#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>
#include <driver/spi_master.h>



#define SPI_MISO                                (25)
#define SPI_MOSI                                (23)
#define SPI_CS_DEV1                             (22)
#define SPI_CS_DEV2                             (21)
#define SPI_SCK                                 (19)
#define NOT_USE_PIN                             (-1)
#define SPI_HOST                                (SPI3_HOST)


uint32_t counter = 0;

spi_bus_config_t bus_config = {
    .miso_io_num   = SPI_MISO,
    .mosi_io_num   = SPI_MOSI,
    .sclk_io_num   = SPI_SCK,
    .quadwp_io_num = NOT_USE_PIN,
    .quadhd_io_num = NOT_USE_PIN

};

spi_device_handle_t device1 = {};
spi_device_handle_t device2 = {};



void tr_pre(spi_transaction_t *trans){ counter ++; }
void tr_pos(spi_transaction_t *trans){ counter ++;}


void app_main(void)
{

    printf("Init program \n");
    printf("Iniciamos el periférico\n");
    esp_err_t err  = ESP_OK;
    err = spi_bus_initialize(SPI_HOST,&bus_config,SPI_DMA_CH_AUTO);

    spi_device_interface_config_t dev1_config ={
        .clock_speed_hz = 50000,
        .mode = 0,
        .clock_source = SPI_CLK_SRC_DEFAULT, //
        .spics_io_num = SPI_CS_DEV1,
        .queue_size = 10,
        .pre_cb = tr_pre,
        .post_cb=tr_pos,
        .flags = SPI_DEVICE_NO_DUMMY
    };

    spi_device_interface_config_t dev2_config ={
        .clock_speed_hz = 10000,
        .mode = 0,
        .clock_source = SPI_CLK_SRC_DEFAULT, //
        .spics_io_num = SPI_CS_DEV2,
        .queue_size = 10,
        .pre_cb = NULL,
        .post_cb=NULL,
        .flags = SPI_DEVICE_NO_DUMMY
    };
    //Agrego el dispositivo
    printf("Agregar un nuevo dispositivo al bus spi\n");
    spi_bus_add_device(SPI_HOST,&dev1_config,&device1);
    spi_bus_add_device(SPI_HOST,&dev2_config,&device2);

    #define MAX_LEN         100
    #define MSG_FORMAT      "Contador:%lu\n"

    uint8_t buffer[MAX_LEN]={0};
    size_t len = 0;
    
    spi_transaction_t tr = {
        .flags  = 0x00,
        .length = len,
        .tx_buffer = buffer
    };
    
    
    while(1){
        sprintf((char*)buffer,MSG_FORMAT,counter);
        len = strlen((char*)buffer);
        printf((char*)buffer);
        tr.length = 8 * len;
        tr.tx_buffer = buffer;
        spi_device_transmit(device1,&tr);
        vTaskDelay(10/portTICK_PERIOD_MS);
        spi_device_transmit(device2,&tr);

        vTaskDelay(1000/ portTICK_PERIOD_MS);        
    }
}