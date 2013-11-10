#include "simplegl/Scene.h"
#include "simplegl/StateMachine.h"
#include "simplegl/states/ToggleScene.h"
#include "simplegl/states/ToggleLens.h"
#include "simplegl/tools/NavigationTool.h"
#include "simplegl/tools/MoveTool.h"
#include "simplegl/objects/Snowman.h"
#include "simplegl/objects/Plane.h"
#include "simplegl/objects/Model.h"
#include "simplegl/objects/Wall.h"
#include "simplegl/objects/Player.h"
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
StateMachine* states;
Scene* scene;
Scene* walls;
Camera* thirdPerson;
Camera* firstPerson;
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
    plane->setColor(0.6, 0.7, 1);
    
    // Snowmans
    Snowman* snowman1 = new Snowman(2.5, 0, 2.5);
    Snowman* snowman2 = new Snowman(-2.5, 0, 2.5);
    Snowman* snowman3 = new Snowman(-2.5, 0, -2.5);
    Snowman* snowman4 = new Snowman(2.5, 0, -2.5);
    
    // Walls
    walls = new Scene();
    Wall* wall1 = new Wall(2.5, 0, 1.5, 4, 1.5, 0.2);
    Wall* wall2 = new Wall(-4.9, 0, 0, 0.2, 1.5, 10);
    wall1->setColor(0.8, 0.8, 0.8);
    wall2->setColor(0.8, 0.8, 0.8);
    
    walls->addObject("wall1", wall1);
    walls->addObject("wall2", wall2);
    
    scene->addObject("plane", plane);
    scene->addObject("snowman1", snowman1);
    scene->addObject("snowman2", snowman2);
    scene->addObject("snowman3", snowman3);
    scene->addObject("snowman4", snowman4);
    scene->addObject("walls", walls);
}

void configureStates()
{
    states = new StateMachine();
    
    // TOOLS
    InspectTool* inspector = new InspectTool(engine);
    
    ToggleScene* wallsToggle = new ToggleScene("walls", scene, walls);
    ToggleLens* lensToggle = new ToggleLens(thirdPerson);
    
    lensToggle->addLens(perspective);
    lensToggle->addLens(orthogonal);

    // Add tools
    states->add('r', inspector);
    states->add('v', wallsToggle);
    states->add('p', lensToggle);
    
    // Car
    Player* car = new Player("porsche.obj", firstPerson);
    car->positionateBottomCenter(0, 0, 0);
    
    NavigationTool* navigator = new NavigationTool();
    navigator->add(car);
    
    scene->addObject("car", car);
    states->setGlobal(navigator);
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
    // Create the engine
    engine = new Engine();
    
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
