#version 460 core

out vec4 FragColor;
in vec3 v_position;
in vec3 v_normal;
in vec2 v_tcoord;
in vec2 v_resolution;
in float v_time;

uniform sampler2D albedo;

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


void main() {
    vec4 color = texture(albedo, v_tcoord);
    float scaledDepth = (4.0 * gl_FragCoord.z - gl_DepthRange.near - gl_DepthRange.far) / (gl_DepthRange.far - gl_DepthRange.near);
	float clipDepth = scaledDepth / gl_FragCoord.w;

    //vec2 seed = vec2(v_time, v_time * 2.0) + gl_FragCoord.xy;
	//vec3 rand_v3 = hash32(seed);
	//float poly_rand = rand_v3.r * rand_v3.g;// * rand_v3.b;
    //
    //if(clipDepth < poly_rand || color.a < poly_rand) {
    //    discard;
    //}

    int roundedTime = int(v_time * 8.0);
    vec2 hashInput = vec2(float(roundedTime) * 7.0, float(roundedTime) * 11.0);
    vec3 hashOutput = hash32(hashInput + (gl_FragCoord.xy * 0.1)); 
    float offset = ((hashOutput.x * hashOutput.y * hashOutput.z) * 0.2) + 0.8;

    int x = int(mod(gl_FragCoord.x, 16.0));
	int y = int(mod(gl_FragCoord.y, 16.0));

	float limit = (float(dither_table[x + y * 16] + 1.)) / 256.0;
	
    if(clipDepth < limit || color.a < limit) {
        discard;
    }

    FragColor = vec4(color.rgb, 1.0);
}

