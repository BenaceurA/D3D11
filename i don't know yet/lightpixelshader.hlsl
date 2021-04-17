cbuffer pcb
{
    float4 lightpos;
    float4 _color;
};

float4 main() : SV_Target
{
    float4 color = _color;

    return color;
}
