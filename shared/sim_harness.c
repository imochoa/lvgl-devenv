#include <SDL2/SDL.h>
#include "lvgl.h"
#include "sim_harness.h"
#include <string.h>
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

static SDL_Window   *window;
static SDL_Renderer *renderer;
static SDL_Texture  *texture;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[DISP_HOR_RES * 10];
static lv_color_t framebuffer[DISP_HOR_RES * DISP_VER_RES];

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
        "LVGL Simulator",
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

    app_create_ui();

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
