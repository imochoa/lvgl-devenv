# AGENTS.md

LVGL simulator dev environment. Builds C apps to WebAssembly via Emscripten + SDL2, runs in the browser. All compilation happens inside a podman container (`docker.io/emscripten/emsdk:latest`); nothing compiles on the host.

## Project layout

```
├── apps/                   # Each subdirectory is a standalone LVGL app
│   ├── hello_world/        #   main.c + 1-line CMakeLists.txt
│   ├── counter/
│   └── shapes/
├── shared/
│   ├── sim_harness.h       # Contract: app implements app_create_ui()
│   └── sim_harness.c       # SDL init, LVGL display driver, main loop
├── cmake/
│   └── lvgl_app.cmake      # add_lvgl_app() function — all per-target boilerplate
├── CMakeLists.txt           # Root: FetchContent LVGL 8.4.0, includes cmake module, add_subdirectory per app
├── lv_conf.h                # Shared LVGL config (16-bit RGB565, libc malloc, SDL tick source)
├── shell_minimal.html       # Emscripten HTML shell template (shared by all apps)
├── justfile                 # Build commands (podman wrappers)
├── .devcontainer/           # VS Code devcontainer using emscripten/emsdk image
└── .vscode/                 # Tasks, launch config, IntelliSense config
```

## Adding a new app

1. Create `apps/<name>/main.c` — implement `void app_create_ui(void)`.
2. Create `apps/<name>/CMakeLists.txt` containing one line: `add_lvgl_app(<name> SOURCES main.c)`.
3. Add `add_subdirectory(apps/<name>)` to the root `CMakeLists.txt`.
4. Add `"<name>"` to the `options` array in both `.vscode/tasks.json` and `.vscode/launch.json` inputs.

Custom display resolution: `add_lvgl_app(<name> SOURCES main.c HOR_RES 320 VER_RES 240 SCALE 2)`.

## Build and run

Requires `podman` and `just` on the host. No other toolchain needed.

```
just configure          # emcmake cmake (first time or after CMakeLists changes)
just build              # build all apps
just build counter      # build one app
just server counter     # livereload on port 35729, open printed URL in browser
just dev                # interactive bash inside the emsdk container
```

Build outputs land in `build/apps/<name>/` — three files per app: `<name>.html`, `<name>.js`, `<name>.wasm`.

## LVGL version and API

LVGL **8.4.0** fetched via CMake FetchContent from `zephyrproject-rtos/lvgl`. This is the v8 API:

- Display driver: `lv_disp_drv_t`, `lv_disp_draw_buf_t`, `lv_disp_drv_register()`
- Widget creation: `lv_label_create(lv_scr_act())`, `lv_obj_align()`, `lv_style_set_*`
- Timers: `lv_timer_create(callback, period_ms, NULL)`
- Tick source: `LV_TICK_CUSTOM` using `SDL_GetTicks()` (configured in `lv_conf.h`)

Do NOT use LVGL v9 API (`lv_display_*`, `lv_tick_set_cb`, etc.) — it will not compile.

## Key conventions

- Apps only implement `app_create_ui(void)`. All SDL/LVGL boilerplate lives in `shared/sim_harness.c`.
- `lv_conf.h` is shared across all apps. Widget-specific `#define LV_USE_*` must be enabled there before an app can use that widget.
- Currently enabled widgets: `LV_USE_LABEL`, `LV_USE_LINE`. Other widgets (btn, bar, arc, etc.) need explicit enablement in `lv_conf.h`.
- Display defaults are 160x80 @ 4x scale (defined in `sim_harness.h`), overridable per-app via CMake.
- The `_deps/` directory (FetchContent downloads) is in the source tree intentionally — it survives build directory wipes.

## Container and devcontainer

- Image: `docker.io/emscripten/emsdk:latest` (runs as root)
- Podman rootless: `--userns=keep-id:uid=1000,gid=1000`
- SDL2 port cache persisted in named volume `emscripten-sdl2`
- Git safe.directory set to `*` via container env vars (required for FetchContent inside container)
- No zsh, no custom tools — bash only

## Files you should not modify without understanding the impact

- `shared/sim_harness.c` — all apps link against this; changing the function signature or display init breaks every app.
- `lv_conf.h` — shared LVGL config; disabling a widget breaks any app using it.
- `cmake/lvgl_app.cmake` — the `add_lvgl_app()` function is the only CMake interface apps use; changing its parameter contract affects all app CMakeLists.txt files.
- `shell_minimal.html` — Emscripten HTML shell used by all apps at link time.
