#include "HNode.hpp"
using namespace std;

//GLuint textureId;  //will be generated - we dont have to create explicitly

int HNode::loadBMP_custom(const char * imagepath){
    //data read from header of bmp file
    unsigned char header[54];       //each BMP file begins with by a 54 byte header
    unsigned int dataPos;       //position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;     //width*height*3
    //actual RGB data
    unsigned char * data1;
    //open the file
    FILE * file = fopen(imagepath, "rb");
    if(!file){
        cout<<"image could not be opened."<<endl;
        return 0;
    }
    if(fread(header, 1, 54, file) != 54){
        //if 54 bytes not read - problem
        cout<<"not a correct bmp file"<<endl;
        return 0;
    }
    if((header[0] != 'B') || header[1] != 'M'){
        cout<<"not a correct bmp file"<<endl;
        return 0;
    }

    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);

    //some BMP images are misformatted, guess missing info
    if(imageSize == 0){
        imageSize = width*height*3; //*3 =1 byte for each R,G,B
    }
    if(dataPos == 0){dataPos = 54;}

    //create a buffer
    data1 = new unsigned char [imageSize];

    //read the actual data from the file into the buffer
    fread(data1, 1, imageSize, file);

    //everything in memory now, can close file
    fclose(file);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // glGenTextures(2, textureId);
    glGenTextures(1, &textureId);
    //bind the newly created texture; all future texture functions will modify this tecture
    // glBindTexture(GL_TEXTURE_2D, textureId[0]);
    glBindTexture(GL_TEXTURE_2D, textureId);

    //give image to openGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data1);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    /*for 3D*/
    // glBindTexture(GL_TEXTURE_2D, textureId[1]);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data1);
}

HNode::HNode(HNode* par){
  tx = 0;
  ty = 0;
  tz = 0;
  rx = 0;
  ry = 0;
  rz = 0;
  ptx=pty=ptz=0;

  R=G=B=0;

  if(par!=NULL){
    parent=par;
    parent->add_child(this);
  }

    texMapping = 0;
  obj_type = -1;
  quad  = gluNewQuadric();
  base = inRadius = 0;
  top = outRadius = 0;
  height = 0;
  slices = 50;
  nsides = 0;
  stacks = 10;
  rings = 0;
  height = length = breadth = 0;
  triangle_x1, triangle_y1, triangle_x2, triangle_y2 = 0;
}

void HNode::add_child(HNode *child){
  children.push_back(child);
}

void HNode::render(){
  //Translate the origin
  glTranslatef(tx,ty,tz);
  //Rotate at translated origin
  glRotatef(rx, 1.0, 0.0, 0.0);
  glRotatef(ry, 0.0, 1.0, 0.0);
  glRotatef(rz, 0.0, 0.0, 1.0);
  //Translate before rotation(if any)
  glTranslatef(ptx,pty,ptz);

  if(obj_type == 0){
    if(texMapping == 1){
      loadBMP_custom(image_path);
      glEnable(GL_TEXTURE_2D);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

      //control the generation of texture coordinates
      // GLfloat param1[4] = {1.0, 0.0, 0.0, 0.0};
      // GLfloat param2[4] = {0.0, 0.0, 1.0, 0.0};

      /*
    GLfloat param1[4] = {1.0, 0.0, 0.0, 0.0};
    GLfloat param2[4] = {0.0, 0.0, 1.0, 0.0};
      */

      // glEnable(GL_TEXTURE_GEN_S);
      // glEnable(GL_TEXTURE_GEN_T);
      // glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
      // glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
      // glTexGenfv(GL_S, GL_OBJECT_PLANE, param1);
      // glTexGenfv(GL_T, GL_OBJECT_PLANE, param2);

      gluQuadricNormals(quad, GLU_SMOOTH);
      gluQuadricTexture(quad, GL_TRUE);
      glBindTexture(GL_TEXTURE_2D, textureId);

      gluCylinder(quad, base, top, height, slices, stacks);

      glDisable(GL_TEXTURE_2D);
    }
    else{
      gluQuadricNormals(quad, GLU_SMOOTH);
      gluCylinder(quad, base, top, height, slices, stacks);
    }
  }

  else if(obj_type == 1){
    //cuboid

    // //to draw unfilled cuboid
    // glClear(GL_COLOR_BUFFER_BIT);
    // glMatrixMode( GL_PROJECTION );
    // glLineWidth(5);

    glBegin(GL_QUADS);
 
      //Front
      glNormal3f(0.0f, 0.0f, 1.0f);
      glVertex3f(0.0f, 0.0f, breadth);
      glVertex3f(length, 0.0f, breadth);
      glVertex3f(length, height, breadth);
      glVertex3f(0.0f, height, breadth);
   
      //Right
      glNormal3f(1, 0, 0);
      glVertex3f(length, 0.0f, breadth);
      glVertex3f(length, 0, 0);
      glVertex3f(length, height, 0);
      glVertex3f(length, height, breadth);
   
      //Back
      glNormal3f(0.0f, 0.0f, -1.0f);
      glVertex3f(0, 0, 0);
      glVertex3f(0, height, 0);
      glVertex3f(length, height, 0);
      glVertex3f(length, 0, 0);
   
      //Left
      glNormal3f(-1.0f, 0.0f, 0.0f);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(0, 0.0f, breadth);
      glVertex3f(0, height, breadth);
      glVertex3f(0, height, 0);

      //top
      glNormal3f(0.0f, 1.0f, 0.0f);
      glVertex3f(0, height, 0);
      glVertex3f(0, height, breadth);
      glVertex3f(length, height, breadth);
      glVertex3f(length, height, 0);
   
      //bottom
      glNormal3f(0.0f, -1.0f, 0.0f);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(length, 0.0f, 0.0f);
      glVertex3f(length, 0.0f, breadth);
      glVertex3f(0, 0, breadth);
 
    glEnd();
    glFlush();
  }

  else if(obj_type == 2){
    glutSolidTorus(inRadius, outRadius, nsides, rings);
  }

  else if(obj_type == 3){
    glBegin(GL_TRIANGLES);
      glVertex3f(tx, ty, tz);
      glVertex3f(triangle_x1, triangle_y1, tz);
      glVertex3f(triangle_x2, triangle_y2, tz);
    glEnd();
  }

  else if(obj_type == 4){
    if(texMapping == 1){
      //glBindTexture(GL_TEXTURE_2D, textureId); -- already did
      // texMap_init(file_name);
      loadBMP_custom(image_path);
      glEnable(GL_TEXTURE_2D);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      glBindTexture(GL_TEXTURE_2D, textureId);

      glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(quad_vertices[0], quad_vertices[1], quad_vertices[2]);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(quad_vertices[3], quad_vertices[4], quad_vertices[5]);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(quad_vertices[6], quad_vertices[7], quad_vertices[8]);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(quad_vertices[9], quad_vertices[10], quad_vertices[11]);
      glEnd();
      glDisable(GL_TEXTURE_2D);
    }
    else{
      glBegin(GL_QUADS);
        glVertex3f(quad_vertices[0], quad_vertices[1], quad_vertices[2]);
        glVertex3f(quad_vertices[3], quad_vertices[4], quad_vertices[5]);
        glVertex3f(quad_vertices[6], quad_vertices[7], quad_vertices[8]);
        glVertex3f(quad_vertices[9], quad_vertices[10], quad_vertices[11]);
      glEnd();
    }
  }

  else if(obj_type == 5){       //sliced quadrilateral (for shading)
    glTranslatef(-length/2,-height/2,0);
    int num = slices;
    for (float w1 = 0; w1 < length; w1+=length/num)
    {
        for (float h1 = 0; h1 < height; h1+=height/num)
        {
            // cout << "line 328 w1 " << w1 << " h1 " << h1 << endl;
            //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
            //glEnable(GL_COLOR_MATERIAL);
            glBegin(GL_QUADS);
            glNormal3f(0,1,0);
            glVertex3f(w1, h1, 0);
            glNormal3f(0,1,0);
            glVertex3f(w1, h1 + height/num, 0);
            glNormal3f(0,1,0);
            glVertex3f(w1 + length/num, h1 + height/num, 0);
            glNormal3f(0,1,0);
            glVertex3f(w1 + length/num, h1, 0);
            glEnd();
        }
    }
  }

  else if(obj_type == 6){   //light1
    GLfloat light_pos[] = {0,0,0,1};
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
    GLfloat spot_direction[] = {0,0,1};
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
  }
}

void HNode::set_color(float R, float G, float B){
  this->R = R;
  this->G = G;
  this->B = B;
}

void HNode::change_parameters(float tx,float ty,float tz,float rx,float ry,float rz){
  this->tx=tx;
  this->ty=ty;
  this->tz=tz;
  this->rx=rx;
  this->ry=ry;
  this->rz=rz;
}

void HNode::change_preparameters(float ptx, float pty, float ptz){
  this->ptx = ptx;
  this->pty = pty;
  this->ptz = ptz;
}

void HNode::render_tree(){
  glPushMatrix();
  glColor3f(R,G,B);
  float surface_color[] = {R,G,B};
  float specular[] = {0.2,0.2,0.2,0};
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, surface_color); 
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  render();
  for(int i=0;i<children.size();i++){
    children[i]->render_tree();
  }
  glPopMatrix();
}