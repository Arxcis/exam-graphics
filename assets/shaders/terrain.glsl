#prop Cull on back
#shader vertex
#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 vertex_color;

out vec3 frag_Pos;
flat out vec3 frag_Normal;
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


    frag_Pos = vec3(m2w * position);    
    frag_UV = uv;
    frag_Normal = normal;
}

#shader fragment
#version 410
#define MAX_LIGHTS 8

// in vec3 frag_Pos;
flat in vec3 frag_Normal;
in vec2 frag_UV;
out vec4 out_color;



struct OK_Light_Directional {
    vec4 direction;
    vec4 intensities;
};

struct OK_Light_Point {
    vec4 position;
    vec4 intensities;
    float constant;
    float linear;
    float quadratic;
    float alignment;
};

layout(std140) uniform OK_Lights{
    OK_Light_Point light[MAX_LIGHTS];
    OK_Light_Directional sun;
};

layout(std140) uniform OK_Times {
    float elapsed_time;
    float timeofday_seconds;
    float timeofyear_days;
    float timeoftide_percent;
};


// @doc https://learnopengl.com/Lighting/Basic-Lighting - 2018-05-12
vec3 OK_DirectionalLight(in vec3 lightDir, in vec3 intensities, in vec3 in_normal) 
{
    //Ambience
    float ambientStrength = 3;
    vec3 ambient = ambientStrength * intensities;


    //Diffuse
    lightDir = normalize(lightDir);
    float diffusion = max(dot(in_normal, lightDir), 0.0);
    vec3 diffuse = diffusion * intensities;

    return (ambient + 0.1*diffuse);
}

uniform sampler2D map_diffuse;

uniform float time = 0;

void main() {
    
    float suntime = time * 0.5;
    vec3 sunDirectionByTime = vec3(cos(suntime)*1, sin(suntime)*1, sun.direction.z);


    vec3 sunlight = OK_DirectionalLight(sunDirectionByTime, sun.intensities.rgb, frag_Normal);  

    vec3 diffuse = texture(map_diffuse, frag_UV).rgb;
    out_color = vec4(sunlight * diffuse, 1);
}