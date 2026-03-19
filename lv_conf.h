/**
 * @file lv_conf.h
 * LVGL 8.4.0 configuration for the Emscripten/SDL2 simulator.
 *
 * Settings mirror the Zephyr prj.conf for the 10_demo_display app,
 * with adjustments for the SDL2 backend (no byte-swap, use host malloc).
 * Any setting not overridden here gets its default from lv_conf_internal.h.
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*====================
   COLOR SETTINGS
 *====================*/

/* 16-bit RGB565 — same as the Zephyr build (CONFIG_LV_Z_BITS_PER_PIXEL=16) */
#define LV_COLOR_DEPTH 16

/* NOT swapped for SDL2 rendering (Zephyr sets CONFIG_LV_COLOR_16_SWAP=y
   because the ST7735R SPI display needs it; SDL2 does not) */
#define LV_COLOR_16_SWAP 0

/*=========================
   MEMORY SETTINGS
 *=========================*/

/* Use standard malloc/free on the host (Zephyr uses CONFIG_LV_MEM_CUSTOM=y
   backed by k_malloc; here we use libc) */
#define LV_MEM_CUSTOM 1
#define LV_MEM_CUSTOM_INCLUDE <stdlib.h>
#define LV_MEM_CUSTOM_ALLOC malloc
#define LV_MEM_CUSTOM_FREE free
#define LV_MEM_CUSTOM_REALLOC realloc

/*====================
   HAL SETTINGS
 *====================*/

#define LV_DISP_DEF_REFR_PERIOD 30  /* ms */
#define LV_INDEV_DEF_READ_PERIOD 30 /* ms */

/* Use SDL_GetTicks() as the tick source so we don't need lv_tick_inc() */
#define LV_TICK_CUSTOM 1
#define LV_TICK_CUSTOM_INCLUDE <SDL2/SDL.h>
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (SDL_GetTicks())

#define LV_DPI_DEF 130

/*=======================
 * FEATURE CONFIGURATION
 *=======================*/

#define LV_DRAW_COMPLEX 1

/* Logging — enable at WARN level for debugging display issues */
#define LV_USE_LOG 0

/* Asserts */
#define LV_USE_ASSERT_NULL 1
#define LV_USE_ASSERT_MALLOC 1
#define LV_USE_ASSERT_STYLE 0
#define LV_USE_ASSERT_MEM_INTEGRITY 0
#define LV_USE_ASSERT_OBJ 0

/* Show FPS / CPU — handy while iterating */
#define LV_USE_PERF_MONITOR 1
#if LV_USE_PERF_MONITOR
#define LV_USE_PERF_MONITOR_POS LV_ALIGN_BOTTOM_RIGHT
#endif

#define LV_USE_USER_DATA 1

/*==================
 *   FONT USAGE
 *===================*/

#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_DEFAULT &lv_font_montserrat_14

/*=================
 *  TEXT SETTINGS
 *=================*/

#define LV_TXT_ENC LV_TXT_ENC_UTF8

/*==================
 *  WIDGET USAGE
 *================*/

/* Enable only the widgets the demo app uses (matches prj.conf) */
#define LV_USE_LABEL 1
#if LV_USE_LABEL
#define LV_LABEL_TEXT_SELECTION 1
#define LV_LABEL_LONG_TXT_HINT 1
#endif
#define LV_USE_LINE 1

/*==================
 * EXTRA COMPONENTS
 *==================*/

/* Themes — keep default theme for sensible styling */
#define LV_USE_THEME_DEFAULT 1
#if LV_USE_THEME_DEFAULT
#define LV_THEME_DEFAULT_DARK 0
#define LV_THEME_DEFAULT_GROW 1
#define LV_THEME_DEFAULT_TRANSITION_TIME 80
#endif

#define LV_USE_THEME_BASIC 1

/* Layouts */
#define LV_USE_FLEX 1
#define LV_USE_GRID 1

/* Don't build bundled examples/demos */
#define LV_BUILD_EXAMPLES 1

#endif /* LV_CONF_H */
