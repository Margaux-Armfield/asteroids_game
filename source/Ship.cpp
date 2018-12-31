//
//  Ship.cpp
//  Asteroids
//
//  Created by Brian Summa on 6/5/15.
//
//

#include "common.h"

//Ship constructor
Ship::Ship(){
    //Set up initial state here
    state.velocity = vec2(0,0);
    state.cur_location = vec2(0,0);
    state.acceleration = vec2(0,0);
    state.pointing = vec2(0,-1);

};

//Called everytime an animation tick happens
void Ship::update_state(){
    // Things to do:
    //a = F

    //Force is in the direction the ship is pointing

    if (state.thruster_on) {
        state.acceleration = _ACC*state.pointing;
        //v  = old_velocity + a*dt
        state.velocity += state.acceleration;
    }



    //Clamp velocity at some maximum value

    if (state.velocity.x > _MAX_SPEED) {
        state.velocity.x = _MAX_SPEED;
    }

    // Dampen the velocity at every timestep to lessen intertia
        state.velocity = state.velocity*_DAMPING;

    //p  = old_position + v*dt
    state.cur_location += state.velocity;
    // Wrap the ship position when at the boundary

    if (state.cur_location.x >= 1 or state.cur_location.x <= -1) {
        state.cur_location.x = state.cur_location.x * -1;
    }

    if (state.cur_location.y >= 1 or state.cur_location.y <= -1) {
        state.cur_location.y = state.cur_location.y * -1;
    }

}

//Initialize the gl state and variables
void Ship::gl_init(){
    //Ship



    int i;
    for (i = 0; i<4; i++) {
        ship_color[i] = vec3(1,1,0);
    }
    ship_color[4] = vec3(1,1,0);
    for (i = 5; i<7; i++) {
        ship_color[i] = vec3(1,0,0);
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
    glBufferData( GL_ARRAY_BUFFER, sizeof(ship_vert) + sizeof(ship_color), NULL, GL_STATIC_DRAW );
    //First part of array holds vertices
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(ship_vert), ship_vert );
    //Second part of array hold colors

    glBufferSubData( GL_ARRAY_BUFFER, sizeof(ship_vert), sizeof(ship_color), ship_color );

    glEnableVertexAttribArray(GLvars.vpos_location);
    glEnableVertexAttribArray(GLvars.vcolor_location );

    glVertexAttribPointer( GLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    glVertexAttribPointer( GLvars.vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(ship_vert)) );

    glBindVertexArray(0);


}

//Draw a ship
void Ship::draw(mat4 proj){

    glUseProgram( GLvars.program );
    glBindVertexArray( GLvars.vao );

    mat4 M = Translate(state.cur_location.x, state.cur_location.y, 0.0)*RotateZ(state.angle)*RotateZ(180)*Scale(0.3, 0.3, 1);

    //If you have a modelview matrix, pass it with proj
    glUniformMatrix4fv( GLvars.M_location, 1, GL_TRUE, proj*M );

    //Draw something

    glDrawArrays(GL_LINE_LOOP, 0, 4);

    if(state.thruster_on){

//        //Draw something

        glDrawArrays(GL_TRIANGLES, 4, 3);
        glBindVertexArray(0);
        glUseProgram(0);


    }

    
}

vec2 Ship::getLocation(){
    return state.cur_location;
}

vec2 Ship::getVelocity(){
    return state.velocity;
}
vec2 Ship::getPointing(){
    return state.pointing;
}
Bullet* Ship::fire(){
    return new Bullet(state.velocity, state.cur_location, state.pointing);
 };
