//
//  Bullet.h
//  asteroids
//
//  Created by Margaux Armfield on 10/5/18.
//

#ifndef Bullet_hpp
#define Bullet_hpp

#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#include <list>
#include <vector>
#include "utils/Angel.h"

using namespace Angel;

#include "shader.h"


class Bullet{
     public:
    vec2 bull_vert[1];
    vec3 bull_color[1];
    

    //Record of the bullet's state
    struct {
        vec2 cur_location;   //Current position of the center
        vec2 velocity;       //Velocity
        vec2 direction;
    } bullet_state;
    
    Bullet(vec2 velocity, vec2 position, vec2 direction);
    

    
    //OpenGL variables for a bullet
    struct {
        GLuint vao;           //Vertex array object
        GLuint program;       //shader program
        GLuint buffer;        //Vertex buffer objects
        GLuint vertex_shader, fragment_shader;  //Shaders
        GLint vpos_location, vcolor_location;   //reference to pos and color in shaders
        GLint M_location;     //Reference to matrix in shader
    } GLvars;
    

//    Bullets(){};
////    ~Bullets(){
////        for(unsigned int i=0; i < bullets.size(); i++)
////            delete b[i];
////        particles.clear();
////    }
    void update_state();
    void gl_init();
    void draw(mat4 proj);
    
    
};

#endif /* Bullet_hpp */
