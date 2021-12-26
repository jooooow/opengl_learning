#version 330 core

layout(location = 0) out vec4 color;

in vec3 vertex_color;
uniform float color_rotation;

float fmod(float a, float b);

void main()
{
	float r = vertex_color.x;
	float g = vertex_color.y;
	float b = vertex_color.z;

	float max_val = max(max(r,g),b);
	float min_val = min(min(r,g),b);
	float V = max_val;
	float S = 1.0f - min_val / max_val;
	float H;
	if(max_val == min_val)
        H = 0.0f;
    else if(max_val == r && g >= b)
        H = 60.0f * (g - b) / (max_val - min_val) + 0.0f;
    else if(max_val == r && g < b)
        H = 60.0f * (g - b) / (max_val - min_val) + 360.0f;
    else if(max_val == g)
        H = 60.0f * (b - r) / (max_val - min_val) + 120.0f;
    else
        H = 60.0f * (r - g) / (max_val - min_val) + 240.0f;

	H += color_rotation;
    H = fmod(H, 360.0f);

	float C = V * S;
    float X = C * (1 - abs(fmod(H / 60.0f, 2) - 1));
    float m = V - C;
    int flag = int(H / 60.0f);

    float R, G, B;

    switch (flag)
    {
    case 0:R = C, G = X, B = 0; break;
    case 1:R = X, G = C, B = 0; break;
    case 2:R = 0, G = C, B = X; break;
    case 3:R = 0, G = X, B = C; break;
    case 4:R = X, G = 0, B = C; break;
    case 5:R = C, G = 0, B = X; break;
    }

    r = R + m;
    g = G + m;
    b = B + m;

	color = vec4(r, g , b, 1.0);
};

float fmod(float a, float b)
{
	uint c = uint(a / b);
	return a - b * c;
}
