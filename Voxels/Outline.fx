/************* Resources *************/

cbuffer CBufferPerObject
{
    float4x4 WorldViewProjection : WORLDVIEWPROJECTION; 
	float4x4 PositionMatrix : POSITIONVECTOR;
}

/************* Data Structures *************/

struct VS_INPUT
{
    float4 Position : POSITION;
	float4 Normal : NORMAL;
	float2 Tex : TEXCOORD;
};

struct VS_OUTPUT 
{
    float4 Position : SV_Position;
	float4 Normal : NORMAL;
	float2 Tex : TEXCOORD;
};

RasterizerState FrontCull
{
    CullMode = FRONT;
};

RasterizerState BackCull
{
	CullMode = BACK;
};

RasterizerState DisableCull
{
	CullMode = NONE;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;
    
	//float4 world = mul(IN.Position, WorldViewProjection);
	//OUT.Position = mul(world, PositionMatrix);
	float4 pos = mul(IN.Position, PositionMatrix);
	OUT.Position = mul(pos, WorldViewProjection);
	OUT.Normal = IN.Normal;
    OUT.Tex = IN.Tex;
	
    return OUT;
}

VS_OUTPUT vertex_outline_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;
    
	float4 world = mul(IN.Position, WorldViewProjection);
	float4 position = mul(world, PositionMatrix);
	float4 normal = mul(IN.Normal, WorldViewProjection);
	float4 normalPos = mul(normal, PositionMatrix);
	//OUT.Position = position + mul(0.02f, normalPos);
	//OUT.Position = world + mul(0.02f, normal);
	OUT.Position = IN.Position;
    OUT.Tex = IN.Tex;
	
    return OUT;
}

/************* Pixel Shader *************/

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
	float4 rgb;
	rgb.r = abs(IN.Tex.x * 6.0f - 3.0f) - 1.0f;
	rgb.g = 2.0f - abs(IN.Tex.x * 6.0f - 2.0f);
	rgb.b = 2.0f - abs(IN.Tex.x * 6.0f - 4.0f);
	rgb.a = 1.0f;
	rgb = saturate(rgb);

	return rgb;
	//return float4(1.0f, 1.0f, 0.0f, 1.0f);
}

float4 pixel_outline_shader(VS_OUTPUT IN) : SV_Target
{
	return float4(0.0f, 1.0f, 0.0f, 1.0f);
}

/************* Techniques *************/

technique11 main11
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
		
		SetRasterizerState(BackCull);
    }
	pass p1
	{
		SetVertexShader(CompileShader(vs_5_0, vertex_outline_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_outline_shader()));

        SetRasterizerState(FrontCull);
	}
}