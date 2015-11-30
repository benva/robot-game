#ifndef OBJMODEL_H
#define OBJMODEL_H

class OBJModel {
private:
  const char * path;
  std::vector<VECTOR3D> vertices;
  std::vector<VECTOR3D> uvs;
  std::vector<VECTOR3D> normals;
public:
  OBJModel() {};

  bool load(const char * path);
  void draw();
  
};
#endif
