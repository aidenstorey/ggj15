cbuffer cbPerObject
{
	float4x4 gWVP;
};

Texture2D gTexture;

BlendState SrcAlphaBlendingAdd
{
	AlphaToCoverageEnable = TRUE;
	BlendEnable[ 0 ] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ONE;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[ 0 ] = 0x0F;
};

SamplerState gDefaultSam
{
};

struct VS_IN
{
	float3 pos		: POSITION;
	float2 texc		: TEXCOORD;
};

struct PS_IN
{
	float4 posH		: SV_POSITION;
	float2 texc		: TEXCOORD;
};

PS_IN VS( VS_IN vIn )
{
	PS_IN vOut;

	vOut.posH	= mul( float4( vIn.pos, 1.0f ), gWVP );
	vOut.texc	= vIn.texc;

	return ( vOut );
}

float4 PS( PS_IN pIn ) : SV_Target
{
	return ( gTexture.Sample( gDefaultSam, pIn.texc ) );
}

technique10 TexturedTech
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
		SetBlendState( SrcAlphaBlendingAdd, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
	}
}