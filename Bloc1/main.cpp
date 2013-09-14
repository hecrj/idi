/* 
 * File:   main.cpp
 * Author: hector
 *
 * Created on September 10, 2013, 8:09 AM
 */
#if defined(__APPLE__)
  #include <OpenGL/OpenGl.h>
  #include <GLUT/GLUT.h>
#else
  #include <GL/gl.h>
  #include <GL/freeglut.h>
#endif

#include <iostream>

using namespace std;

struct Vertex
{
    double x, y, z;
    
    Vertex(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    
    ~Vertex()
    { }
    
    void draw()
    {
        glVertex3d(x, y, z);
    }
};

class Triangle
{
    Vertex* a;
    Vertex* b;
    Vertex* c;
    
public:
    static Triangle* def()
    {
        return new Triangle(
            new Vertex(-0.5, -1.0 / 3.0, 0),
            new Vertex(0.5, -1.0 / 3.0, 0),
            new Vertex(0, 2.0 / 3.0, 0)
        );
    }
    
    Triangle(Vertex* a, Vertex* b, Vertex *c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
    }
    
    ~Triangle()
    {
        delete a;
        delete b;
        delete c;
    }
    
    void draw()
    {
        glBegin(GL_TRIANGLES);
        glColor3d(1, 0, 0);
        
        a->draw();
        b->draw();
        c->draw();
        
        glEnd();
    }
};

struct Viewport
{
    int x;
    int y;
    int width;
    int height;
    
    Viewport(int width, int height)
    {
        x = 0;
        y = 0;
        this->width = width;
        this->height = height;
    }
    
    void reshape(int windowWidth, int windowHeight)
    {
        x = y = 0;
        width = windowWidth;
        height = windowHeight;
        
        if(width > height)
        {
            x = (width - height) / 2;
            width = height;
        }
        else
        {
            y = (height - width) / 2;
            height = width;
        }

        glViewport(x, y, width, height);
    }
    
    Vertex* getVertex(int mx, int my)
    {
        mx = mx - x;
        my = (height - my) + y;
        
        double vx = ((double)mx / width) * 2 - 1;
        double vy = ((double)my / height) * 2 - 1;
        
        return new Vertex(vx, vy, 0);
    }
};

struct Window
{
    int width;
    int height;
    Viewport* viewport;
    
    Window(int width, int height)
    {
        this->width = width;
        this->height = height;
        
        viewport = new Viewport(width, height);
    }
    
    void reshape(int width, int height)
    {
        this->width = width;
        this->height = height;
        
        viewport->reshape(width, height);
    }
    
    void init(int* argc, char** argv)
    {
        glutInit(argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
        glutInitWindowSize(width, height);
        glutCreateWindow("Laboratori IDI - Bloc 1");
    }
    
    Vertex* getViewportVertex(int x, int y)
    {
        return viewport->getVertex(x, y);
    }
};

class State
{
    unsigned char keyTrigger;
    
public:
    State(unsigned char keyTrigger)
    {
        this->keyTrigger = keyTrigger;
    }
    
    char getKeyTrigger()
    {
        return keyTrigger;
    }
    
    // Description
    virtual string getDescription(){ return "Unknow description"; }
    
    // Actions called when entering/leaving a state
    virtual void enter()
    {
        cout << "-------------------" << endl;
        cout << getDescription() << endl;
    }
    
    virtual void leave() { }
    
    // Default mouse state actions
    virtual void mouseMotion(int x, int y){ }
    virtual void mousePressed(int buttonId, int released, int x, int y) { }
};

class BackgroundSelector : public State
{
    Window* window;
    
public:
    BackgroundSelector(unsigned char keyTrigger, Window* window)
            : State(keyTrigger)
    {
        this->window = window;
    }
    
    string getDescription()
    {
        return "Background selector:\n"
                "Hold the mouse pressed and move it around the window to select the background color.";
    }
    
    void mouseMotion(int x, int y)
    {
        double red = getRangedValue(x, window->width);
        double blue = getRangedValue(y, window->height);
        
        glClearColor(red, 0, blue, 1);
        glutPostRedisplay();
    }
    
private:
    double getRangedValue(int k, int max)
    {
        if(k < 0)
            k = -k;
        
        int a = k / max;
        
        double ranged = (double)(k % max);;
        
        if(a % 2 != 0)
            ranged = max - ranged;
            
        return ranged / max;
    }
};

class TriangleSelector : public State
{
    Window* window;
    
    Triangle** triangle;
    Vertex* a;
    Vertex* b;
    Vertex* c;
    bool completed;
    
public:
    TriangleSelector(unsigned char keyTrigger, Window* window, Triangle** triangle)
            : State(keyTrigger)
    {
        this->window = window;
        this->triangle = triangle;
        
        a = b = c = NULL;
    }
    
    string getDescription()
    {
        return "Triangle selector:\n"
                "Click to define every vertex of the new triangle.";
    }
    
    void enter()
    {
        a = b = c = NULL;
        
        State::enter();
    }
    
    void leave()
    {
        if(a != NULL)
            delete a;
        
        if(b != NULL)
            delete b;
        
        if(c != NULL)
            delete c;
    }
    
    void mousePressed(int buttonId, int released, int x, int y)
    {
        if(released == 0)
            return;
        
        Vertex* v = window->getViewportVertex(x, y);
        
        cout << "New vertex defined at: (" << v->x << ", " << v->y << ", " << v->z << ")" << endl;
        
        if(a == NULL)
            a = v;
        else if(b == NULL)
            b = v;
        else
            c = v;
        
        if(c != NULL)
        {
            *triangle = new Triangle(a, b, c);
            a = b = c = NULL;
            
            glutPostRedisplay();
        }
    }
};

#define WINDOW_WIDTH_DEFAULT  600
#define WINDOW_HEIGHT_DEFAULT 600
#define STATE_NUM             2

// Global variables
Triangle* triangle;
Window* window;
State* availableStates[STATE_NUM];
State* currentState;

void help()
{
    cout << "-------------------" << endl;
    cout << "Available actions:" << endl;

    for(int i = 0; i < STATE_NUM; ++i)
    {
        cout << availableStates[i]->getDescription() << endl;
        cout << "Press '" << availableStates[i]->getKeyTrigger() << "' to enable it." << endl;
        cout << endl;
    }
    
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
    currentState->mousePressed(buttonId, released, x, y);
}

void mouseMotion(int x, int y)
{    
    currentState->mouseMotion(x, y);
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
    
    for(int i = 0; i < STATE_NUM; ++i)
    {
        if(availableStates[i]->getKeyTrigger() == key)
        {
            currentState->leave();
            currentState = availableStates[i];
            currentState->enter();
        }
    }
}

void initStates()
{
    availableStates[0] = new BackgroundSelector('f', window);
    availableStates[1] = new TriangleSelector('t', window, &triangle);
    
    // Default state
    currentState = availableStates[0];
    currentState->enter();
}

void initGL(int argc, char **argv)
{
    // Default triangle
    triangle = Triangle::def();
    
    // Default window
    window = new Window(WINDOW_WIDTH_DEFAULT, WINDOW_HEIGHT_DEFAULT);
    
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
