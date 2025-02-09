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

uniform LightInfo Light[1];


struct SpotLightInfo{
   vec4 position; 
   vec3 intensity;
   vec3 direction;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float exponent;
   float cutoff; 

   float constant; 
   float linear; 
   float quadratic;
}; 

uniform SpotLightInfo Spot; 

struct MaterialInfo {
 vec3 Ka; // Ambient reflectivity
 vec3 Kd; // Diffuse reflectivity
 vec3 Ks; // Specular reflectivity
 float Shininess; // Specular shininess factor
};

uniform MaterialInfo Material;
uniform sampler2D diffuseTexture;

out vec3 LightIntensity;
out vec2 uv; 
out vec4 Fcolor;

// https://math.hws.edu/graphicsbook/c7/s2.html 


vec3 adsWithSpotlight(){

   vec3 tnorm = normalize( NormalMatrix * vNormals);
   vec4 eyeCoords = ModelViewMatrix * vec4(vPos,1.0);

   vec3 s = normalize(vec3(Spot.position) - vec3(eyeCoords));
   float angle = acos(dot(-s, Spot.direction));
   float cutoff = radians( clamp(Spot.cutoff, 0.0, 90.0 ) );
   vec3 ambient = Spot.intensity * Material.Ka;
   if(angle < cutoff) {
      float spotFactor = pow( dot(-s, Spot.direction), Spot.exponent);
      vec3 v = normalize(vec3(-eyeCoords));
      vec3 h = normalize(v + s);

      return ambient + spotFactor * Spot.intensity * 
            (Material.Kd * max(dot(s, tnorm), 0.0 ) +
            Material.Ks * pow(max(dot(h,tnorm), 0.0),Material.Shininess));

   } else {
      return ambient; 
   }

}

// https://learnopengl.com/Lighting/Light-casters
void learnSpotlight(){
   vec4 eyeCoords = ModelViewMatrix * vec4(vPos,1.0);

   vec3 lightDir = normalize(vec3(Spot.position - eyeCoords));
   float theta = dot(lightDir, normalize(-Spot.direction)); 
    
    if(theta > Spot.cutoff) // remember that we're working with angles as cosines instead of degrees so a '>' is used.
    {    
        // ambient
        vec3 ambient = Spot.ambient * texture(diffuseTexture, vTextureCoords).rgb;
        
        // diffuse 
        vec3 norm = normalize(vNormals);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = Spot.diffuse * diff * texture(diffuseTexture, vTextureCoords).rgb;  
        
        // specular
        vec3 viewDir = normalize(vec3(Spot.position - eyeCoords));
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), Material.Shininess);
        vec3 specular = Spot.specular * spec * texture(diffuseTexture, vTextureCoords).rgb;  
        
        // attenuation
        float distance = length(Spot.position - eyeCoords);
        float attenuation = 1.0 / (Spot.constant + Spot.linear * distance + Spot.quadratic * (distance * distance));    

        // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branch
        diffuse   *= attenuation;
        specular *= attenuation;   
            
        vec3 result = ambient + diffuse + specular;
        Fcolor = vec4(result, 1.0);
    }
    else 
    {
        // else, use ambient light so scene isn't completely dark outside the spotlight.
        Fcolor = vec4(Spot.ambient * texture(diffuseTexture, vTextureCoords).rgb, 1.0);
    }
}

void main()
{
   uv = vTextureCoords;

   vec3 tnorm = normalize( NormalMatrix * vNormals);
   vec4 eyeCoords = ModelViewMatrix * vec4(vPos,1.0);

   vec3 s = normalize(vec3(Light[0].Position - eyeCoords));
   vec3 v = normalize(-eyeCoords.xyz);
   //vec3 r = reflect( -s, tnorm );
   vec3 r = 2*dot(s, tnorm)*tnorm - s;

   vec3 ambient = Light[0].La * Material.Ka;

   float sDotN = max( dot(s,tnorm), 0.0 );

   vec3 diffuse = Light[0].Ld * Material.Kd * sDotN;
   
   vec3 spec = vec3(0.0);
   if( sDotN > 0.0 )
      spec = Light[0].Ls * Material.Ks *
            pow( max( dot(r,v), 0.0 ), 80); //Material.Shininess

   vec3 spotlightInfo = adsWithSpotlight();
   
   //LightIntensity = + ambient + diffuse + spec;
   LightIntensity = spotlightInfo;
   //LightIntensity = + ambient + diffuse + spec + spotlightInfo;
   //learnSpotlight();
   gl_Position = MVP * vec4(vPos, 1.0);

}
