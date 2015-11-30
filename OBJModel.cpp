#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <vector>

#include "VECTOR3D.h"
#include "OBJModel.hpp"

using namespace std;

void OBJModel::draw() {
  glBindTexture(GL_TEXTURE_2D, 2007);
  glPushMatrix();
  glTranslatef(2,0,-2);
  glScalef(0.12,0.12,0.12);
  for(unsigned int i=0; i<vertices.size(); i+=3) {
    glBegin(GL_TRIANGLES);
    glTexCoord2f(uvs[i].x, uvs[i].y);
    glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
    glTexCoord2f(uvs[i+1].x, uvs[i+1].y);
    glVertex3f(vertices[i+1].x, vertices[i+1].y, vertices[i+1].z);
    glTexCoord2f(uvs[i+2].x, uvs[i+2].y);
    glVertex3f(vertices[i+2].x, vertices[i+2].y, vertices[i+2].z);
    glEnd();
  }
  glPopMatrix();
}

bool OBJModel::load(const char * path) {
  std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
  std::vector<VECTOR3D> tmp_vertices;
  std::vector<VECTOR3D> tmp_uvs;
  std::vector<VECTOR3D> tmp_normals;

  FILE * file = fopen(path, "r");
  if(file == NULL) {
    printf("File can't be opened");
    return false;
  }

  while(1) {
    char lineHeader[128];
    
    int res = fscanf(file,"%s", lineHeader);
    if( res == EOF )
      break;

    if( strcmp( lineHeader, "v") == 0) {
      VECTOR3D vertex;
      fscanf(file,"%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
      tmp_vertices.push_back(vertex);
    } else if( strcmp( lineHeader, "vt" ) == 0) {
      VECTOR3D uv;
      fscanf(file,"%f %f\n", &uv.x, &uv.y );
      tmp_uvs.push_back(uv);
    } else if( strcmp( lineHeader, "vn" ) == 0) {
      VECTOR3D normal;
      fscanf(file,"%f %f %f\n", &normal.x, &normal.y, &normal.z );
      tmp_normals.push_back(normal);
    } else if( strcmp( lineHeader, "f") == 0) {
      std::string vertex1, vertex2, vertex3;
      unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
      int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
      //int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
      if(matches != 9) {
	printf("Parse error %d\n", matches);
	return false;
      }
      for(int i=0; i<3; i++) {
	vertexIndices.push_back(vertexIndex[i]);
	uvIndices.push_back(uvIndex[i]);
	normalIndices.push_back(normalIndex[i]);
      }
    }  
  }

  for(unsigned int i=0; i<vertexIndices.size(); i++) {
    unsigned int vertexIndex = vertexIndices[i];
    vertices.push_back(tmp_vertices[vertexIndex-1]);
  }

  for(unsigned int i=0; i<uvIndices.size(); i++) {
    unsigned int uvIndex = uvIndices[i];
    uvs.push_back(tmp_uvs[uvIndex-1]);
  }

  for(unsigned int i=0; i<normalIndices.size(); i++) {
    unsigned int normalIndex = normalIndices[i];
    normals.push_back(tmp_normals[normalIndex-1]);
  }

  return true;
}
