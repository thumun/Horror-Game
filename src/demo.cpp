// Bryn Mawr College, alinen, 2020
//

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"
#include "plymesh.h"

using namespace std;
using namespace glm;
using namespace agl;

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);

    string fileName = "../models/victorianscene.ply";
    meshes.push_back(fileName);

    shaders.push_back("phong-vertex");
    string s = "phong-vertex";

    renderer.loadShader(s, "../shaders/"+s+".vs", "../shaders/"+s+".fs");

    meshIndx = 0; 
    shaderIndx = 0;
  }

  ~Viewer(){
    meshes.clear();
  }

  void mouseMotion(int x, int y, int dx, int dy) {
  }

  void mouseDown(int button, int mods) {
  }

  void mouseUp(int button, int mods) {
  }

  void scroll(float dx, float dy) {
  }

  void keyDown(int key, int mods) {
    if (key == GLFW_KEY_W){
      eyePos = eyePos - stepSize*n;
    } else if (key == GLFW_KEY_A){
      eyePos = eyePos + stepSize*v;

    } else if (key == GLFW_KEY_S){
      eyePos = eyePos + stepSize*n;

    } else if (key == GLFW_KEY_D){
      eyePos = eyePos - stepSize*v;

    }
  }

  void draw() {
    renderer.beginShader(shaders[shaderIndx]);
    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

    n = normalize(eyePos-lookPos);
    v = cross(up, n);
    up = normalize(cross(n, v));

    renderer.lookAt(eyePos, lookPos, up);

    //http://learnwebgl.brown37.net/09_lights/lights_combined.html
    // used above to find numbers for lights 
    renderer.setUniform("Light[0].Position", 20, 20, 20, 1);
    renderer.setUniform("Light[0].La", 0.4, 0.2, 0.2);
    // hoping for a yellow color for light 
    renderer.setUniform("Light[0].Ld", 1.0, 1.0, 0.7);
    renderer.setUniform("Light[0].Ls", 1.0, 1.0, 0.7);

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
    renderer.rotate(vec3(-M_PI/2,0,0));
    renderer.scale(vec3(0.25f));
    // renderer.scale(vec3(meshes[meshIndx].getScaleRatio())); 
    renderer.translate(meshes[meshIndx].getTranslateVal());
    renderer.mesh(meshes[meshIndx]);
    renderer.pop();

    renderer.endShader();

  }

protected:
  std::vector<PLYMesh> meshes; 
  std::vector<string> shaders;

  vec3 eyePos = vec3(0,0,2);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  vec3 n = normalize(eyePos-lookPos);
  vec3 v = cross(up, n);
  float stepSize = 0.1f;

private: 
  int meshIndx;
  int shaderIndx;


};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
