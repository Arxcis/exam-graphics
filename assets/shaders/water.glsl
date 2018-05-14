#prop Cull on back
#prop Blend SrcAlpha OneMinusSrcAlpha
#shader vertex
#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 vert_color;


out vec4 frag_Position;
out float frag_Height;
out vec2 frag_UV;
out vec3 frag_Normal;

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
#define WaveRange 0.025
#define TideRange 3
#define Offset .06

void main() {



    //
    // Adjust vertx height
    //
    float sineMixin = sin(elapsed_time*0.5 * PI)+0.5;

    vec4 waveadjusted_position  = position              + vec4(0,(sineMixin * WaveRange),0,0);
    vec4 tideadjusted_position  = waveadjusted_position + vec4(0, TideRange * timeoftide_percent/100, 0,0);
   
    vec4 randomadjusted_positon = tideadjusted_position * vec4(1, vert_color.r/10.0, 1,1);

    vec4 offsetAdjusted_position = randomadjusted_positon + vec4(0, Offset, 0,0);


    vec4 out_position = MVP(offsetAdjusted_position);

    gl_Position = out_position;


    //
    // Adjust normals
    //
    frag_Normal = normalize(
        vec3(
            sin(elapsed_time * vert_color.b * 1.2)*0.1+1.0, 
            0, 
            cos(elapsed_time * vert_color.g * 1.2)*0.1+1.0
        )
    );

    //
    // Buffer data to framgment shader
    //
    frag_Position  = m2w * position;
    frag_Height    = position.y;
    frag_UV        = uv;

    vert_Color = vert_color;
}


#shader fragment
#version 410

#define PI 3.141592
#define MAX_LIGHTS 8
#define NumbersOfSecondsInADay 86400.0
#define NumberOfDaysInAYear 365

in vec4  frag_Position;
in vec3  frag_Normal;
in float frag_Height;
in vec2  frag_UV;
in vec4  vert_Color;
out vec4 out_color;

uniform sampler2D height_map;

uniform float time = 0;


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

layout(std140) uniform OK_Matrices{
    mat4 projection;
    mat4 view;
    vec4 view_position;
};

// @doc https://learnopengl.com/Lighting/Basic-Lighting - 2018-05-12
vec3 OK_DirectionalLight(in vec3 lightDirection, in vec3 intensities) 
{
    // Normalize input vectors
    lightDirection = normalize(lightDirection);

    //Ambience
    float ambientStrength = 0;
    vec3 ambient = ambientStrength * intensities;

    //Diffuse
    float diffuseScaler = 0.00001; 
    float diffusion     = max(dot(frag_Normal, lightDirection), 0.0);
    vec3  diffuse       = diffuseScaler * diffusion * intensities;


    //Specularity
    float specularScale  = 6;

    vec3 viewDirection    = normalize(view_position.xyz - frag_Position.xyz);
    vec3 reflectDirection = reflect(-lightDirection, normalize(frag_Normal));

    float specularPower   = pow(max(dot(viewDirection, reflectDirection), 0.0), 64);
    vec3  specular        = specularScale * specularPower * intensities;

    return (ambient + diffuse + specular);
}


void main() {
    float cosine_correction = - PI/2;
    float suntime = (timeofday_seconds / NumbersOfSecondsInADay) * 2 * PI + cosine_correction;

    vec3  sunDirectionByTime = vec3(cos(suntime), sin(suntime), sun.direction.z);

    vec3 sunlight = OK_DirectionalLight(sunDirectionByTime, sun.intensities.rgb);  

    float alpha = .30;
    vec3 color = vec3(0,0,.24);
    out_color = vec4(sunlight+ color, alpha);
}