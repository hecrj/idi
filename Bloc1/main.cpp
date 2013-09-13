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

struct Triangle
{
    Vertex* a;
    Vertex* b;
    Vertex* c;
    
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

// Global variables
Triangle* triangle;

void refresh(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    triangle->draw();
    
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    int x, y;
    x = y = 0;
    
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

void initGL(int argc, char **argv)
{
    triangle = Triangle::def();
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Laboratori IDI - Bloc 1");
    glutDisplayFunc(refresh);
    glutReshapeFunc(reshape);
    glClearColor(0, 0, 0, 1);
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
