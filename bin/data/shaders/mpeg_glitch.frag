#version 410
#define TINY 0.000001

uniform sampler2D u_imageTex;
uniform sampler2D u_noiseTex;
uniform vec2 u_resolution;
uniform float u_blockSize;
uniform float u_time;

in vec2 texCoordVarying;

out vec4 outputColor;

void main()
{
    float time = u_time / 100;

	vec2 uv = texCoordVarying.xy;
	vec2 block = floor(uv * vec2(u_blockSize)) /  vec2(u_blockSize);
	vec2 uv_noise = block;

	uv_noise += mod(time * vec2(0.001234, 0.004657), 10.0) / 10;
	//uv_noise += time * vec2(0.001234, 0.004657);
	
	float block_thresh = pow(fract(time * 0.01236), 2.0) * 0.1;
	float line_thresh  = pow(fract(time * 0.02236), 3.0) * 0.5;
	
	vec2 uv_r = uv, uv_g = uv, uv_b = uv;

	// glitch some blocks and lines
	if (texture(u_noiseTex, uv_noise).r < block_thresh ||
		texture(u_noiseTex, vec2(uv_noise.y, 0.0)).g < line_thresh) {

		vec2 dist = (fract(uv_noise) - 0.5) * 0.3;
		uv_r += dist * 0.1;
		uv_g += dist * 0.2;
		uv_b += dist * 0.125;
	}

	outputColor.r = texture(u_imageTex, uv_r).r ;
	outputColor.g = texture(u_imageTex, uv_g).g;
	outputColor.b = texture(u_imageTex, uv_b).b;
    outputColor.a = texture(u_imageTex, uv  ).a;

	// loose luma for some blocks   
	if (texture(u_noiseTex, uv_noise).g < block_thresh)
		outputColor.rgb = outputColor.ggg ;
    
    
	// discolor block lines
	if (texture(u_noiseTex, vec2(uv_noise.y, 0.0)).b * 3.5 < line_thresh)
		outputColor.rgb = vec3(0.0, dot(outputColor.rgb, vec3(1.0)), 0.0);
    
	// interleave lines in some blocks
	if (texture(u_noiseTex, uv_noise).g * 1.5 < block_thresh ||
		texture(u_noiseTex, vec2(uv_noise.y, 0.0)).g * 2.5 < line_thresh) {
		float line = fract(texCoordVarying.y / 3.0);
		vec3 mask = vec3(1.0, 0.1, 0.1);
		if (line > 0.333)
			mask = vec3(0.1, 1.0, 0.1);
		if (line > 0.666)
			mask = vec3(0.1, 0.1, 1.0);
		
		outputColor.xyz *= mask;
	}
    
    //outputColor = texture(u_noiseTex, uv_noise);
}  