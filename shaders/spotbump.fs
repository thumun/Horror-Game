#version 400

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

struct MaterialInfo {
  vec3 diffuse;
  vec3 ambient;
  vec3 specular;
  float shininess;
};

uniform sampler2D diffuseTexture;
uniform sampler2D normalmap;

uniform LightInfo Light; 
uniform MaterialInfo Material;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;
uniform bool useNormalMap;

uniform bool noLight; 

in vec4 position;
in vec4 lightpos;
in vec3 normal;
in vec2 uv;
in vec4 spotPos; 
in vec3 spotDir; 

out vec4 FragColor;

vec3 phongModel(in vec3 ePos, in vec3 eNormal) {
  vec3 L = normalize(lightpos.xyz - lightpos.w * ePos);
  vec3 v = normalize(-ePos);
  vec3 r = normalize(reflect(v, eNormal));

  vec3 ambient = Light.color * Material.ambient;

  float angle = max( dot(L,eNormal), 0.0 ); 
  vec3 diffuse = angle * Light.color * Material.diffuse;

  vec3 mainColor = texture(diffuseTexture, uv).rgb;
  vec3 color = mainColor*(ambient + diffuse);

  float base = max(dot(r, -v), 0.0);
  vec3 spec = Light.color * Material.specular * pow(base, Material.shininess);

  return color;
}

vec3 adsWithSpotlight(){

   vec3 tnorm = normalize(normal);
   vec4 eyeCoords = position;
   

   vec3 s = normalize(vec3(spotPos) - vec3(eyeCoords));
   float angle = acos(dot(-s, spotDir));
   float cutoff = radians( clamp(Spot.cutoff, 0.0, 90.0 ) );
   vec3 ambient = Spot.intensity * Material.ambient;
   
   if(abs(angle) < cutoff) {
      float spotFactor = pow( dot(-s, spotDir), Spot.exponent);

      vec3 texNormal = normalize(2*(texture(normalmap, uv).xyz-0.5f));
      vec3 color = phongModel(position.xyz, useNormalMap? texNormal : normal);

      return ambient + spotFactor * Spot.intensity * color;

   } else {
      return ambient; 
   }

}

void main() {
  // vec3 texNormal = normalize(2*(texture(normalmap, uv).xyz-0.5f));
  // vec3 color = phongModel(position.xyz, useNormalMap? texNormal : normal);
  vec3 spotColor = adsWithSpotlight();

  // trying flashing light idea 
  // if (noLight){
  //   FragColor = vec4(0, 0, 0, 1.0);
  // } else {
  //   FragColor = vec4(min(color, spotColor), 1.0);
  // }

  FragColor = vec4(spotColor, 1.0);
  
}