/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Davide Stasio $
   $Notice: (C) Copyright 2020 by Davide Stasio. All Rights Reserved. $
   ======================================================================== */
#include "win32_renderer_d3d11.h"
#include "syspec_math.h"  // @todo: find a way to remove this?

inline void
cat(char *src0, char *src1, char *dest)
{
    while (*src0)  *(dest++) = *(src0++);
    while (*src1)  *(dest++) = *(src1++);
    *dest = '\0';
}

inline PLATFORM_INIT_SQUARE_MESH(d3d11_init_square_mesh)
{
    D11_Renderer *d11 = (D11_Renderer *)renderer->platform;
    r32 square_verts[] = {
        0.f, -1.f,  0.f, 1.f,
        1.f, -1.f,  1.f, 1.f,
        1.f,  0.f,  1.f, 0.f,

        1.f,  0.f,  1.f, 0.f,
        0.f,  0.f,  0.f, 0.f,
        0.f, -1.f,  0.f, 1.f
    };

    u32 vertices_size = sizeof(square_verts);
    renderer->square.vert_stride  = 4*sizeof(r32);

    //
    // vertex buffer
    //
    D3D11_SUBRESOURCE_DATA raw_vert_data = {square_verts};
    D3D11_BUFFER_DESC vert_buff_desc     = {};
    vert_buff_desc.ByteWidth             = sizeof(square_verts);
    vert_buff_desc.Usage                 = D3D11_USAGE_IMMUTABLE;
    vert_buff_desc.BindFlags             = D3D11_BIND_VERTEX_BUFFER;
    vert_buff_desc.StructureByteStride   = renderer->square.vert_stride;
    d11->device->CreateBuffer(&vert_buff_desc, &raw_vert_data, (ID3D11Buffer **)&renderer->square.vert);

    //
    // input layout description
    //
    D3D11_INPUT_ELEMENT_DESC in_desc[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    d11->device->CreateInputLayout(in_desc, 2, shader->vertex_file.data, shader->vertex_file.size, (ID3D11InputLayout **)&renderer->square.platform);
}

PLATFORM_RELOAD_SHADER(d3d11_reload_shader)
{
    D11_Renderer *d11 = (D11_Renderer *)renderer->platform;

    char vertex_path[MAX_PATH] = {};
    char  pixel_path[MAX_PATH] = {};
    cat("assets\\", name, vertex_path);
    cat(vertex_path, ".vsh", vertex_path);
    cat("assets\\", name,  pixel_path);
    cat( pixel_path, ".psh",  pixel_path);
    shader->vertex_file.path = vertex_path;
    shader->pixel_file.path  =  pixel_path;

    if(win32_reload_file_if_changed(&shader->vertex_file))
        d11->device->CreateVertexShader(shader->vertex_file.data, shader->vertex_file.size, 0, (ID3D11VertexShader **)&shader->vertex);

    if(win32_reload_file_if_changed(&shader->pixel_file))
        d11->device->CreatePixelShader(shader->pixel_file.data, shader->pixel_file.size, 0, (ID3D11PixelShader **)&shader->pixel);
}

PLATFORM_INIT_TEXTURE(d3d11_init_texture)
{
    D11_Renderer *d11 = (D11_Renderer *)renderer->platform;
    D3D11_TEXTURE2D_DESC tex_desc = {};
    tex_desc.Width              = texture->width;
    tex_desc.Height             = texture->height;
    tex_desc.MipLevels          = 0;
    tex_desc.ArraySize          = 1;
    tex_desc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
    tex_desc.SampleDesc.Count   = 1;
    tex_desc.SampleDesc.Quality = 0;
    tex_desc.Usage              = D3D11_USAGE_DEFAULT;
    tex_desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE|D3D11_BIND_RENDER_TARGET;
    tex_desc.MiscFlags          = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    ID3D11Texture2D **handle = (ID3D11Texture2D **)&texture->handle;
    ID3D11ShaderResourceView **view = (ID3D11ShaderResourceView **)&texture->platform;
    d11->device->CreateTexture2D(&tex_desc, 0, handle);
    d11->device->CreateShaderResourceView(*handle, 0, view);
    d11->context->UpdateSubresource(*handle, 0, 0, texture->bytes, texture->width*4, 0);
    d11->context->GenerateMips(*view);
}

PLATFORM_LOAD_WEXP(d3d11_load_wexp)
{
    Assert(shader);
    Assert(buffers->wexp);
    D11_Renderer *d11 = (D11_Renderer *)renderer->platform;
    Wexp_Header *wexp = buffers->wexp;

    u32 vertices_size = wexp->indices_offset - wexp->vert_offset;
    u32 indices_size  = wexp->eof_offset - wexp->indices_offset;
    u16 index_count  = truncate_to_u16(indices_size / 2); // two bytes per index
    u8  vert_stride  = 8*sizeof(r32);

    //
    // vertex buffer
    //
    D3D11_SUBRESOURCE_DATA raw_vert_data = {byte_offset(wexp, wexp->vert_offset)};
    D3D11_BUFFER_DESC vert_buff_desc     = {};
    vert_buff_desc.ByteWidth             = vertices_size;
    vert_buff_desc.Usage                 = D3D11_USAGE_IMMUTABLE;
    vert_buff_desc.BindFlags             = D3D11_BIND_VERTEX_BUFFER;
    vert_buff_desc.StructureByteStride   = vert_stride;
    d11->device->CreateBuffer(&vert_buff_desc, &raw_vert_data, (ID3D11Buffer **)&buffers->vert);

    //
    // input layout description
    //
    D3D11_INPUT_ELEMENT_DESC in_desc[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    d11->device->CreateInputLayout(in_desc, 3, shader->vertex_file.data, shader->vertex_file.size, (ID3D11InputLayout **)&buffers->platform);

    //
    // index buffer
    //
    D3D11_SUBRESOURCE_DATA index_data = {byte_offset(wexp, wexp->indices_offset)};
    D3D11_BUFFER_DESC index_buff_desc = {};
    index_buff_desc.ByteWidth         = indices_size;
    index_buff_desc.Usage             = D3D11_USAGE_IMMUTABLE;
    index_buff_desc.BindFlags         = D3D11_BIND_INDEX_BUFFER;
    d11->device->CreateBuffer(&index_buff_desc, &index_data, (ID3D11Buffer **)&buffers->index);
}

PLATFORM_LOAD_RENDERER(win32_load_d3d11)
{
    D11_Renderer *d11 = (D11_Renderer *)renderer->platform;

    D3D11_VIEWPORT Viewport = {};
    Viewport.TopLeftX = 0;
    Viewport.TopLeftY = 0;
    Viewport.Width = WIDTH;
    Viewport.Height = HEIGHT;
    Viewport.MinDepth = 0.f;
    Viewport.MaxDepth = 1.f;
    d11->context->RSSetViewports(1, &Viewport);

    //
    // allocating rgb and depth buffers
    //
    D3D11_TEXTURE2D_DESC depth_buffer_desc = {};
    depth_buffer_desc.Width = WIDTH;
    depth_buffer_desc.Height = HEIGHT;
    depth_buffer_desc.MipLevels = 1;
    depth_buffer_desc.ArraySize = 1;
    depth_buffer_desc.Format = DXGI_FORMAT_D32_FLOAT;
    depth_buffer_desc.SampleDesc.Count = 1;
    depth_buffer_desc.SampleDesc.Quality = 0;
    depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depth_buffer_desc.MiscFlags = 0;

    ID3D11Texture2D *depth_texture;
    d11->device->CreateTexture2D(&depth_buffer_desc, 0, &depth_texture);

    { // Depth states.
        D3D11_DEPTH_STENCIL_VIEW_DESC depth_view_desc = {DXGI_FORMAT_D32_FLOAT, D3D11_DSV_DIMENSION_TEXTURE2D};
        d11->device->CreateRenderTargetView(d11->backbuffer, 0, &d11->render_target_rgb);
        d11->device->CreateDepthStencilView(depth_texture, &depth_view_desc, &d11->render_target_depth);

        D3D11_DEPTH_STENCIL_DESC depth_stencil_settings;
        depth_stencil_settings.DepthEnable    = 1;
        depth_stencil_settings.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depth_stencil_settings.DepthFunc      = D3D11_COMPARISON_LESS;
        depth_stencil_settings.StencilEnable  = 0;
        depth_stencil_settings.StencilReadMask;
        depth_stencil_settings.StencilWriteMask;
        depth_stencil_settings.FrontFace      = {D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS};
        depth_stencil_settings.BackFace       = {D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS};

        d11->device->CreateDepthStencilState(&depth_stencil_settings, &d11->depth_nostencil_state);

        depth_stencil_settings.DepthEnable    = 0;
        depth_stencil_settings.DepthFunc      = D3D11_COMPARISON_ALWAYS;

        d11->device->CreateDepthStencilState(&depth_stencil_settings, &d11->nodepth_nostencil_state);
    }

    ID3D11SamplerState *sampler;
    D3D11_SAMPLER_DESC sampler_desc = {};
    sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;//D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
    sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
    sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
    sampler_desc.MipLODBias = -1;
    sampler_desc.MaxAnisotropy = 16;
    sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    sampler_desc.MinLOD = 0;
    sampler_desc.MaxLOD = 100;
    d11->device->CreateSamplerState(&sampler_desc, &sampler);
    d11->context->PSSetSamplers(0, 1, &sampler);

    //
    // constant buffer setup
    //
    D3D11_BUFFER_DESC MatrixBufferDesc = {};
    MatrixBufferDesc.ByteWidth = 3*sizeof(m4);
    MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    MatrixBufferDesc.MiscFlags = 0;
    MatrixBufferDesc.StructureByteStride = sizeof(m4);
    d11->device->CreateBuffer(&MatrixBufferDesc, 0, &d11->matrix_buff);
    d11->context->VSSetConstantBuffers(0, 1, &d11->matrix_buff);

    D3D11_BUFFER_DESC light_buff_desc = {};
    light_buff_desc.ByteWidth = 16*3;
    light_buff_desc.Usage = D3D11_USAGE_DYNAMIC;
    light_buff_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    light_buff_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    light_buff_desc.MiscFlags = 0;
    light_buff_desc.StructureByteStride = sizeof(v3);
    d11->device->CreateBuffer(&light_buff_desc, 0, &d11->light_buff);
    d11->context->PSSetConstantBuffers(0, 1, &d11->light_buff);

    //
    // rasterizer set-up
    //
    D3D11_RASTERIZER_DESC raster_settings = {};
    raster_settings.FillMode = D3D11_FILL_SOLID;
    raster_settings.CullMode = D3D11_CULL_BACK;
    raster_settings.FrontCounterClockwise = 1;
    raster_settings.DepthBias = 0;
    raster_settings.DepthBiasClamp = 0;
    raster_settings.SlopeScaledDepthBias = 0;
    raster_settings.DepthClipEnable = 1;
    raster_settings.ScissorEnable = 0;
    raster_settings.MultisampleEnable = 0;
    raster_settings.AntialiasedLineEnable = 0;

    ID3D11RasterizerState *raster_state = 0;
    d11->device->CreateRasterizerState(&raster_settings, &raster_state);
    d11->context->RSSetState(raster_state);

    //
    // Alpha blending.
    //
    CD3D11_BLEND_DESC blend_state_desc = {};
    blend_state_desc.RenderTarget[0].BlendEnable = 1;
    //blend_state_desc.RenderTarget[0].LogicOpEnable = 0;
    blend_state_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blend_state_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blend_state_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blend_state_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blend_state_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blend_state_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    //blend_state_desc.RenderTarget[0].LogicOp;
    blend_state_desc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

    ID3D11BlendState *blend_state = 0;
    d11->device->CreateBlendState(&blend_state_desc, &blend_state);
    d11->context->OMSetBlendState(blend_state, 0, 0xFFFFFFFF);
}


inline PLATFORM_SET_ACTIVE_MESH(d3d11_set_active_mesh)
{
    D11_Renderer *d11 = (D11_Renderer *)renderer->platform;
    ID3D11InputLayout *in_layout = (ID3D11InputLayout *)buffers->platform;

    u32 offsets = 0;
    u32 stride = buffers->vert_stride;
    d11->context->IASetVertexBuffers(0, 1, (ID3D11Buffer **)&buffers->vert, &stride, &offsets);
    d11->context->IASetInputLayout(in_layout);
    if(buffers->index)  d11->context->IASetIndexBuffer((ID3D11Buffer *)buffers->index, DXGI_FORMAT_R16_UINT, 0);
    d11->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

inline PLATFORM_SET_ACTIVE_TEXTURE(d3d11_set_active_texture)
{
    D11_Renderer *d11 = (D11_Renderer *)renderer->platform;
    d11->context->PSSetShaderResources(0, 1, (ID3D11ShaderResourceView **) &texture->platform); 
}

inline PLATFORM_SET_ACTIVE_SHADER(d3d11_set_active_shader)
{
    D11_Renderer *d11 = (D11_Renderer *)renderer->platform;
    d11->context->VSSetShader((ID3D11VertexShader *)shader->vertex, 0, 0);
    d11->context->PSSetShader((ID3D11PixelShader  *)shader->pixel,  0, 0);
}

inline PLATFORM_DRAW_RECT(d3d11_draw_rect)
{
    D11_Renderer *d11 = (D11_Renderer *)renderer->platform;
    renderer->set_active_shader(renderer, shader);

    D3D11_MAPPED_SUBRESOURCE matrices_map = {};
    d11->context->Map(d11->matrix_buff, 0, D3D11_MAP_WRITE_DISCARD, 0, &matrices_map);

    m4 *matrix_buffer = (m4 *)matrices_map.pData;

    size.x /= (r32)WIDTH;
    size.y /= (r32)HEIGHT;
    pos.x  /= (r32)WIDTH;
    pos.y  /= (r32)HEIGHT;
    pos.x   =  (pos.x*2.f - 1.f);
    pos.y   = -(pos.y*2.f - 1.f);

    matrix_buffer[0] = Translation_m4(pos.x, pos.y, 0)*Scale_m4(size*2.f);
    d11->context->Unmap(d11->matrix_buff, 0);

    d11->context->OMSetDepthStencilState(d11->nodepth_nostencil_state, 1);
    d11->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Mesh m = {};
    m.buffers = renderer->square;
    renderer->set_active_mesh(renderer, &m.buffers);
    d11->context->Draw(6, 0);
}

r32
d3d11_draw_char(Platform_Renderer *renderer, Platform_Shader *shader, Platform_Font *font, char character, v2 pos)
{
    D11_Renderer *d11 = (D11_Renderer *)renderer->platform;
    renderer->set_active_shader(renderer, shader);

    Assert(character >= first_nonwhite_char);
    Assert(character <=  last_nonwhite_char);
    Platform_Texture *texture = &font->chars[character - first_nonwhite_char];
    if (!texture->platform)
    {
        // Loading character bitmap
        u8 *font_bitmap = stbtt_GetCodepointBitmap(&font->info, 0, stbtt_ScaleForPixelHeight(&font->info, font->height),
                                                   character, (i32 *)&texture->width, (i32 *)&texture->height, 0, 0);
        Assert(texture->width  < 0x80000000);
        Assert(texture->height < 0x80000000);
        texture->size = texture->width*texture->height*4;
        Assert(texture->size/4 < 2500);
        u32 texture_data[2500] = {};

        for(u32 y = 0; y < texture->height; ++y) {
            for(u32 x = 0; x < texture->width; ++x) {
                u32 src = font_bitmap[y*texture->width + x];
                texture_data[y*texture->width + x] = ((src << 24) | (src << 16) | (src << 8) | src);
            }
        }
        stbtt_FreeBitmap(font_bitmap, font->info.userdata);

        D3D11_TEXTURE2D_DESC tex_desc = {};
        tex_desc.Width              = texture->width;
        tex_desc.Height             = texture->height;
        tex_desc.MipLevels          = 1;
        tex_desc.ArraySize          = 1;
        tex_desc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
        tex_desc.SampleDesc.Count   = 1;
        tex_desc.SampleDesc.Quality = 0;
        tex_desc.Usage              = D3D11_USAGE_IMMUTABLE;
        tex_desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;

        D3D11_SUBRESOURCE_DATA subres = {texture_data, texture->width*4};
        d11->device->CreateTexture2D(&tex_desc, &subres, (ID3D11Texture2D **)&texture->handle);
        d11->device->CreateShaderResourceView((ID3D11Texture2D *)texture->handle, 0, (ID3D11ShaderResourceView **)&texture->platform);
    }

    renderer->set_active_texture(renderer, texture);
    v2 size = {(r32)texture->width, (r32)texture->height};
    //size *= 32.f/(r32)texture->height;
    renderer->draw_rect(renderer, shader, size, pos);
    return size.x;
}

PLATFORM_DRAW_TEXT(d3d11_draw_text)
{
    D11_Renderer *d11 = (D11_Renderer *)renderer->platform;
    i32 ascent, descent, line_gap;
    stbtt_GetFontVMetrics(&font->info, &ascent, &descent, &line_gap);
    ascent   = (i32)((r32)ascent   * font->scale);
    descent  = (i32)((r32)descent  * font->scale);
    line_gap = (i32)((r32)line_gap * font->scale);
    v2 pos = pivot;
    i32 min_x, min_y, max_x, max_y;
    stbtt_GetFontBoundingBox(&font->info, &min_x, &min_y, &max_x, &max_y);
    v2 font_min = make_v2((r32)min_x, (r32)min_y)*font->scale;
    v2 font_max = make_v2((r32)max_x, (r32)max_y)*font->scale;
    for(char *c = text; *c != '\0'; ++c) {
        if (*c == '\n') {
            pos = pivot;
            pos.y += ascent - descent + line_gap;
        }
        else if (*c == ' ') {
            pos.x += font_max.x;
        }
        else {
            stbtt_GetCodepointBox(&font->info, *c, &min_x, &min_y, &max_x, &max_y);
            v2 min = make_v2((r32)min_x, (r32)min_y)*font->scale;
            v2 max = make_v2((r32)max_x, (r32)max_y)*font->scale;
            d3d11_draw_char(renderer, shader, font, *c,
                      make_v2(pos.x + min.x - font_min.x, pos.y + font_max.y - max.y));
            pos.x += font_max.x;
        }
    }
}
