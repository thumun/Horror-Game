// Bryn Mawr College, alinen, 2020
//

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"
#include "plymesh.h"

#include "fmod/fmod_errors.h"
#include <cstdlib>
#include <iostream>
#include "fmod/fmod.hpp"

using namespace std;
using namespace glm;
using namespace agl;

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void ERRCHECK(FMOD_RESULT result) {
  if (result != FMOD_OK)
  {
    printf("FMOD error! (%d) %s\n", 
       result, FMOD_ErrorString(result));
    exit(-1);
  }
}

  void setup() {
    setWindowSize(1000, 1000);

    meshData.insert({"victorianscene", PLYMesh("../models/victorianscene.ply")});
    meshData.insert({"table", PLYMesh("../models/table.ply")});
    meshData.insert({"chandelier", PLYMesh("../models/chandelier.ply")});
    meshData.insert({"doorroom", PLYMesh("../models/doorroom.ply")});
    meshData.insert({"wall", PLYMesh("../models/wall.ply")});
    meshData.insert({"viola", PLYMesh("../models/viola.ply")});
    meshData.insert({"fireplacewall", PLYMesh("../models/fireplacewall.ply")});
    meshData.insert({"fireplace", PLYMesh("../models/fireplace.ply")});
    meshData.insert({"couch", PLYMesh("../models/couch.ply")});
    meshData.insert({"books1", PLYMesh("../models/books1.ply")});
    meshData.insert({"books2", PLYMesh("../models/books2.ply")});
    meshData.insert({"books3", PLYMesh("../models/books3.ply")});

    meshData.insert({"monster", PLYMesh("../models/monster.ply")});

    string fileName = "../models/victorianscene.ply";
    meshes.push_back(fileName);

    renderer.loadShader("phong-vertex",
      "../shaders/phong-vertex.vs",
      "../shaders/phong-vertex.fs");

    renderer.loadShader("bumpmap", 
      "../shaders/bumpmap.vs",
      "../shaders/bumpmap.fs");

    renderer.loadShader("spotbump", 
      "../shaders/spotbump.vs",
      "../shaders/spotbump.fs");

    renderer.loadShader("shadertoy",
        "../shaders/shadertoy.vs",
        "../shaders/shadertoy.fs");

    renderer.loadTexture("victorianscene", "../textures/victorianscene.png", 0);
    renderer.loadTexture("table", "../textures/table.png", 1);
    renderer.loadTexture("chandelier", "../textures/chandelier.png", 2);
    renderer.loadTexture("monster", "../textures/monster.jpg", 3);
    renderer.loadTexture("doorroom", "../textures/doorroom.png", 4);
    renderer.loadTexture("wall", "../textures/woodwall.png", 5);
    renderer.loadTexture("fireplace", "../textures/fireplace.png", 6);
    renderer.loadTexture("chair", "../textures/chair.png", 7);
    renderer.loadTexture("books1", "../textures/books1.png", 8);
    renderer.loadTexture("books2", "../textures/books2.png", 9);
    renderer.loadTexture("fireplacewall", "../textures/fireplacewall.png", 10);
    renderer.loadTexture("viola", "../textures/viola.png", 11);

    renderer.loadTexture("victorianscene-normal", "../normaltextures/victorianroom.png", 12);
    renderer.loadTexture("doorroom-normal", "../normaltextures/doorroom.png", 13);
    renderer.loadTexture("wall-normal", "../normaltextures/wall.png", 14);
    renderer.loadTexture("books1-normal", "../normaltextures/books1.png", 15);
    renderer.loadTexture("books2-normal", "../normaltextures/books2.png", 16);
    renderer.loadTexture("chair-normal", "../normaltextures/chair.png", 17);
    renderer.loadTexture("chandelier-normal", "../normaltextures/chandelier.png", 18);
    renderer.loadTexture("fireplace-normal", "../normaltextures/fireplace.png", 19);
    renderer.loadTexture("fireplacewall-normal", "../normaltextures/fireplacewall.png", 20);
    renderer.loadTexture("table-normal", "../normaltextures/table.png", 21);
    renderer.loadTexture("viola-normal", "../normaltextures/viola.png", 22);
    renderer.loadTexture("monster-normal", "../normaltextures/monster.jpg", 23);

    renderer.loadTexture("noise", "../textures/noisetexture.jpeg", 24);
    renderer.loadTexture("gameover", "../textures/gameover.png", 25);
    renderer.loadTexture("gameover-normal", "../normaltextures/gameover.png", 25);

    meshIndx = 0; 
    shaderIndx = 0;
  
    result = FMOD::System_Create(&system);		
    ERRCHECK(result);

    result = system->init(100, FMOD_INIT_NORMAL, 0);	
    ERRCHECK(result);

    // Initialize background music
    result = system->createStream(
      "../sounds/itsinthefog.wav", 
      FMOD_DEFAULT, 0, &music);
    ERRCHECK(result);

    result = system->createStream(
      "../sounds/mixkit-terror-transition-2484.wav", 
      FMOD_DEFAULT, 0, &meow);
	  ERRCHECK(result);

    result = system->createStream(
      "../sounds/beat-n-bass-128-bpm.wav", 
      FMOD_DEFAULT, 0, &monster);
	  ERRCHECK(result);

    result = music->setMode(FMOD_LOOP_NORMAL);
    ERRCHECK(result);

    result = system->playSound(music, 0, false, &backgroundChannel);
    ERRCHECK(result);

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

      if (abs(dx) > abs(dy)){
            
          azimuth += xoffset; 
            
         } else if (abs(dy) > abs(dx)) { 
            
          elevation += yoffset; 
            
         }

         // clamp: 
         if (elevation > 89.0f){
            elevation = 89.0f;

         } else if (elevation < -89.0f){
            elevation = -89.0f;
         }


      vec3 camPos;
      camPos.x = cos(radians(azimuth))*cos(radians(elevation));
      camPos.y = sin(radians(elevation));
      camPos.z = sin(radians(azimuth))*cos(radians(elevation));

      cameraFront = glm::normalize(camPos);
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

  // when using WASD keys, can control the movement of "person"
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
    else if (key == GLFW_KEY_UP){
      monsterMov += stepSize;
      cout << monsterMov << endl;
    }
    else if (key == GLFW_KEY_DOWN){
      monsterMov -= stepSize;
      cout << monsterMov << endl;
    }
  }

  void draw() {

    if (endTime > 0 && elapsedTime() > (endTime+2.0f)){
      endscreen = true; 
      if (monsterTime <= 0){
        monsterTime = elapsedTime();
      }
    }

    if (elapsedTime() >= 5.0f && !gameover) {
      gameover = true; 
    }

    if (monsterTime > 0 && elapsedTime() > (monsterTime+5.0f)) {
      death = true; 
    }

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 1.0f, 75.0f);

    vec3 view = vec3(cameraFront.x, 0, cameraFront.z);

    if (wkey){
      cameraPos += stepSize * view;
    } else if (akey){
      cameraPos -= glm::normalize(glm::cross(view, cameraUp)) * stepSize;
    } else if (skey){
      cameraPos -= stepSize * view;
    } else if (dkey){
      cameraPos += glm::normalize(glm::cross(view, cameraUp)) * stepSize;
    }

    renderer.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    if (!endscreen){

      renderer.beginShader("spotbump");

      if(gameover){

        result = system->playSound(meow, 0, false, 0);
			  ERRCHECK(result);	
        system->update();
      
        if(int(elapsedTime()*10) % 2 == 0){
          renderer.setUniform("noLight", true);
        } else {
          renderer.setUniform("noLight", false);
        }

        if (endTime <= 0.1){
          endTime = elapsedTime();
        }
      
      } else {
        renderer.setUniform("noLight", false);
      }

      renderer.setUniform("Material.specular", 1.0f, 1.0f, 1.0f);
      renderer.setUniform("Material.diffuse", vec3(0.6f, 0.8f, 1.0f));
      renderer.setUniform("Material.ambient", 0.1f, 0.1f, 0.1f);
      renderer.setUniform("Material.shininess", 80.0f);
      renderer.setUniform("Light.position", vec4(cameraPos, 1));
      renderer.setUniform("Light.color", 1.0f, 1.0f, 1.0f);
      renderer.setUniform("useNormalMap", useNormalMap);

      renderer.setUniform("Spot.position",vec4(cameraPos, 1));
      renderer.setUniform("Spot.intensity", 0.8f, 0.8f, 0.5f);
      renderer.setUniform("Spot.direction", cameraFront);
      renderer.setUniform("Spot.exponent", 20.0f);
      renderer.setUniform("Spot.cutoff", 60.0f);


      renderer.push();
      renderer.texture("diffuseTexture", "victorianscene");
      renderer.texture("normalmap", "victorianscene-normal");
      renderer.rotate(vec3(-M_PI/2,0,0));
      renderer.scale(vec3(4.0f));
      renderer.translate(meshData["victorianscene"].getTranslateVal());
      renderer.translate(vec3(0, 0, 1.0f));
      renderer.mesh(meshData["victorianscene"]);
      renderer.pop();

      renderer.push();
      renderer.texture("diffuseTexture", "doorroom");
      renderer.texture("normalmap", "doorroom-normal");
      renderer.rotate(vec3(-M_PI/2,0,0));
      renderer.scale(vec3(4.0f));
      renderer.translate(meshData["doorroom"].getTranslateVal());
      renderer.translate(vec3(2.7f, 0, 0.2f));
      renderer.mesh(meshData["doorroom"]);
      renderer.pop();

      renderer.push();
      renderer.texture("diffuseTexture", "wall");
      renderer.texture("normalmap", "wall-normal");
      renderer.rotate(vec3(-M_PI/2,0,0));
      renderer.scale(vec3(6.0f));
      renderer.translate(meshData["wall"].getTranslateVal());
      renderer.translate(vec3(0, 2.0f, 0));
      renderer.mesh(meshData["wall"]);
      renderer.pop();

      renderer.push();
      renderer.texture("diffuseTexture", "table");
      renderer.texture("normalmap", "table-normal");
      renderer.rotate(vec3(-M_PI/2,0,0));
      renderer.scale(vec3(5.0f));
      renderer.translate(meshData["table"].getTranslateVal());
      renderer.translate(vec3(5.5f, 0, -1.5f));
      renderer.mesh(meshData["table"]);
      renderer.pop();

      renderer.push();
      renderer.texture("diffuseTexture", "chandelier");
      renderer.texture("normalmap", "chandelier-normal");
      renderer.rotate(vec3(-M_PI/2,0,0));
      renderer.scale(vec3(3.0f));
      renderer.translate(meshData["chandelier"].getTranslateVal());
      renderer.translate(vec3(0, 0, 3.8f));
      renderer.mesh(meshData["chandelier"]);
      renderer.pop();

      renderer.push();
      renderer.texture("diffuseTexture", "fireplace");
      renderer.texture("normalmap", "fireplace-normal");
      renderer.rotate(vec3(-M_PI/2,0,0));
      renderer.scale(vec3(5.0f));
      renderer.translate(meshData["fireplace"].getTranslateVal());
      renderer.translate(vec3(0.3f, 3.2f, -0.8f));
      renderer.mesh(meshData["fireplace"]);
      renderer.pop();

      renderer.push();
      renderer.texture("diffuseTexture", "fireplacewall");
      renderer.texture("normalmap", "fireplacewall-normal");
      renderer.rotate(vec3(-M_PI/2,0,0));
      renderer.scale(vec3(5.0f));
      renderer.translate(meshData["fireplacewall"].getTranslateVal());
      renderer.translate(vec3(0.3f, 3.2f, -0.8f));
      renderer.mesh(meshData["fireplacewall"]);
      renderer.pop();

      renderer.push();
      renderer.texture("diffuseTexture", "chair");
      renderer.texture("normalmap", "chair-normal");
      renderer.rotate(vec3(-M_PI/2,sqrt(3)/2,0));
      renderer.scale(vec3(5.0f));
      renderer.translate(meshData["couch"].getTranslateVal());
      renderer.translate(vec3(4.2f, -2.5f, -1.2f));
      renderer.mesh(meshData["couch"]);
      renderer.pop();

      renderer.push();
      renderer.texture("diffuseTexture", "chair");
      renderer.texture("normalmap", "chair-normal");
      renderer.rotate(vec3(-M_PI/2,-sqrt(3)/2,0));
      renderer.scale(vec3(5.0f));
      renderer.translate(meshData["couch"].getTranslateVal());
      renderer.translate(vec3(4.2f, 6.1f, -1.2f));
      renderer.mesh(meshData["couch"]);
      renderer.pop();

      renderer.push();
      renderer.texture("diffuseTexture", "books1");
      renderer.texture("normalmap", "books1-normal");
      renderer.rotate(vec3(-M_PI/2,0,0));
      renderer.scale(vec3(5.0f));
      renderer.translate(meshData["books1"].getTranslateVal());
      renderer.translate(vec3(-3.0f, 0.3f, 0.2f));
      renderer.mesh(meshData["books1"]);
      renderer.pop();

      renderer.push();
      renderer.texture("diffuseTexture", "books2");
      renderer.texture("normalmap", "books2-normal");
      renderer.rotate(vec3(-M_PI/2,0,0));
      renderer.scale(vec3(5.0f));
      renderer.translate(meshData["books2"].getTranslateVal());
      renderer.translate(vec3(-3.0f, 0.9f, 0.16f));
      renderer.mesh(meshData["books2"]);
      renderer.pop();

      renderer.push();
      renderer.texture("diffuseTexture", "books2");
      renderer.texture("normalmap", "books2-normal");
      renderer.rotate(vec3(-M_PI/2,0,0));
      renderer.scale(vec3(5.0f));
      renderer.translate(meshData["books1"].getTranslateVal());
      renderer.translate(vec3(-3.3f, 0.9f, 0.90f));
      renderer.mesh(meshData["books1"]);
      renderer.pop();

      renderer.push();
      renderer.texture("diffuseTexture", "books2");
      renderer.texture("normalmap", "books2-normal");
      renderer.rotate(vec3(-M_PI/2,0,0));
      renderer.scale(vec3(5.0f));
      renderer.translate(meshData["books1"].getTranslateVal());
      renderer.translate(vec3(-3.3f, 3.0f, 0.90f));
      renderer.mesh(meshData["books1"]);
      renderer.pop();

      renderer.push();
      renderer.texture("diffuseTexture", "books1");
      renderer.texture("normalmap", "books1-normal");
      renderer.rotate(vec3(-M_PI/2,0,0));
      renderer.scale(vec3(5.0f));
      renderer.translate(meshData["books1"].getTranslateVal());
      renderer.translate(vec3(-3.0f, -0.9f, 0.16f));
      renderer.mesh(meshData["books1"]);
      renderer.pop();

      renderer.push();
      renderer.texture("diffuseTexture", "viola");
      renderer.texture("normalmap", "viola-normal");
      renderer.rotate(vec3(-M_PI/2,0,0));
      renderer.scale(vec3(5.0f));
      renderer.translate(meshData["viola"].getTranslateVal());
      renderer.translate(vec3(-2.8f, 0, -1.5f));
      renderer.rotate(vec3(-M_PI/2,sqrt(3)/2,0));
      renderer.mesh(meshData["viola"]);
      renderer.pop();

      renderer.endShader();
    
    } else {
      result = system->playSound(music, 0, true, &backgroundChannel);
      ERRCHECK(result);
      result = system->playSound(monster, 0, true, &backgroundChannel);
      ERRCHECK(result);

      renderer.lookAt(vec3(0, 4.0f, 4.0f), vec3(0, 2.0f, 0), vec3(0, 1, 0));
      renderer.ortho(-10, 10, -10, 10, -10, 10);

      if (!death){
        renderer.beginShader("bumpmap");

        renderer.setUniform("Material.specular", 1.0f, 1.0f, 1.0f);
        renderer.setUniform("Material.diffuse", vec3(0.6f, 0.8f, 1.0f));
        renderer.setUniform("Material.ambient", 0.1f, 0.1f, 0.1f);
        renderer.setUniform("Material.shininess", 80.0f);
        renderer.setUniform("Light.position", vec4(0, 0, 3.0f, 1));
        renderer.setUniform("Light.color", 1.0f, 1.0f, 1.0f);
        renderer.setUniform("useNormalMap", useNormalMap);
        renderer.push();
        renderer.texture("diffuseTexture", "monster");
        renderer.texture("normalmap", "monster");
        renderer.translate(vec3(meshData["monster"].getTranslateVal()));
        renderer.translate(vec3(1.0f, 0, 10.0f));
        renderer.scale(vec3(meshData["monster"].getScaleRatio()));
        renderer.scale(vec3(5.0f));
        renderer.mesh(meshData["monster"]);
        renderer.pop();

        renderer.endShader();

      } else { 
        renderer.beginShader("bumpmap");

        renderer.setUniform("Material.specular", 1.0f, 1.0f, 1.0f);
        renderer.setUniform("Material.diffuse", vec3(0.6f, 0.8f, 1.0f));
        renderer.setUniform("Material.ambient", 0.1f, 0.1f, 0.1f);
        renderer.setUniform("Material.shininess", 80.0f);
        renderer.setUniform("Light.position", vec4(0, 0, 3.0f, 1));
        renderer.setUniform("Light.color", 1.0f, 1.0f, 1.0f);
        renderer.push();
        renderer.texture("diffuseTexture", "gameover");
        renderer.translate(vec3(0, 7.0f, 8.0f));
        renderer.scale(vec3(10.0f));
        renderer.plane();
        renderer.pop();

        renderer.endShader();
      }
    }

  }

protected:
  map<string, PLYMesh> meshData; 
  std::vector<PLYMesh> meshes; 
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

  // vec3 monsterMov;
  float monsterMov = 10.0f; 
  // vec3 currentPos;

  FMOD_RESULT result;

  FMOD::System *system = NULL;
  FMOD::Channel *backgroundChannel = NULL;
  FMOD::Sound *music;
  FMOD::Sound *meow;
  FMOD::Sound *monster;

  bool firstMouse = true; 
  float lastX = 500;
  float lastY = 500;
  float yaw = -90.0f;
  float pitch = 0;

  float azimuth = -90.0f; 
  float elevation = 0; 

  bool mousePressed = false; 

  bool useNormalMap = true;
  bool animateLight = false;

  bool gameover = false;
  bool endscreen = false;

  float endTime = 0.0f; 
  bool death = false; 
  float monsterTime = 0.0f; 


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
