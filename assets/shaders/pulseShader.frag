uniform sampler2D texture;
uniform float time;

void main(){
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	float brightness = 0.5 + 0.5 * sin(time * 2.0);
	gl_FragColor = vec4(pixel.rgb * brightness, pixel.a);
}