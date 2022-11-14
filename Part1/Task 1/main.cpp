#include <stdlib.h>
#include <glut.h>
#include "ImageLoading.h"
#include <math.h>
#include "glm.h"

#define PI 3.14

static int pos=2,shoulder = 0, elbow = 0, fingerBase = 0, fingerUp = 0, Body = 0, ankel1 = 0, leg = 0, leg2 = 0, ankle2 = 0, shoes1 = 0, shoes2 = 0, shoulder2 = 0, legright = 0,  legleft = 0;
int poses[5][15] = { {0,0,0,0,0,40,60,0,0,0,0,0,0,0,2},
                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
                     {0,0,0,0,0,-20,-30,0,0,0,0,0,0,0,2},
                     {0,0,0,0,0,-20,-30,0,0,0,0,0,0,0,4},
                    {160,50,0,0,0,-20,-30,0,0,0,0,0,0,0,6} };
int moving, startx, starty;


GLfloat angle = 0.0;   /* in degrees */
GLfloat angle2 = 0.0;   /* in degrees */
double eye[] = { 0.0, 0.0, 8.0 };
double center[] = { 0.0, 0.0, 0.0 };
double up[] = { 0.0, 1.0, 0.0 };
const char* imagename = "textures/floor.bmp";
const char* modelname = "data/fireExtinguisher.obj";

// RGBA
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat light_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
// x , y, z, w
GLfloat light_position[] = { 0.5, 5.0, 0.0, 1.0 };
GLfloat lightPos1[] = { -0.5, -5.0, -2.0, 1.0 };
static int year = 0, day = 0;


//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(ImageLoading* image)
{
    GLuint textureId;
    glGenTextures(1, &textureId);			 //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D,				  //Always GL_TEXTURE_2D
        0,							  //0 for now
        GL_RGB,					  //Format OpenGL uses for image
        image->width, image->height, //Width and height
        0,							  //The border of the image
        GL_RGB,					  //GL_RGB, because pixels are stored in RGB format
        GL_UNSIGNED_BYTE,			  //GL_UNSIGNED_BYTE, because pixels are stored
                                     //as unsigned numbers
        image->pixels);			  //The actual pixel data
    return textureId;						  //Returns the id of the texture
}

GLuint _textureId; //The id of the texture


void initRendering()
{
    ImageLoading* image = loadBMP((char*)imagename);
    //ImageLoading* image = loadBMP(filename);
    _textureId = loadTexture(image);
    delete image;
    // Turn on the power
    glEnable(GL_LIGHTING);
    // Flip light switch
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    // assign light parameters
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    // Material Properties
    GLfloat lightColor1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
    glEnable(GL_NORMALIZE);
    //Enable smooth shading
    glShadeModel(GL_SMOOTH);
    // Enable Depth buffer
    glEnable(GL_DEPTH_TEST);
}

void drawmodel(char* filename)
{
    GLMmodel* model = glmReadOBJ(filename);
    glmUnitize(model);
    glmFacetNormals(model);
    glmVertexNormals(model, 90.0);
    glmScale(model, .15);
    glmDraw(model, GLM_SMOOTH | GLM_MATERIAL);
}

void init(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    //glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glShadeModel(GL_FLAT);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(60.0,
        1.0,
        1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);

    glShadeModel(GL_FLAT);
}

void display(void)
{
    initRendering();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(eye[0], eye[1], eye[2],
        center[0], center[1], center[2],
        up[0], up[1], up[2]);
    
    //////////////////////
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-4, -4, 4);
    glTexCoord2f(3.0f, 0.0f); glVertex3f(4, -4, 4);
    glTexCoord2f(3.0f, 3.0f); glVertex3f(4, -4, -4);
    glTexCoord2f(0.0f, 3.0f); glVertex3f(-4, -4, -4);
    glEnd();
    glDisable(GL_TEXTURE_2D);


    glPushMatrix();
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glPopMatrix();
    ////////////////////////
    glPushMatrix();
    glRotatef(angle2, 1.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glTranslatef(pos, -2.5, 2.0);
    glScalef(10, 10, 10);
    drawmodel((char*)modelname);
    glPopMatrix();

   ////////////////////////
    glTranslatef(-0.5, 0.0, 0.0);
    glPushMatrix();
    glRotatef(90, 0.0, 1.0, 0.0);
    glRotatef(angle2, 1.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glTranslatef(-1.0, 0.0, 0.0);
    //glRotatef((GLfloat)Body, 1.0, 0.0, 0.0);
    //head
    glPushMatrix();
    glTranslatef(0, 3, 0);
    glutWireSphere(0.3, 10, 10);
    glPopMatrix();
    //body
    glPushMatrix();
    glTranslatef(0, 0.8, 0.0);
    glScalef(1, 3, 0.4);
    glutWireCube(1.0);
    glPopMatrix();
    ///////////

    glPushMatrix();
    glTranslatef(0.7, 2, 0.0);
    glRotatef((GLfloat)shoulder, 0.0, 0.0, 1.0);
    glRotatef((GLfloat)shoulder2, 0.0, 1.0, 0.0);
    glTranslatef(0.125, -0.5, 0.0);
    glPushMatrix();
    glScalef(0.15, 1, 0.4);
    glutWireCube(1.0);
    glPopMatrix();

    glTranslatef(-0.5,-0.5, 0.0);
    glRotatef((GLfloat)elbow, -1.0, 0.0, 0.0);
    glTranslatef(0.5, -0.5, 0.0);
    glPushMatrix();
    glScalef(0.15, 1, 0.4);
    glutWireCube(1.0);
    glPopMatrix();

    //Draw finger flang 1 
    glPushMatrix();
    glTranslatef(0, -0.65, -0.15);
    glRotatef((GLfloat)fingerBase, 0.0, 0.0, -1.0);
    glTranslatef(0.05, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.1);
    glScalef(0.05, 0.3, 0.05);
    glutWireCube(1);
    glPopMatrix();

    //Draw finger flang 2 
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.25);
    glScalef(0.05, 0.3, 0.05);
    glutWireCube(1);
    glPopMatrix();

    //Draw finger flang 3
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.35);
    glScalef(0.05, 0.3, 0.05);
    glutWireCube(1);
    glPopMatrix();

    //Draw finger flang 4
    glPushMatrix();
    glTranslatef(0.0, 0.0, -0.05);
    glScalef(0.05, 0.3, 0.05);
    glutWireCube(1);
    glPopMatrix();

    //Draw finger flang 1 up 
    glTranslatef(0.05, -0.15, 0.0);
    glRotatef((GLfloat)fingerUp, 0.0, 0.0, -1.0);
    glTranslatef(-0.05, -0.15, 0.0);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.1);
    glScalef(0.05, 0.3, 0.05);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 2 up 
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.25);
    glScalef(0.05, 0.3, 0.05);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 3 up
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.35);
    glScalef(0.05, 0.3, 0.05);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 4 up
    glPushMatrix();
    glTranslatef(0.0, 0.0, -0.05);
    glScalef(0.05, 0.3, 0.05);
    glutWireCube(1);
    glPopMatrix();

    glPopMatrix();

    //Draw finger flang 5
    glTranslatef(-0.2, -0.65, -0.15);
    glRotatef((GLfloat)fingerBase, 0.0, 0.0, -1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.05, 0.3, 0.05);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 5 up
    glTranslatef(0.05, -0.1, 0.0);
    glRotatef((GLfloat)fingerUp, -1.0, 0.0, 0.0);
    glTranslatef(-0.05, -0.2, 0.0);
    glPushMatrix();
    glScalef(0.05, 0.3, 0.05);
    glutWireCube(1);
    glPopMatrix();
    glPopMatrix(); //elkbera awyy

    //SHOULDER_2
    glPushMatrix();

    glTranslatef(-0.68, 1.9, 0.0);
    glRotatef((GLfloat)shoulder, 0.0, 0.0, -1.0);
    glRotatef((GLfloat)shoulder2, 0.0, -1.0, 0.0);
    glTranslatef(-0.12, -0.5, 0.0);
    glPushMatrix();
    glScalef(0.15, 1, 0.4);
    glutWireCube(1.0);
    glPopMatrix();
    //elbow_2   
    glTranslatef(0.5, -0.5, 0.0);
    glRotatef((GLfloat)elbow, -1.0, 0.0, 0.0);
    glTranslatef(-0.5, -0.5, 0.0);
    glPushMatrix();
    glScalef(0.15, 1, 0.4);
    glutWireCube(1.0);
    glPopMatrix();

    //Draw finger flang 1 
    glPushMatrix();
    glTranslatef(0.0, -0.5, -0.15);
    glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
    glTranslatef(-0.05, -0.15, 0.0);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.1);
    glScalef(0.05, 0.3, 0.05);
    glutWireCube(1);
    glPopMatrix();

    //Draw finger flang 2 
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.25);
    glScalef(0.05, 0.3, 0.05);
    glutWireCube(1);
    glPopMatrix();

    //Draw finger flang 3
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.35);
    glScalef(0.05, 0.3, 0.05);
    glutWireCube(1);
    glPopMatrix();

    //Draw finger flang 4
    glPushMatrix();
    glTranslatef(0.0, 0.0, -0.05);
    glScalef(0.05, 0.3, 0.05);
    glutWireCube(1);
    glPopMatrix();


    //Draw finger flang 1 up 
    glTranslatef(0.05, -0.15, 0.0);
    glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
    glTranslatef(-0.05, -0.14, 0.0);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.1);
    glScalef(0.05, 0.3, 0.05);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 2 up 
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.25);
    glScalef(0.05, 0.3, 0.05);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 3 up
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.35);
    glScalef(0.05, 0.3, 0.05);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 4 up
    glPushMatrix();
    glTranslatef(0.0, 0.0, -0.05);
    glScalef(0.05, 0.3, 0.05);
    glutWireCube(1);
    glPopMatrix();

    glPopMatrix();

    //glTranslatef(-0.05, -0.5, -0.15);
    //Draw finger flang 5
    glTranslatef(0.1, -0.5, -0.18);
    glRotatef((GLfloat)fingerBase, -1.0, 0.0, 0.0);
    glTranslatef(-0.05, -0.15, 0.0);
    glPushMatrix();
    glScalef(0.05, 0.3, 0.05);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 5 up
    glTranslatef(0.05, -0.18, 0.0);
    glRotatef((GLfloat)fingerUp, -1.0, 0.0, 0.0);
    glTranslatef(-0.05, -0.125, 0.0);
    glPushMatrix();
    glScalef(0.05, 0.3, 0.05);
    glutWireCube(1);
    glPopMatrix();


    glPopMatrix();


    //LEGS
    ///////////////leg_1////////////////////////////////////
    glPushMatrix();
    glTranslatef(0.4, -0.7, 0.0);
    glRotatef((GLfloat)leg, 1.0, 0.0, 0.0);
    glRotatef((GLfloat)legright, 0.0, 0.0, 1.0);

    glTranslatef(0, -0.75, 0.0);
    
    glPushMatrix();
    glScalef(0.2, 1.5, 0.4);
    glutWireCube(1.0);
    glPopMatrix();
    ////ankel_1
    glTranslatef(-0.15, -0.5, 0.0);
    glRotatef((GLfloat)ankel1, 1.0, 0.0, 0.0);
    glTranslatef(0.15, -1, 0.0);
    
    glPushMatrix();
    glScalef(0.2, 1.5, 0.4);
    glutWireCube(1.0);
    glPopMatrix();
    ///shoes1
    glTranslatef(-0.1, 0.1, 0.0);
    glRotatef((GLfloat)shoes1, 1.0, 0.0, 0.0);
    glTranslatef(0.15, -1, 0.0);
    glPushMatrix();
    glScalef(0.6, 0.4, 1.5);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
    
    //leg_2
    glPushMatrix();
    glTranslatef(-0.5, -0.7, 0.0);
    glRotatef((GLfloat)leg2, 1.0, 0.0, 0.0);
    glRotatef((GLfloat)legleft, 0.0, 0.0, -1.0);
    glTranslatef(0.1, -0.75, 0.0);
    glPushMatrix(); 
    glScalef(0.2, 1.5, 0.4);
    //glTranslatef(-2.3, 0.0, 0.0);
    glutWireCube(1.0);
    glPopMatrix();
    //ankel_2
    glTranslatef(-0.15, -0.5, 0.0);
    glRotatef((GLfloat)ankle2, 1.0, 0.0, 0.0);
    glTranslatef(0.15, -1, 0.0);
    glPushMatrix();
    glScalef(0.2, 1.5, 0.4);
    //glTranslatef(-2.3, 0.0, 0.0);
    glutWireCube(1.0);
    glPopMatrix();
    ///shoes2
    glTranslatef(-0.2, 0.1, 0.0);
    glRotatef((GLfloat)shoes2, 1.0, 0.0, 0.0);
    glTranslatef(0.15, -1, 0.0);
    glPushMatrix();
    glScalef(0.6, 0.4, 1.5);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
   
    




    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(85.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
}


void rotatePoint(double a[], double theta, double p[])
{

    double temp[3];
    temp[0] = p[0];
    temp[1] = p[1];
    temp[2] = p[2];

    temp[0] = -a[2] * p[1] + a[1] * p[2];
    temp[1] = a[2] * p[0] - a[0] * p[2];
    temp[2] = -a[1] * p[0] + a[0] * p[1];

    temp[0] *= sin(theta/6);
    temp[1] *= sin(theta/6);
    temp[2] *= sin(theta/6);

    temp[0] += (1 - cos(theta/6)) * (a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
    temp[1] += (1 - cos(theta/6)) * (a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
    temp[2] += (1 - cos(theta/6)) * (a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

    temp[0] += cos(theta/6) * p[0];
    temp[1] += cos(theta/6) * p[1];
    temp[2] += cos(theta/6) * p[2];

    p[0] = temp[0];
    p[1] = temp[1];
    p[2] = temp[2];
}

void crossProduct(double a[], double b[], double c[])
{
    c[0] = a[1] * b[2] - a[2] * b[1];
    c[1] = a[2] * b[0] - a[0] * b[2];
    c[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(double a[])
{
    double norm;
    norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
    norm = sqrt(norm);
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}

void lookRight()
{
    rotatePoint(up, PI / 80, eye);
}

void lookLeft()
{
    rotatePoint(up, -PI / 80, eye);
}

void lookUp()
{
    double horizontal[3];
    double look[] = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };
    crossProduct(up, look, horizontal);
    normalize(horizontal);
    rotatePoint(horizontal, PI / 100, eye);
    rotatePoint(horizontal, PI / 100, up);
}

void lookDown()
{
    double horizontal[3];
    double look[] = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };
    crossProduct(up, look, horizontal);
    normalize(horizontal);
    rotatePoint(horizontal, -PI / 100, eye);
    rotatePoint(horizontal, -PI / 100, up);
}

void moveForward()
{
    double direction[3];
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];
    float speed = 0.1;
    eye[0] += direction[0] * speed;
    eye[1] += direction[1] * speed;
    eye[2] += direction[2] * speed;

    center[0] += direction[0] * speed;
    center[1] += direction[1] * speed;
    center[2] += direction[2] * speed;
}

void moveBackword()
{
    double direction[3];
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];
    float speed = -0.1;
    eye[0] += direction[0] * speed;
    eye[1] += direction[1] * speed;
    eye[2] += direction[2] * speed;

    center[0] += direction[0] * speed;
    center[1] += direction[1] * speed;
    center[2] += direction[2] * speed;
}

void reset()
{
    double e[] = { 0.0, 0.0, 8.0 };
    double c[] = { 0.0, 0.0, 0.0 };
    double u[] = { 0.0, 1.0, 0.0 };
    for (int i = 0; i < 3; i++)
    {
        eye[i] = e[i];
        center[i] = c[i];
        up[i] = u[i];
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 's':
        if (shoulder < 180 )//or shoulder2 > 180)
        {
            shoulder = (shoulder + 5) % 360;
            //shoulder2 = (shoulder2 - 5) % 360;
            glutPostRedisplay();

        }
        break;
    case 'S':
        if (shoulder > 0)
        {
            shoulder = (shoulder - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'e':
        if (elbow < 140)
        {
            elbow = (elbow + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'E':
        if (elbow > -140)
        {
            elbow = (elbow - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'f':
        if (fingerBase < 45)
        {
            fingerBase = (fingerBase + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'F':
        if (fingerBase > 0)
        {
            fingerBase = (fingerBase - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'g':
        if (fingerUp < 90)
        {
            fingerUp = (fingerUp + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'G':
        if (fingerUp > 0)
        {
            fingerUp = (fingerUp - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'l':
        if (leg > -95)
        {
            leg = (leg - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'L':
        if (leg < 45)
        {
            leg = (leg + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'o':
        if (leg2 > -95)
        {
            leg2 = (leg2 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'O':
        if (leg2 < 45)
        {
            leg2 = (leg2 + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'K':
        if (ankel1 > 0)
        {
            ankel1 = (ankel1 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'k':
        if (ankel1 < 60)
        {
            ankel1 = (ankel1 + 5) % 360;
            glutPostRedisplay();
        }
        break;

    case 'I':
        if (ankle2 > 0)
        {
            ankle2 = (ankle2 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'i':
        if (ankle2 < 60)
        {
            ankle2 = (ankle2 + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'd':
        if (shoulder2 < 90)
        {
            shoulder2 = (shoulder2 +5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'D':
        if (shoulder2 > -90)
        {
            shoulder2 = (shoulder2 - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'p':
        if (legright < 90)
        {
            legright = (legright + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'P':
        if (legright > 0)
        {
            legright = (legright - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'm':
        if (legleft < 90)
        {
            legleft = (legleft + 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'M':
        if (legleft > 0)
        {
            legleft = (legleft - 5) % 360;
            glutPostRedisplay();
        }
        break;
    case 'z':
        moveForward();
        glutPostRedisplay();
        break;
    case 'Z':
        moveBackword();
        glutPostRedisplay();
        break;
    case 'r':
        reset();
        glutPostRedisplay();
        break;



    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

void specialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        lookLeft();
        break;
    case GLUT_KEY_RIGHT:
        lookRight();
        break;
    case GLUT_KEY_UP:
            lookUp();
        break;
    case GLUT_KEY_DOWN:
        lookDown();
        break;
    }
    glutPostRedisplay();
}

static void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            moving = 1;
            startx = x;
            starty = y;
        }
        if (state == GLUT_UP) {
            moving = 0;
        }
    }
}


static void motion(int x, int y)
{
    if (moving) {
        angle = angle + (x - startx);
        angle2 = angle2 + (y - starty);
        startx = x;
        starty = y;
        glutPostRedisplay();
    }
}
void setPoses(int Frame)
{
    shoulder=poses[Frame][0];
    elbow = poses[Frame][1]; 
    fingerBase = poses[Frame][2]; 
    fingerUp = poses[Frame][3];
    Body = poses[Frame][4];
    ankel1 = poses[Frame][5];
    leg = poses[Frame][6];
    leg2 = poses[Frame][7];
    ankle2 = poses[Frame][8];
    shoes1 = poses[Frame][9];
    shoes2 = poses[Frame][10];
    shoulder2 = poses[Frame][11];
    legright = poses[Frame][12];
    legleft = poses[Frame][13];
    pos = poses[Frame][14];
}
static int f = 0;
void timer(int value)
{
    f = f % 5;
    setPoses(f);
    f++;
    glutPostRedisplay();
    glutTimerFunc(500, timer, 1);
}

void texturemenu(int v)
{
    switch (v)
    {
    case 0: 
        glutSetWindowTitle("celtstone");
        imagename = "textures/celtstone.bmp";
        break;
    case 1: 
        glutSetWindowTitle("floor");
        imagename = "textures/floor.bmp";
        break;
    case 2:
        glutSetWindowTitle("shields");
        imagename = "textures/shields.bmp";
        break;
    case 3:
        glutSetWindowTitle("soiltex");
        imagename = "textures/soiltex.bmp";
        break;
    case 4:
        glutSetWindowTitle("view");
        imagename = "textures/view.bmp";
        break;

    }
    reset();
    glutPostRedisplay();
}

void objectmenu(int value)
{

    switch (value)
    {
    case 0:
        modelname = "data/tetrapak.obj";
        break;
    case 1:
        modelname = "data/table_football.obj";
        break;
    case 2:
        modelname = "data/rectangle-swimming-pool.obj";
        break;
    case 3:
        modelname = "data/fireExtinguisher.obj";
        break;

    }
    reset();
    glutPostRedisplay();
}


void MenuInit()
{
    int sub1 = glutCreateMenu(texturemenu) ;
    glutAddMenuEntry("celtstone", 0);
    glutAddMenuEntry("floor", 1);
    glutAddMenuEntry("shields", 2);
    glutAddMenuEntry("soiltex", 3);
    glutAddMenuEntry("view", 4);
    glutCreateMenu(texturemenu);

    int sub2 = glutCreateMenu(objectmenu);
    
    glutAddMenuEntry("milk", 0);
    glutAddMenuEntry("football table", 1);
    glutAddMenuEntry("swimming - pool", 2);
    glutAddMenuEntry("fireExtinguisher", 3);
    glutCreateMenu(objectmenu);
    //glutCreateMenu(objectmenu);
    glutAddSubMenu("textures", sub1);
    glutAddSubMenu("load object", sub2);

    /*int sub2 = glutCreateMenu(objectmenu);
    glutAddMenuEntry("Models", 0);
    glutAddMenuEntry("", 0);
    glutAddMenuEntry("milk", '1');
    glutAddMenuEntry("football table", '2');
    glutAddMenuEntry("swimming - pool", '3');
    glutAddMenuEntry("fireExtinguisher", '4');
    glutCreateMenu(objectmenu);
    glutAddSubMenu("load object", sub2);*/

    glutAttachMenu(GLUT_RIGHT_BUTTON);

    
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    ////////////////
    glutCreateWindow("Floor");
    initRendering();

    glMatrixMode(GL_PROJECTION);
    gluPerspective(60, 1.0, 0.1, 10);
    ///////////////
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    //glutCreateWindow(argv[0]);
    init();
    MenuInit();
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}































//#include <stdlib.h>
//#include <GL/glut.h>
//
//
//static int shoulder = 0, elbow = 0, fingerBase = 0, fingerUp = 0, Body = 0, shoulder2 = 0, leg = 0;
//int moving, startx, starty;
//
//
//GLfloat angle = 0.0;   /* in degrees */
//GLfloat angle2 = 0.0;   /* in degrees */
//
//
//void init(void)
//{
//    glClearColor(0.0, 0.0, 0.0, 0.0);
//    glShadeModel(GL_FLAT);
//}
//
//void display(void)
//{
//    glClear(GL_COLOR_BUFFER_BIT);
//    glPushMatrix();
//    glRotatef(angle2, 1.0, 0.0, 0.0);
//    glRotatef(angle, 0.0, 1.0, 0.0);
//    glTranslatef(-1.0, 0.0, 0.0);
//    glRotatef((GLfloat)Body, 1.0, 0.0, 0.0);
//   //head
//    glPushMatrix();
//    glTranslatef(0,3,0);
//    glutWireSphere(0.3, 10, 10);
//    glPopMatrix();
//    
//    //body
//    glPushMatrix();
//    glTranslatef(0, 0.8, 0.0);
//    glScalef(1, 3, 1);
//    glutWireCube(1.0);
//    glPopMatrix();
//    ///////////
//    
//
//    glPushMatrix();
//    glTranslatef(1, 1.8, 0.0);
//    glRotatef((GLfloat)shoulder, 0.0, 0.0, 1.0);
//    glTranslatef(1.0, 0.0, 0.0);
//    glPushMatrix();
//    glScalef(1.5, 0.25, 1.0);
//    glutWireCube(1.0);
//    glPopMatrix();
//    
//    glTranslatef(0.5, 0.0, 0.0);
//    glRotatef((GLfloat)elbow, 0.0, 0.0, 1.0);
//    glTranslatef(1.0, 0.0, 0.0);
//    glPushMatrix();
//    glScalef(1.5, 0.25, 1.0);
//    glutWireCube(1.0);
//    glPopMatrix();
//    
//    //Draw finger flang 1 
//    glPushMatrix();
//    glTranslatef(0.75, -0.1, -0.15);
//    glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
//    glTranslatef(0.15, 0.0, 0.0);
//    glPushMatrix();
//    glScalef(0.3, 0.1, 0.1);
//    glutWireCube(1);
//    glPopMatrix();
//
//    //Draw finger flang 2 
//    glPushMatrix();
//    glTranslatef(0.0, 0.0, 0.18);
//    glScalef(0.3, 0.1, 0.1);
//    glutWireCube(1);
//    glPopMatrix();
//
//    //Draw finger flang 3
//    glPushMatrix();
//    glTranslatef(0.0, 0.0, 0.38);
//    glScalef(0.3, 0.1, 0.1);
//    glutWireCube(1);
//    glPopMatrix();
//
//    //Draw finger flang 4
//    glPushMatrix();
//    glTranslatef(0.0, 0.0, -0.18);
//    glScalef(0.3, 0.1, 0.1);
//    glutWireCube(1);
//    glPopMatrix();
//
//    //Draw finger flang 1 up 
//    glTranslatef(0.15, 0.0, 0.0);
//    glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
//    glTranslatef(0.15, 0.0, 0.0);
//    glPushMatrix();
//    glScalef(0.3, 0.1, 0.1);
//    glutWireCube(1);
//    glPopMatrix();
//    //Draw finger flang 2 up 
//    glPushMatrix();
//    glTranslatef(0.0, 0.0, 0.18);
//    glScalef(0.3, 0.1, 0.1);
//    glutWireCube(1);
//    glPopMatrix();
//    //Draw finger flang 3 up
//    glPushMatrix();
//    glTranslatef(0.0, 0.0, 0.38);
//    glScalef(0.3, 0.1, 0.1);
//    glutWireCube(1);
//    glPopMatrix();
//    //Draw finger flang 4 up
//    glPushMatrix();
//    glTranslatef(0.0, 0.0, -0.18);
//    glScalef(0.3, 0.1, 0.1);
//    glutWireCube(1);
//    glPopMatrix();
//
//    glPopMatrix();
//
//    //Draw finger flang 5
//    glTranslatef(0.75, 0.0, 0.4);
//    glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
//    glTranslatef(0.15, 0.0, 0.0);
//    glPushMatrix();
//    glScalef(0.3, 0.1, 0.1);
//    glutWireCube(1);
//    glPopMatrix();
//    //Draw finger flang 5 up
//    glTranslatef(0.15, 0.0, 0.0);
//    glRotatef((GLfloat)fingerUp, 0.0, 1.0, 0.0);
//    glTranslatef(0.15, 0.0, 0.0);
//    glPushMatrix();
//    glScalef(0.3, 0.1, 0.1);
//    glutWireCube(1);
//    glPopMatrix();
//    glPopMatrix(); //elkbera awyy
//
//    //SHOULDER_2
//   glPushMatrix();
//
//   glTranslatef(-2.8, 1.8, 0.0);
//   glRotatef((GLfloat)shoulder, 0.0, 0.0, -1.0);
//   glTranslatef(1.0, 0.0, 0.0);
//   glPushMatrix();
//   glScalef(1.5, 0.25, 1.0);
//   glutWireCube(1.0);
//   glPopMatrix();
//   //elbow_2   
// 
//   //glTranslatef(-3, 0.0, 0.0);
//   //glRotatef((GLfloat)elbow, 0.0, 0.0, -1.0);
//   //glTranslatef(1.0, 0.0, 0.0);
//   //glPushMatrix();
//   //glScalef(2.0, 0.6, 1.0);
//   //glutWireCube(1.0);
//   //glPopMatrix();
//
//
//   glPopMatrix();
//
//
//   //LEGS
//
//   glTranslatef(0.35, 2.8, 0.0);
//   glRotatef((GLfloat)leg, 0.0, 0.0, 1.0);
//   glTranslatef(0.0, -5, 0.0);
//   glPushMatrix();
//   glScalef(0.3, 3, 1);
//   glutWireCube(1.0);
//   glPopMatrix();
//
//   glPushMatrix();
//   glScalef(0.3, 3, 1);
//   glTranslatef(-2.3, 0.0, 0.0);
//
//   glutWireCube(1.0);
//   glPopMatrix();
//
//
//
//
//    glPopMatrix();
//    glutSwapBuffers();
//}
//
//void reshape(int w, int h)
//{
//    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(85.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    glTranslatef(0.0, 0.0, -5.0);
//}
//
//void keyboard(unsigned char key, int x, int y)
//{
//    switch (key)
//    {
//    case 's':
//        if (shoulder < 90 or shoulder2 > 180)
//        {
//            shoulder = (shoulder + 5) % 360;
//            shoulder2 = (shoulder2 - 5) % 360;
//            glutPostRedisplay();
//
//        }
//        break;
//    case 'S':
//        if (shoulder > -90)
//        {
//            shoulder = (shoulder - 5) % 360;
//            glutPostRedisplay();
//        }
//        break;
//    case 'e':
//        if (elbow < 140)
//        {
//            elbow = (elbow + 5) % 360;
//            glutPostRedisplay();
//        }
//        break;
//    case 'E':
//        if (elbow > 0)
//        {
//            elbow = (elbow - 5) % 360;
//            glutPostRedisplay();
//        }
//        break;
//    case 'f':
//        if (fingerBase < 45)
//        {
//            fingerBase = (fingerBase + 5) % 360;
//            glutPostRedisplay();
//        }
//        break;
//    case 'F':
//        if (fingerBase > 0)
//        {
//            fingerBase = (fingerBase - 5) % 360;
//            glutPostRedisplay();
//        }
//        break;
//    case 'g':
//        if (fingerUp < 90)
//        {
//            fingerUp = (fingerUp + 5) % 360;
//            glutPostRedisplay();
//        }
//        break;
//    case 'G':
//        if (fingerUp > 0)
//        {
//            fingerUp = (fingerUp - 5) % 360;
//            glutPostRedisplay();
//        }
//        break;
//
//    case 27:
//        exit(0);
//        break;
//    default:
//        break;
//    }
//}
//
//static void mouse(int button, int state, int x, int y)
//{
//    if (button == GLUT_LEFT_BUTTON) {
//        if (state == GLUT_DOWN) {
//            moving = 1;
//            startx = x;
//            starty = y;
//        }
//        if (state == GLUT_UP) {
//            moving = 0;
//        }
//    }
//}
//
//
//static void motion(int x, int y)
//{
//    if (moving) {
//        angle = angle + (x - startx);
//        angle2 = angle2 + (y - starty);
//        startx = x;
//        starty = y;
//        glutPostRedisplay();
//    }
//}
//
//
//
//int main(int argc, char** argv)
//{
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
//    glutInitWindowSize(500, 500);
//    glutInitWindowPosition(100, 100);
//    glutCreateWindow(argv[0]);
//    init();
//    glutMouseFunc(mouse);
//    glutMotionFunc(motion);
//    glutDisplayFunc(display);
//    glutReshapeFunc(reshape);
//    glutKeyboardFunc(keyboard);
//    glutMainLoop();
//    return 0;
//}
