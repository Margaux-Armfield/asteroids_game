//
//  Bullet.cpp
//  asteroids
//
//  Created by Margaux Armfield on 10/5/18.
//

#include "Bullet.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#include <list>
#include <vector>
#include "utils/Angel.h"

using namespace Angel;

#include "shader.h"


Bullet::Bullet(vec2 velocity, vec2 position, vec2 direction){
    bullet_state.velocity = velocity + direction*.008;
    std::cout << "direction" << direction << "\n";
    bullet_state.cur_location = position;
    bullet_state.direction = direction;
//    bullet_state.color = vec3(1.0,0.0,1.0);
};


//Initialize the gl state and variables
void Bullet::gl_init(){
    
    bull_vert[0] = vec2(0, 0);
    bull_color[0] = vec3(1,1,0);
 
    
    
    std::string vshader = shader_path + "vshader_Ship.glsl";
    std::string fshader = shader_path + "fshader_Ship.glsl";
    
    GLchar* vertex_shader_source = readShaderSource(vshader.c_str());
    GLchar* fragment_shader_source = readShaderSource(fshader.c_str());
    
    GLvars.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(GLvars.vertex_shader, 1, (const GLchar**) &vertex_shader_source, NULL);
    glCompileShader(GLvars.vertex_shader);
    check_shader_compilation(vshader, GLvars.vertex_shader);
    
    GLvars.fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(GLvars.fragment_shader, 1, (const GLchar**) &fragment_shader_source, NULL);
    glCompileShader(GLvars.fragment_shader);
    check_shader_compilation(fshader, GLvars.fragment_shader);
    
    GLvars.program = glCreateProgram();
    glAttachShader(GLvars.program, GLvars.vertex_shader);
    glAttachShader(GLvars.program, GLvars.fragment_shader);
    
    glLinkProgram(GLvars.program);
    check_program_link(GLvars.program);
    
    glBindFragDataLocation(GLvars.program, 0, "fragColor");
    
    GLvars.vpos_location   = glGetAttribLocation(GLvars.program, "vPos");
    GLvars.vcolor_location = glGetAttribLocation(GLvars.program, "vColor" );
    GLvars.M_location = glGetUniformLocation(GLvars.program, "M" );
    
    // Create a vertex array object
    glGenVertexArrays( 1, &GLvars.vao );
    //Set GL state to use vertex array object
    glBindVertexArray( GLvars.vao );
    
    //Generate buffer to hold our vertex data
    glGenBuffers( 1, &GLvars.buffer );
    //Set GL state to use this buffer
    glBindBuffer( GL_ARRAY_BUFFER, GLvars.buffer );
    
    //Create GPU buffer to hold vertices and color
    glBufferData( GL_ARRAY_BUFFER, sizeof(bull_vert) + sizeof(bull_color), NULL, GL_STATIC_DRAW );
    //First part of array holds vertices
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(bull_vert), bull_vert );
    //Second part of array hold colors
    
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(bull_vert), sizeof(bull_color), bull_color );
    
    glEnableVertexAttribArray(GLvars.vpos_location);
    glEnableVertexAttribArray(GLvars.vcolor_location );
    
    glVertexAttribPointer( GLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    glVertexAttribPointer( GLvars.vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(bull_vert)) );
    
    glBindVertexArray(0);
    

};

void Bullet::update_state(){
        bullet_state.cur_location += bullet_state.velocity;
};

void Bullet::draw(mat4 proj){
      glPointSize(5.0);
    glUseProgram( GLvars.program );
    glBindVertexArray( GLvars.vao );
    mat4 M = Translate(bullet_state.cur_location.x, bullet_state.cur_location.y, 0.0);
    glUniformMatrix4fv( GLvars.M_location, 1, GL_TRUE, proj*M);
//    
//    //Draw something
//    
   glDrawArrays(GL_POINTS, 0, 1);
//    
//    
   glBindVertexArray(0);
    glUseProgram(0);
//    
};

