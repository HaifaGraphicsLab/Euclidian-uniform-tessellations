#version 330 core


struct Light
{
	vec3 pos;
	vec3 ambientRGB;
	vec3 diffuseRGB;
	vec3 specularRGB;
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;
};

// We set this field's properties from the C++ code
uniform int numOfLights;
uniform Light lights[10];
uniform vec3 viewPos;

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec4 vertexColor;
// The final color of the fragment (pixel)
out vec4 FragColor;

//vec3 GetAmbientColor(Light l, Material m) {
//	return l.ambientRGB * m.ambientRGB * l.ambientStrength;
//}
//
//vec3 GetDiffuseColor(Light light, Material m, vec3 l, vec3 n) {
//	return light.diffuseRGB  * m.diffuseRGB * max(dot(l, n), 0.0f) * light.diffuseStrength;
//}
//vec3 GetSpecularColor(Light light, Material m, vec3 r, vec3 v) {
//	return light.specularRGB  * m.specularRGB * pow(max(dot(r, v), 0.0f), m.shininess) * light.specularStrength;
//}
//
//vec3 GetColor(Material m, Light light, vec3 fragPos, vec3 viewDirection, vec3 normal) {
//	vec3 n = normal;
//	vec3 v = viewDirection;
//	vec3 l = normalize(light.pos - fragPos);
//	vec3 r = reflect(-l, n);
//	vec3 color = vec3(0.0f);
//	color += GetAmbientColor(light, m);
//	color += GetDiffuseColor(light, m, l, n);
//	color += GetSpecularColor(light, m, r, v);
//	return color;
//}


void main()
{
	//vec3 normal;
	//if (normalMap == 1) {
	//	normal = texture(material.normalMap, fragTexCoords).rgb;
	//	normal = TBN * (normal * 2.0 - 1.0);
	//}
	//else {
	//	normal = fragNormal;
	//}

	//vec3 final_color = vec3(0.0f);
	//for (int i = 0; i < numOfLights; i++) {
	//	final_color += GetColor(material, lights[i], fragPos, normalize(viewPos - fragPos), normalize(normal));
	//}
	//if (hasTexture == 1) {
	//	if (cylinderMapping == 1) {
	//		float eps = atan(orig_fragPos[1] - orig_fragPos[0]);
	//		float theta = acos(orig_fragPos[2] / length(orig_fragPos));
	//		vec2 tex_coords = vec2(eps / 3.1416, theta / 3.1416);
	//		final_color *= vec3(texture(material.textureMap, tex_coords));
	//	}
	//	else {
	//		final_color *= vec3(texture(material.textureMap, fragTexCoords));
	//	}
	//}
	FragColor = vertexColor;
}
