#prop Cull on back
#prop Blend SrcAlpha OneMinusSrcAlpha
#shader vertex
#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 vert_color;

out vec3 frag_Pos;
out vec2 frag_UV;
out vec4 vert_Color;

uniform mat4 m2w;
layout(std140) uniform OK_Matrices{
    mat4 projection;
    mat4 view;
    vec4 view_position;
};

layout(std140) uniform OK_Times {
    float elapsed_time;
    float timeofday_seconds;
    float timeofyear_days;
    float timeoftide_percent;
};


vec4 MVP(in vec4 position) {
    return projection * view * m2w * position;
}


#define PI 3.141592
#define WaveRange 0.005
#define TideRange 1.0

void main() {

    vec4 out_position = MVP(position);


    float sinmaxmintime = sin(elapsed_time*0.5 * PI)+0.5;

    vec4 waveadjusted_position = out_position          + vec4(0,(sinmaxmintime * WaveRange)-0.2,0,0);
    vec4 tideadjusted_position = waveadjusted_position + vec4(0, TideRange * timeoftide_percent/100, 0,0);




    gl_Position = tideadjusted_position;



    frag_Pos = vec3(m2w * position);
    
    frag_UV = uv;
    vert_Color = vert_color;
}

#shader fragment
#version 410

in vec3 frag_Pos;
in vec2 frag_UV;
in vec4  vert_Color;
out vec4 out_color;

uniform sampler2D height_map;

uniform float time = 0;



void main() {
  
    float height = frag_Pos.y;

    out_color = vec4(vert_Color.rgb, .25);
}