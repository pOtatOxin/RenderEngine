#version 120

uniform mat4 model, world, view, projection;
uniform vec3 light, camera;
varying vec2 uv;
varying vec3 lightDir;
varying vec3 cameraDir;
varying vec3 frag_normal;
uniform sampler2D myTextureSampler;

const vec3 pointLightColor = vec3(0.8, 0.8, 1);
const vec3 ambientLightColor = vec3(0.3, 0.3, 0.3);
const float diffusePower = 50.0f;
const float specularPower = 100.0f;

void main()
{
	vec3 materialColor = (texture2D(myTextureSampler,uv).xyz);
	float distance = length(lightDir);
	vec3 l = normalize(lightDir);
	vec3 n = normalize(frag_normal);
	vec3 e = normalize(cameraDir);
	vec3 r =reflect(-l, n);
	float cosAlpha = clamp( dot( e,r ), 0,1 );
	float cosTheta = clamp( dot( n,l ), 0,1 );
	gl_FragColor = vec4(ambientLightColor * materialColor + 
				   materialColor * (cosTheta * diffusePower + pow(cosAlpha,5) * specularPower) 
				   * pointLightColor / (distance * distance),1);
}