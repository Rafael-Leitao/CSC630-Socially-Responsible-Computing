#version 330 core
in vec3 fragNor;
out vec4 color;

uniform vec3 MatAmb;
uniform vec3 lightP;

void main()
{
	vec3 normal = normalize(fragNor);
	vec3 normalLight = normalize(lightP);
	float cosTheta = dot(normal, normalLight);
	color = vec4(cosTheta * MatAmb, 1.0);
}
