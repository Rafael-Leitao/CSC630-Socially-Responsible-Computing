#version 330 core 
in vec3 fragNor;
out vec4 color;

uniform vec3 MatAmb;

void main()
{
	vec3 normal = normalize(fragNor);
	// fix these
	//float dc = ?;
	//vec3 halfV = ?;
	//float sc = ?;
	color = vec4(MatAmb, 1.0);

}
