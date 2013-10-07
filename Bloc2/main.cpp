#include "simplegl/Engine.h"
#include "simplegl/StateMachine.h"
#include "simplegl/tools/RotationTool.h"
#include "simplegl/tools/MoveTool.h"
#include "simplegl/objects/Snowman.h"
#include "simplegl/objects/Plane.h"

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
    engine->getWindow()->reshape(width, height);
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
    engine = new Engine("IDI Laboratory - Block 2");
    states = new StateMachine();
    RotationTool* rotator = new RotationTool();
    MoveTool* mover = new MoveTool(engine->getWindow());
    
    rotator->add(engine);
    
    states->add('r', rotator);
    states->add('m', mover);
    
    engine->init(&argc, argv);
    
    Plane* plane = new Plane(0, -0.4, 0, 1.5);
    plane->setColor(0.8, 0.8, 0.8);
    
    engine->addObject("plane", plane);
    engine->addObject("snowman", new Snowman(0, 0, 0));
    
    glutDisplayFunc(refresh);
    glutReshapeFunc(reshape);
    glutMouseFunc(mousePressed);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyPressed);
    
    engine->loop();
    
    return 0;
}
