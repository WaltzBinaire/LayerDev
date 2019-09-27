#version 410
#define TINY 0.000001

uniform sampler2D u_imageTex;
uniform vec2  u_srtPos;
uniform vec2  u_resolution;
uniform vec2  u_pos;
uniform float u_blur;
uniform float u_size;
uniform int   u_shape;
uniform int   u_drawing;

in vec2 texCoordVarying;

out vec4 outputColor;

#define CIRCLE    0
#define LINE      1
#define RECTANGLE 2

float sdBox( in vec2 p, in vec2 b )
{
    vec2 d = abs(p)-b;
    return length(max(d,vec2(0))) + min(max(d.x,d.y),0.0);
}

float sdEllipse( in vec2 p, in vec2 ab )
{
    p = abs(p); if( p.x > p.y ) {p=p.yx;ab=ab.yx;}
    float l = ab.y*ab.y - ab.x*ab.x;
    float m = ab.x*p.x/l;      float m2 = m*m; 
    float n = ab.y*p.y/l;      float n2 = n*n; 
    float c = (m2+n2-1.0)/3.0; float c3 = c*c*c;
    float q = c3 + m2*n2*2.0;
    float d = c3 + m2*n2;
    float g = m + m*n2;
    float co;
    if( d<0.0 )
    {
        float h = acos(q/c3)/3.0;
        float s = cos(h);
        float t = sin(h)*sqrt(3.0);
        float rx = sqrt( -c*(s + t + 2.0) + m2 );
        float ry = sqrt( -c*(s - t + 2.0) + m2 );
        co = (ry+sign(l)*rx+abs(g)/(rx*ry)- m)/2.0;
    }
    else
    {
        float h = 2.0*m*n*sqrt( d );
        float s = sign(q+h)*pow(abs(q+h), 1.0/3.0);
        float u = sign(q-h)*pow(abs(q-h), 1.0/3.0);
        float rx = -s - u - c*4.0 + 2.0*m2;
        float ry = (s - u)*sqrt(3.0);
        float rm = sqrt( rx*rx + ry*ry );
        co = (ry/sqrt(rm-rx)+2.0*g/rm-m)/2.0;
    }
    vec2 r = ab * vec2(co, sqrt(1.0-co*co));
    return length(r-p) * sign(p.y-r.y);
}

float sdStar(in vec2 p, in float r, in int n, in float m)
{
    // next 4 lines can be precomputed for a given shape
    float an = 3.141593/float(n);
    float en = 6.283185/m;
    vec2  acs = vec2(cos(an),sin(an));
    vec2  ecs = vec2(cos(en),sin(en)); // ecs=vec2(0,1) for regular polygon,

    float bn = mod(atan(p.x,p.y),2.0*an) - an;
    p = length(p)*vec2(cos(bn),abs(sin(bn)));
    p -= r*acs;
    p += ecs*clamp( -dot(p,ecs), 0.0, r*acs.y/ecs.y);
    return length(p)*sign(p.x);
}

void main()
{
    vec2 _uv     = vec2( texCoordVarying.x, texCoordVarying.y);
    vec2 pos     = _uv - vec2(u_pos.x, u_pos.y);
            
    vec2  base_uv = texture( u_imageTex, _uv).rg;
    float base_a  = texture( u_imageTex, _uv).a;

    vec2 srt_uv = vec2(u_srtPos.x, u_srtPos.y) - vec2(u_pos.x, u_pos.y) + _uv;
    
    
    vec2 a_ratio = vec2( 1.0, u_resolution.x/u_resolution.y);
    
    float sdf = 1.0;
    if( u_shape == CIRCLE){
        sdf = sdEllipse( pos, u_size * 0.1 * a_ratio);
    } else if (u_shape == LINE){
        sdf = sdBox( pos, vec2(1.0, u_size * 0.1 * a_ratio));     
    } else if (u_shape == RECTANGLE ){
        sdf = sdBox( pos, u_size * 0.1 * a_ratio);    
    }
        
    
    float d_pos = smoothstep(u_blur * 0.1, 0.0, sdf);

    vec2 uv = mix( base_uv, srt_uv, d_pos);
    
    if(u_drawing == 1){    
        outputColor = vec4(uv.x, uv.y, 0.0, 1.0);
        //outputColor = vec4(srt_uv.x, srt_uv.y, 0.0, 1.0);
        //outputColor = vec4(base_uv.x, base_uv.y, 0.0, 1.0);
        //outputColor = vec4(d_pos, d_pos, d_pos, 1.0);
    } else{
        outputColor = vec4(_uv.x, _uv.y, 0.0, 1.0);
    }
    
}  