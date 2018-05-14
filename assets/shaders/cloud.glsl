#prop Cull back
#prop Blend SrcAlpha OneMinusSrcAlpha
#shader vertex
#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 vertex_color;

out vec3 frag_Pos;
out vec2 tex_coord;

uniform mat4 m2w;
layout(std140) uniform OK_Matrices{
    mat4 projection;
    mat4 view;
    vec4 view_position;
};

vec4 MVP(in vec4 position) {
    return projection * view * m2w * position;
}

void main() {
    vec4 out_position = MVP(position);
    gl_Position = out_position;
    frag_Pos = vec3(m2w * position);
    
    tex_coord = uv;
}


#shader fragment
#version 410
#define PI 3.141592

in vec3  frag_Pos;
in vec2  tex_coord;
out vec4 out_color;

uniform sampler2D map_diffuse;

uniform float time = 0;

// 2D random
float random (vec2 st) {
    return fract(sin(dot(
                st.xy, 
                vec2(12.9898, 28.233)
            )
        ) * 43758.5453123);
}


// 2D Noise based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Generate four corners of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    // Cubic Hermine Curve.  Same as SmoothStep()
    vec2 u = f*f*(3.0-2.0*f);

    // Mix(Interpolate) 4 corner percentages
    return mix(a,b, u.x) +
              (c - a) * u.y * (1.0 - u.x) + 
              (d - b) * u.x * u.y;
}


void main() {
    float n = noise(frag_Pos.xz * 0.05 + vec2(time*0.2) + frag_Pos.y);
    out_color = vec4(1,1,1,n-0.6);
}