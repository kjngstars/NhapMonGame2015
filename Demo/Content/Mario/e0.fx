struct VS_output
{
	float2 texCoord0 : TEXCOORD0;
};

struct PS_output
{
	float4 c : COLOR0;
};

uniform extern texture texture0;

sampler textureS0 = sampler_state
{
	Texture = <texture0>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

float4 BloomExtract(float4 color)
{
	return saturate(
		(color - 0.5f) /
		(1 - 0.5f));
}

float4 PS_main(VS_output input) : COLOR0
{
	float4 result = tex2D(textureS0, input.texCoord0);

	//return result;
	return float4(1, 0, 0, 1);
}

technique DefaultTech
{
	pass pass0
	{
		pixelShader = compile ps_2_0 PS_main();
	}
}