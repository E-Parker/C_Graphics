#version 460 core

struct Light {
    vec3 position;        // 16   0
    vec3 direction;       // 16   16
    vec3 color;           // 16   32
    float attenuation;    // 4    48
};


layout (std140, binding = 0) uniform FrameData {
    mat4 u_view;
    vec3 u_position;
    vec3 u_direction;
    float u_time;
};

layout (std140, binding = 1) uniform LightData {
    int u_activeLights;
    vec3 u_ambientColor;
    Light u_Lights[128];
};

in vec3 v_position;
in vec3 v_normal;
in vec2 v_tcoord;

uniform sampler2D albedo;
uniform sampler2D emissive;
uniform sampler2D normal;
uniform sampler2D specular;

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

vec3 point_light(Light light, vec3 texNormal, float ambientFactor, float diffuseFactor, float specularPower, vec3 ambientColor) {
    vec3 lighting = phong(v_position, light.position, light.color, texNormal, ambientFactor, diffuseFactor, specularPower);
    float attenuation = clamp(light.attenuation / length(light.position - v_position), 0.0, 1.0);
    lighting = vec3_Lerp(ambientColor, lighting, attenuation.xxx);
    return lighting;
}

void main() {

    vec4 textureAlbedo = texture(albedo, v_tcoord);
    vec3 textureNormal = texture(normal, v_tcoord).rgb * 0.5 + 0.5;
    vec3 textureEmissive = texture(emissive, v_tcoord).rgb;
    float textureSpecular = texture(specular, v_tcoord).r;
    float textureMetallic = texture(specular, v_tcoord).g;

    if (textureAlbedo.a < 0.5) {
        discard;
    }

    vec3 lighting = vec3(0.0);

    for(int i = 0; i < u_activeLights && i < 128; i++) {
        lighting += point_light(u_Lights[i], textureNormal, 0.2, 0.8, textureSpecular, u_ambientColor);
    }

    //vec4 finalLighting;
    //finalLighting = textureEmissive.xyzx;// vec4(vec3_CubeLerp(lighting, textureEmissive * textureEmissive * textureEmissive, textureEmissive), 1.0);
    
    FragColor = textureAlbedo * vec4(lighting, 1.0);
}