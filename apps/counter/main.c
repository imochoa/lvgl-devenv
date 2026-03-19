#include "sim_harness.h"
#include <stdio.h>

static lv_obj_t *counter_label;
static uint32_t  seconds;
static char      buf[16];

static void timer_cb(lv_timer_t *t)
{
    (void)t;
    seconds++;
    snprintf(buf, sizeof(buf), "%u s", seconds);
    lv_label_set_text(counter_label, buf);
}

void app_create_ui(void)
{
    lv_obj_t *title = lv_label_create(lv_scr_act());
    lv_label_set_text(title, "Counter");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 5);

    counter_label = lv_label_create(lv_scr_act());
    lv_label_set_text(counter_label, "0 s");
    lv_obj_align(counter_label, LV_ALIGN_CENTER, 0, 0);

    lv_timer_create(timer_cb, 1000, NULL);
}
