/**
 * Generic library to reuse code between blocks.
 */

#if defined(__APPLE__)
  #include <OpenGL/OpenGl.h>
  #include <GLUT/GLUT.h>
#else
  #include <GL/gl.h>
  #include <GL/freeglut.h>
#endif

#include <iostream>
#include <map>
#include <vector>

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
    
    Window(int width = 600, int height = 600)
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
public:
    State()
    {
    }
    
    // Description
    virtual string getDescription() const { return "Unknow description"; }
    
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

class StateMachine
{
    map<unsigned char, State*> states;
    State* current;
    
public:
    StateMachine()
    {
    }
    
    void add(unsigned char key, State* state)
    {
        if(states.empty())
            current = state;
        
        states[key] = state;
    }
    
    void init()
    {
        current->enter();
    }
    
    void trigger(unsigned char keyTrigger)
    {
        map<unsigned char, State*>::const_iterator it = states.find(keyTrigger);
        
        if(it != states.end())
        {
            current->leave();
            current = it->second;
            current->enter();
        }
    }
    
    void printHelp()
    {
        cout << "-------------------" << endl;
        cout << "Available actions:" << endl;
        
        map<unsigned char, State*>::const_iterator it = states.begin();
        
        while(it != states.end())
        {
            cout << it->second->getDescription() << endl;
            cout << "Press '" << it->first << "' to enable it." << endl;
            cout << endl;
            
            ++it;
        }
    }
    
    State* getCurrentState()
    {
        return current;
    }
};

class BackgroundSelector : public State
{
    Window* window;
    
public:
    BackgroundSelector(Window* window)
    {
        this->window = window;
    }
    
    string getDescription() const
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
    TriangleSelector(Window* window, Triangle** triangle)
    {
        this->window = window;
        this->triangle = triangle;
        
        a = b = c = NULL;
    }
    
    string getDescription() const
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
    
    void mousePressed(int buttonId, int state, int x, int y)
    {
        if(buttonId != GLUT_LEFT_BUTTON || state == GLUT_DOWN)
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
