/* 
 * File:   main.cpp
 * Author: hector
 *
 * Created on September 10, 2013, 8:09 AM
 */

// Load generic code
#include <idi.cpp>

// Global variables
Triangle* triangle;
Window* window;
StateMachine* states;

void help()
{
    states->printHelp();
    
    cout << "Press 'h' to show this help message." << endl;
    cout << "Press ESC to quit." << endl;
}

void refresh()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    triangle->draw();
    
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    window->reshape(width, height);
}

void mousePressed(int buttonId, int released, int x, int y)
{
    states->getCurrentState()->mousePressed(buttonId, released, x, y);
}

void mouseMotion(int x, int y)
{    
    states->getCurrentState()->mouseMotion(x, y);
}

void keyPressed(unsigned char key, int x, int y)
{
    if(key == 27) // if ESC pressed
        exit(0);  // Exit app
    
    if(key == 'h')
    {
        help(); // Show help
        return;
    }
    
    states->trigger(key);
}

void initStates()
{
    states = new StateMachine();
    
    states->add('f', new BackgroundSelector(window));
    states->add('t', new TriangleSelector(window, &triangle));
    
    states->init();
}

void initGL(int argc, char **argv)
{
    // Default triangle
    triangle = Triangle::def();
    
    // Default window
    window = new Window();
    
    // Init states
    initStates();
    
    // Window intialization
    window->init(&argc, argv);
    
    // Default clear color
    glClearColor(0, 0, 0, 1);
    
    // Callbacks
    glutDisplayFunc(refresh);
    glutReshapeFunc(reshape);
    glutMouseFunc(mousePressed);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyPressed);
    
    // Start app
    glutMainLoop();
}

/*
 * 
 */
int main(int argc, char** argv)
{
    initGL(argc, argv);
    
    return 0;
}
