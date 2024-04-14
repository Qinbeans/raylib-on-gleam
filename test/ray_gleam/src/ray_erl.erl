-module(ray_erl).
-export([raylib_close/0, raylib_init/3, raylib_should_close/0, raylib_set_target_fps/1]).  % Corrected to match C function signatures
-on_load(init/0).

init() ->
    ok = erlang:load_nif("priv/ray_erl", 0).

raylib_close() ->
    exit(nif_library_not_loaded).

raylib_init(Width, Height, Title) ->
    exit(nif_library_not_loaded).

raylib_should_close() ->
    exit(nif_library_not_loaded).

raylib_set_target_fps(Fps) ->
    exit(nif_library_not_loaded).