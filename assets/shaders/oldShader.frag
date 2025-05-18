uniform sampler2D texture;

void main(){
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	float r = dot(pixel.rgb, vec3(0.393, 0.869, 0.189));
	float g = dot(pixel.rgb, vec3(0.349, 0.686, 0.168));
	float b = dot(pixel.rgb, vec3(0.272, 0.534, 0.134));
	gl_FragColor = vec4(vec3(r, g, b), pixel.a);
}