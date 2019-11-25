#line 2

layout (local_size_x = 8, local_size_y = 8) in;
layout (rgba32f, binding = 0) uniform image2D img_out;

uniform vec2 center;
uniform float scale;

const int max_iter = 300;

// from https://en.wikipedia.org/wiki/Mandelbrot_set
int mandelbrot(float x0, float y0)
{
    float x = 0.0;
    float y = 0.0;

    int iter = 0;

    while (x*x + y*y < 2*2 && iter < max_iter)
    {
        float xtemp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtemp;
        iter++;
    }

    return iter;
}

void main()
{
    vec2 pixel_xy = gl_GlobalInvocationID.xy;
    vec2 xy = scale * (pixel_xy - center);

    int value = mandelbrot(xy.x, xy.y);

    vec4 color = vec4(0, 0, 0, 1);
    if (value < max_iter)
    {
        color = vec4(RGB_Palette_3[value % RGB_Palette_3_Size], 1.0);
    }

    imageStore(img_out, ivec2(pixel_xy), color);
}

