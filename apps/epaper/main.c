#include "sim_harness.h"
#include <stdio.h>

void app_create_ui(void)
{
    lv_disp_t *disp = lv_disp_get_default();
    lv_obj_t *screen = lv_scr_act();

    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Hello, LVGL!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    lv_disp_set_rotation(disp, LV_DISP_ROT_90);
}
