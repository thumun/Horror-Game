//--------------------------------------------------
// Author: Neha Thumu
// Date: 3/23/2023
// Description: Loads PLY files in ASCII format
// Notable funcs: 
// Able to get the max and min bounds of meshes 
// can get the positions and normals as vectors 
// getScaleRatio: scales a model to fit in 10x10x10 window
// getTranslateVal: centers a model at origin 
//--------------------------------------------------

#ifndef plymeshmodel_H_
#define plymeshmodel_H_

#include "agl/aglm.h"
#include "agl/mesh/triangle_mesh.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace agl {

   struct VertexInfo
   {
      int x;
      int y;
      int z;

      VertexInfo(int _x, int _y, int _z)
      {
         x = _x;
         y = _y;
         z = _z;
      }
   };

   class PLYMesh : public TriangleMesh
   {
   public:

      PLYMesh(const std::string& filename);
      PLYMesh();

      virtual ~PLYMesh();

      // Initialize this object with the given file
      // Returns true if successfull. false otherwise.
      bool load(const std::string& filename);

      // Return the minimum point of the axis-aligned bounding box
      glm::vec3 minBounds() const;

      // Return the maximum point of the axis-aligned bounding box
      glm::vec3 maxBounds() const;

      // Return number of vertices in this model
      int numVertices() const;

      // Positions in this model
      const std::vector<GLfloat>& positions() const;

      // Positions in this model
      const std::vector<GLfloat>& normals() const;

      // Return number of faces in this model
      int numTriangles() const;

      // face indices in this model
      const std::vector<GLuint>& indices() const;

      // getting the amount to scale a model by to fit in 10x10x10 window
      float getScaleRatio(); 

      // getting the vector for centering model at 0,0 
      glm::vec3 getTranslateVal(); 

      std::string _filename; 

   protected:
      void init();

   protected:

      std::vector<GLfloat> _positions;
      std::vector<GLfloat> _normals;
      std::vector<GLuint> _faces;

   private:
      
      // splits string based on delimeter 
      // src:https://www.geeksforgeeks.org/cpp-string-to-vector-using-delimiter/
      std::vector<std::string> split(std::string str, char delimiter);

   };
}

#endif