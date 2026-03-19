set positional-arguments := true
set shell := ["bash", "-euco", "pipefail"]

[no-cd]
_default:
    @just --list --list-submodules

configure:
    podman run --rm \
        -v "{{ justfile_directory() }}:/src" \
        -v emscripten-sdl2:/emsdk/upstream/emscripten/cache/ports/sdl2 \
        -w /src \
        -e GIT_CONFIG_COUNT=1 \
        -e GIT_CONFIG_KEY_0=safe.directory \
        -e "GIT_CONFIG_VALUE_0=*" \
        docker.io/emscripten/emsdk:latest \
        -- \
        emcmake cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Build all apps, or a single one: just build [hello_world]
build target="":
    podman run --rm \
        -v "{{ justfile_directory() }}:/src" \
        -v emscripten-sdl2:/emsdk/upstream/emscripten/cache/ports/sdl2 \
        -w /src \
        -e GIT_CONFIG_COUNT=1 \
        -e GIT_CONFIG_KEY_0=safe.directory \
        -e "GIT_CONFIG_VALUE_0=*" \
        docker.io/emscripten/emsdk:latest \
        -- \
        cmake --build ./build -j4{{ if target != "" { " --target " + target } else { "" } }}

dev:
    podman run --rm -it \
        -v "{{ justfile_directory() }}:/src" \
        -v emscripten-sdl2:/emsdk/upstream/emscripten/cache/ports/sdl2 \
        -w /src \
        -e GIT_CONFIG_COUNT=1 \
        -e GIT_CONFIG_KEY_0=safe.directory \
        -e "GIT_CONFIG_VALUE_0=*" \
        --entrypoint bash \
        docker.io/emscripten/emsdk:latest

# Serve an app: just server hello_world
serve app="hello_world":
    printf "%s\n" "http://127.0.0.1:35729/{{ app }}/{{ app }}.html"
    uvx livereload --port 35729 "./build/apps"
