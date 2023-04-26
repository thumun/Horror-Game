// Bryn Mawr College, alinen, 2020
//

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"
#include "plymesh.h"
// #include "osutils.h"

using namespace std;
using namespace glm;
using namespace agl;


// struct ModelData
// {
//   string name;
//   PLYMesh file;

//   ModelData(string _name)
//   {
//       name = _name;
//       file = _name;
//   }
// };

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);

    meshData.insert({"victorianscene", PLYMesh("../models/victorianscene.ply")});
    meshData.insert({"table", PLYMesh("../models/table.ply")});
    meshData.insert({"chandelier", PLYMesh("../models/chandelier.ply")});
    meshData.insert({"doorroom", PLYMesh("../models/doorroom.ply")});
    meshData.insert({"wall", PLYMesh("../models/wall.ply")});

    meshData.insert({"fireplacewall", PLYMesh("../models/fireplacewall.ply")});
    meshData.insert({"fireplace", PLYMesh("../models/fireplace.ply")});
    meshData.insert({"couch", PLYMesh("../models/couch.ply")});
    meshData.insert({"books1", PLYMesh("../models/books1.ply")});
    meshData.insert({"books2", PLYMesh("../models/books2.ply")});

    meshData.insert({"monster", PLYMesh("../models/monster.ply")});

    // std::vector<std::string> dir = GetFilenamesInDir("../models", "ply"); 
    // for (string file: dir){
    //   meshes.push_back(file);
    // }

    // for (string filename: meshes){
    //   meshes.push_back(filename);
    // }

    string fileName = "../models/victorianscene.ply";
    // meshData.insert({fileName, })
    // meshes.insert({fileName[], fileName});
    meshes.push_back(fileName);

    shaders.push_back("phong-vertex");
    string s = "phong-vertex";

    renderer.loadShader(s, "../shaders/"+s+".vs", "../shaders/"+s+".fs");

    renderer.loadTexture("victorianscene", "../textures/victorianscene.png", 0);
    renderer.loadTexture("table", "../textures/table.png", 1);
    renderer.loadTexture("chandelier", "../textures/chandelier.png", 2);
    renderer.loadTexture("monster", "../textures/monster.jpg", 3);
    renderer.loadTexture("doorroom", "../textures/doorroom.png", 4);
    renderer.loadTexture("wall", "../textures/woodwall.png", 5);

    meshIndx = 0; 
    shaderIndx = 0;

    meshes[meshIndx].getTexCoords();
  }

  ~Viewer(){
    // meshes.clear();
  }

  void mouseMotion(int x, int y, int dx, int dy) {
    if (mousePressed){
      if (firstMouse){
        lastX = x;
        lastY = y;
        firstMouse = false;
      }
        
      float xoffset = x - lastX;
      float yoffset = lastY - y; 
      lastX = x;
      lastY = y;

      float sensitivity = 0.1f;
      xoffset *= sensitivity;
      yoffset *= sensitivity;

      yaw += xoffset;
      pitch += yoffset;

      if(pitch > 89.0f){
        pitch = 89.0f;
      }
          
      if(pitch < -89.0f){
        pitch = -89.0f;
      }
          
      glm::vec3 direction;
      direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
      direction.y = sin(glm::radians(pitch));
      direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
      cameraFront = glm::normalize(direction);
    }
   
  }

  void mouseDown(int button, int mods) {  
    mousePressed = true; 
  }

  void mouseUp(int button, int mods) {
    mousePressed = false; 
  }

  void scroll(float dx, float dy) {
  }

  void keyUp(int key, int mods) {
    if (key == GLFW_KEY_W){
      wkey = false;
    } else if (key == GLFW_KEY_A){
      akey = false; 

    } else if (key == GLFW_KEY_S){
      skey = false; 

    } else if (key == GLFW_KEY_D){
      dkey = false; 

    } 
  }

  void keyDown(int key, int mods) {
    if (key == GLFW_KEY_W){
      wkey = true;
    } else if (key == GLFW_KEY_A){
      akey = true; 
    } else if (key == GLFW_KEY_S){
      skey = true; 
    } else if (key == GLFW_KEY_D){
      dkey = true; 
    }
  }

  void draw() {
    renderer.beginShader(shaders[shaderIndx]);

    renderer.setUniform("isTexture", true);

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

  

    if (wkey){
      //eyePos = eyePos - stepSize*n;
      cameraPos += stepSize * cameraFront;
    } else if (akey){
      //eyePos = eyePos + stepSize*v;
      cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * stepSize;

    } else if (skey){
      //eyePos = eyePos + stepSize*n;
      cameraPos -= stepSize * cameraFront;
    } else if (dkey){
      //eyePos = eyePos - stepSize*v;
      cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * stepSize;

    }

    n = normalize(eyePos-lookPos);
    v = cross(up, n);
    up = normalize(cross(n, v));

    // renderer.lookAt(eyePos, lookPos, up);
    renderer.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    //http://learnwebgl.brown37.net/09_lights/lights_combined.html
    // used above to find numbers for lights 
    renderer.setUniform("Light[0].Position", 20, 20, 20, 1);
    renderer.setUniform("Light[0].La", 0.4, 0.2, 0.2);
    // hoping for a yellow color for light 
    renderer.setUniform("Light[0].Ld", 1.0, 1.0, 0.7);
    renderer.setUniform("Light[0].Ls", 1.0, 1.0, 0.7);

    //http://learnwebgl.brown37.net/09_lights/lights_combined.html
    // used above to find numbers for lights 
    renderer.setUniform("Spot.position",vec4(eyePos, 1));
    renderer.setUniform("Spot.intensity", 0.8f, 0.8f, 0.5f);
    renderer.setUniform("Spot.direction", -n);
    renderer.setUniform("Spot.exponent", 1.0f);
    renderer.setUniform("Spot.cutoff", 8.0f);

    //https://learnopengl.com/Lighting/Materials
    // http://devernay.free.fr/cours/opengl/materials.html
    // above link has table of materials -- used below (pearl)
    renderer.setUniform("Material.Ka", 0.25, 0.20725, 0.20725);
    renderer.setUniform("Material.Kd", 1, 0.829, 0.829);
    renderer.setUniform("Material.Ks", 0.296648, 0.296648, 0.296648);
    renderer.setUniform("Material.Shininess", 0.088f);

    // draw plane
    // renderer.push();
    // renderer.translate(vec3(0.0, -0.5, 0));
    // renderer.scale(vec3(2.0f));
    // renderer.plane();
    // renderer.pop();

    renderer.push();
    renderer.texture("diffuseTexture", "victorianscene");
    renderer.rotate(vec3(-M_PI/2,0,0));
    renderer.scale(vec3(4.0f));
    // renderer.scale(vec3(meshData["victorianscene"].getScaleRatio())); 
    renderer.translate(meshData["victorianscene"].getTranslateVal());
    renderer.translate(vec3(0, 0, 1.0f));
    // renderer.scale(vec3(meshes[meshIndx].getScaleRatio())); 
    // renderer.translate(meshes[meshIndx].getTranslateVal());
    // renderer.mesh(meshData["victorianscene"]);
    renderer.mesh(meshData["victorianscene"]);
    renderer.pop();

    renderer.push();
    renderer.texture("diffuseTexture", "doorroom");
    renderer.rotate(vec3(-M_PI/2,0,0));
    renderer.scale(vec3(4.0f));
    renderer.translate(meshData["doorroom"].getTranslateVal());
    renderer.translate(vec3(8.2f, 0, 0));
    renderer.mesh(meshData["doorroom"]);
    renderer.pop();

    // renderer.push();
    // renderer.texture("diffuseTexture", "wall");
    // renderer.rotate(vec3(-M_PI/2,0,0));
    // renderer.scale(vec3(4.0f));
    // renderer.translate(meshData["wall"].getTranslateVal());
    // renderer.mesh(meshData["wall"]);
    // renderer.pop();

    renderer.push();
    renderer.texture("diffuseTexture", "table");
    renderer.rotate(vec3(-M_PI/2,0,0));
    renderer.scale(vec3(5.0f));
    renderer.translate(meshData["table"].getTranslateVal());
    renderer.translate(vec3(0, 0, -1.5f));
    renderer.mesh(meshData["table"]);
    renderer.pop();

    renderer.push();
    renderer.texture("diffuseTexture", "chandelier");
    renderer.rotate(vec3(-M_PI/2,0,0));
    renderer.scale(vec3(3.0f));
    renderer.translate(meshData["chandelier"].getTranslateVal());
    renderer.translate(vec3(0, 0, 3.8f));
    renderer.mesh(meshData["chandelier"]);
    renderer.pop();

    // make horror follow 
    renderer.push();
    renderer.texture("diffuseTexture", "monster");
    renderer.rotate(vec3(-M_PI/2,0,0));
    renderer.scale(vec3(2.0f));
    renderer.translate(meshData["monster"].getTranslateVal());
    renderer.translate(vec3(3.0f, 0, -3.0f));
    renderer.mesh(meshData["monster"]);
    renderer.pop();

    // renderer.push();
    // renderer.rotate(vec3(-M_PI/2,0,0));
    // renderer.scale(vec3(meshData[""].getScaleRatio())); 
    // renderer.translate(meshData[meshIndx].getTranslateVal());
    // renderer.mesh(meshData[meshIndx]);
    // renderer.pop();

    renderer.endShader();

  }

protected:
  map<string, PLYMesh> meshData; 
  std::vector<PLYMesh> meshes; 
  // std::vector<PLYMesh> meshesPLY; 
  std::vector<string> shaders;

  vec3 eyePos = vec3(0,0,0.5f);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  vec3 n = normalize(eyePos-lookPos);
  vec3 v = cross(up, n);
  float stepSize = 0.1f;

  // the idea from class w/ eyepos was being weird so followed a tutorial 
  // to set up camera/player: https://learnopengl.com/Getting-started/Camera
  vec3 cameraPos = vec3(0.0f, 0.0f,  3.0f);
  vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
  vec3 cameraUp = vec3(0.0f, 1.0f,  0.0f);

  bool firstMouse = true; 
  float lastX = 500;
  float lastY = 300;
  float yaw = -90.0f;
  float pitch = 0;

  bool mousePressed = false; 


private: 
  int meshIndx;
  int shaderIndx;

  bool wkey = false; 
  bool akey = false; 
  bool skey = false; 
  bool dkey = false; 


};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
