#include "sim_harness.h"

void app_create_ui(void)
{
    static lv_style_t rect_style;
    static lv_point_t rect_points[] = {
        {0, 0}, {120, 0}, {120, 20}, {0, 20}, {0, 0}
    };

    lv_style_init(&rect_style);
    lv_style_set_line_color(&rect_style, lv_color_hex(0x0000FF));
    lv_style_set_line_width(&rect_style, 3);

    lv_obj_t *rect = lv_line_create(lv_scr_act());
    lv_obj_add_style(rect, &rect_style, 0);
    lv_line_set_points(rect, rect_points,
                       sizeof(rect_points) / sizeof(rect_points[0]));
    lv_obj_align(rect, LV_ALIGN_TOP_MID, 0, 5);

    static lv_style_t circle_style;
    const uint32_t circle_radius = 15;

    lv_style_init(&circle_style);
    lv_style_set_radius(&circle_style, circle_radius);
    lv_style_set_bg_opa(&circle_style, LV_OPA_100);
    lv_style_set_bg_color(&circle_style, lv_color_hex(0xFF0000));

    lv_obj_t *circle = lv_obj_create(lv_scr_act());
    lv_obj_set_size(circle, circle_radius * 2, circle_radius * 2);
    lv_obj_add_style(circle, &circle_style, 0);
    lv_obj_align(circle, LV_ALIGN_CENTER, 0, 5);

    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Shapes");
    lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -5);
}
