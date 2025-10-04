#version 460 core

struct Light {
    vec3 position;        // 16   0
    vec3 direction;       // 16   16
    vec3 color;           // 16   32
    float attenuation;    // 4    48
};

layout(binding = 0) uniform sampler2D albedo;
layout(binding = 1) uniform sampler2D emissive;
layout(binding = 2) uniform sampler2D normal;
layout(binding = 3) uniform sampler2D specular;

layout (std140, binding = 4) uniform FrameData {
    mat4 u_view;
    vec3 u_position;
    vec3 u_direction;
    vec2 u_resolution;
    float u_time;
};

layout (std140, binding = 5) uniform LightData {
    int u_activeLights;
    vec3 u_ambientColor;
    Light u_Lights[128];
};

in vec3 v_position;
in vec3 v_normal;
in vec2 v_tcoord;


out vec4 FragColor;

vec3 phong(vec3 surfacePosition, vec3 lightPosition, vec3 color, vec3 texNormal, float ambientFactor, float diffuseFactor, float specularPower) {
    
    vec3 surfaceNormal = normalize(v_normal);//normalize(v_normal + texNormal);
    
    vec3 lightDirection = normalize(lightPosition - surfacePosition);
    float lightToSurfaceDistance = distance(lightPosition, surfacePosition);

    vec3 V = normalize(u_position - surfacePosition);
    vec3 R = normalize(reflect(-lightDirection, surfaceNormal));

    float dotNL = max(dot(surfaceNormal, lightDirection), 0.0);
    float dotVR = max(dot(V, R), 0.0);

    vec3 lighting = vec3(0.0);

    //Intensity of the diffuse light.
    float diffuseIntensity = clamp(dot(surfaceNormal, lightDirection), 0.0, 1.0);

    // Calculate the diffuse light factoring in light color, power and the attenuation.
    lighting += diffuseIntensity * color * (diffuseFactor / lightToSurfaceDistance);

    //Calculate the half vector between the light vector and the view vector.
    vec3 H = normalize(lightDirection + u_direction);

    //Intensity of the specular light
    float NdotH = dot(surfaceNormal, H);
    float specularIntensity = pow(clamp(NdotH, 0.0, 1.0), specularPower);

    return lighting;
}

vec3 vec3_Lerp(vec3 v0, vec3 v1, vec3 t) {
  return v0 + t * (v1 - v0);
}

vec3 vec3_CubeLerp(vec3 v0, vec3 v1, vec3 t) {
    vec3 t1 = t * t;
    vec3 t2 = -(t - vec3(1.0)) * (t - vec3(1.0)) + vec3(1.0);
    return vec3_Lerp(v0, v1, vec3_Lerp(t1, t2, t));
}

vec4 point_light(Light light, vec3 texNormal, float ambientFactor, float diffuseFactor, float specularPower) {
    vec4 lighting = vec4(phong(v_position, light.position, light.color, texNormal, ambientFactor, diffuseFactor, specularPower).xyz, 0.0);
    float attenuation = 1.0 - clamp(light.attenuation / length(light.position - v_position), 0.0, 1.0);
    lighting = vec4(lighting.xyz, attenuation);
    return lighting;
}

void main() {
    vec4 textureAlbedo = texture(albedo, v_tcoord);
    vec3 textureNormal = texture(normal, v_tcoord).rgb;
    vec3 textureEmissive = texture(emissive, v_tcoord).rgb;
    float textureSpecular = texture(specular, v_tcoord).r;
    float textureMetallic = texture(specular, v_tcoord).g;

    if (textureAlbedo.a < 0.5) {
        discard;
    }

    vec4 lightingCalc = vec4(0.0);
    vec3 lighting = vec3(0.0);

    for(int i = 0; i < u_activeLights && i < 128; i++) {
        lightingCalc += point_light(u_Lights[i], textureNormal, 0.2, 0.8, textureSpecular);
    }

    lighting = vec3_Lerp(lightingCalc.xyz, u_ambientColor, lightingCalc.www);
    vec4 finalLighting = vec4(vec3_CubeLerp(lighting, textureEmissive * textureEmissive * textureEmissive, textureEmissive), 1.0);    
    FragColor = textureAlbedo * finalLighting;
}