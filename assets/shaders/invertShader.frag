uniform sampler2D texture;

void main(){
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	gl_FragColor = vec4(1.0 - pixel.rgb, pixel.a);
}