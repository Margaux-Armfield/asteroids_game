//
//  Asteroid.h
//  asteroids
//
//  Created by Margaux Armfield on 9/16/18.
//

#ifndef Asteroid_h
#define Asteroid_h

#include <stdio.h>
#include "common.h"


class Asteroid{
 public:
    //Placeholders so everything compiles.  Customize for your ship
    vec2 ast_vert[11]    ={
        vec2(0.0,0),
        vec2(0.044,  0.095),
        vec2(-0.042, 0.062),
        vec2(-0.086, -0.028),
        vec2(-0.004, -0.080),
        vec2(0.049,  -0.046),
        vec2(0.018, -0.014),
        vec2(0.019, 0.005),
        vec2(0.08, -0.018),
        vec2(0.090, 0.012),
        vec2(0.044,  0.095),
    };
    
    vec3 ast_color[11];
    

    
//   Record of the asteroid's state

//
    //OpenGL variables for a ship
    struct {
        GLuint vao;           //Vertex array object
        GLuint program;       //shader program
        GLuint buffer;        //Vertex buffer objects
        GLuint vertex_shader, fragment_shader;  //Shaders
        GLint vpos_location, vcolor_location;   //reference to pos and color in shaders
        GLint M_location;     //Reference to matrix in shader
    } GLvars;
    
    struct {
        vec2 cur_location;   //Current position of the center
        vec2 velocity;       //Velocity
        float angle;
        float scale;
    } ast_state;
    

    Asteroid(float x, vec2 startingPoint);
    
    inline void rotateLeft() {
        ast_state.angle+=1;
    }
    void update_state();
    
    void gl_init();
    
    void draw(mat4 proj);
    
    
    };

#endif /* Asteroid_h */
