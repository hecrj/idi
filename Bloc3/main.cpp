#include "simplegl/Scene.h"
#include "simplegl/StateMachine.h"
#include "simplegl/tools/RotationTool.h"
#include "simplegl/tools/MoveTool.h"
#include "simplegl/objects/Snowman.h"
#include "simplegl/objects/Plane.h"
#include "simplegl/objects/Model.h"
#include "simplegl/Engine.h"
#include "simplegl/lens/OrthogonalLens.h"
#include "simplegl/lens/PerspectiveLens.h"

#if defined(__APPLE__)
  #include <OpenGL/OpenGl.h>
  #include <GLUT/GLUT.h>
#else
  #include <GL/gl.h>
  #include <GL/freeglut.h>
#endif

using namespace std;

Engine* engine;
Camera* ortogonal;
Camera* perspective;
Scene* scene;
StateMachine* states;

Lens* lens[] = { new OrthogonalLens(), new PerspectiveLens() };
int activeLens = 0;


void refreshPerspective()
{
    perspective->render();
}

void refreshOrtogonal()
{
   ortogonal->render();
   perspective->redisplay();
}

void reshapeOrtogonal(int width, int height)
{
    ortogonal->reshape(width, height);
}

void reshapePerspective(int width, int height)
{
    perspective->reshape(width, height);
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
    engine = new Engine();
    
    // CAMERAS
    ortogonal = new Camera(new Viewport("IDI Laboratory - Block 3 - Orthogonal view"));
    perspective = new Camera(new Viewport("IDI Laboratory - Block 3 - Perspective view"));
    
    // SCENE
    scene = new Scene();
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
    
    // Place the objects in the engine
    scene->addObject("plane", plane);
    scene->addObject("snowman", snowman);
    scene->addObject("legoman", legoman);
    
    // TOOLS
    // Rotation tool
    RotationTool* rotator = new RotationTool();
    rotator->add(ortogonal);
    rotator->add(perspective);
    
    // Move tool
    MoveTool* mover = new MoveTool(ortogonal->getViewport());
    mover->add(legoman); // Move the legoman
    
    // Add tools
    states->add('r', rotator); // Enable rotation when r is pressed, by default
    states->add('c', mover); // Enable mover when c is pressed
    
    // Initialize glut
    engine->init(&argc, argv);
    
    // Initialize first camera
    ortogonal->init();
    
    // Configure GLUT
    glutDisplayFunc(refreshOrtogonal);
    glutReshapeFunc(reshapeOrtogonal);
    glutMouseFunc(mousePressed);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyPressed);
    
    // Initialize second camera
    perspective->init();
    perspective->setLens(new PerspectiveLens());
    
    glutDisplayFunc(refreshPerspective);
    glutReshapeFunc(reshapePerspective);
    
    // Focus the scene
    ortogonal->focus(scene, 2);
    perspective->focus(scene, 2);
    
    // Start rendering!
    engine->loop();
    
    return 0;
}
