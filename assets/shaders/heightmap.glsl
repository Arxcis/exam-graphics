#prop Cull on back
#shader vertex
#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 vertex_uv;
layout(location = 3) in vec4 vertex_color;

out vec2 frag_UV;

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

    frag_UV = vertex_uv;
}



#shader fragment
#version 410

#define PI 3.141592
#define MAX_LIGHTS 8

in vec2  frag_UV;
out vec4 out_color;


uniform sampler2D map_diffuse;

uniform float ambientScale = 0;
uniform float diffuseScale = 1;

void main() 
{
    vec3 ambient = ambientScale * vec3(1); 
    vec3 diffuse = diffuseScale * texture(map_diffuse, frag_UV).rgb;

    vec3 light = ambient + diffuse;
    out_color = vec4(light, 1);
}

