#version 100

attribute vec2 loc;

uniform mat3 mvp_matrix;

void main()
{
	gl_Position.xyw = mvp_matrix * vec3(loc, 1.0);
	gl_Position.z = 0.0;
}
