#version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormals;
layout (location = 2) in vec2 vTextureCoords;

uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 MVP;
uniform bool HasUV;

struct LightInfo {
 vec4 Position; // Light position in eye coords.
 vec3 La; // Ambient light intensity
 vec3 Ld; // Diffuse light intensity
 vec3 Ls; // Specular light intensity
};

uniform LightInfo Light;

struct MaterialInfo {
 vec3 Ka; // Ambient reflectivity
 vec3 Kd; // Diffuse reflectivity
 vec3 Ks; // Specular reflectivity
 float Shininess; // Specular shininess factor
};

uniform MaterialInfo Material;

out vec3 LightIntensity;
out vec2 uv; 

void main()
{
   uv = vPos.xy;//vTextureCoords;

   vec3 tnorm = normalize( NormalMatrix * vNormals);
   vec4 eyeCoords = ModelViewMatrix * vec4(vPos,1.0);

   vec3 s = normalize(vec3(Light.Position - eyeCoords));
   vec3 v = normalize(-eyeCoords.xyz);
   //vec3 r = reflect( -s, tnorm );
   vec3 r = 2*dot(s, tnorm)*tnorm - s;

   vec3 ambient = Light.La * Material.Ka;

   float sDotN = max( dot(s,tnorm), 0.0 );

   vec3 diffuse = Light.Ld * Material.Kd * sDotN;
   
   vec3 spec = vec3(0.0);
   if( sDotN > 0.0 )
      spec = Light.Ls * Material.Ks *
            pow( max( dot(r,v), 0.0 ), 80);//Material.Shininess);
   
   LightIntensity = ambient + diffuse + spec;
   gl_Position = MVP * vec4(vPos, 1.0);

}
