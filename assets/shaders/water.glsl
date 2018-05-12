#prop Cull on back
#prop Blend SrcAlpha OneMinusSrcAlpha
#shader vertex
#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 vert_color;

out vec3 frag_Pos;
out vec2 tex_coord;
out vec4 vert_Color;

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
    vert_Color = vert_color;
}

#shader fragment
#version 410

in vec3 frag_Pos;
in vec2 tex_coord;
in vec4  vert_Color;
out vec4 out_color;

uniform sampler2D height_map;

uniform float time = 0;

void main() {
  
    float height = frag_Pos.y;

    out_color = vec4(vert_Color.rgb,.15);
}