
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;		// uv coordinates
	float3 worldPos		: POSITION;
};

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------


struct DirectionalLight
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 direction;
};

struct PointLight
{
	float4 color;
	float3 position;
};

cbuffer externalData : register(b0)
{
	DirectionalLight dLight1;
	DirectionalLight dLight2;
	PointLight pLight;
	float3 CameraPosition;
	float4 color;
	int onCanvas;
}

Texture2D DiffuseTexture : register (t0);
SamplerState Sampler : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);

// Diffuse Light (directional)
float3 dirToLight1 = -normalize(dLight1.direction);
float NdotL1 = dot(input.normal, dirToLight1);		// calculate N dot L which gives the light amount
NdotL1 = saturate(NdotL1);							// Clamp between 0 and 1
float4 lightValue1 = dLight1.diffuseColor * NdotL1 + dLight1.ambientColor;

float3 dirToLight2 = -normalize(dLight2.direction);
float NdotL2 = dot(input.normal, dirToLight2);		// calculate N dot L which gives the light amount
NdotL2 = saturate(NdotL2);							// Clamp between 0 and 1
float4 lightValue2 = (dLight2.diffuseColor * NdotL2) + dLight2.ambientColor;

// Point Light
float3 pLightDir = normalize(pLight.position - input.worldPos);
float NdotL3 = dot(input.normal, pLightDir);		// calculate N dot L which gives the light amount
NdotL3 = saturate(NdotL3);
float4 lightValue3 = pLight.color * NdotL3;

// Specular Light
float3 dirToCamera = normalize(CameraPosition - input.worldPos);

// incoming light dir
float3 refl = reflect(-pLightDir, input.normal);

float specExp = 128;
float spec = pow(saturate(dot(dirToCamera, refl)), specExp);

float4 surfaceColor = DiffuseTexture.Sample(Sampler,input.uv);

float4 result;
if (onCanvas) {
	if (surfaceColor.r == 1.0f && surfaceColor.b == 1.0f)
		discard;
	result = surfaceColor + color;
}
else {
	result = (lightValue2 * surfaceColor + lightValue1 * surfaceColor + lightValue3 * surfaceColor + spec.xxxx * surfaceColor) + color;
}
return result;
//return (lightValue2  + lightValue1  + lightValue3  + spec.xxxx );
// Just return the input color
// - This color (like most values passing through the rasterizer) is 
//   interpolated for each pixel between the corresponding vertices 
//   of the triangle we're rendering
//return float4(1, 0, 0, 1);
}