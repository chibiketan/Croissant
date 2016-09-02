#version 140

/********************Entrant********************/
in vec4 VertexPosition;
in vec4 VertexColor;

/********************Uniformes********************/
uniform mat4 WorldViewProjMatrix;
uniform mat4 ModelWorldMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

/********************Fonctions********************/
out vec4 vcolor;

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelWorldMatrix * vec4(VertexPosition.xyz, 1.0);
	/*gl_Position = WorldViewProjMatrix * vec4(VertexPosition, 1.0);*/
	/*gl_Position = matrix * vec4(VertexPosition, 1.0);*/
	vcolor = VertexColor;
	/*gl_Position = vec4(VertexPosition.xyz, 1.0);*/
}
