#line 2

layout (local_size_x = 8, local_size_y = 8) in;
layout (rgba32f, binding = 0) uniform image2D img_out;

//
// Focal length
//

uniform float f;

//
// Principal point
//

uniform vec2 p;

//
// Spatial resolution (mm)
//

uniform vec2 s;

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
// Return value: (t, u, v)
// t : "Time" value along the ray direction.
//     No intersection detected if t < 0
// u : Horizontal texture coordinate
// v : Vertical texture coordinate
//
vec3 
quad_intersect(
    vec3 p, // Upper-left corner in world space
    vec3 n, // Plane normal vector in world space
    vec3 e1,// Horizontal direction vector, ||e1|| == width
    vec3 e2,// Vertical direction vector,   ||e2|| == height
    vec3 origin, vec3 direction // Ray parameters
    )
{
    vec3 invalid = vec3(-1.0, 0.0, 0.0);
    float denom = dot(direction, n);
    if (denom > 1e-6)
    {
        vec3 diff = p - origin;
        float t = dot(diff, n);
        if (t >= 0.0)
        {
            vec3 p_i = origin + t * direction;
            vec3 diff = p_i - p;

            float w = dot(diff, e1) / length(e1);
            if (w < 0 || (w > length(e1))) { return invalid; }

            float h = dot(diff, e2) / length(e2);
            if (h < 0 || (h > length(e2))) { return invalid; }

            return vec3(t, w / length(e1), h / length(e2));
        }
    }

    return invalid;
}

vec4 color_quad(float u, float v)
{
    //
    // 8x8 chess board, black & white.
    //
    float num_cells_x = 8.0;
    float num_cells_y = 8.0;

    int x = int(num_cells_x * u);
    int y = int(num_cells_y * v);

    if (((x + y) & 1) == 0)
    {
        return vec4(0.0, 0.0, 0.0, 1.0);
    }
    else
    {
        return vec4(1.0, 1.0, 1.0, 1.0);
    }
}

void main()
{
    vec3 origin = vec3(0.0, 0.0, 0.0);

    vec2 size = imageSize(img_out);

    // vec3 U = vec3(s.x, 0.0, 0.0);
    // vec3 V = vec3(0.0, -s.y, 0.0);

    //vec3 U = vec3(4.0, 0.0, 0.0);
    //vec3 V = vec3(0.0, -3.0, 0.0);

    vec3 U = vec3(0.00376, 0.0, 0.0);
    vec3 V = vec3(0.0, -0.00274, 0.0);

    vec3 upper_left = vec3(-0.5 * U.x, -0.5 * V.y, -0.0036);
    //vec3 upper_left = vec3(-0.5 * U.x, -0.5 * V.y, -f);
    vec2 pixel_xy = gl_GlobalInvocationID.xy;
    vec2 uv = pixel_xy / size;

    vec3 direction = upper_left + uv.x * U + uv.y * V;

    //
    // TODO: Model matrix uniform parameter.
    //

    float quad_width = 4.0;
    float quad_height = 4.0;
    vec3 quad_point = vec3(-quad_width / 2.0, -quad_height / 2.0, -3.0);

    //
    // Normal faces directly into camera, so we should get a big red screen.
    //
    
    vec3 quad_w = vec3(quad_width,         0.0,  0.0);
    vec3 quad_h = vec3(0.0       , quad_height,  0.0);

    vec3 n = normalize(vec3(0.0, 0.0, -1.0));

    vec3 hit_test =
        quad_intersect(quad_point, n, quad_w, quad_h, origin, direction);
    if (hit_test.x > 0.0) 
    {
        vec4 color = color_quad(hit_test.y, hit_test.z);
        imageStore(img_out, ivec2(pixel_xy), color);
    }
    else
    {
        vec4 color = vec4(ray_to_color(origin, direction), 1.0);
        imageStore(img_out, ivec2(pixel_xy), color);
    }
}

