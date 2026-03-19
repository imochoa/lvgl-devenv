add_lvgl_app(counter SOURCES main.c)

add_definitions(
    -D DISP_HOR_RES=200
    -D DISP_VER_RES=200
    -D SCALE_FACTOR=1
)
