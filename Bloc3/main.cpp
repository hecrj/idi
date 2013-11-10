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
Camera* thirdPerson;
Camera* firstPerson;
Scene* scene;
StateMachine* states;
PerspectiveLens* perspective = new PerspectiveLens();
OrthogonalLens* orthogonal = new OrthogonalLens();
FirstPersonLens* perspectiveFirstPerson = new FirstPersonLens();

void refresh()
{
    engine->render();
}

void reshape(int width, int height)
{
    engine->reshape(width, height);
}

void mousePressed(int buttonId, int state, int x, int y)
{
    states->getCurrentState()->mousePressed(buttonId, state, x, y);
}

void mouseMotion(int x, int y)
{
    states->getCurrentState()->mouseMotion(x, y);
}

void keyUp(unsigned char key, int x, int y)
{
    engine->trigger(key);
    states->keyUp(key, x, y);
}

void keyDown(unsigned char key, int x, int y)
{
    states->keyDown(key, x, y);
}

void idle()
{
    states->idle();
}

void createScene()
{
    scene = new Scene();
    
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
    Wall* wall2 = new Wall(-4.9, 0, 0, 0.2, 1.5, 10);
    wall1->setColor(0.8, 0.8, 0.8);
    wall2->setColor(0.8, 0.8, 0.8);
    
    scene->addObject("plane", plane);
    scene->addObject("snowman1", snowman1);
    scene->addObject("snowman2", snowman2);
    scene->addObject("snowman3", snowman3);
    scene->addObject("snowman4", snowman4);
    scene->addObject("wall1", wall1);
    scene->addObject("wall2", wall2);
}

void configureStates()
{
    states = new StateMachine();
    
    // TOOLS
    InspectTool* inspector = new InspectTool();
    inspector->add(thirdPerson);
    inspector->addToMouseRotation(thirdPerson);
    
    NavigationTool* navigator = new NavigationTool();
    navigator->addToMouseRotation(firstPerson);

    // Add tools
    states->add('r', inspector);
    states->add('f', navigator);
}

void configureCallbacks()
{
    glutDisplayFunc(refresh);
    glutReshapeFunc(reshape);
    glutMouseFunc(mousePressed);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutIdleFunc(idle);
}

int main(int argc, char** argv)
{   
    // Viewport
    Viewport* viewport = new Viewport("IDI Laboratory - Block 3");
    
    // Create cameras
    thirdPerson = new Camera(viewport, perspective);
    firstPerson = new Camera(viewport, perspectiveFirstPerson);
    firstPerson->translate(0, 1, 0);
    
    // Create scene
    createScene();
    
    // Configure actions and states
    configureStates();
    
    // Create the engine
    engine = new Engine();
    
    // Add cameras
    engine->addCamera('r', thirdPerson);
    engine->addCamera('f', firstPerson);
    
    // Initialize glut
    engine->init(&argc, argv);
    viewport->init();
    
    // Configure GLUT
    configureCallbacks();
    
    // Focus the scene
    thirdPerson->focus(scene);
    firstPerson->focus(scene);
    
    // Start rendering!
    engine->loop();
    
    return 0;
}
