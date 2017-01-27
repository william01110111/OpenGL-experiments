#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;

const float blurSize=0.04;
const int blurIter=12;

void main()
{
	vec3 clrAvg=vec3(0, 0, 0);
	
	for (int x=0; x<blurIter; x++)
	{
		for (int y=0; y<blurIter; y++)
		{
			clrAvg+=texture(ourTexture, vec2(TexCoord.x+float(x)*blurSize/blurIter, TexCoord.y+float(y)*blurSize/blurIter)).rgb;
		}
	}
	
	//color = texture(ourTexture, TexCoord);
	color=vec4(clrAvg/(blurIter*blurIter), 1.0f);
}

