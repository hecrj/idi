#include "simplegl/Scene.h"
#include "simplegl/StateMachine.h"
#include "simplegl/tools/NavigationTool.h"
#include "simplegl/objects/Snowman.h"
#include "simplegl/objects/Plane.h"
#include "simplegl/objects/Model.h"
#include "simplegl/objects/Wall.h"
#include "simplegl/Engine.h"
#include "simplegl/Color.h"
#include "simplegl/lens/OrthogonalLens.h"
#include "simplegl/lens/PerspectiveLens.h"
#include "simplegl/lens/FirstPersonLens.h"
#include "simplegl/objects/Light.h"
#include "simplegl/togglers/Teleporter.h"

#if defined(__APPLE__)
  #include <OpenGL/OpenGl.h>
  #include <GLUT/GLUT.h>
#else
  #include <GL/gl.h>
  #include <GL/freeglut.h>
#endif

using namespace std;

Engine* engine;
Scene* carScene;
Light* light0;
Light* light1;
Light* light2;

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
    engine->addAction('v', walls, "Show/Hide walls");
    
    // Car scene
    carScene = new Scene();
    
    Model* car = new Model("porsche.obj");
    car->positionateBottomCenter(0, 0, 0);
    
    firstPerson->translate(0, 1, 0);
    firstPerson->rotate(0, 180, 0);
    carScene->setCamera(firstPerson);
    
    carScene->addObject("car", car);
    
    scene->addObject("plane", plane);
    scene->addObject("snowman1", snowman1);
    scene->addObject("snowman2", snowman2);
    scene->addObject("snowman3", snowman3);
    scene->addObject("snowman4", snowman4);
    scene->addObject("walls", walls);
    scene->addObject("carScene", carScene);

    return scene;
}

void configureStates()
{
    StateMachine* states = engine->getStates();
    
    // TOOLS
    InspectTool* inspector = new InspectTool(engine);
    
    NavigationTool* navigator = new NavigationTool();
    navigator->add(carScene);
    
    // Add states
    states->setGlobal(inspector);
    states->add('r', navigator);
}

void configureLights(Scene* scene)
{
    // Global light
    light1 = new Light(GL_LIGHT1, true);
    light1->draw();
    
    // Scene light
    light0 = new Light(GL_LIGHT0, false);
    light0->setColor(Color::YELLOW);
    light0->translate(-5, 2, -5);
    light0->toggle(); // Off by default

    scene->addObject("light0", light0);
    
    // Teleport light
    Teleporter* teleporter = new Teleporter();
    teleporter->addObject(light0);
    
    teleporter->addPoint(-5, 2, -5);
    teleporter->addPoint(-5, 2, 5);
    teleporter->addPoint(5, 2, 5);
    teleporter->addPoint(5, 2, -5);
    teleporter->addPoint(0, 2, 0);
    
    engine->addAction('m', teleporter, "Move yellow light");
    
    // Car light
    light2 = new Light(GL_LIGHT2, false);
    light2->translate(0, 1, 0);
    light2->toggle(); // Off by default
    
    carScene->addObject("light", light2);
    
    // Add toggles
    engine->addAction('1', light1, "Enable/Disable third person camera light");
    engine->addAction('0', light0, "Enable/Disable scene yellow light");
    engine->addAction('2', light2, "Enable/Disable car light");
}

void reset()
{
    light1->enable();
    light0->disable();
    light2->disable();
}

int main(int argc, char **argv)
{
    // Initialize glut
    Engine::initGlut(&argc, argv);
    
    // Create the engine
    engine = new Engine();
    
    // Viewport
    Viewport* viewport = new Viewport("IDI Laboratory - Block 4");
    
    // Create cameras
    Camera* thirdPerson = new Camera(viewport, new PerspectiveLens());
    thirdPerson->addLens(new OrthogonalLens());
    
    Camera* firstPerson = new Camera(viewport, new FirstPersonLens());
    
    // Create scene
    Scene* scene = createScene(firstPerson);
    
    // Add cameras
    engine->addCamera('r', thirdPerson, "General camera");
    engine->addCamera('f', firstPerson, "Car camera");
    
    // Focus the scene
    thirdPerson->focus(scene);
    firstPerson->focus(scene);
    
    // Initialize engine
    engine->init(viewport);
    
    // Configure states
    configureStates();
    
    // Configure lights
    configureLights(scene);
    
    engine->setResetCallback(reset);
    
    // Start rendering!
    engine->loop();
    
    return 0;
}
