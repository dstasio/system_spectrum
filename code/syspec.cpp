/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Davide Stasio $
   $Notice: (C) Copyright 2020 by Davide Stasio. All Rights Reserved. $
   ======================================================================== */
#include <d3d11.h>
#include "syspec.h"
#include <string.h>
#include "win32_renderer_d3d11.h"

inline Platform_Font *
load_font(Platform_Font *font, Platform_Read_File *read_file, char *path, r32 height)
{
    Input_File font_file = read_file(path);
    stbtt_InitFont(&font->info, font_file.data, stbtt_GetFontOffsetForIndex(font_file.data, 0));
    font->height = height;
    font->scale = stbtt_ScaleForPixelHeight(&font->info, height);
    return font;
}

GAME_UPDATE_AND_RENDER(UpdateAndRender)
{
    Game_State *state = (Game_State *)memory->storage;
    D11_Renderer *d11 = (D11_Renderer *)renderer->platform;
    if(!memory->is_initialized)
    {
        Memory_Pool mempool = {};
        mempool.base = (u8 *)memory->storage;
        mempool.size = memory->storage_size;
        mempool.used = sizeof(Game_State);

        renderer->load_renderer(renderer);

        //
        // Shaders
        //
        // @todo: remove need for pre-allocation
        state->font_shader  = push_struct(&mempool, Platform_Shader);
        state->rect_shader  = push_struct(&mempool, Platform_Shader);
        renderer->reload_shader(state->font_shader, renderer, "fonts");
        renderer->reload_shader(state->rect_shader, renderer, "rect");
        
        renderer->init_square_mesh(renderer, state->font_shader);
        load_font(&state->inconsolata, memory->read_file, "assets/Inconsolata.ttf", 32);

        memory->is_initialized = true;
    }

    //
    // ---------------------------------------------------------------
    // Input Processing.
    {
//        if (input->held.w)     1=1;
//        if (input->held.s)     1=1;
//        if (input->held.d)     1=1;
//        if (input->held.a)     1=1;
//        if (input->held.shift) 1=1;
//        if (input->held.ctrl)  1=1;
    }

#if SYSPEC_INTERNAL
    renderer->reload_shader(state->font_shader,  renderer, "fonts");
    renderer->reload_shader(state->rect_shader,  renderer, "rect");
#endif
    d11->context->OMSetRenderTargets(1, &d11->render_target_rgb, d11->render_target_depth);

    r32 ClearColor[] = {0.06f, 0.06f, 0.1f, 1.f};
    d11->context->ClearRenderTargetView(d11->render_target_rgb, ClearColor);
    d11->context->ClearDepthStencilView(d11->render_target_depth, D3D11_CLEAR_DEPTH, 1.f, 1);

    d11->context->OMSetDepthStencilState(d11->depth_nostencil_state, 1);

    char *text = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz\n0123456789 ?!\"'.,;<>[]{}()-_+=*&^%$#@/\\~`";
    renderer->draw_text(renderer, state->font_shader, &state->inconsolata, text, make_v2(0, 0));

    r32 n_frequencies = 50.f;
    for (r32 x = 0.f; x < 1.f; x += 1.f/n_frequencies)
    {
        r32 func = Sin(x*2.f*PI-PI*0.5f)*0.5f + 0.5f;//Sin(x*PI);
        v2 size = {};
        size.x = 1.f/n_frequencies*(r32)WIDTH;
        size.y = 0.7f*func*(r32)HEIGHT;
        v2 pos = {};
        pos.x = x*(r32)WIDTH;
        pos.y = (r32)HEIGHT-size.y;

        renderer->draw_rect(renderer, state->rect_shader, size, pos);
    }
}
