#ifndef _POST_PROCESS
#define _POST_PROCESS

#include "register.fx"

// ====================
// PostProcess Shader
// mesh : RectMesh
// BS_TYPE : Default
// DS_TYPE : NoTest NoWrite 
// ====================
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};


struct VS_OUT
{
    float4 vPosition : SV_Position;    
    float2 vUV : TEXCOORD;
};

VS_OUT VS_PostProcess(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_PostProcess(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
        
    // VS_OUT 으로 전달한 SV_Position 값은 PixelShader 에 입력될 때 픽셀좌표로 변환해서 입력
    float2 vUV = _in.vPosition.xy / g_vRenderResolution;    
    
    if (distance(_in.vUV, float2(0.5, 0.5)) >= 0.3f)
    {
        float2 fAdd = float2(g_Noise.Sample(g_sam_0, _in.vUV + g_fAccTime * 0.2f).x
                         , g_Noise.Sample(g_sam_0, _in.vUV + float2(0.1f, 0.f) + g_fAccTime * 0.2f).x);
        fAdd -= fAdd / 2.f;
        vUV += fAdd * 0.05f;
    }
    
    vColor = g_RTCopyTex.Sample(g_sam_0, vUV);    
    return vColor;
}

float4 PS_PostProcess_Darkroom(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
        
    // VS_OUT 으로 전달한 SV_Position 값은 PixelShader 에 입력될 때 픽셀좌표로 변환해서 입력
    float2 vUV = _in.vPosition.xy / g_vRenderResolution;
    vColor = g_RTCopyTex.Sample(g_sam_0, vUV);
    
    if (abs(_in.vUV.x - 0.5f) > 0.4f || abs(_in.vUV.y - 0.5f) > 0.4f)
    {
        if (abs(_in.vUV.x - 0.5f) >= abs(_in.vUV.y - 0.5f))
        {
            vColor.xyz *= (abs(_in.vUV.x - 0.5f) - 0.4f) / 0.1f;
        }
        else
        {
            vColor.xyz *= (abs(_in.vUV.y - 0.5f) - 0.4f) / 0.1f;
        }
    }
    else
    {
        vColor.xyz *= 0.f;
    }
    
    return vColor;
}




#endif

