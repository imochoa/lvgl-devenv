# add_lvgl_app(<name>
#   SOURCES  src1.c src2.c ...
#   [HOR_RES  pixels]          # default 160
#   [VER_RES  pixels]          # default 80
#   [SCALE    factor]          # default 4
# )
function(add_lvgl_app NAME)
    cmake_parse_arguments(APP "" "HOR_RES;VER_RES;SCALE" "SOURCES" ${ARGN})

    if(NOT APP_SOURCES)
        message(FATAL_ERROR "add_lvgl_app(${NAME}): SOURCES is required")
    endif()

    add_executable(${NAME}
        ${APP_SOURCES}
        "${PROJECT_SOURCE_DIR}/shared/sim_harness.c"
    )

    target_include_directories(${NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}"         # lv_conf.h
        "${PROJECT_SOURCE_DIR}/shared"  # sim_harness.h
        "${CMAKE_CURRENT_SOURCE_DIR}"   # app-local headers
    )

    # Per-app display overrides (defaults live in sim_harness.h)
    if(APP_HOR_RES)
        target_compile_definitions(${NAME} PRIVATE "DISP_HOR_RES=${APP_HOR_RES}")
    endif()
    if(APP_VER_RES)
        target_compile_definitions(${NAME} PRIVATE "DISP_VER_RES=${APP_VER_RES}")
    endif()
    if(APP_SCALE)
        target_compile_definitions(${NAME} PRIVATE "SCALE_FACTOR=${APP_SCALE}")
    endif()

    target_link_libraries(${NAME} PRIVATE lvgl)

    if(EMSCRIPTEN)
        target_compile_options(${NAME} PRIVATE "SHELL:-s USE_SDL=2")
        target_link_options(${NAME} PRIVATE
            "SHELL:-s USE_SDL=2"
            "SHELL:-s WASM=1"
            "SHELL:-s ALLOW_MEMORY_GROWTH=1"
            "SHELL:-s INITIAL_MEMORY=16777216"
            "--shell-file" "${PROJECT_SOURCE_DIR}/shell_minimal.html"
        )
        set_target_properties(${NAME} PROPERTIES SUFFIX ".html")
    else()
        target_link_libraries(${NAME} PRIVATE SDL2::SDL2)
    endif()
endfunction()
