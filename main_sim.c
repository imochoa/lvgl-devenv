#include <SDL2/SDL.h>
#include "lvgl.h"
#include <string.h>
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define DISP_HOR_RES  160
#define DISP_VER_RES  80
#define SCALE_FACTOR  4

static SDL_Window   *window;
static SDL_Renderer *renderer;
static SDL_Texture  *texture;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[DISP_HOR_RES * 10];
static lv_color_t framebuffer[DISP_HOR_RES * DISP_VER_RES];

static lv_obj_t  *counter_label;
static uint32_t   last_second_tick;
static uint32_t   seconds_elapsed;
static char        count_buf[16];

static void sdl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area,
                          lv_color_t *color_p)
{
    for (int32_t y = area->y1; y <= area->y2; y++) {
        memcpy(&framebuffer[y * DISP_HOR_RES + area->x1],
               color_p,
               (area->x2 - area->x1 + 1) * sizeof(lv_color_t));
        color_p += (area->x2 - area->x1 + 1);
    }

    SDL_UpdateTexture(texture, NULL, framebuffer,
                      DISP_HOR_RES * sizeof(lv_color_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    lv_disp_flush_ready(drv);
}

/*
 * UI creation — mirrors ../src/main.c
 * Edit this function to iterate on your display layout,
 * then copy changes back to ../src/main.c.
 */
static void create_ui(void)
{
    lv_obj_t *hello_label;
    lv_obj_t *rect;
    lv_obj_t *circle;
    static lv_style_t rect_style;
    static lv_style_t circle_style;
    static lv_point_t rect_points[5] = {
        {0, 0}, {120, 0}, {120, 20}, {0, 20}, {0, 0}
    };
    const uint32_t circle_radius = 15;

    hello_label = lv_label_create(lv_scr_act());
    lv_label_set_text(hello_label, "Hello, World!");
    lv_obj_align(hello_label, LV_ALIGN_TOP_MID, 0, 5);

    counter_label = lv_label_create(lv_scr_act());
    lv_obj_align(counter_label, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_style_init(&rect_style);
    lv_style_set_line_color(&rect_style, lv_color_hex(0x0000FF));
    lv_style_set_line_width(&rect_style, 3);

    rect = lv_line_create(lv_scr_act());
    lv_obj_add_style(rect, &rect_style, 0);
    lv_line_set_points(rect, rect_points,
                       sizeof(rect_points) / sizeof(rect_points[0]));
    lv_obj_align(rect, LV_ALIGN_TOP_MID, 0, 0);

    lv_style_init(&circle_style);
    lv_style_set_radius(&circle_style, circle_radius);
    lv_style_set_bg_opa(&circle_style, LV_OPA_100);
    lv_style_set_bg_color(&circle_style, lv_color_hex(0xFF0000));

    circle = lv_obj_create(lv_scr_act());
    lv_obj_set_size(circle, circle_radius * 2, circle_radius * 2);
    lv_obj_add_style(circle, &circle_style, 0);
    lv_obj_align(circle, LV_ALIGN_CENTER, 0, 5);
}

static void main_loop(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
#ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
#else
            exit(0);
#endif
        }
    }

    uint32_t now = SDL_GetTicks();
    if (now - last_second_tick >= 1000) {
        last_second_tick = now;
        seconds_elapsed++;
        snprintf(count_buf, sizeof(count_buf), "%u", seconds_elapsed);
        lv_label_set_text(counter_label, count_buf);
    }

    lv_task_handler();
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "LVGL Sim (160x80)",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        DISP_HOR_RES * SCALE_FACTOR, DISP_VER_RES * SCALE_FACTOR,
        0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, DISP_HOR_RES, DISP_VER_RES);

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB565,
        SDL_TEXTUREACCESS_STREAMING,
        DISP_HOR_RES, DISP_VER_RES);

    lv_init();

    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, DISP_HOR_RES * 10);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res  = DISP_HOR_RES;
    disp_drv.ver_res  = DISP_VER_RES;
    disp_drv.flush_cb = sdl_flush_cb;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    create_ui();

    last_second_tick = SDL_GetTicks();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 1);
#else
    while (1) {
        main_loop();
        SDL_Delay(5);
    }
#endif

    return 0;
}
