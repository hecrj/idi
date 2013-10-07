#include "simplegl/Engine.h"
#include "simplegl/objects/Teapot.h"
#include "simplegl/StateMachine.h"
#include "simplegl/tools/RotationTool.h"
#include "simplegl/tools/MoveTool.h"
#include "simplegl/objects/Sphere.h"
#include "simplegl/objects/Cone.h"

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
    engine = new Engine("Laboratori IDI - Bloc 2");
    states = new StateMachine();
    RotationTool* rotator = new RotationTool();
    MoveTool* mover = new MoveTool(engine->getWindow());
    
    rotator->add(engine);
    
    states->add('r', rotator);
    states->add('m', mover);
    
    engine->init(&argc, argv);
    engine->addObject("sphere1", new Sphere(0, 0, 0, 0.4));
    engine->addObject("sphere2", new Sphere(0, 0.6, 0, 0.2));
    
    Cone* cone = new Cone(0.1, 0.6, 0, 0.1, 0.2);
    cone->rotate(0, 90, 0);
    engine->addObject("cone", cone);
    
    glutDisplayFunc(refresh);
    glutReshapeFunc(reshape);
    glutMouseFunc(mousePressed);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyPressed);
    
    engine->loop();
    
    return 0;
}
