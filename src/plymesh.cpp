//--------------------------------------------------
// Author: Neha Thumu
// Date: 3/23/2023
// Description: Loads PLY files in ASCII format
// Also has useful extension methods like getting max and min bounds,  
// centering a model, and scaling a model to fit in 10x10x10 window 
//--------------------------------------------------

#include "plymesh.h"

using namespace std;
using namespace glm;

namespace agl {

   PLYMesh::PLYMesh(const std::string& filename) {
      _filename = filename;
      load(filename);
   }

   PLYMesh::PLYMesh() {
   }

   void PLYMesh::init() {
      assert(_positions.size() != 0);
      initBuffers(&_faces, &_positions, &_normals, &_uvs);
   }

   PLYMesh::~PLYMesh() {
      _positions.clear();
      _normals.clear();
      _faces.clear();
   }

   bool PLYMesh::load(const std::string& filename) {
      if (_positions.size() != 0) {
         std::cout 
            << "WARNING: Cannot load different files with the same PLY mesh\n";
         return false;
      }
      // reading file inputted 
      std::ifstream plyFile; 
      std::string line; 
      plyFile.open(filename);

      int lineCount = 0; 
      std::vector<string> lineItems;
      int verticiesNum = 0;
      int polygonNum = 0;

      // cout << "test" << endl;

      if (plyFile.is_open()) {

         getline(plyFile, line);
         if (line.find("ply") == string::npos){
            // cout << "skipped:  " << line << endl;
            return false;
         }

         while(plyFile){
            getline(plyFile, line);
            if (line.find("element vertex") == string::npos){
               // cout << "skipped:  " << line << endl;
               continue;
            } else {
               lineItems = split(line, ' ');
               verticiesNum = stoi(lineItems[lineItems.size() - 1]);
               // cout << line << endl;
               // cout << "vertex: " << verticiesNum << endl;
               break; 
            }
         }

         while(plyFile){
            getline(plyFile, line);
            if (line.find("element face") == string::npos){
               // cout << "skipped:  " << line << endl;
               continue;
            } else {
               lineItems = split(line, ' ');
               polygonNum = stoi(lineItems[lineItems.size() - 1]);
               // cout << line << endl;
               // cout << "polygon: " << polygonNum << endl;
               break;
            }
         }

         while(plyFile){
            getline(plyFile, line);
            if (line.find("end_header") == string::npos){
               // cout << "skipped:  " << line << endl;
               continue;
            } else {
               // cout << "skipped:  " << line << endl;
               break;
            }
         }

         while(plyFile){
            // doing if statement here to differentiate vertex & polygon
            if (verticiesNum > 0){
               getline(plyFile, line);
               lineItems = split(line, ' ');
               _positions.push_back(std::stof(lineItems[0])); // x
               _positions.push_back(std::stof(lineItems[1])); // y
               _positions.push_back(std::stof(lineItems[2])); // z

               _normals.push_back(std::stof(lineItems[3])); // nx
               _normals.push_back(std::stof(lineItems[4])); // ny
               _normals.push_back(std::stof(lineItems[5])); // nz

               _uvs.push_back(std::stof(lineItems[6])); // u
               _uvs.push_back(std::stof(lineItems[7])); // v

               verticiesNum -= 1; 
            } else if (polygonNum > 0){
               getline(plyFile, line);
               lineItems = split(line, ' ');
               _faces.push_back(std::stoi(lineItems[1]));
               _faces.push_back(std::stoi(lineItems[2]));
               _faces.push_back(std::stoi(lineItems[3]));

               polygonNum -= 1;  
            } else {
               break;
            }
         }
      }

      if (plyFile.is_open()){
         plyFile.close();
      }

      return true;
   }

   glm::vec3 PLYMesh::minBounds() const {

      glm::vec3 min = glm::vec3(0); 
      min.x = _positions[0];
      min.y = _positions[1];
      min.z = _positions[2];

      for (int i = 3; i < _positions.size(); i+=3){
         min.x = fminf(min.x, _positions[i]);
         min.y = fminf(min.y, _positions[i+1]);
         min.z = fminf(min.z, _positions[i+2]);
      }

      return min;
   }

   glm::vec3 PLYMesh::maxBounds() const {
      glm::vec3 max = glm::vec3(0); 
      max.x = _positions[0];
      max.y = _positions[1];
      max.z = _positions[2];

      for (int i = 3; i < _positions.size(); i+=3){
         max.x = fmaxf(max.x, _positions[i]);
         max.y = fmaxf(max.y, _positions[i+1]);
         max.z = fmaxf(max.z, _positions[i+2]);
      }

      return max; 
   }

   int PLYMesh::numVertices() const {
      return _positions.size()/3;
   }

   int PLYMesh::numTriangles() const {
      return _faces.size()/3;
   }

   const std::vector<GLfloat>& PLYMesh::positions() const {
      return _positions;
   }

   const std::vector<GLfloat>& PLYMesh::normals() const {
      return _normals;
   }

   const std::vector<GLuint>& PLYMesh::indices() const {
      return _faces;
   }

   const std::vector<GLfloat>& PLYMesh::texCoords() const {
      return _uvs;
   }

   void PLYMesh::getTexCoords(){
      for (GLfloat i: _uvs){
         // cout << i << endl; 
      }
   }
   
   float PLYMesh::getScaleRatio() {
      float scaleRatio; 
      vec3 magnitude; 
      float scale;
      magnitude.x = abs(maxBounds().x - minBounds().x);
      magnitude.y = abs(maxBounds().y - minBounds().y);
      magnitude.z = abs(maxBounds().z - minBounds().z);

      scale = 6.0f/(fmaxf(fmaxf(magnitude.x, magnitude.y), magnitude.z));

      return scale;
   }

   vec3 PLYMesh::getTranslateVal() {
      vec3 mediumPos; 

      float scaleRatio = 1.0f;
      mediumPos.x = -1*scaleRatio*(maxBounds().x+minBounds().x)/2;
      mediumPos.y = -1*scaleRatio*(maxBounds().y+minBounds().y)/2;
      mediumPos.z = -1*scaleRatio*(maxBounds().z+minBounds().z)/2;

      return mediumPos;
   }

   // extension functions

   std::vector<std::string> PLYMesh::split(std::string str, char delimiter){
      std::stringstream ss;
      ss << str;
      vector<string> lineItems;
      string output;
      while (getline(ss, output, delimiter)) {
         lineItems.push_back(output);
      }
      return lineItems;
   }
}