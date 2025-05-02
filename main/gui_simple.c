

#include "gui_simple.h"
#include <stdio.h>
#include <string.h>
#include "lvgl.h"
#include <stdbool.h>
#include <driver/gpio.h>


int counter = 0;
uint8_t buffer[20]={0};
/* Función de callback para el evento del botón */
void btn_event_handler(lv_event_t *e) {
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        lv_obj_t *btn = lv_event_get_target(e);
        counter++;
        sprintf((char*)buffer,"%d",counter);
        lv_label_set_text(lv_obj_get_child(btn, 0), (char*)buffer);
    }
}


void button_read_cb(lv_indev_t * indev, lv_indev_data_t * data) {
    bool current_state = gpio_get_level(0);
    
    data->state = (current_state == 0) ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

void create_ui(void) {
    gpio_set_direction(0,GPIO_MODE_INPUT);
    /* 1. Crear pantalla */
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_scr_load(screen);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x3498db), LV_PART_MAIN);

    /* 2. Crear y configurar botón */
    lv_obj_t *btn = lv_btn_create(screen);
    lv_obj_set_size(btn, 120, 50);
    lv_obj_center(btn); // Centramos el botón
    lv_obj_update_layout(btn); // <<--- 🔥 Forzar layout

    /* 3. Obtener coordenadas del botón */
    lv_area_t btn_coords;
    lv_obj_get_coords(btn, &btn_coords);
    lv_point_t btn_center = {
        .x = btn_coords.x1 + lv_obj_get_width(btn) / 2,
        .y = btn_coords.y1 + lv_obj_get_height(btn) / 2
    };

    /* 4. Definir puntos de botón para el input device */
    static lv_point_t btn_points[] = {
        {0, 0}, // Se sobrescribe
        {-1, -1}
    };
    btn_points[0] = btn_center;

    /* 5. Crear y configurar el dispositivo de entrada */
    lv_indev_t* indev_button = lv_indev_create();
    lv_indev_set_type(indev_button, LV_INDEV_TYPE_BUTTON);
    lv_indev_set_read_cb(indev_button, button_read_cb);
    lv_indev_set_button_points(indev_button, btn_points);

    /* 6. Configurar estilo y texto del botón */
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x2ecc71), LV_PART_MAIN);
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "Counter");
    lv_obj_center(label);

    /* 7. Añadir evento al botón */
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_CLICKED, NULL);
}