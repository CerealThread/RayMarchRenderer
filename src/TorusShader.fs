#version 430 core

#define MAX_STEPS 300
#define MAX_DIST 100.0
#define SURF_DIST 0.01

in vec4 fColor;
uniform float uTime;
uniform vec2 iResolution;
uniform vec2 mPosition;
uniform mat4 viewMatrix;

float mYaw;
float mPitch;

out vec4 fragColor;

float maxcomp(in vec3 p ) { return max(p.x,max(p.y,p.z));}

float sdTorus(vec3 p, vec2 r) {
    float x = length(p.xz)-r.x;
    return length(vec2(x, p.y))-r.y;
}

float sdBox( vec3 p, vec3 b )
{
  vec3  di = abs(p) - b;
  float mc = maxcomp(di);
  return min(mc,length(max(di,0.0)));
}

float opSmoothUnion( float d1, float d2, float k ) {
    float h = clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) - k*h*(1.0-h); 
}

float GetDist(vec3 p) {

    vec3 bp = p-(vec3(0.0, 0.0, 2.0).xyzx * viewMatrix).xyz;

    
    float td = sdTorus(bp, vec2(0.5, 0.2));
    float bd = sdBox(bp, vec3(0.35) * ((sin(uTime/800))/2)+0.4);

    return opSmoothUnion(td, bd, 0.1);
}

float RayMarch(vec3 ro, vec3 rd) {
    float dO=0.0;

    for(int i=0; i<MAX_STEPS;i++) {
        vec3 p = ro + rd*dO;
        float ds = GetDist(p);
        dO += ds;
        if(dO > MAX_DIST || ds<SURF_DIST) break;
    }

    return dO;
}

vec3 GetNormal(vec3 p) {
    float d = GetDist(p);
    vec2 e = vec2(0.01, 0);

    vec3 n = d - vec3(
        GetDist(p-e.xyy),
        GetDist(p-e.yxy),
        GetDist(p-e.yyx)
    );

    return normalize(n);
}

float GetLight(vec3 p) {
    float iTime = uTime * 0.25;

    vec3 lightPos = vec3(0, 5, 6);

    vec3 l = normalize(lightPos-p);
    vec3 n = GetNormal(p);

    float diffuse = clamp(dot(n, l), 0.0, 1.0);

    return diffuse;
}


void main(void)
{

    vec2 uv = (gl_FragCoord.xy-.5*iResolution.xy)/iResolution.y;

    vec3 ro = vec3(0.0, 0.0, 0.0);  
    vec3 rd = (normalize(vec4(uv.x, uv.y, 1, 1) * viewMatrix)).xyz;   

    float d = RayMarch(ro, rd);

    vec3 p = ro + rd * d;

    float ambient = 0.60;

    float diffuse = GetLight(p) + (-(d/40) + ambient);

    vec3 col = vec3(diffuse);
    fragColor = vec4(col, 1.0);
}
