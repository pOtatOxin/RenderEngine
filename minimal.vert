#version 120
// Input vertex data, different for all executions of this shader.
attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 texture;
uniform mat4 model, world, view, projection;
uniform vec3 light, camera;
varying vec2 uv;
varying vec3 lightDir;
varying vec3 cameraDir;
varying vec3 frag_normal;

void main(){
	
	gl_Position = projection * view * world * model * vec4(vertex,1.0);
	vec4 frag_pos = view * world * model * vec4(vertex,1.0);
	lightDir = (view * vec4(light,1) - frag_pos).xyz;
	cameraDir = (view * vec4(camera,1) - frag_pos).xyz;
	frag_normal = (view * world * model * vec4(normal,0)).xyz;
	uv = texture;
}

