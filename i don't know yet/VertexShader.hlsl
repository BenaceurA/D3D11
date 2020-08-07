
struct VS_IN
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VS_Out
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 worldpos : WORLDPOS;
};

cbuffer vcb
{
    matrix transformation;
    matrix world;
};

VS_Out main(VS_IN input) 
{
    VS_Out output;
    
    output.position = mul(float4(input.position, 1.0f), transformation);
    output.normal = normalize(mul(float4(input.normal, 0.0f), world));
    output.worldpos = mul(float4(input.position, 1.0f), world);
    return output;
}