add_lvgl_app(epaper SOURCES main.c)

add_definitions(
    -D DISP_HOR_RES=100
    -D DISP_VER_RES=100
    -D SCALE_FACTOR=1
)
#define DISP_HOR_RES 160
#define DISP_VER_RES 80
#define SCALE_FACTOR 4