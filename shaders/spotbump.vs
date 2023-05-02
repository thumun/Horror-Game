#version 400

layout (location = 0) in vec3 vPositions;
layout (location = 1) in vec3 vNormals;
layout (location = 2) in vec2 vTextureCoords;

struct LightInfo {
  vec4 position;  // 0 => directional light; 1 => point light
  vec3 color; 
};

struct SpotLightInfo{
   vec4 position; 
   vec3 intensity;
   vec3 direction;

   float exponent;
   float cutoff; 
}; 

uniform SpotLightInfo Spot; 

uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ViewMatrix;
uniform mat4 MVP;
uniform LightInfo Light; 

out vec4 position;
out vec4 lightpos;
out vec3 normal;
out vec2 uv;
out vec4 spotPos; 
out vec3 spotDir; 

void main()
{
  position = ModelViewMatrix * vec4( vPositions, 1.0);
  lightpos = ViewMatrix * Light.position;

  spotPos = ViewMatrix * Spot.position; // might have to change this 
  spotDir = normalize(vec3(ViewMatrix * vec4(Spot.direction, 0))); // might have to change this


  //spotPos = Spot.position;
  //spotDir = Spot.direction;

  normal = NormalMatrix * vNormals;
  uv = vTextureCoords;
  gl_Position = MVP * vec4(vPositions, 1.0);
}

