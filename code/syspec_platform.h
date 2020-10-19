#if !defined(WINDY_PLATFORM_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Davide Stasio $
   $Notice: (C) Copyright 2014 by Davide Stasio. All Rights Reserved. $
   ======================================================================== */
#include "datatypes.h"
#include "headers.h"

// @critical: this should probably be moved, or used in a different way.
// As it is, I think it's getting compiled twice
#define STB_TRUETYPE_IMPLEMENTATION 1
#include "stb_truetype.h"

#ifndef MAX_PATH
#define MAX_PATH 100
#endif

struct Input_File
{
    char *path;
    u8   *data;
    u64   size;
    u64   write_time;  // @todo: this should be useless in release build
};

#define PLATFORM_READ_FILE(name) Input_File name(char *Path)
typedef PLATFORM_READ_FILE(Platform_Read_File);
#define PLATFORM_RELOAD_CHANGED_FILE(name) b32 name(Input_File *file)
typedef PLATFORM_RELOAD_CHANGED_FILE(Platform_Reload_Changed_File);

struct Platform_Renderer;
struct Platform_Shader
{
    void *vertex;
    void *pixel;

    Input_File vertex_file;
    Input_File  pixel_file;
};

struct Platform_Mesh_Buffers
{
    void *vert;
    void *index;
    void *platform;

    u16 index_count;
    u8  vert_stride;

    Wexp_Header *wexp;
};

// @todo: add guards for when 'handle == 0' (texture not initalized)
struct Platform_Texture
{
    u32   width;
    u32   height;
    u32   size;
    void *bytes;

    void *handle; // @note: (ID3D11Texture2D*)
    void *platform; // @note: optional (ID3D11ShaderResourceView*)
};

#define first_nonwhite_char '!'
#define last_nonwhite_char  '~'
#define n_supported_characters (1+last_nonwhite_char-first_nonwhite_char)
struct Platform_Font
{
    stbtt_fontinfo info;
    r32 height, scale;
    Platform_Texture chars[n_supported_characters];
};

#define PLATFORM_LOAD_RENDERER(name) void name(Platform_Renderer *renderer)
typedef PLATFORM_LOAD_RENDERER(Platform_Load_Renderer);

#define PLATFORM_RELOAD_SHADER(func) void func(Platform_Shader *shader, Platform_Renderer *renderer, char *name)
typedef PLATFORM_RELOAD_SHADER(Platform_Reload_Shader);

#define PLATFORM_INIT_TEXTURE(name) void name(Platform_Renderer *renderer, Platform_Texture *texture)
typedef PLATFORM_INIT_TEXTURE(Platform_Init_Texture);

// "shader" is needed for d3d11, can be null otherwise
#define PLATFORM_LOAD_WEXP(name) void name(Platform_Renderer *renderer, Platform_Mesh_Buffers *buffers, Platform_Shader *shader)
typedef PLATFORM_LOAD_WEXP(Platform_Load_Wexp);

#define PLATFORM_INIT_SQUARE_MESH(name) void name(Platform_Renderer *renderer, Platform_Shader *shader)
typedef PLATFORM_INIT_SQUARE_MESH(Platform_Init_Square_Mesh);


#define PLATFORM_SET_ACTIVE_MESH(name) void name(Platform_Renderer *renderer, Platform_Mesh_Buffers *buffers)
typedef PLATFORM_SET_ACTIVE_MESH(Platform_Set_Active_Mesh);

#define PLATFORM_SET_ACTIVE_TEXTURE(name) void name(Platform_Renderer *renderer, Platform_Texture *texture)
typedef PLATFORM_SET_ACTIVE_TEXTURE(Platform_Set_Active_Texture);

#define PLATFORM_SET_ACTIVE_SHADER(name) void name(Platform_Renderer *renderer, Platform_Shader *shader)
typedef PLATFORM_SET_ACTIVE_SHADER(Platform_Set_Active_Shader);


// (0,0) = Top-Left; (WIDTH,HEIGHT) = Bottom-Right
// @todo: test sub-pixel placement with AA.
#define PLATFORM_DRAW_RECT(name) void name(Platform_Renderer *renderer, Platform_Shader *shader, v2 size, v2 pos)
typedef PLATFORM_DRAW_RECT(Platform_Draw_Rect);

#define PLATFORM_DRAW_TEXT(name) void name(Platform_Renderer *renderer, Platform_Shader *shader, Platform_Font *font, char *text, v2 pivot)
typedef PLATFORM_DRAW_TEXT(Platform_Draw_Text);

struct Platform_Renderer
{
    Platform_Load_Renderer    *load_renderer;
    Platform_Reload_Shader    *reload_shader;
    Platform_Init_Texture     *init_texture;
    Platform_Load_Wexp        *load_wexp;
    Platform_Init_Square_Mesh *init_square_mesh;

    Platform_Set_Active_Mesh    *set_active_mesh;
    Platform_Set_Active_Texture *set_active_texture;
    Platform_Set_Active_Shader  *set_active_shader;

    Platform_Draw_Rect *draw_rect;
    Platform_Draw_Text *draw_text;

    Platform_Mesh_Buffers  square;

    void *platform;
};

struct Input_Keyboard
{
    u32 up;
    u32 down;
    u32 left;
    u32 right;

    u32 w;
    u32 a;
    u32 s;
    u32 d;

    u32 f;

    u32 space;
    u32 shift;
    u32 ctrl;
    u32 alt;
    u32 esc;
};

struct Input
{
    Input_Keyboard pressed;
    Input_Keyboard held;

    v2i dmouse;
    i16 dwheel;
};

typedef struct Game_Memory
{
    b32 is_initialized;

    u64 storage_size;
    void *storage;

    Platform_Read_File           *read_file;
    Platform_Reload_Changed_File *reload_if_changed;
} game_memory;

#define GAME_UPDATE_AND_RENDER(name) void name(Input *input, r32 dtime, Platform_Renderer *renderer, Game_Memory *memory)
typedef GAME_UPDATE_AND_RENDER(Game_Update_And_Render);

#define WINDY_PLATFORM_H
#endif
