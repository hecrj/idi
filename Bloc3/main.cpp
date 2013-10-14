#include "simplegl/Engine.h"
#include "simplegl/StateMachine.h"
#include "simplegl/tools/RotationTool.h"
#include "simplegl/tools/MoveTool.h"
#include "simplegl/objects/Snowman.h"
#include "simplegl/objects/Plane.h"
#include "simplegl/objects/Model.h"

#if defined(__APPLE__)
  #include <OpenGL/OpenGl.h>
  #include <GLUT/GLUT.h>
#else
  #include <GL/gl.h>
  #include <GL/freeglut.h>
#endif

using namespace std;

Engine* engine;
StateMachine* states;

void refresh()
{
    engine->draw();
}

void reshape(int width, int height)
{
    engine->getCamera()->reshape(width, height);
}

void mousePressed(int buttonId, int state, int x, int y)
{
    states->getCurrentState()->mousePressed(buttonId, state, x, y);
}

void mouseMotion(int x, int y)
{
    states->getCurrentState()->mouseMotion(x, y);
}

void keyPressed(unsigned char key, int x, int y)
{
    if(key == 'h')
        states->printHelp();
    else
        states->trigger(key);
}

int main(int argc, char** argv)
{
    // ENGINES
    engine = new Engine("IDI Laboratory - Block 2");
    states = new StateMachine();
    
    // MAIN OBJECTS
    // Plane
    Plane* plane = new Plane(0, -0.4, 0, 1.5);
    plane->setColor(0.8, 0.8, 0.8);
        
    // Snowman
    Snowman* snowman = new Snowman(0, 0, 0);
    
    // Legoman
    Model* legoman = new Model();
    legoman->load("../legoman.obj");

    double scaleLego = 0.5 / legoman->getHeight();
    legoman->scale(scaleLego, scaleLego, scaleLego); // Scale the legoman
    
    // Positionate the legoman
    legoman->translate(0.75 - legoman->getRFBX(), -0.4 - legoman->getRFBY(), 0.75 - legoman->getRFBZ());
    
    // TOOLS
    // Rotation tool
    RotationTool* rotator = new RotationTool();
    rotator->add(engine); // Rotates the whole engine
    
    // Move tool
    MoveTool* mover = new MoveTool(engine->getCamera()->getViewport());
    mover->add(legoman); // Move the legoman
    
    // Add tools
    states->add('r', rotator); // Enable rotation when r is pressed, by default
    states->add('c', mover); // Enable mover when c is pressed
    
    // Initialize engine
    engine->init(&argc, argv);
    
    // Place the objects in the engine
    engine->addObject("plane", plane);
    engine->addObject("snowman", snowman);
    engine->addObject("legoman", legoman);
    
    // Configure GLUT
    glutDisplayFunc(refresh);
    glutReshapeFunc(reshape);
    glutMouseFunc(mousePressed);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyPressed);
    
    // Start loop!
    engine->focus(1);
    engine->loop();
    
    return 0;
}
