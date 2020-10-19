#if !defined(SYSPEC_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Davide Stasio $
   $Notice: (C) Copyright 2020 by Davide Stasio. All Rights Reserved. $
   ======================================================================== */
#include "syspec_platform.h"

#define WIDTH 1024
#define HEIGHT 720

#if SYSPEC_DEBUG
#define Assert(expr) if(!(expr)) {*(int *)0 = 0;}
#else
#define Assert(expr)
#endif
#include "syspec_math.h"

#define byte_offset(base, offset) ((u8*)(base) + (offset))
inline u16 truncate_to_u16(u32 v) {assert(v <= 0xFFFF); return (u16)v; };

struct Camera
{
    v3 pos;
    v3 target;
    v3 up;
};

struct Mesh
{
    Platform_Mesh_Buffers buffers;

    m4 transform;
    v3 p;
    v3 dp;
    v3 ddp;
};

struct Point_Light
{
    v3 color;
    v3 p;
};

struct Dir_Light
{
    v3 color;
    v3 dir;
};
 
struct Game_State
{
    Platform_Shader  *rect_shader;
    Platform_Shader  *font_shader;

//    Platform_Texture  tex_white;
    Platform_Font     inconsolata;
};

struct Memory_Pool
{
    memory_index size;
    memory_index used;
    u8 *base;
};

#define push_struct(pool, type) (type *)push_size_((pool), sizeof(type))
#define push_array(pool, length, type) (type *)push_size_((pool), (length)*sizeof(type))
inline void *
push_size_(Memory_Pool *pool, memory_index size)
{
    Assert((pool->used + size) < pool->size);

    void *result = pool->base + pool->used;
    pool->used += size;
    return(result);
}

#define SYSPEC_H
#endif
