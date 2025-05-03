

#include "gui_simple.h"
#include "lvgl.h"
#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define MAX_NOTIFICATIONS        3
#define MAX_LEN_NOTIFICATION    30

static lv_obj_t *notification_labels[MAX_NOTIFICATIONS];
static char texts[MAX_NOTIFICATIONS][MAX_LEN_NOTIFICATION];

static int notification_count = 0;
static lv_obj_t *screen = NULL;

/* Refresca los textos en pantalla según el arreglo texts[] */
void refresh_notifications(void) {
    for (int i = 0; i < MAX_NOTIFICATIONS; i++) {
        lv_label_set_text(notification_labels[i], texts[i]);
    }
    vTaskDelay(100/portTICK_PERIOD_MS);

}

/* Callback que elimina la notificación más antigua (posición 0) */
static void clear_notification_cb(lv_timer_t *timer) {
    printf("elimino notificacion\n");
    if (notification_count > 0) {
        // Desplazar textos hacia arriba
        for (int i = 0; i < MAX_NOTIFICATIONS - 1; i++) {
            strncpy(texts[i], texts[i + 1], MAX_LEN_NOTIFICATION);
        }

        // Limpiar el último
        texts[MAX_NOTIFICATIONS - 1][0] = '\0';

        notification_count--;
        refresh_notifications();
    }
    lv_timer_del(timer); // Eliminar el temporizador
   // if (notification_count == 0) lv_timer_del(timer); // Eliminar el temporizador
}

/* Crea la interfaz y los labels vacíos */
void create_ui(void) {
    screen = lv_obj_create(NULL);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);  // Desactiva scroll en screen
    lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF);
    lv_scr_load(screen);

    // Fondo blanco para toda la pantalla
    lv_obj_set_style_bg_color(screen, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    // Título con fondo blanco y borde negro
    lv_obj_t *title = lv_obj_create(screen);
    lv_obj_set_size(title, 128, 25);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(title, lv_color_hex(0xFFFFFF), LV_PART_MAIN); // fondo blanco
    lv_obj_set_style_border_color(title, lv_color_hex(0x000000), LV_PART_MAIN); // borde negro
    lv_obj_set_style_border_width(title, 1, LV_PART_MAIN);
    // Desactiva scroll del contenedor title
    lv_obj_clear_flag(title, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(title, LV_SCROLLBAR_MODE_OFF);

    lv_obj_t *label_title = lv_label_create(title);
    lv_label_set_text(label_title, "Notificaciones");
    lv_obj_clear_flag(label_title, LV_OBJ_FLAG_SCROLLABLE);  // Desactiva scroll en screen
    lv_obj_set_scrollbar_mode(label_title, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_text_color(label_title, lv_color_hex(0x000000), LV_PART_MAIN); // negro
    lv_obj_set_style_text_font(label_title, &lv_font_montserrat_14, 0);
    lv_obj_center(label_title);
    // Crear labels vacíos
    for (int i = 0; i < MAX_NOTIFICATIONS; i++) {
        notification_labels[i] = lv_label_create(screen);
        lv_label_set_text(notification_labels[i], "");
        lv_obj_set_style_text_color(notification_labels[i], lv_color_hex(0x555555), LV_PART_MAIN);
        lv_obj_set_style_text_font(notification_labels[i], &lv_font_montserrat_14, 0);
        lv_obj_align(notification_labels[i], LV_ALIGN_TOP_LEFT, 2, 40 + (i * 30));
    }
}

/* Agrega una nueva notificación */
void add_notification(const char *message) {
    if (notification_count >= MAX_NOTIFICATIONS) {
        printf("No hay espacio para más notificaciones\n");
        return;
    }

    // Copiar el mensaje al arreglo
    strncpy(texts[notification_count], message, MAX_LEN_NOTIFICATION - 1);
    texts[notification_count][MAX_LEN_NOTIFICATION - 1] = '\0';
    refresh_notifications();

    // Crear temporizador para eliminar la notificación más antigua
    lv_timer_t *timer = lv_timer_create(clear_notification_cb, 500, NULL);
    lv_timer_set_repeat_count(timer, 1);

    notification_count++;
}
