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

void refresh(void)
{
    
}

void initGL(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Laboratori IDI - Bloc 1");
    glutDisplayFunc(refresh);
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
