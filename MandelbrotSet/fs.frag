#version 330
out vec3 color;

uniform int WINDOW_SIZE_X;
uniform int WINDOW_SIZE_Y;
uniform float zoomfactor;
uniform float centerx;
uniform float centery;
uniform int useriterations;
float stepx = 4.0 / WINDOW_SIZE_X;
float stepy = 4.0 / WINDOW_SIZE_Y;
const int MaxIterations = 500;


//(x+iy)*(x+iy) = x^2+xiy+xiy+y^2i^2 = x^2+2xyi-y^2 => RE: x^2-y^2 IM: 2xy

void main()
{
	vec2 z, c;
	z.x = 0;
	z.y = 0;

	c.x = (gl_FragCoord.x * stepx-2)*zoomfactor+centerx;
	c.y = (gl_FragCoord.y * stepy-2)*zoomfactor+centery;

	int i;
	for(i = 0; i < useriterations; i++)
	{
		float x = z.x*z.x - z.y*z.y + c.x;
		float y = 2*(z.x*z.y) + c.y;
		z.x = x;
		z.y = y;

		if(((z.x*z.x+z.y*z.y) >= 4))
		{
			break;
		}
	}

	if(i == useriterations)
	{
		color = vec3(0,0,0);
	}
	else
	{
		color = vec3(0,0,(i/255.0f)*(255.0f/useriterations));
	}
}