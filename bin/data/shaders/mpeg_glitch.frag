#version 410
#define TINY 0.000001

uniform sampler2D u_imageTex;
uniform sampler2D u_noiseTex;
uniform float     u_sizeOfKernel;
uniform float     u_stopAmount;
uniform float     u_globalStrength;
uniform vec2      u_resolution;
uniform int       u_greyScale;
uniform int       u_effects;
uniform float     u_time;

#define DISPLACE        int(1 << 0)
#define DISCOLOR_LINE   int(1 << 1)
#define RGB_LINES       int(1 << 2)
#define RGB_LINES_VERT  int(1 << 3)
#define LUMA            int(1 << 4)



in vec2 texCoordVarying;

out vec4 outputColor;

float nrand( vec2 n )
{
	return fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

float n2rand( vec2 n,float seed )
{
	float t = fract( seed );
	float nrnd0 = nrand( n + 0.07*t );
	float nrnd1 = nrand( n + 0.11*t );
	return (nrnd0+nrnd1) / 2.0;
}

vec3 getGlitchDisplace(vec2 uv,vec2 uv_noise,float block_thresh, float line_thresh,float amount) {
	vec2 uv_r = uv;
	vec2 uv_g = uv;
	vec2 uv_b = uv;
	vec3  result; 
	
	if (texture(u_noiseTex, uv_noise).r*amount < block_thresh ||
		texture(u_noiseTex, vec2(uv_noise.y, 0.0)).g*amount < line_thresh) {
		vec2 dist = (fract(uv_noise) - 0.5) * 0.3;
		uv_r += dist * 0.1;
		uv_g += dist * 0.2;
		uv_b += dist * 0.125;
	}
	result.r = texture(u_imageTex, uv_r).r;
	result.g = texture(u_imageTex, uv_g).g;
	result.b = texture(u_imageTex, uv_b).b;	
	return result;
}

vec3 getGlitchDiscolorLine(vec2 uv,vec2 uv_noise,float noiseTime,vec3 color,float line_thresh,float amount){
	vec3 locColor = texture(u_imageTex, vec2(uv.x+noiseTime, uv.y)).rgb ;


	if (texture(u_noiseTex, vec2(uv_noise.y, 0.0)).r * amount < line_thresh) {
		return mix(vec3(0, dot(locColor, vec3(locColor.r,locColor.g,1)), 0.0), locColor,fract(noiseTime*uv.y));
	}   else return color;
}

vec3 getGlitchLuma(vec2 uv_noise,vec3 color,float block_thresh,float amount) {
	if (texture(u_noiseTex, uv_noise).g *amount < block_thresh)
	     return color.ggg;
	else return color;
}

vec3 getGlitchRGBLines(vec2 uv,vec2 uv_noise,vec3 color,float block_thresh,float line_thresh,float amount) {
	if (texture(u_noiseTex, uv_noise).g * amount < block_thresh ||
		texture(u_noiseTex, vec2(uv_noise.y, 0.0)).g * amount*2 < line_thresh) {
		
		float lines = fract( (uv*u_resolution).y / 9.0);
		vec3 mask = vec3(3.0, 0.0, 0.0);
			
		if (lines > 0.333)
			mask = vec3(0.0, 3.0, 0.0);
		if (lines > 0.666)
			mask = vec3(0.0, 0.0, 3.0);
		
		 
			return color.xyz *= mask;
		}
	else return color;
}

vec3 getGlitchRGBLinesVert(vec2 uv,vec2 uv_noise,vec3 color,float block_thresh,float line_thresh,float amount) {

		if (texture(u_noiseTex, vec2(uv.y,0 )).g * amount < block_thresh ||
		texture(u_noiseTex, vec2(uv.x,0)).g * amount < line_thresh) {	
		float lines = fract((uv*u_resolution).y / 9.0);
		vec3 mask = vec3(3.0, 0.0, 0.0);
			
		if (lines > 0.333)
			mask = vec3(0.0, 3.0, 0.0);
		if (lines > 0.666)
			mask = vec3(0.0, 0.0, 3.0);				
			mask *= texture(u_noiseTex, vec2(uv_noise.x,uv_noise.y)).rgb;		
			return color.xyz * mask;
		}
        else return color;
}

float getNoisedTime(float amount, float time) {
	float randStop = n2rand(vec2(time,time),1);
	return randStop > amount? time : floor(time)/10.0;	
}

float getSizeofBlocks(float size,float noiseTime) {

	float sizeOfBlocks=(6*size)+floor(n2rand(vec2(noiseTime,noiseTime),noiseTime)*8);
	return pow(2,sizeOfBlocks);	
}


void main()
{
	float time = u_time;
	vec2 uv = texCoordVarying;
    outputColor = texture(u_imageTex, uv);

    float noiseTime    = getNoisedTime(u_stopAmount, time);
	float sizeOfBlocks = getSizeofBlocks(u_sizeOfKernel * 2.0, noiseTime);
	
	vec2 block = floor(uv * u_resolution / vec2( sizeOfBlocks, sizeOfBlocks));
	vec2 uv_noise = block / vec2(256.0,256.0);
	
	
	float timeLocal = n2rand(vec2(uv_noise.y,noiseTime),noiseTime);
	
	uv_noise += floor(vec2(timeLocal, timeLocal) * vec2(1234.0, 3543.0))/ vec2(256,256);
	
	float block_thresh = pow(fract(timeLocal * 1236.0453), 2.0) * 0.10;
	float line_thresh = pow(fract(timeLocal * 2236.0453), 3.0) * 0.5;

	
    if(bool(u_effects & DISPLACE       )) outputColor.rgb = getGlitchDisplace    (uv, uv_noise,                  block_thresh   ,line_thresh, mix(2.4, 0.2, u_globalStrength));
	if(bool(u_effects & DISCOLOR_LINE  )) outputColor.rgb = getGlitchDiscolorLine(uv, uv_noise, noiseTime      , outputColor.rgb,line_thresh, mix(3.0, 1.4, u_globalStrength));
    if(bool(u_effects & RGB_LINES      )) outputColor.rgb = getGlitchRGBLines    (uv, uv_noise, outputColor.rgb, block_thresh   ,line_thresh, mix(3.0, 0.8, u_globalStrength));
    if(bool(u_effects & RGB_LINES_VERT )) outputColor.rgb = getGlitchRGBLinesVert(uv, uv_noise, outputColor.rgb, block_thresh   ,line_thresh, mix(3.0, 1.2, u_globalStrength));
    if(bool(u_effects & LUMA           )) outputColor.rgb = getGlitchLuma        (    uv_noise, outputColor.rgb, block_thresh,                mix(3.0, 0.3, u_globalStrength));
		
	if (u_greyScale == 1.0) outputColor.rgb = vec3(outputColor.r,outputColor.r,outputColor.r); 

}
	