// HLSL
// High level shading language

uniform extern float4x4 gWVP;

struct OutputVS
{
				// Semantic
	float4 posW: POSITION0;
	float4 col: COLOR0;
};

// Vertex shader
OutputVS TransformVS(float3 posL: POSITION0, float4 col: COLOR0 )
{
	OutputVS outVS = (OutputVS)0;
	outVS.posW = mul(float4(posL, 1.0), gWVP);
	outVS.col = col;
	return outVS;
}

// Pixel shader
float4 TransformPS(OutputVS outVS): COLOR0
{
	return outVS.col;
}

technique TransformTech
{
	pass P0
	{
		vertexShader = compile vs_2_0 TransformVS();
		pixelShader = compile ps_2_0 TransformPS();

		FillMode = Solid;
		CullMode = None;
	}
}