#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "HNode.hpp"
#include <cmath>
#include <math.h>       //for asin

using namespace std;

HNode *node[10];
HNode *frame[8];
HNode *wheel_front;
HNode *wheel_back;
HNode *spoke_front[9];
HNode *spoke_back[9];
HNode *spoke_front2[9];
HNode *spoke_back2[9];
HNode *spoke_front_extra[9];
HNode *spoke_back_extra[9];
HNode *spoke_front2_extra[9];
HNode *spoke_back2_extra[9];
HNode *wheel_axis_f, *wheel_axis_b;
HNode *pedal_cuboid[2];
HNode *gear[2];
HNode *gear_spokes[5];
HNode *seat[2];
HNode *pedal_rod[2];
HNode *pedal_rod_across;
HNode *axle;
HNode *across_gear;
HNode *photoframe;
HNode *photo_in_frame;
HNode *headlight_cylinder;
HNode *ceiling_lamp_connector;
HNode *ceiling_lamp;
HNode *ceiling_lamp_backcover;

HNode *spotlight;

HNode *room[6];

HNode *handle_connect_with_frame, *handle_connect_front_wheel_across, 
      *handle_connect_front_wheel_left, *handle_connect_front_wheel_right,
      *handlebar_connector, *handlebar_connector_across, *handlebar_left, *handlebar_right;

//static variables
  float wheel_outer_radius;// = 24;
  float wheel_tyre_radius;// = 1;
  float bar_radius;// = 2;
  float handlebar_connector_across_length;// = 23;
  float handlebar_connector_length;// = 15;
  float spoke_length;// = 21;
  float spoke_radius;// = 0.3;
  float extra_spoke_length;// = 2;
  float extra_spoke_radius;// = 0.4;
  float handle_connect_with_frame_length;// = 23;
  float handle_connect_front_wheel_across_length;// = 8;
  float handle_connect_front_wheel_length = wheel_outer_radius + bar_radius + 0.5;
  float handlebar_handle_length;// = 10;
  float wheel_axis_length = handle_connect_front_wheel_across_length - 2*bar_radius;

  float frame_upper_horizontal_len;// = 50;  //123
  float frame_lower_horizontal_len;// = 55;
  float frame_right_vertical_len;// = 48;
  float frame_upper_right_horizontal_len;// = 40;
  float frame_lower_right_horizontal_len;// = 35;
  float outer_gear_inRadius ;//= 1;
  float inner_gear_radius ;//= 1.5;

  float axle_length ;//= 8;
  float across_gear_length;// = 4;

  float room_height;// = 300;
  float room_breadth ;//= 600;
  float room_length;// = 400;

  float ceiling_lamp_connector_base;// = 10;
  float ceiling_lamp_connector_height;// = 20;
  float ceiling_lamp_top;// = 30;
  float ceiling_lamp_height;// = 20;

  float photoframe_inradius ;//= 3.50;
  float photoframe_outradius ;//= 50;

  float headlight_cylinder_base ;//= 2;
  float headlight_cylinder_top;// = 4;
  float headlight_cylinder_height;// = 7;

void set_static_variables(string filename){
  string variable_name;
  ifstream static_variables(filename);
  if (static_variables.is_open())
  {
    static_variables>>wheel_outer_radius;
    static_variables>>wheel_tyre_radius;
    static_variables>>bar_radius;
    static_variables>>handlebar_connector_across_length;
    static_variables>>handlebar_connector_length;
    static_variables>>spoke_length;
    static_variables>>spoke_radius;
    static_variables>>extra_spoke_length;
    static_variables>>extra_spoke_radius;
    static_variables>>handle_connect_with_frame_length;
    static_variables>>handle_connect_front_wheel_across_length;
    // static_variables>>handle_connect_front_wheel_length;
    handle_connect_front_wheel_length = wheel_outer_radius + bar_radius + 0.5;
    static_variables>>handlebar_handle_length;
    wheel_axis_length = handle_connect_front_wheel_across_length - 2*bar_radius;

    static_variables>>frame_upper_horizontal_len;
    static_variables>>frame_lower_horizontal_len;
    static_variables>>frame_right_vertical_len;
    static_variables>>frame_upper_right_horizontal_len;
    static_variables>>frame_lower_right_horizontal_len;
    static_variables>>outer_gear_inRadius;
    static_variables>>inner_gear_radius;
    static_variables>>axle_length;
    static_variables>>across_gear_length;
    static_variables>>room_height;
    static_variables>>room_breadth;
    static_variables>>room_length;

    static_variables>>ceiling_lamp_connector_base;
    static_variables>>ceiling_lamp_connector_height;
    static_variables>>ceiling_lamp_top;
    static_variables>>ceiling_lamp_height;

    static_variables>>photoframe_inradius;
    static_variables>>photoframe_outradius;

    static_variables>>headlight_cylinder_base;
    static_variables>>headlight_cylinder_top;
    static_variables>>headlight_cylinder_height;

    static_variables.close();
  }

  else cout << "Unable to open file"; 

  return;
}

//dynamic variables  
  float handle_rotation = 0;
  float wheel_rotation = 0;
  float cycle_posx=0;
  float cycle_posy=0;
  float angle_handle_z=0;
  float cycle_rotate_y=0;
  float back_wheel_rotate=0;
  float front_wheel_rotate=0;

  float delta_x, delta_l, l=80.0;
  float theta, theta_f, theta_f2;

  int camera = 1;


//lighting and shading
  GLfloat light0_position[] = { room_length/2, room_height-30, 0, 1};
  GLfloat light0_diffuse[] = {0.8,0.8,0.8};
  GLfloat light0_specular[] = {1,1,1};
  GLfloat light0_ambient[] = {0.4,0.4,0.4};
  bool l0_status = 1;;
  //GLfloat light1_position[] = { room_length/2, room_height-30, 0, 1};
  GLfloat light1_position[] = { 0,0, 0, 1};
  GLfloat light1_diffuse[] = {0.8,0.8,0.8};
  GLfloat light1_specular[] = {1,1,1};
  GLfloat light1_ambient[] = {0.4,0.4,0.4};
  GLfloat spot_direction[] = { -1.0, -1.0, 0.0 };
  bool l1_status = 1;

int is_playback_active = 0; 
float keyframe_data1[15];
float keyframe_data2[15];
float keyframe_data[15];
bool enable_recording = 0;
int keyframe_number = 0;
int number_of_interpolating_frames = 25;

void save_line(bool enable){
  if (!enable) return;
  ofstream keyframe("keyframes.txt",std::ios_base::app);
  keyframe_number++;
  keyframe<<keyframe_number<<" ";                   //0
  keyframe<< handle_connect_with_frame->rz<<" ";    //1
  keyframe<< wheel_back->rz<<" ";                   //2
  keyframe<< wheel_front->rz<<" ";                  //3
  keyframe<< gear[0]->rz<<" ";                      //4
  keyframe<< wheel_axis_b->tx<<" ";                 //5
  keyframe<< wheel_axis_b->tz<<" ";                 //6
  keyframe<< wheel_axis_b->ry<<" ";                 //7
  keyframe<<camera<<" ";                            //8
  keyframe<<l0_status<<" ";                         //9
  keyframe<<l1_status<<" ";                         //10
  keyframe<<number_of_interpolating_frames<<" ";         //11
  keyframe<<number_of_interpolating_frames<< " ";         //12
  keyframe<<number_of_interpolating_frames<< " ";         //13
  keyframe<<number_of_interpolating_frames<<endl;         //14
  keyframe.close();
  cout<<"Line saved\n";
  //keyframe<< <<endl;
  return;
}

void clear_file(){
  ofstream keyframe("keyframes.txt");
  keyframe.close();
  cout<<"Keyframes cleared\n";
  keyframe_number = 0;
  return;
}

bool first_frame = 1;
int current_frame = 0;
bool update = 0;

ifstream keyframes_playback;

void replay_animation(){
  if(!is_playback_active) return;
  // cout<<"In glutIdleFunc\n";
  //setup the consecutive keyframes to interpolate between
  if(first_frame){
    update = 0;
    for (int i=0; i<15; i++){
      keyframes_playback>>keyframe_data1[i];
    }
    for (int i=0; i<15; i++){
      keyframes_playback>>keyframe_data2[i];
    }
    first_frame = 0;
    update = 1;
    // cout<<"First 2 frames read\n";
  } else if (current_frame == keyframe_data1[14]){
    update = 0;
    cout<<"Reading next frame\n";
    current_frame = 0;
    if(keyframes_playback>>keyframe_data1[0]){
      cout<<keyframe_data1[0]<<endl; 
      for (int i=1; i<15; i++){
        keyframe_data1[i] = keyframe_data2[i];
        keyframes_playback>>keyframe_data2[i];
        cout<<keyframe_data2[i]<<" ";
      }
      cout<<endl;
      update = 1;
    }
    else {
    is_playback_active = 0;       //stop playback
    keyframes_playback.close(); 
    cout<<"Playback complete\n"; 
    return;
    }  
  }

  for(int i=0; i<8; i++){
    keyframe_data[i] = keyframe_data1[i]*(1 - float(current_frame)/keyframe_data1[14])
                         + keyframe_data2[i]*float(current_frame)/keyframe_data1[14];
  }
  // cout<<"Interpolation done\n";
  if(l0_status!=keyframe_data1[9]){
    if(keyframe_data1[9]) glEnable(GL_LIGHT0);
    else glDisable(GL_LIGHT0);
  }
  if(l0_status!=keyframe_data1[10]){
    if(keyframe_data1[10]) glEnable(GL_LIGHT1);
    else glDisable(GL_LIGHT1);
  }
  //camera = keyframe_data1[8];
  // cout<<camera<<" camera"<<endl;
  //glutTimerFunc(40, update_dynamic_variables, is_playback_active);
  glutPostRedisplay();
}

void update_dynamic_variables(int){
  // cout<<"blah\n";
  if(update != 0) {
    handle_connect_with_frame->rz = keyframe_data[1];
    wheel_back->rz = keyframe_data[2];
    wheel_front->rz = keyframe_data[3];
    gear[0]->rz = keyframe_data[4];
    wheel_axis_b->tx = keyframe_data[5];
    wheel_axis_b->tz = keyframe_data[6];
    wheel_axis_b->ry = keyframe_data[7];
    camera = keyframe_data1[8];
    l0_status = keyframe_data1[9];
    l1_status = keyframe_data1[10];
    current_frame++;
    // cout<<current_frame<<" "<<keyframe_data1[14]<< endl;
  }
  glutTimerFunc(40, update_dynamic_variables, is_playback_active);
  // return;
}

//Our function for processing ASCII keys
void processNormalKeys(unsigned char key, int x, int y) {
  switch(key) {
    case 'X': 
      glRotatef(1,1,0,0);
      break;
    case 'Y':
      glRotatef(1,0,1,0);
      break;      
    case 'Z':
      glRotatef(1,0,0,1);
      break;     
    case 'x':
      glRotatef(1,-1,0,0);
      break;
    case 'y':
      glRotatef(1,0,-1,0);
      break;      
    case 'z':
      glRotatef(1,0,0,-1);
      break;    
    case 'L':
      if(l0_status) glDisable(GL_LIGHT0);
      else glEnable(GL_LIGHT0);
      l0_status = !l0_status;
      break;
    case 'H':
      if(l1_status) glDisable(GL_LIGHT1);
      else glEnable(GL_LIGHT1);
      l1_status = !l1_status;
      break;
    case '1':
      camera = 1;
      break;
    case '2':
      camera = 2;
      break;
    case '3':
      camera = 3;
      break;
    case 'R':
      enable_recording = !enable_recording;
      if(enable_recording) cout<<"Recording mode ON"<<endl;
      else cout<<"Recording mode OFF"<<endl;
      break;
    case 'S':
      save_line(enable_recording);
      break;
    case 'C':
      clear_file();
      break;
    case 'P':
      is_playback_active = !is_playback_active;
      current_frame = 0;
      first_frame = 1;
      keyframes_playback.open("keyframes.txt");
      break;
    case 'A':
    	handle_connect_with_frame->rotate_z();
    	break;
  }
  glutPostRedisplay();
  if (key == 27)
  exit(0);
}

//Our function for processing Non-ASCII keys
void processSpecialKeys(int key, int x, int y) {
  switch(key) {
    if(wheel_back->rz>=360) wheel_back->rz-=360;
    if(wheel_front->rz>=360) wheel_front->rz-=360;
    if(wheel_back->rz<0) wheel_back->rz+=360;
    if(wheel_front->rz<0) wheel_front->rz+=360;

    // sin(Off)/(l-deltal) = sin(180 - O)/l = sin(Of)/deltax;
    // Of = O - Off;
    case GLUT_KEY_UP:
      wheel_back->rz++;
      delta_l = wheel_outer_radius*3.1415/180;
      theta = handle_connect_with_frame->rz*3.1415/180;
      theta_f2 = asin(sin(theta)*(l - delta_l)/l);
      theta_f = theta - theta_f2;
      if(theta) delta_x = sin(theta_f)*l/sin(theta);
      else delta_x = delta_l;

      wheel_axis_b->tx -= wheel_outer_radius*cos(theta_f + wheel_axis_b->ry*3.1415/180)*3.1415/180;
      wheel_axis_b->tz += wheel_outer_radius*sin(theta_f + wheel_axis_b->ry*3.1415/180)*3.1415/180;
      wheel_axis_b->ry += theta_f*180/3.1415;
      wheel_front->rz += (delta_x/wheel_outer_radius)*180/3.1415;

      gear[0]->rz+= 6.5/4;
      pedal_cuboid[0]->change_parameters(0,1,0,0,45+gear[0]->rz,0);
      pedal_cuboid[1]->change_parameters(0,-pedal_cuboid[1]->height - 1,0,0,45+gear[0]->rz,0);
      // cout<<(delta_x/wheel_outer_radius)*180/3.1415<<endl;
      // cout<<delta_x<<" "<<theta<<" "<<theta_f<<" "<<theta_f2<<endl;
      // std::cout<<wheel_front->tx<<" "<<wheel_front->ty<<" "<<wheel_front->tz<<endl;
      break;

    case GLUT_KEY_DOWN:
      wheel_back->rz--;
      delta_l = wheel_outer_radius*3.1415/180;
      theta = handle_connect_with_frame->rz*3.1415/180;
      theta_f2 = asin(sin(theta)*(l - delta_l)/l);
      theta_f = theta - theta_f2;
      if(theta) delta_x = sin(theta_f)*l/sin(theta);
      else delta_x = delta_l;

      wheel_axis_b->tx += wheel_outer_radius*cos(theta_f + wheel_axis_b->ry*3.1415/180)*3.1415/180;
      wheel_axis_b->tz -= wheel_outer_radius*sin(theta_f + wheel_axis_b->ry*3.1415/180)*3.1415/180;
      wheel_axis_b->ry -= theta_f*180/3.1415;
      wheel_front->rz -= delta_x/wheel_outer_radius*180/3.1415;

      gear[0]->rz-= 6.5/4;
      pedal_cuboid[0]->change_parameters(0,1,0,0,45+gear[0]->rz,0);
      pedal_cuboid[1]->change_parameters(0,-pedal_cuboid[1]->height - 1,0,0,45+gear[0]->rz,0);
      // std::cout<<wheel_front->tx<<" "<<wheel_front->ty<<" "<<wheel_front->tz<<endl;
      break;

    case GLUT_KEY_LEFT:
      handle_connect_with_frame->rz++;
      if(handle_connect_with_frame->rz > 90 || handle_connect_with_frame->rz < -90) handle_connect_with_frame->rz--;
      break;

    case GLUT_KEY_RIGHT:
      handle_connect_with_frame->rz--;
      if(handle_connect_with_frame->rz > 90 || handle_connect_with_frame->rz < -90) handle_connect_with_frame->rz++;
      break;
  }
  //Redraw
  glutPostRedisplay();
}

void display(void){
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glLoadIdentity();
  if(camera==1){
    gluLookAt(wheel_axis_b->tx,100,wheel_axis_b->tz,
              wheel_axis_b->tx - 10*cos(wheel_axis_b->ry*3.1415/180),100,wheel_axis_b->tz + 10*sin(wheel_axis_b->ry*3.1415/180),
              0,1,0);
  } else if(camera==2){
    gluLookAt(wheel_axis_b->tx + 50*cos(wheel_axis_b->ry*3.1415/180),150,wheel_axis_b->tz - 50*sin(wheel_axis_b->ry*3.1415/180),
              wheel_axis_b->tx - 10*cos(wheel_axis_b->ry*3.1415/180),100,wheel_axis_b->tz + 10*sin(wheel_axis_b->ry*3.1415/180),
              0,1,0);
  } else {
    gluLookAt(room_length/2 - 50, room_height-50, room_breadth/2 - 50,
              0,0,0,
              0,1,0);
  }

  node[0]->render_tree();
  glutSwapBuffers();
}

void init(void){
  // Use depth buffering for hidden surface elimination.
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  // Setup the view of the cube.
  glMatrixMode(GL_PROJECTION);
  //gluPerspective(field of view in degree, aspect ratio, Z near, Z far);
  gluPerspective(80.0, 1.0, 1.0, 1000.0);

  glMatrixMode(GL_MODELVIEW);
  gluLookAt(0, room_height/2, 180.0,  // eye is at (0,0,0)
            0.0, room_height/2, 0.0,      // center is at (0,0,0)
            0.0, 1.0, 0.0);      // up is in positive Y direction

  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);

  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);

  glShadeModel (GL_SMOOTH);
}

int main(int argc, char **argv)
{

  set_static_variables("static_variables.txt");
  // of the first node - vetical cylinder containing the front handle
  node[0] = new HNode(NULL);
  node[1] = new HNode(NULL);
  
  for(int i=0; i<6; i++){
    room[i] = new HNode(NULL);
    node[0]->add_child(room[i]);
  }

  wheel_axis_f = new HNode(NULL);
  wheel_axis_b = new HNode(NULL);
 
  node[0]->add_child(wheel_axis_b);
  wheel_axis_b->add_child(node[1]);

  frame[0] = new HNode(NULL);
  node[1]->add_child(frame[0]);

  wheel_front = new HNode(NULL);
  wheel_front->add_child(wheel_axis_f);
  wheel_back = new HNode(NULL);
  node[1]->add_child(wheel_back);

  for(int i=0; i<9; i++){
    spoke_back[i] = new HNode(NULL);
    spoke_front[i] = new HNode(NULL);
    wheel_front->add_child(spoke_front[i]);
    wheel_back->add_child(spoke_back[i]);

    spoke_back2[i] = new HNode(NULL);
    spoke_front2[i] = new HNode(NULL);
    wheel_front->add_child(spoke_front2[i]);
    wheel_back->add_child(spoke_back2[i]);

    spoke_back2_extra[i] = new HNode(NULL);
    spoke_back_extra[i] = new HNode(NULL);
    spoke_front_extra[i] = new HNode(NULL);
    spoke_front2_extra[i] = new HNode(NULL);
    spoke_back[i]->add_child(spoke_back_extra[i]);
    spoke_back2[i]->add_child(spoke_back2_extra[i]);
    spoke_front[i]->add_child(spoke_front_extra[i]);
    spoke_front2[i]->add_child(spoke_front2_extra[i]);
  }

//handle
  handle_connect_with_frame = new HNode(NULL);
  node[1]->add_child(handle_connect_with_frame);

  handle_connect_front_wheel_across = new HNode(NULL);
  handle_connect_front_wheel_across->add_child(wheel_front);
  handle_connect_front_wheel_right = new HNode(NULL);
  handle_connect_front_wheel_left = new HNode(NULL);
  handle_connect_with_frame->add_child(handle_connect_front_wheel_across);
  handle_connect_front_wheel_across->add_child(handle_connect_front_wheel_left);
  handle_connect_front_wheel_across->add_child(handle_connect_front_wheel_right);

  handlebar_connector = new HNode(NULL);
  handle_connect_with_frame->add_child(handlebar_connector);
  headlight_cylinder = new HNode(NULL);
  handle_connect_with_frame->add_child(headlight_cylinder);
  handlebar_connector_across = new HNode(NULL);
  handlebar_connector->add_child(handlebar_connector_across);
  handlebar_left = new HNode(NULL);
  handlebar_right = new HNode(NULL);
  handlebar_connector_across->add_child(handlebar_left);
  handlebar_connector_across->add_child(handlebar_right);

//room
  room[0]->obj_type = 5;  //ceiling
  room[0]->length = room_length;
  room[0]->height = room_breadth;
  room[0]->change_parameters(0,room_height,0,90,0,0);
  room[0]->set_color(0,1,1);

  ceiling_lamp_connector = new HNode(NULL);
  ceiling_lamp_backcover = new HNode(NULL);
  ceiling_lamp = new HNode(NULL);
  room[0]->add_child(ceiling_lamp_connector);
  ceiling_lamp_connector->add_child(ceiling_lamp_backcover);
  ceiling_lamp_connector->add_child(ceiling_lamp);

  ceiling_lamp_connector->obj_type = 0;
  ceiling_lamp_connector->base=ceiling_lamp_connector_base;
  ceiling_lamp_connector->top=ceiling_lamp_connector_base;
  ceiling_lamp_connector->height=ceiling_lamp_connector_height;
  ceiling_lamp_connector->slices=4;
  ceiling_lamp_connector->stacks=10;
  ceiling_lamp_connector->change_parameters(room_length/2, room_height/2, 0, 0,0,0);
  ceiling_lamp_connector->set_color(0,0,0);  

  ceiling_lamp->obj_type = 0;
  ceiling_lamp->base=ceiling_lamp_connector_base;
  ceiling_lamp->top=ceiling_lamp_top;
  ceiling_lamp->height=ceiling_lamp_height;
  ceiling_lamp->slices=4;
  ceiling_lamp->stacks=10;
  ceiling_lamp->change_parameters(0,0,ceiling_lamp_connector_height,0,0,0);
  ceiling_lamp->set_color(0.7,0.7,0.7);

  ceiling_lamp_backcover->obj_type = 5;
  ceiling_lamp_backcover->length = sqrt(2)*ceiling_lamp_connector_base;
  ceiling_lamp_backcover->height = sqrt(2)*ceiling_lamp_connector_base;
  ceiling_lamp_backcover->slices = 1;
  ceiling_lamp_backcover->change_parameters(0,0,ceiling_lamp_connector_height,0,0,0);

  room[1]->obj_type = 4;  //floor
  float floor_vertices[12] = {0,0,0, room_length,0,0, room_length,0,room_breadth, 0,0,room_breadth};
  room[1]->quad_vertices = floor_vertices;
  room[1]->change_parameters(-room_length/2, 0, -room_breadth/2, 0,0,0);
  room[1]->set_color(1,1,1);
  room[1]->texMapping = 1;
  char imagePath[] = "./floor.bmp";
  room[1]->image_path = imagePath;

  room[2]->obj_type = 5;  //front
  room[2]->length = room_length;
  room[2]->height = room_height;
  room[2]->slices = 20;
  room[2]->change_parameters(0, room_height/2, room_breadth/2, 0,0,0);
  room[2]->set_color(0,1,0);  

  room[3]->obj_type = 5;  //back
  room[3]->length = room_length;
  room[3]->height = room_height;
  room[3]->slices = 20;
  room[3]->change_parameters(0, room_height/2, -room_breadth/2, 0,0,0);
  room[3]->set_color(0,1,0);

    photoframe = new HNode(NULL);
    room[3]->add_child(photoframe);
    photoframe->obj_type = 2;
    photoframe->inRadius = photoframe_inradius;
    photoframe->outRadius = photoframe_outradius;
    photoframe->nsides=10;  
    photoframe->rings=4;
    photoframe->change_parameters(room_length/2, room_height/2, 0, 0, 0, 45);

    photo_in_frame = new HNode(NULL);
    photoframe->add_child(photo_in_frame);
    photo_in_frame->obj_type = 4;
    float photo_in_frame_vertices[12] = {50,0,1, 0,50,1, -50,0,1, 0,-50,1};
    photo_in_frame->quad_vertices = photo_in_frame_vertices;
    photo_in_frame->set_color(1,1,1);
    photo_in_frame->texMapping=1;
    char imagePath1[] = "./frame_pic.bmp";
    photo_in_frame->image_path=imagePath1;

  room[4]->obj_type = 5;  //left
  room[4]->length = room_breadth;
  room[4]->height = room_height;
  room[4]->slices = 20;
  room[4]->change_parameters(-room_length/2, room_height/2,0, 0,90,0);
  room[4]->set_color(0,1,0);

  room[5]->obj_type = 5;  //right
  room[5]->length = room_breadth;
  room[5]->height = room_height;
  room[5]->slices = 20;
  room[5]->change_parameters(room_length/2,room_height/2,0,0,90,0);
  room[5]->set_color(0,1,0);

//headlight
  headlight_cylinder->obj_type = 0;
  headlight_cylinder->base=headlight_cylinder_base;
  headlight_cylinder->top=headlight_cylinder_top;
  headlight_cylinder->height=headlight_cylinder_height;
  headlight_cylinder->slices=50;
  headlight_cylinder->stacks=10;
  headlight_cylinder->change_preparameters(handle_connect_with_frame_length/4,0,0);
  headlight_cylinder->change_parameters(0,0,0,0,-90,0);
  headlight_cylinder->set_color(0.6,0.6,0.6);             //silverish color

  //spotlight
  spotlight = new HNode(NULL);
  headlight_cylinder->add_child(spotlight);
  spotlight->obj_type = 6;

//front wheel...this is child of handle_connect_front_wheel_across!
  wheel_front->obj_type = 2;
  wheel_front->inRadius=wheel_tyre_radius;
  wheel_front->outRadius=wheel_outer_radius;
  wheel_front->nsides=50;
  wheel_front->rings=50;
  wheel_front->change_parameters(0,-26.5, handle_connect_front_wheel_across_length/2, 0, 0, 0);

//back wheel
  wheel_back->obj_type = 2;
  wheel_back->inRadius=wheel_tyre_radius;
  wheel_back->outRadius=wheel_outer_radius;
  wheel_back->nsides=50;  
  wheel_back->rings=50;
  wheel_back->change_parameters(50, 0, 0, 0, 0, 0);

//the spokes
  for(int i=0; i<9; i++){
    spoke_front[i]->obj_type = 0;
    spoke_front[i]->base=spoke_radius;
    spoke_front[i]->top=spoke_radius;
    spoke_front[i]->height=spoke_length;
    spoke_front[i]->slices=50;
    spoke_front[i]->stacks=10;
    spoke_front[i]->change_preparameters(2,0,0);
    spoke_front[i]->change_parameters(0,0,0,90,40*i,0);
    spoke_front[i]->set_color(0.6,0.6,0.6);             //silverish color

    spoke_front_extra[i]->obj_type = 0;
    spoke_front_extra[i]->base=extra_spoke_radius;
    spoke_front_extra[i]->top=extra_spoke_radius;
    spoke_front_extra[i]->height=extra_spoke_length;
    spoke_front_extra[i]->slices=50;
    spoke_front_extra[i]->stacks=10;
    //spoke_front_extra[i]->change_preparameters(2,0,0);
    spoke_front_extra[i]->change_parameters(0,0,21,0,0,0);
    spoke_front_extra[i]->set_color(0.855,0.647,0.125);       //goldenrod color

    spoke_back[i]->obj_type = 0;
    spoke_back[i]->base=spoke_radius;
    spoke_back[i]->top=spoke_radius;
    spoke_back[i]->height=spoke_length;
    spoke_back[i]->slices=50;
    spoke_back[i]->stacks=10;
    spoke_back[i]->change_preparameters(2,0,0);
    spoke_back[i]->change_parameters(0,0,0,90,40*i,0);
    spoke_back[i]->set_color(0.6,0.6,0.6);

    spoke_back_extra[i]->obj_type = 0;
    spoke_back_extra[i]->base=extra_spoke_radius;
    spoke_back_extra[i]->top=extra_spoke_radius;
    spoke_back_extra[i]->height=extra_spoke_length;
    spoke_back_extra[i]->slices=50;
    spoke_back_extra[i]->stacks=10;
    //spoke_back_extra[i]->change_preparameters(2,0,0);
    spoke_back_extra[i]->change_parameters(0,0,21,0,0,0);
    spoke_back_extra[i]->set_color(0.855,0.647,0.125);

    spoke_front2[i]->obj_type = 0;
    spoke_front2[i]->base=spoke_radius;
    spoke_front2[i]->top=spoke_radius;
    spoke_front2[i]->height=spoke_length;
    spoke_front2[i]->slices=50;
    spoke_front2[i]->stacks=10;
    spoke_front2[i]->change_preparameters(-2,0,0);
    spoke_front2[i]->change_parameters(0,0,0,90,40*i-10,0);
    spoke_front2[i]->set_color(0.6,0.6,0.6);

    spoke_front2_extra[i]->obj_type = 0;
    spoke_front2_extra[i]->base=extra_spoke_radius;
    spoke_front2_extra[i]->top=extra_spoke_radius;
    spoke_front2_extra[i]->height=extra_spoke_length;
    spoke_front2_extra[i]->slices=50;
    spoke_front2_extra[i]->stacks=10;
    //spoke_front2_extra[i]->change_preparameters(2,0,0);
    spoke_front2_extra[i]->change_parameters(0,0,21,0,0,0);
    spoke_front2_extra[i]->set_color(0.855,0.647,0.125);

    spoke_back2[i]->obj_type = 0;
    spoke_back2[i]->base=spoke_radius;
    spoke_back2[i]->top=spoke_radius;
    spoke_back2[i]->height=spoke_length;
    spoke_back2[i]->slices=50;
    spoke_back2[i]->stacks=10;
    spoke_back2[i]->change_preparameters(-2,0,0);
    spoke_back2[i]->change_parameters(0,0,0,90,40*i-10,0);
    spoke_back2[i]->set_color(0.6,0.6,0.6);

    spoke_back2_extra[i]->obj_type = 0;
    spoke_back2_extra[i]->base=extra_spoke_radius;
    spoke_back2_extra[i]->top=extra_spoke_radius;
    spoke_back2_extra[i]->height=extra_spoke_length;
    spoke_back2_extra[i]->slices=50;
    spoke_back2_extra[i]->stacks=10;
    //spoke_back2_extra[i]->change_preparameters(2,0,0);
    spoke_back2_extra[i]->change_parameters(0,0,21,0,0,0);
    spoke_back2_extra[i]->set_color(0.855,0.647,0.125);
  }

//the wheel axes
  wheel_axis_f->obj_type=0;
  wheel_axis_f->base=bar_radius;
  wheel_axis_f->top=bar_radius;
  wheel_axis_f->height=wheel_axis_length;
  wheel_axis_f->slices=50;
  wheel_axis_f->stacks=10;
  wheel_axis_f->change_parameters(0,0,-wheel_axis_length/2,0,0,0);

  wheel_axis_b->obj_type=0;
  wheel_axis_b->base=bar_radius;
  wheel_axis_b->top=bar_radius;
  wheel_axis_b->height=wheel_axis_length;
  wheel_axis_b->slices=50;
  wheel_axis_b->stacks=10;
  wheel_axis_b->change_parameters(50,wheel_outer_radius,0,0,0,0);
  node[1]->change_parameters(-50,0, wheel_axis_length/2,0,0,0);

//handle of the cycle
  handle_connect_with_frame->obj_type=0;
  handle_connect_with_frame->base=bar_radius;
  handle_connect_with_frame->top=bar_radius;
  handle_connect_with_frame->height=handle_connect_with_frame_length;
  handle_connect_with_frame->slices=50;
  handle_connect_with_frame->stacks=10;
  handle_connect_with_frame->change_parameters(-30,24,0,-90,25,0);
  handle_connect_with_frame->set_color(1,0,0);    

  handlebar_connector->obj_type=0;
  handlebar_connector->base=bar_radius;
  handlebar_connector->top=bar_radius;
  handlebar_connector->height=handlebar_connector_length;
  handlebar_connector->slices=50;
  handlebar_connector->stacks=10;
  handlebar_connector->change_preparameters(0,0,0);
  handlebar_connector->change_parameters(0,0,handle_connect_with_frame->height,0,-90,0);
  handlebar_connector->set_color(0.7,0.7,0.7);  

  handlebar_connector_across->obj_type=0;
  handlebar_connector_across->base=bar_radius;
  handlebar_connector_across->top=bar_radius;
  handlebar_connector_across->height=handlebar_connector_across_length;
  handlebar_connector_across->slices=50;
  handlebar_connector_across->stacks=10;
  handlebar_connector_across->change_preparameters(0,0,-handlebar_connector_across->height/2);
  handlebar_connector_across->change_parameters(0,0,handlebar_connector->height,90,0,0);
  handlebar_connector_across->set_color(0.7,0.7,0.7);

  handlebar_left->obj_type=0;
  handlebar_left->base=bar_radius;
  handlebar_left->top=bar_radius;
  handlebar_left->height=handlebar_handle_length;
  handlebar_left->slices=50;
  handlebar_left->stacks=10;
  handlebar_left->change_parameters(0,0,handlebar_connector_across->height,0,0,0);
  handlebar_left->set_color(0,0,0);

  handlebar_right->obj_type=0;
  handlebar_right->base=bar_radius;
  handlebar_right->top=bar_radius;
  handlebar_right->height=handlebar_handle_length;
  handlebar_right->slices=50;
  handlebar_right->stacks=10;
  handlebar_right->change_parameters(0,0,-handlebar_right->height,0,0,0);
  handlebar_right->set_color(0,0,0);

  handle_connect_front_wheel_across->obj_type=0;
  handle_connect_front_wheel_across->base=bar_radius;
  handle_connect_front_wheel_across->top=bar_radius;
  handle_connect_front_wheel_across->height=handle_connect_front_wheel_across_length;
  handle_connect_front_wheel_across->slices=50;
  handle_connect_front_wheel_across->stacks=10;
  handle_connect_front_wheel_across->change_preparameters(0,0,-handle_connect_front_wheel_across->height/2);
  handle_connect_front_wheel_across->change_parameters(0,0,0,90,0,0);
  handle_connect_front_wheel_across->set_color(0,0,0);  

  handle_connect_front_wheel_left->obj_type=0;
  handle_connect_front_wheel_left->base=bar_radius;
  handle_connect_front_wheel_left->top=bar_radius;
  handle_connect_front_wheel_left->height=handle_connect_front_wheel_length;
  handle_connect_front_wheel_left->slices=50;
  handle_connect_front_wheel_left->stacks=10;
  handle_connect_front_wheel_left->change_parameters(0,0,handle_connect_front_wheel_across->height,90,0,0);
  handle_connect_front_wheel_left->set_color(0.5,0.5,0.5); 

  handle_connect_front_wheel_right->obj_type=0;
  handle_connect_front_wheel_right->base=bar_radius;
  handle_connect_front_wheel_right->top=bar_radius;
  handle_connect_front_wheel_right->height=25.5;
  handle_connect_front_wheel_right->slices=50;
  handle_connect_front_wheel_right->stacks=10;
  handle_connect_front_wheel_right->change_parameters(0,0,0,90,0,0);
  handle_connect_front_wheel_right->set_color(0.5,0.5,0.5);

/********the main frame****************/

  //frame[0] = frame_upper_horizontal
  frame[0]-> obj_type=0;
  frame[0]->base=bar_radius;
  frame[0]->top=bar_radius;
  frame[0]->height=frame_upper_horizontal_len;
  frame[0]->slices=50;
  frame[0]->stacks=10;
  frame[0]->change_parameters(-25,40,0,-90,105,0);
  frame[0]->set_color(1,0,0);
  node[1]->add_child(frame[0]);
  frame[0]->texMapping=1;
  char imagePath_frame0[] = "./stripes3.bmp";
  frame[0]->image_path = imagePath_frame0;

  //frame[1] = frame_lower_horizontal
  frame[1] = new HNode(NULL);
  frame[1]-> obj_type=0;
  frame[1]->base=bar_radius;
  frame[1]->top=bar_radius;
  frame[1]->height=frame_lower_horizontal_len;
  frame[1]->slices=50;
  frame[1]->stacks=10;
  frame[1]->change_parameters(-26,35,0,-90,130,0);
  frame[1]->set_color(1,0,0);
  frame[1]->texMapping=1;
  char imagePath_frame1[] = "./stripes3.bmp";
  frame[1]->image_path = imagePath_frame1;

  //frame[2] = frame_right_vertical
  frame[2] = new HNode(NULL);
  frame[2]->obj_type=0;
  frame[2]->base=bar_radius;
  frame[2]->top=bar_radius;
  frame[2]->height=frame_right_vertical_len;
  frame[2]->slices=50;
  frame[2]->stacks=10;
  frame[2]->change_parameters(17,-1,0,-90,12,0);
  frame[2]->set_color(1,0,0);

  //frame upper right horizontal
  frame[3] = new HNode(NULL);
  frame[3]->obj_type=0;
  frame[3]->base=bar_radius;
  frame[3]->top=bar_radius;
  frame[3]->height=frame_upper_right_horizontal_len*3/8;
  frame[3]->slices=50;
  frame[3]->stacks=10;
  frame[3]->change_parameters(22,28,0,-90,135,0);
  frame[3]->set_color(1,0,0);
  node[1]->add_child(frame[3]);

  //axle
  axle = new HNode(NULL);
  axle->obj_type=0;
  axle->base=bar_radius;
  axle->top=bar_radius;
  axle->height=axle_length;
  axle->change_preparameters(0,14,-axle_length/2);
  axle->change_parameters(0,0,0,90,0,0);
  axle->set_color(0,0,0);
  frame[3]->add_child(axle);

  //rear wheel front upper
  frame[4] = new HNode(NULL);
  frame[4]->obj_type=0;
  frame[4]->base=bar_radius;
  frame[4]->top=bar_radius;
  frame[4]->height=(frame_upper_right_horizontal_len)*5/8;
  frame[4]->change_parameters(0,0,axle_length,-90,0,0);
  frame[4]->set_color(0.5,0.5,0.5);
  axle->add_child(frame[4]);

  //rear wheel back upper
  frame[5] = new HNode(NULL);
  frame[5]->obj_type=0;
  frame[5]->base=bar_radius;
  frame[5]->top=bar_radius;
  frame[5]->height=(frame_upper_right_horizontal_len)*5/8;
  frame[5]->change_parameters(0,0,0,-90,0,0);
  frame[5]->set_color(0.5,0.5,0.5);
  axle->add_child(frame[5]);

  //seat
  seat[0] = new HNode(NULL);
  seat[0]->obj_type=3;
  seat[0]->triangle_x1=30;
  seat[0]->triangle_y1=23;
  seat[0]->triangle_x2=30;
  seat[0]->triangle_y2=30;
  seat[0]->change_parameters(10,23,0,0,0,0);
  seat[0]->set_color(0,0,0);
  node[1]->add_child(seat[0]);

  seat[1] = new HNode(NULL);
  seat[1]->obj_type=3;
  seat[1]->triangle_x1=10;
  seat[1]->triangle_y1=26;
  seat[1]->triangle_x2=25;
  seat[1]->triangle_y2=26;
  seat[1]->change_parameters(10,23,0,0,0,0);
  seat[1]->set_color(0,0,0);
  node[1]->add_child(seat[1]);

//gear
  gear[0] = new HNode(NULL);
  gear[0]->obj_type=2;
  gear[0]->change_parameters(15,0,2+2.5,0,0,0);
  gear[0]->inRadius=outer_gear_inRadius;
  gear[0]->outRadius=6.5;
  gear[0]->nsides=10;
  gear[0]->rings=50;
  gear[0]->set_color(0.6, 0.6, 0.6);
  node[1]->add_child(gear[0]);
  node[1]->add_child(frame[1]);
  node[1]->add_child(frame[2]);

  gear[1] = new HNode(NULL);
  gear[1]->obj_type=2;
  gear[1]->set_color(0.6, 0.6, 0.6);
  gear[1]->inRadius=inner_gear_radius;
  gear[1]->outRadius=inner_gear_radius;
  gear[1]->nsides=10;
  gear[1]->rings=50;
  gear[0]->add_child(gear[1]);

  // spokes in gear
  for(int i=0;i<5;i++){
    gear_spokes[i] = new HNode(NULL);
    gear_spokes[i]->obj_type=3;
    gear_spokes[i]->triangle_x1=(6.5 - outer_gear_inRadius)*(cos(0.31416 + (1.26*i)));
    gear_spokes[i]->triangle_y1=(6.5 - outer_gear_inRadius)*(sin(0.31416 + (1.26*i)));
    gear_spokes[i]->triangle_x2 = 1.5*(sin(1.26*(i+2)));
    gear_spokes[i]->triangle_y2 = -(1.5*(cos(1.26*(i+2))));
    gear_spokes[i]->change_parameters((1.5*(sin(1.26*(i+1)))),(-(1.5*(cos(1.26*(i+1))))),1,0,0,0);
    gear_spokes[i]->set_color(0.6, 0.6, 0.6);
    gear[0]->add_child(gear_spokes[i]);
  }

//pedals
  //pedal rods
  pedal_rod_across = new HNode(NULL);
  pedal_rod_across->obj_type=0;
  pedal_rod_across->base=1;
  pedal_rod_across->top=1;
  pedal_rod_across->height=13;
  pedal_rod_across->slices=50;
  pedal_rod_across->stacks=10;
  pedal_rod_across->change_parameters(0, 0, -11, 0, 0, 0);
  pedal_rod_across->set_color(0.5,0.5,0.5);
  gear[0]->add_child(pedal_rod_across);

  pedal_rod[0] = new HNode(NULL);
  pedal_rod[0]->obj_type=0;
  pedal_rod[0]->base=1;
  pedal_rod[0]->top=1;
  pedal_rod[0]->height=15;
  pedal_rod[0]->slices=50;
  pedal_rod[0]->stacks=10;
  pedal_rod[0]->change_parameters(10, 10, 0, -90, -135, 0);
  pedal_rod[0]->set_color(0.5,0.5,0.5);
  pedal_rod_across->add_child(pedal_rod[0]);

  pedal_rod[1] = new HNode(NULL);
  pedal_rod[1]->obj_type=0;
  pedal_rod[1]->base=1;
  pedal_rod[1]->top=1;
  pedal_rod[1]->height=15;
  pedal_rod[1]->slices=50;
  pedal_rod[1]->stacks=10;
  pedal_rod[1]->change_parameters(-10, -10, 13, -90, 45, 0);
  pedal_rod[1]->set_color(0.5,0.5,0.5);
  pedal_rod_across->add_child(pedal_rod[1]);

  //pedal cuboids
  pedal_cuboid[0] = new HNode(NULL);
  pedal_cuboid[0]->obj_type=1;
  pedal_cuboid[0]->breadth=7;
  pedal_cuboid[0]->height=4;
  pedal_cuboid[0]->length=3.5;
  pedal_cuboid[0]->set_color(0.3,0.3,0.3);
  pedal_cuboid[0]->change_preparameters(-pedal_cuboid[0]->length/2,0,-pedal_cuboid[0]->breadth/2);
  pedal_cuboid[0]->change_parameters(0,1,0,0,45-gear[0]->rz,0);  //1 is pedal_rod radius
  pedal_rod[0]->add_child(pedal_cuboid[0]);       //backside pedal

  pedal_cuboid[1] = new HNode(NULL);
  pedal_cuboid[1]->obj_type=1;
  pedal_cuboid[1]->breadth=7;
  pedal_cuboid[1]->height=4;
  pedal_cuboid[1]->length=3.5;
  pedal_cuboid[1]->set_color(0.3,0.3,0.3);
  pedal_cuboid[1]->change_preparameters(-pedal_cuboid[1]->length/2,0,-pedal_cuboid[1]->breadth/2);
  pedal_cuboid[1]->change_parameters(0,-pedal_cuboid[1]->height - 1,0,0,45-gear[0]->rz,0);
  pedal_rod[1]->add_child(pedal_cuboid[1]);       //frontside pedal

  // //rod across gear
  // across_gear = new HNode(NULL);
  // across_gear->obj_type=0;
  // across_gear->base=bar_radius;
  // across_gear->top=bar_radius;
  // across_gear->height=across_gear_length;
  // across_gear->change_parameters(0,0,-2,0,0,0);
  // across_gear->set_color(0.3,0.3,0.3);
  // gear[1]->add_child(across_gear);
  // wheel_axis_f->change_parameters(0,0,-wheel_axis_length/2,0,0,0);

//frame lower right horizontal -- front
  frame[6] = new HNode(NULL);
  frame[6]->obj_type=0;
  frame[6]->base=bar_radius/2;
  frame[6]->top=bar_radius;
  frame[6]->height=frame_lower_right_horizontal_len;
  frame[6]->change_parameters(15,0,across_gear_length-bar_radius,0,90-4,0);
  frame[6]->set_color(1,0,0);
  node[1]->add_child(frame[6]);

  //frame lower right horizontal -- back
  frame[7] = new HNode(NULL);
  frame[7]->obj_type=0;
  frame[7]->base=bar_radius/2;
  frame[7]->top=bar_radius;
  frame[7]->height=frame_lower_right_horizontal_len;
  frame[7]->change_parameters(15,0,-across_gear_length+bar_radius,0,90+4,0);
  frame[7]->set_color(1,0,0);
  node[1]->add_child(frame[7]);

//
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(512,512);
  glutCreateWindow("Hierarchical Modeling");
  glutDisplayFunc(display);
  glutKeyboardFunc(processNormalKeys);
  glutSpecialFunc(processSpecialKeys);
  init();
  glutTimerFunc(40, update_dynamic_variables, 0);
  glutIdleFunc(replay_animation);
  glutMainLoop();
  return 0;
}