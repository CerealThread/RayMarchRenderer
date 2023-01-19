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


float GetDist(vec3 p) {
    vec4 s = vec4(0.5, 0.5, 0.5, 0.20);

    float sphereDist = length(mod(p, 1.0)-s.xyz)-s.w;

    return sphereDist;
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
    lightPos.xy += vec2(sin(iTime), cos(iTime)) * 2.0;

    vec3 l = normalize(lightPos-p);
    vec3 n = GetNormal(p);

    float diffuse = clamp(dot(n, l), 0.0, 1.0);

    //float d = RayMarch(p+n*SURF_DIST*2.0, l);
    //if(d<length(lightPos-p)) diffuse *= 0.1;

    return diffuse;
}


void main(void)
{

    vec2 uv = (gl_FragCoord.xy-.5*iResolution.xy)/iResolution.y;

    vec3 ro = vec3(0, 10, pow(uTime/800, 1.3));
    vec3 rd = (normalize(vec4(uv.x, uv.y, 1, 1) * viewMatrix)).xyz;   

    float d = RayMarch(ro, rd);

    vec3 p = ro + rd * d;

    float ambient = 0.60;

    float diffuse = GetLight(p) + (-(d/40) + ambient);

    vec3 col = vec3(diffuse);
    fragColor = vec4(col, 1.0);
}
