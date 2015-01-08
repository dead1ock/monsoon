////////////////////////////////////////////////////////////////////////////////
// Filename: sprite.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer SpriteSheetBuffer
{
	int index;
	int sliceSizeX;
	int sliceSizeY;
	int sheetWidth;
	int sheetHeight;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType SpriteVertexShader(VertexInputType input)
{
    PixelInputType output;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	 // Store the texture coordinates for the pixel shader.
	int slicesX = sheetWidth / sliceSizeX;
	int slicesY = sheetHeight / sliceSizeY;
		
	output.tex = float2((input.tex.x / slicesX) + (((sheetWidth/slicesX) * float(index))/sheetWidth), (input.tex.y / slicesY) + (((sheetHeight/slicesY) * 0.0f)/sheetHeight));
    
    return output;
}