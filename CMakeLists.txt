cmake_minimum_required(VERSION 3.14)   # FetchContent needs 3.14+
project(lvgl_sim C)

include(FetchContent)

# Keep FetchContent downloads in the source tree (survives build-dir wipes)
set(FETCHCONTENT_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/_deps"
    CACHE PATH "FetchContent download directory")

# ---------------------------------------------------------------------------
# LVGL library  (fetched at configure time — no manual clone needed)
# ---------------------------------------------------------------------------
set(LV_CONF_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/lv_conf.h"
    CACHE STRING "Path to lv_conf.h" FORCE)

FetchContent_Declare(
    lvgl
    GIT_REPOSITORY https://github.com/zephyrproject-rtos/lvgl.git
    GIT_TAG        2b498e6f36d6b82ae1da12c8b7742e318624ecf5  # LVGL 8.4.0
)
FetchContent_MakeAvailable(lvgl)

# ---------------------------------------------------------------------------
# Platform setup  (applied once to the shared LVGL target)
# ---------------------------------------------------------------------------
if(EMSCRIPTEN)
    target_compile_options(lvgl PUBLIC "SHELL:-s USE_SDL=2")
else()
    find_package(SDL2 REQUIRED HINTS "/emsdk/upstream/emscripten/cache/ports/sdl2/SDL-release-2.32.10")
endif()

# ---------------------------------------------------------------------------
# Per-app helper
# ---------------------------------------------------------------------------
include(cmake/lvgl_app.cmake)

# ---------------------------------------------------------------------------
# Apps
# ---------------------------------------------------------------------------
add_subdirectory(apps/hello_world)
add_subdirectory(apps/counter)
add_subdirectory(apps/shapes)
add_subdirectory(apps/epaper)