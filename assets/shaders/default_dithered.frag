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

out vec4 FragColor;
in vec3 v_position;
in vec3 v_normal;
in vec2 v_tcoord;
in float v_time;

const int[256] dither_table = int[](
    0, 128, 32, 160, 8, 136, 40, 168, 2, 130, 34, 162, 10, 138, 42, 170,
    192, 64, 224, 96, 200, 72, 232, 104, 194, 66, 226, 98, 202, 74, 234, 106,
    48, 176, 16, 144, 56, 184, 24, 152, 50, 178, 18, 146, 58, 186, 26, 154,
    240, 112, 208, 80, 248, 120, 216, 88, 242, 114, 210, 82, 250, 122, 218, 90,
    12, 140, 44, 172, 4, 132, 36, 164, 14, 142, 46, 174, 6, 134, 38, 166,
    204, 76, 236, 108, 196, 68, 228, 100, 206, 78, 238, 110, 198, 70, 230, 102,
    60, 188, 28, 156, 52, 180, 20, 148, 62, 190, 30, 158, 54, 182, 22, 150,
    252, 124, 220, 92, 244, 116, 212, 84, 254, 126, 222, 94, 246, 118, 214, 86, 
    3, 131, 35, 163, 11, 139, 43, 171, 1, 129, 33, 161, 9, 137, 41, 169,
    195, 67, 227, 99, 203, 75, 235, 107, 193, 65, 225, 97, 201, 73, 233, 105,
    51, 179, 19, 147, 59, 187, 27, 155, 49, 177, 17, 145, 57, 185, 25, 153,
    243, 115, 211, 83, 251, 123, 219, 91, 241, 113, 209, 81, 249, 121, 217, 89,
    15, 143, 47, 175, 7, 135, 39, 167, 13, 141, 45, 173, 5, 133, 37, 165,
    207, 79, 239, 111, 199, 71, 231, 103, 205, 77, 237, 109, 197, 69, 229, 101,
    63, 191, 31, 159, 55, 183, 23, 151, 61, 189, 29, 157, 53, 181, 21, 149,
    255, 127, 223, 95, 247, 119, 215, 87, 253, 125, 221, 93, 245, 117, 213, 85
);

vec3 hash32(vec2 p) {
	vec3 p3 = vec3(p.xyx) * vec3(443.8975, 397.2973, 491.1871);
	p3 = p3 - floor(p3);
    p3 += dot(p3, p3.yxz + 19.19);
	vec3 r3 = vec3((p3.x + p3.y) * p3.z, (p3.x + p3.z) * p3.y, (p3.y + p3.z) * p3.x);
	r3 = r3 - floor(r3);
	return r3;
}

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

    int roundedTime = int(v_time * 8.0);

    vec2 hashInput = vec2(float(roundedTime) * 7.0, float(roundedTime) * 11.0);
    vec3 offset = hash32(hashInput) * 16.0; 

    int x = int(mod(gl_FragCoord.x + offset.x, 16.0));
	int y = int(mod(gl_FragCoord.y + offset.y, 16.0));

	float limit = (float(dither_table[x + y * 16] + 1.)) / 256.0;
    
    if (textureAlbedo.a < limit) {
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
