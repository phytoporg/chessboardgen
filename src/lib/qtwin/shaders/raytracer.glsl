#line 2

layout (local_size_x = 8, local_size_y = 8) in;
layout (rgba32f, binding = 0) uniform image2D img_out;

//
// Blend from blue to white based on the up/downness of the input ray.
//

vec3 ray_to_color(vec3 origin, vec3 direction)
{
    vec3 normal = normalize(direction);
    float t = 0.5 * normal.y + 1.0;
    return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

//
// TODO: actual ray-object intersection with something of interest.
//

void main()
{
    vec3 origin = vec3(0.0, 0.0, 0.0);
    vec3 lower_left = vec3(-2.0, -1.0, -1.0);
    vec3 horizontal = vec3(4.0, 0.0, 0.0);
    vec3 vertical = vec3(0.0, 2.0, 0.0);

    vec2 pixel_xy = gl_GlobalInvocationID.xy;
    vec2 size = imageSize(img_out);
    vec2 uv = pixel_xy / size;

    vec3 direction = lower_left + uv.x * horizontal + uv.y * vertical;
    vec4 color = vec4(ray_to_color(origin, direction), 1.0);

    imageStore(img_out, ivec2(pixel_xy), color);
}

