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

# find ./build/default -type f -iname "*.html" -print0 | xargs -0 printf "%s\n"

# Serve an app: just server hello_world
serve app="epaper" configure="default" port="35729":
    #!/usr/bin/env bash
    pushd "./build/{{ configure }}/apps" 2>&1>/dev/null
    # find . -type f -iname "*.html" -print0 | xargs -0 printf "http://127.0.0.1:35729/%s\n"
    # fd -ehtml . "./build/{{ configure }}/apps" -x printf "http://127.0.0.1:{{ port }}/{//}/{/}\n"
    # fd -ehtml . "build/{{ configure }}/apps" -x printf "http://127.0.0.1:{{ port }}/{}\n"
    fd -ehtml  | xargs printf "http://127.0.0.1:35729/%s\n"
    # printf "%s\n" "http://127.0.0.1:{{ port }}/{{ app }}/{{ app }}.html"
    # uvx livereload --port {{ port }} "./build/{{ configure }}/apps"
    uvx livereload --port {{ port }} "."
