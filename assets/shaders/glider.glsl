#prop Cull on back
#shader vertex
#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 vertex_color;

out float frag_Height;
out vec2  frag_UV;
out vec3  frag_Normal;
out vec4  frag_Position;

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

    frag_Position = m2w * position;
    frag_Height = position.y;    
    frag_UV = uv;
    frag_Normal = normal;
}







#shader fragment
#version 410

#define PI 3.141592
#define MAX_LIGHTS 8
#define NumbersOfSecondsInADay 86400.0
#define NumberOfDaysInAYear 365

in  float frag_Height;
in  vec3  frag_Normal;
in  vec2  frag_UV;
in  vec4  frag_Position;
out vec4  out_color;


uniform sampler2D map_diffuse;

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
    float ambientStrength = 2;
    vec3 ambient = ambientStrength * intensities;

    //Diffuse
    float diffuseScaler = 0.05;
    float diffusion     = max(dot(frag_Normal, lightDirection), 0.0);
    vec3  diffuse       = diffuseScaler * diffusion * intensities;


    //Specularity
    float specularScale  = 20.0;

    vec3 viewDirection    = normalize(view_position.xyz - frag_Position.xyz);
    vec3 reflectDirection = reflect(-lightDirection, normalize(frag_Normal));

    float specularPower   = pow(max(dot(viewDirection, reflectDirection), 0.0), 128);
    vec3  specular        = specularScale * specularPower * intensities;

    return (ambient + diffuse + specular);
}



void main()
{
    float cosine_correction = - PI/2;
    float suntime = (timeofday_seconds / NumbersOfSecondsInADay) * 2 * PI + cosine_correction;

    vec3  sunDirectionByTime = vec3(cos(suntime), sin(suntime), sun.direction.z);

    vec3 sunlight = OK_DirectionalLight(sunDirectionByTime, sun.intensities.rgb);  

    //
    // Map greyscale heightmap to terrain
    //
    vec3 diffuse = texture(map_diffuse, frag_UV).rgb;
    out_color = vec4(sunlight * diffuse, 1);
    


}

