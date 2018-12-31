
#include "common.h"

using namespace Angel;

enum{_GAME, _GAME_OVER, _NUM_OBJ, };
int current_object;


Ship ship;
std::vector < Asteroid * > asteroids;

std::vector < Bullet * > bullets;


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (current_object ==1) {
        return;
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        ship.rotateLeft();
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        ship.rotateRight();
    if (key == GLFW_KEY_SPACE){
        if(action == GLFW_PRESS){
            ship.start_thruster();
        }
        if(action == GLFW_RELEASE){
            ship.stop_thruster();
        }
    }
    if (key == GLFW_KEY_Z && action == GLFW_PRESS){
        //!!!!!!!!Fire bullet
        bullets.push_back(ship.fire());
        for ( int i=0; i<bullets.size(); ++i ) {
            bullets[i]->gl_init();
        }
        
    }
}


void init(){
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glPointSize(10.0);
    glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
    ship.gl_init();
    asteroids.resize(5);
    for ( int i=0; i<asteroids.size(); ++i ) {
        
      asteroids[i] = new Asteroid(1.0, normalize(vec2((rand() % 21 - 10), (rand() % 21 - 10))));
        asteroids[i]->gl_init();
    }
    
    
    
}



bool PointInPoly(vec2 polygon[11], vec2 point, int vertices){
    //polygon->resize(vertices);
    bool inside = false; 
    
    int i;
    vec2 p1; 
    vec2 p2;
    
    for (i = 0; i<vertices-1; i++){  // iterate through each edge
        p1 = polygon[i]; //get first edge endpoint from polygon
        p2 = polygon[i+1]; // get second edge endpoint from polygon 
        
        if (p1.y > p2.y) {
            vec2 temp;
            temp = p1;
            p1 = p2;
            p2 = temp;
        }
            //want p1 to p2 to point in +y direction if (point[1] > p1[1]){ //above lower edge endpoint
        if (point.y > p1.y) {
            
            if (point.y <= p2.y){ //below upper edge endpoint
                if (p1.y != p2.y){  //if edge is not horizontal do half-plane check
                    
                    // z of crossproduct(point-p1, p2-p1) > 0) means we’re to the right
                    if ((point.x - p1.x) * (p2.y - p1.y) - ((p2.x - p1.x) * (point.y - p1.y)) > 0) inside = !inside;
                    // if true, point is to right of edge (and thus in the “strip”)
                }
            }
        }
        }
        return inside;
} 

vec2 shipPoints [5];
vec2 astPoints [12];

void collisionTest(Ship ship, Asteroid* asteroid){
    // This function takes the vertices of the ship and an asteroid, makes new arrays of their vertices translated to where they should be based on the current location and uses Point in poly to test for collision. If there is a collision, current object is increasd so that the Game Over case is activated.
    
    int i;
    for (i=0; i<4; i++) {
        shipPoints[i] = ship.ship_vert[i]*0.3 + ship.state.cur_location;
    }
    shipPoints[4] = shipPoints[0];
    
    for (i= 0; i<11; i++) {
        astPoints[i] = asteroid->ast_vert[i]*asteroid->ast_state.scale + asteroid->ast_state.cur_location;
    }
    astPoints[11] = astPoints[0];
    for (i= 0; i<12; i++) {
        if (PointInPoly(shipPoints, astPoints[i], 5) == true) {
            current_object = 1;
        }
    }
    
}
void animate(){
    
    int j;
    int t;
    
    if(glfwGetTime() > 0.0167){
        glfwSetTime(0.0);
        ship.update_state();
        
        for ( int j = bullets.size()-1; j>= 0; --j ) {
            bullets[j]->update_state();
        }
        for ( int i= asteroids.size()-1; i>= 0; --i ) {
            asteroids[i]->update_state();
            
            // Test
            
            collisionTest(ship, asteroids[i]);
            
            for ( int j = bullets.size()-1; j>= 0; --j ) {
                vec2 poly[12];
                
                for ( int f=0; f<11; ++f ) {
                    poly[f] = asteroids[i]->ast_vert[f]*asteroids[i]->ast_state.scale + asteroids[i]->ast_state.cur_location;
                }
                poly[11] = poly[0];
                
                if (PointInPoly(poly, bullets[j]->bullet_state.cur_location, 12) == true) {
                    
                    // Add 3 new asteroids
                    if (asteroids[i]->ast_state.scale > 0.5) {
                   
                    asteroids.push_back(new Asteroid(0.5, asteroids[i]->ast_state.cur_location));
                    asteroids[asteroids.size()-1]->gl_init();
                    asteroids.push_back(new Asteroid(0.5, asteroids[i]->ast_state.cur_location));
                    asteroids[asteroids.size()-1]->gl_init();
                    asteroids.push_back(new Asteroid(0.5, asteroids[i]->ast_state.cur_location));
                    asteroids[asteroids.size()-1]->gl_init();
                    std::cout<<asteroids.size();
                 }
                    // Delete asteroid and bullet that collided with asteroid
                    asteroids.erase(asteroids.begin()+i);
                    bullets.erase(bullets.begin()+j);
                }
   
            }
            
        }
        
    }
}
//Call update function 30 times a second
void animate1(){
  
    int j;
    int t;
    
    if(glfwGetTime() > 0.0167){
        glfwSetTime(0.0);
       
        ship.update_state();
    
        for ( int i= asteroids.size()-1; i>= 0; --i ) {
            asteroids[i]->update_state();
            
            // Test 
            
            //collisionTest(ship, asteroids[i]);
        }
            for ( int j = bullets.size()-1; j>= 0; --j ) {
                bullets[j]->update_state();
            }
    
}
}



int main(void)
{
    srand( time (NULL));
    
    GLFWwindow* window;
    
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit())
        exit(EXIT_FAILURE);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    glfwWindowHint(GLFW_SAMPLES, 10);
    
    
    window = glfwCreateWindow(1024, 768, "Asteroids!", NULL, NULL);
    if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwSetKeyCallback(window, key_callback);
    
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);
    
    glClearColor(1.0, 1.0, 1.0, 1.0);
    
    init();
    
    while (!glfwWindowShouldClose(window)){
        
        
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        
        //Pick a coordinate system that makes the most sense to you
        //(left, right, top, bottom)
        
        mat4 proj = Ortho2D(-1.0, 1.0, 1.0, -1.0);
        
        
        
        
        switch(current_object){
                
            case _GAME:
                glClear(GL_COLOR_BUFFER_BIT);
                animate();
                ship.draw(proj);
                for ( int i=0; i<asteroids.size(); ++i ) {
                    asteroids[i]->draw(proj);
                }
                for ( int i=0; i<bullets.size(); ++i ) {
                    bullets[i]->draw(proj);
                    
                }
                
                
                //
                break;
                
            case _GAME_OVER:
                glClear(GL_COLOR_BUFFER_BIT);

                glClearColor((rand()/(float)RAND_MAX), (rand()/(float)RAND_MAX), (rand()/(float)RAND_MAX), (rand()/(float)RAND_MAX));
                
                ship.draw(proj);
            
                for ( int i=0; i<asteroids.size(); ++i ) {
                    asteroids[i]->draw(proj);
                }
                for ( int i=0; i<bullets.size(); ++i ) {
                    bullets[i]->draw(proj);
                    
                }
                std::cout<<"GAME OVER";
                break;
                
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    glfwDestroyWindow(window);
    
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

