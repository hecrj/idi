#include "simplegl/Scene.h"
#include "simplegl/StateMachine.h"
#include "simplegl/tools/NavigationTool.h"
#include "simplegl/tools/MoveTool.h"
#include "simplegl/objects/Snowman.h"
#include "simplegl/objects/Plane.h"
#include "simplegl/objects/Model.h"
#include "simplegl/objects/Wall.h"
#include "simplegl/objects/Player.h"
#include "simplegl/Engine.h"
#include "simplegl/Color.h"
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
Player* car;

Scene* createScene(Camera* firstPerson)
{
    Scene* scene = new Scene();
    
    // Floor
    Plane* plane = new Plane(0, 0, 0, 10);
    plane->setColor(Color::BLUE);
    
    // Snowmans
    Snowman* snowman1 = new Snowman(2.5, 0, 2.5);
    Snowman* snowman2 = new Snowman(-2.5, 0, 2.5);
    Snowman* snowman3 = new Snowman(-2.5, 0, -2.5);
    Snowman* snowman4 = new Snowman(2.5, 0, -2.5);
    
    // Walls
    Scene* walls = new Scene();
    Wall* wall1 = new Wall(2.5, 0, 1.5, 4, 1.5, 0.2);
    Wall* wall2 = new Wall(-4.9, 0, 0, 0.2, 1.5, 10);
    wall1->setColor(Color::GREEN);
    wall2->setColor(Color::GREEN);
    
    walls->addObject("wall1", wall1);
    walls->addObject("wall2", wall2);
    
    // Toggle walls
    engine->addToggle('v', walls, "Show/Hide walls");
    
    // Car
    car = new Player("porsche.obj", firstPerson);
    car->positionateBottomCenter(0, 0, 0);
    
    scene->addObject("plane", plane);
    scene->addObject("snowman1", snowman1);
    scene->addObject("snowman2", snowman2);
    scene->addObject("snowman3", snowman3);
    scene->addObject("snowman4", snowman4);
    scene->addObject("walls", walls);
    scene->addObject("car", car);
    
    return scene;
}

void configureStates(StateMachine* states)
{
    // TOOLS
    InspectTool* inspector = new InspectTool(engine);
    
    NavigationTool* navigator = new NavigationTool();
    navigator->add(car);
    
    // Add states
    states->setGlobal(inspector);
    states->add('r', navigator);
}

int main(int argc, char** argv)
{
    // Create the engine
    engine = new Engine();
    
    // Viewport
    Viewport* viewport = new Viewport("IDI Laboratory - Block 4");
    
    // Create cameras
    Camera* thirdPerson = new Camera(viewport, new PerspectiveLens());
    thirdPerson->addLens(new OrthogonalLens());
    
    Camera* firstPerson = new Camera(viewport, new FirstPersonLens());
    firstPerson->translate(0, 1, 0);
    
    // Create scene
    Scene * scene = createScene(firstPerson);
    
    // Add cameras
    engine->addCamera('r', thirdPerson);
    engine->addCamera('f', firstPerson);
    
    // Initialize glut
    engine->init(&argc, argv);
    viewport->init();
    
    engine->configureCallbacks(viewport);
    
    // Focus the scene
    thirdPerson->focus(scene);
    firstPerson->focus(scene);
    
    // Start rendering!
    engine->loop();
    
    return 0;
}
