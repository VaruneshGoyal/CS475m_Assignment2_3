#include <iostream>
#include <GL/glut.h>
#include <vector>

#ifndef _HNODE_HPP_
#define _HNODE_HPP_

void vertexcopy(float *src,float *dest);

// A simple class that represents a node in the hierarchy tree
class HNode {
private:
  
  std::vector<HNode*> children;		//vector containing pointers to children nodes
  HNode* parent;					//pointer to parent node
  
  //default = -1
  //0 - cylinder
  //1 - cuboid
  //2 - torus
  //3 - triangle
  //4 - quad
  //5 - mesh quad

public:

  float tx,ty,tz;   //translation from its origin
  float rx,ry,rz;   //rotation about x, y, z
  float ptx,pty,ptz;  //translation before rotation needed in some cases simplifies stuff
  float R,G,B;
  
  GLUquadric* quad;
  double base;
  double inRadius;
  double top;
  double outRadius;
  GLint slices;
  GLint nsides;
  GLint stacks;
  GLint rings;
  double height, length, breadth;
  int obj_type;
  float triangle_x1, triangle_y1, triangle_x2, triangle_y2;
  float* quad_vertices; //{x,y,z , x,y,z , x,y,z , x,y,z} format
  double matrix[16];

  bool texMapping;  //1 => texture mapping present
  GLuint textureId;  //will be generated - we dont have to create explicitly
  //char* file_name;  //filename = the bmp file that stores the textures
  char* image_path;

  HNode (HNode* par);
  void add_child(HNode*);		//function to add a child node to this node
  void render();				//function to render(create) this node
  void change_parameters(float tx,float ty,float tz,float rx,float ry,float rz);		//change parameters of this node - rotate/translate it
  void change_preparameters(float ptx, float pty, float ptz);
  void render_tree();			//function to render the whole tree - this + children (NOT parents)
  void set_color(float R, float G, float B);
  // void set_material();
  // void (*render_function)();

  int loadBMP_custom(const char * imagepath);
  void rotate_z();
  
};

#endif