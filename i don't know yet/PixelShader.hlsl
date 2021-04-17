
struct PS_IN
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 worldpos : WORLDPOS;
};
cbuffer pcb
{
    float3 lightpos;
    float padding;
    float3 _color;
};
float4 main(PS_IN input) : SV_Target
{
    float ambientStrength = 0.1f;
    float3 ambientColor = { 0.8f, 0.8f, 0.8f };
    float3 diffuseStrength = 1.2f;
    float3 diffuseColor = _color;
    float k1 = 1.0f;
    float k2 = 0.045f;
    float k3 = 0.0075f;
    float4 lp = { lightpos, 1.0f };
    float dist = length(lightpos - input.worldpos);
    float attenuation = 1.0f / (k1 + (k2 * dist) + (k3 * dist * dist));
    
    float3 ambient = ambientStrength * ambientColor;
    
    
    float3 vecToLight = normalize(lightpos - input.worldpos);
    
    float diffuseIntensity = max(dot(vecToLight, input.normal), 0.0f);
    float3 diffuse =  attenuation*diffuseIntensity * diffuseStrength * diffuseColor ;
    
    
    float4 color = { ambient + diffuse,1.0f };
    
    return color;     
    
}