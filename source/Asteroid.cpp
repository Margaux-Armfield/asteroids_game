//
//  Asteroid.cpp
//  asteroids
//
//  Created by Margaux Armfield on 9/16/18.
//

#include "Asteroid.h"

#include "common.h"

//asteroid constructor
Asteroid::Asteroid(float x, vec2 startingPoint){
    //Set up initial state here
    ast_state.velocity=   normalize(vec2((rand() % 21 - 10), (rand() % 21 - 10))) * 0.002;
    ast_state.cur_location = startingPoint;
    //normalize(vec2((rand() % 21 - 10), (rand() % 21 - 10)));
    ast_state.scale = x;
};

//Called everytime an animation tick happens
void Asteroid::update_state(){
    
    Asteroid::rotateLeft();
    
    //p  = old_position + v*dt
    ast_state.cur_location += ast_state.velocity;
    if (ast_state.cur_location.x >= 1.1 or ast_state.cur_location.x <= -1.1) {
        ast_state.cur_location.x = ast_state.cur_location.x * -0.99;
        ast_state.cur_location.y = (rand() % 21 - 10)/10;
    }
    if (ast_state.cur_location.y >= 1.1 or ast_state.cur_location.y <= -1.1) {
        ast_state.cur_location.y = ast_state.cur_location.y * -0.99;
        ast_state.cur_location.x = (rand() % 21 - 10)/10;
    }
    
}

//Initialize the gl state and variables
void Asteroid::gl_init(){
    
    int i;
    for (i = 0; i<12; i++) {
        ast_color[i] = vec3(1,0,1);
    }
    
    
    
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
    glBufferData( GL_ARRAY_BUFFER, sizeof(ast_vert) + sizeof(ast_color), NULL, GL_STATIC_DRAW );
    //First part of array holds vertices
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(ast_vert), ast_vert );
    //Second part of array hold colors (offset by sizeof(triangle))
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(ast_vert), sizeof(ast_color), ast_color );
    
    glEnableVertexAttribArray(GLvars.vpos_location);
    glEnableVertexAttribArray(GLvars.vcolor_location );
    
    glVertexAttribPointer( GLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    glVertexAttribPointer( GLvars.vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(ast_vert)) );
    
    glBindVertexArray(0);
    
}



//Draw an asteroid
void Asteroid::draw(mat4 proj){
    
    glUseProgram( GLvars.program );
    glBindVertexArray( GLvars.vao );
    
    
    mat4 M = Translate(ast_state.cur_location.x, ast_state.cur_location.y, 0.0)*RotateZ(ast_state.angle)*Scale(ast_state.scale, ast_state.scale, 1);
    //If you have a modelview matrix, pass it with proj
    
    glUniformMatrix4fv( GLvars.M_location, 1, GL_TRUE, proj*M );
    
    //Draw something
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, 11);
    
    
    glBindVertexArray(0);
    glUseProgram(0);
    
}
