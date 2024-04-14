#include <erl_nif.h>
#include <raylib.h>

static ERL_NIF_TERM raylib_init(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    // get the width, height, and title from the arguments
    int width, height;
    ErlNifBinary title_bin;
    if (!enif_get_int(env, argv[0], &width) || !enif_get_int(env, argv[1], &height)|| !enif_inspect_binary(env, argv[2], &title_bin)) {
        return enif_make_badarg(env);
    }
    char* title = (char*) enif_alloc(title_bin.size + 1);
    title[title_bin.size] = '\0';
    // Manually copy the binary data to the newly allocated string
    for (unsigned int i = 0; i < title_bin.size; ++i) {
        title[i] = title_bin.data[i];
    }
    title[title_bin.size] = '\0';  // Null-terminate the string
    InitWindow(width, height, title);
    enif_free(title);  // Free the allocated memory for the title
    return enif_make_atom(env, "ok");
}

static ERL_NIF_TERM raylib_close(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    CloseWindow();
    return enif_make_atom(env, "ok");
}

static ERL_NIF_TERM raylib_should_close(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_int(env, WindowShouldClose());
}

static ERL_NIF_TERM raylib_set_target_fps(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    int fps;
    if (!enif_get_int(env, argv[0], &fps)) {
        return enif_make_badarg(env);
    }
    SetTargetFPS(fps);
    return enif_make_atom(env, "ok");
}

static ErlNifFunc nif_funcs[] = {
    {"raylib_init", 3, raylib_init},
    {"raylib_close", 0, raylib_close},
    {"raylib_should_close", 0, raylib_should_close},
    {"raylib_set_target_fps", 1, raylib_set_target_fps}
};

ERL_NIF_INIT(ray_erl,nif_funcs,NULL,NULL,NULL,NULL)