#version 140

/********************Entrant********************/
in vec4 vcolor;
/********************Sortant********************/
/*out vec4 RenderTarget0;*/
/********************Uniformes********************/
/*uniform vec4 Color;*/

/********************Fonctions********************/
void main()
{
	gl_FragColor = vcolor;
/*	RenderTarget0 = Color;*/
}
