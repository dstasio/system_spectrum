struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 txc : TEXCOORD;
};

#if VERTEX_HLSL // ---------------------------------------------------

struct VS_INPUT
{
    float2 pos    : POSITION;
    float2 txc    : TEXCOORD;
};

cbuffer Matrices: register(b0)
{
    float4x4 model;
}

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = mul(model, float4(input.pos.xy, 0.f, 1.f));
    output.txc = input.txc;
    if (input.txc.x <= 0.5f)
    {
        output.txc.x = 1.f;//input.pos.x;
        output.txc.y = 0.1f;
    }
    else
    {
        output.txc.x = 0.1f;//input.pos.x;
        output.txc.y = 1.f;
    }

    return output;
}

#elif PIXEL_HLSL // --------------------------------------------------

float4 main(VS_OUTPUT input) : SV_TARGET
{
    float4 color = float4(input.txc.x, 0.f, input.txc.y, 1.f);
    return color;
}

#endif
