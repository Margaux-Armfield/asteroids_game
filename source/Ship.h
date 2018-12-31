//
//  Ship.h
//  Asteroids
//
//  Created by Brian Summa on 6/5/15.
//
//

#ifndef __Asteroids__Ship__
#define __Asteroids__Ship__

#include "common.h"

#include "Bullet.hpp"


#define _MAX_SPEED 0.45
#define _DAMPING 0.98
#define _ACC 0.0035
#define _ROT 35

class Ship{
public:
  //Placeholders so everything compiles.  Customize for your ship
    vec2 ship_vert[7]    = {vec2(0, 0.1),
        vec2( 0.17, -.20),
        vec2(0, -0.1),
        vec2(-0.17, -.20),
        vec2(0,-0.1),
        vec2(-0.07,-0.25),
        vec2(0.07,-0.25),};
    
  vec3 ship_color[7];
//    

  
  //Record of the ship's state
  struct {
    vec2 cur_location;   //Current position of the center
    float angle;         //Rotation angle
    vec2 pointing;       //Vector pointing to the front of the ship
    //This function will be helpful to keep track of the direction the ship
    //is pointing
    
      mat2 RotateZ2D( const GLfloat theta ){
      GLfloat angle = DegreesToRadians * theta;
      mat2 c;
      c[0][0] = c[1][1] = cos(angle);
      c[1][0] = sin(angle);
      c[0][1] = -c[1][0];
      return c;
      }
      
    vec2 velocity;       //Velocity
    vec2 acceleration;
    bool thruster_on;    //Boolean if a thruster is on
  } state;
  
  //OpenGL variables for a ship
  struct {
    GLuint vao;           //Vertex array object
    GLuint program;       //shader program
    GLuint buffer;        //Vertex buffer objects
    GLuint vertex_shader, fragment_shader;  //Shaders
    GLint vpos_location, vcolor_location;   //reference to pos and color in shaders
    GLint M_location;     //Reference to matrix in shader
  } GLvars;

  

  

  Ship();
  
  inline void start_thruster(){ state.thruster_on= true;}
  inline void stop_thruster() { state.thruster_on= false;}

  inline void rotateLeft() {
      state.angle-=_ROT;
      state.pointing = state.RotateZ2D(state.angle) * vec2(0.0, -1.0);
  }
  inline void rotateRight(){
      state.angle+=_ROT;
      state.pointing = state.RotateZ2D(state.angle) * vec2(0.0, -1.0);
  }
    
  void update_state();
  
  void gl_init();  
  
  void draw(mat4 proj);
    
vec2 getLocation();
    
vec2 getVelocity();

    vec2 getPointing();
    
    
Bullet * fire();

};


#endif /* defined(__Asteroids__Ship__) */
