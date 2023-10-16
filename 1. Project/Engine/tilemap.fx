#ifndef _TILEMAP
#define _TILEMAP

#include "register.fx"
#include "func.fx"

struct tTile
{
    float2 vLeftTop;
    float2 vSlice;
};

// ===============
// TileMap Shader
#define AtlasTex g_tex_0
#define LeftTop g_vec2_0
#define Slice  g_vec2_1
#define TileCount g_vec2_2;
StructuredBuffer<tTile> TileBuffer : register(t17);
// ===============


struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float3 vWorldPos : POSITION;
};

VS_OUT VS_TileMap(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vUV = _in.vUV * TileCount;
    
    return output;
}

float4 PS_TileMap(VS_OUT _in) : SV_Target
{        
    int bufferIdx = g_vec2_2.x * floor(_in.vUV.y) + _in.vUV.x;
    float2 uv = frac(_in.vUV);
    
    if (uv.x > 0.99f)
    {
        uv.x = 0.99f;
    }
    if (uv.y > 0.99f)
    {
        uv.y = 0.99f;
    }
    float2 vUV = TileBuffer[bufferIdx].vLeftTop + (TileBuffer[bufferIdx].vSlice * uv);
    
    float4 vOutColor = AtlasTex.Sample(g_sam_1, vUV);
    
    // ±¤¿ø Ã³¸®
    tLightColor color = (tLightColor) 0.f;
    
    for (int i = 0; i < g_iLight2DCount; ++i)
    {
        CalcLight2D(color, _in.vWorldPos, i);
    }
    
    vOutColor.xyz *= color.vDiff.xyz;
    
    return vOutColor;
}


#endif
