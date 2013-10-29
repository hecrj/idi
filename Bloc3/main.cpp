#include "simplegl/Scene.h"
#include "simplegl/StateMachine.h"
#include "simplegl/tools/NavigationTool.h"
#include "simplegl/tools/MoveTool.h"
#include "simplegl/objects/Snowman.h"
#include "simplegl/objects/Plane.h"
#include "simplegl/objects/Model.h"
#include "simplegl/objects/Wall.h"
#include "simplegl/Engine.h"
#include "simplegl/lens/OrthogonalLens.h"
#include "simplegl/lens/PerspectiveLens.h"
#include "simplegl/lens/FirstPersonLens.h"

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

void refreshPerspective()
{
    perspective->render();
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

void keyDown(unsigned char key, int x, int y)
{
    states->keyDown(key, x, y);
}

void idle()
{
    states->idle();
}

void keyUp(unsigned char key, int x, int y)
{
    states->keyUp(key, x, y);
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
    // Floor
    Plane* plane = new Plane(0, 0, 0, 10);
    plane->setColor(0.5, 0.5, 0.5);
    
    // Snowmans
    Snowman* snowman1 = new Snowman(2.5, 0, 2.5);
    Snowman* snowman2 = new Snowman(-2.5, 0, 2.5);
    Snowman* snowman3 = new Snowman(-2.5, 0, -2.5);
    Snowman* snowman4 = new Snowman(2.5, 0, -2.5);
    
    // Walls
    Wall* wall1 = new Wall(2.5, 0, 1.5, 4, 1.5, 0.2);
    wall1->setColor(0.8, 0.8, 0.8);
    
    Wall* wall2 = new Wall(-4.9, 0, 0, 0.2, 1.5, 10);
    wall2->setColor(0.8, 0.8, 0.8);
    
    // Place the objects in the engine
    scene->addObject("plane", plane);
    scene->addObject("snowman1", snowman1);
    scene->addObject("snowman2", snowman2);
    scene->addObject("snowman3", snowman3);
    scene->addObject("snowman4", snowman4);
    scene->addObject("wall1", wall1);
    scene->addObject("wall2", wall2);
    
    // TOOLS
    // Rotation tool
    NavigationTool* navigator = new NavigationTool();
    navigator->add(ortogonal);
    navigator->add(perspective);

    // Add tools
    states->add('r', navigator); // Enable rotation when r is pressed, by default
    
    // Initialize glut
    engine->init(&argc, argv);
    
    // Initialize first camera
    perspective->init();
    perspective->setLens(new FirstPersonLens());
    perspective->translate(0, 1, 0);
    
    // Configure GLUT
    glutDisplayFunc(refreshPerspective);
    glutReshapeFunc(reshapePerspective);
    glutMouseFunc(mousePressed);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutIdleFunc(idle);
    
    perspective->focus(scene);
    
    // Start rendering!
    engine->loop();
    
    return 0;
}
