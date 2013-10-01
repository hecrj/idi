#include <idi.cpp>

using namespace std;

Engine* engine;
Teapot* teapot;

void refresh()
{
    engine->draw();
}

void reshape(int width, int height)
{
    engine->getWindow()->reshape(width, height);
}

/*
 * 
 */
int main(int argc, char** argv)
{
    engine = new Engine("Laboratori IDI - Bloc 2");
    teapot = new Teapot(0.5);
    
    engine->init(&argc, argv);
    
    glutDisplayFunc(refresh);
    glutReshapeFunc(reshape);
    
    engine->addObject("teapot", teapot);
    engine->loop();
    
    return 0;
}
