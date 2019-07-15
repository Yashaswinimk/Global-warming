#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<GL/glut.h>
#define PI 3.1416
GLfloat y=-1.45,yc=-1.38;
GLfloat dy=0.01;
float theta = 0;
GLfloat val=1;
struct point
{
    GLfloat x, y, z;
};

void update()
{
    GLfloat temp=-1.45,temp1=-1.38;
    y=y-dy;
    yc=yc-dy;
    if(y<-3.5&&yc<-3.5)
    {
      y=temp;
      yc=temp1;
    }

}

void drawbox(float w,float h,float d)
{
    glBegin(GL_QUADS);
        //front
        glColor3f(0.862,0.952,1);
        glVertex3f(0.f,h,d);
        glVertex3f(0.f,0.f,d);
        glVertex3f(w,0.f,d);
        glVertex3f(w,h,d);

        //back
        glColor3f(0.862,0.952,1);
        glVertex3f(0.f,h,0.f);
        glVertex3f(w,h,0.f);
        glVertex3f(w,0.f,0.f);
        glVertex3f(0.f,0.f,0.f);

        //top
        glColor3f(0.624,0.819,0.827);
        glVertex3f(0.f,h,0.f);
        glVertex3f(0.f,h,d);
        glVertex3f(w,h,d);
        glVertex3f(w,h,0.f);

        //bottom
        glColor3f(0.729,0.949,0.937);
        glVertex3f(0.f,0.f,0.f);
        glVertex3f(w,0.f,0.f);
        glVertex3f(w,0.f,d);
        glVertex3f(0.f,0.f,d);

        //left
        glColor3f(0.624,0.819,0.827);
        glVertex3f(0.f,h,0.f);
        glVertex3f(0.f,0.f,0.f);
        glVertex3f(0.f,0.f,d);
        glVertex3f(0.f,h,d);

        //right
        glColor3f(0.862,0.952,1);
        glVertex3f(w,h,d);
        glVertex3f(w,0.f,d);
        glVertex3f(w,0.f,0.f);
        glVertex3f(w,h,0.f);

    glEnd();
}

int factorial(int n)
{
    if (n<=1)
        return(1);
    else
        n=n*factorial(n-1);
    return n;
}

void computeNcR(int n, int *hold_ncr_values)
{
    int r;
    for(r=0; r<=n; r++) //start from nC0, then nC1, nC2, nC3 till nCn
    {
        hold_ncr_values[r] = factorial(n) / (factorial(n-r) * factorial(r));
    }
}

void computeBezierPoints(float t, point *actual_bezier_point, int number_of_control_points, point *control_points_array, int *hold_ncr_values)
{
    int i, n = number_of_control_points-1;

    float bernstein_polynomial;

    actual_bezier_point -> x = 0;
    actual_bezier_point -> y = 0;
    actual_bezier_point -> z = 0;

    for(i=0; i < number_of_control_points; i++)
    {
        bernstein_polynomial = hold_ncr_values[i] * pow(t, i) * pow( 1-t, n-i);

        actual_bezier_point -> x += bernstein_polynomial * control_points_array[i].x;
        actual_bezier_point -> y += bernstein_polynomial * control_points_array[i].y;
        actual_bezier_point -> z += bernstein_polynomial * control_points_array[i].z;
    }
}

void bezier(point *control_points_array, int number_of_control_points, int number_of_bezier_points)
{
    point actual_bezier_point;
    float t;
    int *hold_ncr_values, i;

    hold_ncr_values = new int[number_of_control_points]; // to hold the nCr values

    computeNcR(number_of_control_points-1, hold_ncr_values); // call nCr function to calculate nCr values
glLineWidth(10);
    glBegin(GL_LINE_STRIP);
        for(i=0; i<=number_of_bezier_points; i++)
        {
            t=float(i)/float(number_of_bezier_points);

            computeBezierPoints(t, &actual_bezier_point, number_of_control_points, control_points_array, hold_ncr_values);
           // glLineWidth(100);
            //glBegin(GL_LINES);
            glVertex2f(actual_bezier_point.x, actual_bezier_point.y);
            //glEnd();
            //glFlush();
        }
    glEnd();
    glFlush();
    delete[] hold_ncr_values;
}

void myReshape(int w,int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(w<=h)
        glOrtho(-2, 2, -2*(GLfloat)h/(GLfloat)w, 2*(GLfloat)h/(GLfloat)w, -10, 10);
    else
        glOrtho(-2*(GLfloat)w/(GLfloat)h, 2*(GLfloat)w/(GLfloat)h, -2, 2, -10, 10);

    glMatrixMode(GL_MODELVIEW);
    //glutPostRedisplay();
}
void display1()
{
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);->this will clear the screen so the text will disappear.
       glDisable(GL_DEPTH_TEST);//otherwise half of the background covers the text.
       glColor3f(1.0f,0.0f,0.0f);
       glBegin(GL_QUADS);
               glVertex2f(-5, 5);
               glColor3f(1.0f,1.0f,0.0f);
               glVertex2f(-5, -5);
               glColor3f(1.0f,1.0f,0.0f);
               glVertex2f(5, -5);
               glColor3f(1.0f,0.0f,0.196f);
               glVertex2f(5.0, 5.0);
       glEnd();
       //glutSwapBuffers();-> we are calling it anyway in the display func.
}
void water()
{

    int number_of_control_points = 4, number_of_bezier_points = 20;

    point control_points_array[4] = {{-5.5, -1.5, 0}, {-4, -1, 0}, {0, -2.4, 0},{6, -1.5, 0}};

    control_points_array[1].x += sin(theta * PI/180.0);
    control_points_array[1].y += sin(theta * PI/180.0);

    control_points_array[2].x -= sin((theta+30) * PI/180.0);
    control_points_array[2].y -= sin((theta+30) * PI/180.0);

    control_points_array[3].x -= sin((theta-30) * PI/180.0);
    control_points_array[3].y += sin((theta-30) * PI/180.0);

    theta += 1;

    glLineWidth(10);
    glPushMatrix();
    glPointSize(100);
    glTranslated(0,-0.001,0);
    glColor3f(0.709f,0.956f,0.952f); //Indian flag: Saffron color code
    for(int i=0; i<1000; i++)
    {
        glTranslatef(0,-0.001,0);//glColor3f(0.709f,0.956f,0.952f);
        glEnable(GL_LINE_WIDTH);
        bezier(control_points_array, number_of_control_points, number_of_bezier_points);
    }
    glFlush();
    glPopMatrix();
    /*glPushMatrix();
    glColor3f(0.709f,0.956f,0.952f);
    glTranslated(0,-0.001,0);
    for(int i=0; i<100; i++)
    {
      glTranslatef(0, -0.001, 0);
      bezier(control_points_array, number_of_control_points, number_of_bezier_points);
    }

    /*glColor3f(0.709f,0.956f,0.952f);
    glTranslated(0,-0.001,0); //Indian flag: green color code
    for(int i=0; i<100; i++)
    {
        glTranslatef(0, -0.001, 0);
        bezier(control_points_array, number_of_control_points, number_of_bezier_points);
    }*/
    glFlush();
    glPopMatrix();
    //glLineWidth(10);
}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    display1();//for the background
    water();
    glEnable(GL_DEPTH_TEST);//otherwise the text looks wierd.
                            //and since we are calling it after display1(), the background will come properly.
    glScalef(val,val,val);
    glRotatef(10,1,0,0); //tilt xaxis by 10 deg. basically rotating the camera wrt to the corresponding axis
    glRotatef(10,0,1,0); //tilt yaxis by 5 deg.
    //glRotatef(-5,0,0,1); //tilt z axis by -5 deg.


    //F
    glTranslatef(-3,0.5,0);
    drawbox(0.2,1,-0.3);

    glPushMatrix();
    glTranslatef(0,1,0);
    drawbox(0.8,0.2,-0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0.5,0);
    drawbox(0.5,0.2,-0.3);
    glPopMatrix();


    //u
    glTranslatef(1,0,0);
    drawbox(0.2,1,-0.3);

    glPushMatrix();
    glTranslatef(0,0,0);
    drawbox(0.6,0.2,-0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.6,0,0);
    drawbox(0.2,1,-0.3);
    glPopMatrix();

    //t
    glTranslatef(1,0.9,0);
    drawbox(1,0.2,-0.3);

    glPushMatrix();
    glTranslatef(0.4,0,0);
    drawbox(0.2,-0.9,-0.3);
    glPopMatrix();

    //u


    //glScalef(val,val,val);
    glTranslatef(1.2,-0.9,0);

    drawbox(0.2,1,-0.3);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0,0,0);

    drawbox(0.6,0.2,-0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.6,0,0);
    drawbox(0.2,1,-0.3);
    glPopMatrix();


    //R
    glTranslatef(1,0,0);
    drawbox(0.2,0.9,-0.3);

    glPushMatrix();
    glTranslatef(0,0.9,0);
    drawbox(0.7,0.2,-0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.5,0.6,-0.3);
    drawbox(0.2,0.4,0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.1,0.5,0);
    drawbox(0.6,0.2,-0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.1,0.5,0);
    glRotatef(45,0,0,1);
    drawbox(0.2,-0.8,-0.3);
    glPopMatrix();


    //E
    glTranslatef(1,0,0);
    drawbox(0.2,0.9,-0.3);

    glPushMatrix();
    glTranslatef(0,0.9,0);
    drawbox(0.7,0.2,-0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0.45,0);
    drawbox(0.5,0.2,-0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,0);
    drawbox(0.7,0.2,-0.3);
    glPopMatrix();

    //drop
    static int slices=10;
    static int stacks=10;
    const double t=glutGet(GLUT_ELAPSED_TIME)/1000.0;
    const double a=t*90.0;

    //drop f
    glColor3f(0.709f,0.956f,0.952f);
    glPushMatrix();
        glTranslated(-4,yc,-6);
        glRotated(80,-1,0,0);
        //glRotated(a,0,0,1);
        glutSolidCone(0.12,0.18,slices,stacks);
    glPopMatrix();
    glColor3f(0.709f,0.956f,0.952f);
    glPushMatrix();
        glTranslated(-4,y,-6);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        //glTranslatef(-4,y1,-6);
        glutSolidSphere(0.145,slices,stacks);
    glPopMatrix();
    update();

    //drop u
        glColor3f(0.709f,0.956f,0.952f);
    glPushMatrix();
        glTranslated(-2.8,yc,-6);
        glRotated(80,-1,0,0);
        //glRotated(a,0,0,1);
        glutSolidCone(0.12,0.18,slices,stacks);

    glPopMatrix();
    glColor3f(0.709f,0.956f,0.952f);
    glPushMatrix();
        glTranslated(-2.8,y,-6);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidSphere(0.145,slices,stacks);
    glPopMatrix();
    update();

    //drop t
        glColor3f(0.709f,0.956f,0.952f);
    glPushMatrix();
        glTranslated(-1.6,yc,-6);
        glRotated(80,-1,0,0);
        //glRotated(a,0,0,1);
        glutSolidCone(0.12,0.18,slices,stacks);

    glPopMatrix();
    glColor3f(0.709f,0.956f,0.952f);
    glPushMatrix();
        glTranslated(-1.6,y,-6);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidSphere(0.145,slices,stacks);
    glPopMatrix();
    //Sleep(600);
    update();

    //drop u
        glColor3f(0.709f,0.956f,0.952f);
    glPushMatrix();
        glTranslated(-0.6,yc,-6);
        glRotated(80,-1,0,0);
        //glRotated(a,0,0,1);
        glutSolidCone(0.12,0.18,slices,stacks);

    glPopMatrix();
    glColor3f(0.709f,0.956f,0.952f);
    glPushMatrix();
        glTranslated(-0.6,y,-6);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidSphere(0.145,slices,stacks);
    glPopMatrix();
    //Sleep(600);
    update();

    //drop r
        glColor3f(0.709f,0.956f,0.952f);
    glPushMatrix();
        glTranslated(0.5,yc,-6);
        glRotated(80,-1,0,0);
        //glRotated(a,0,0,1);
        glutSolidCone(0.12,0.18,slices,stacks);

    glPopMatrix();
    glColor3f(0.709f,0.956f,0.952f);
    glPushMatrix();
        glTranslated(0.5,y,-6);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidSphere(0.145,slices,stacks);
    glPopMatrix();
    //Sleep(800);
    update();

    //drop e
        glColor3f(0.709f,0.956f,0.952f);
    glPushMatrix();
        glTranslated(1.5,yc,-6);
        glRotated(80,-1,0,0);
        //glRotated(a,0,0,1);
        glutSolidCone(0.12,0.18,slices,stacks);

    glPopMatrix();
    glColor3f(0.709f,0.956f,0.952f);
    glPushMatrix();
        glTranslated(1.5,y,-6);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidSphere(0.145,slices,stacks);
    glPopMatrix();
    //Sleep(800);
    update();

    // glEnable(GL_DEPTH_TEST);
    glutPostRedisplay();
    glutSwapBuffers();
}
void update1(int value)
{
    val-=0.05;
    if(val<=0.6)
    {
        val+=0.5;
    }


    glutTimerFunc(1000,update1,0);
    glutPostRedisplay();
}
void init()
{
    glClearColor(1,1,1,1);//setting to white
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(0,500,0,500,0,500);
    glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char **argv)
{

    glutInit(&argc, argv);                      // initialize the graphics system
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);  // SINGLE also works
    glutInitWindowSize(1200, 500);    // 800 by 800 size..you can change it
    glutInitWindowPosition(0, 0);       // where do you wanna see your window
    glutCreateWindow("STOP GLOBAL WARMING");     // title

    init();
    glutReshapeFunc(myReshape);                                           // initialize the canvas
    //calling the display1 function here does not work,the entire screen turns to black and no text is displayed
    //even though we are calling it before display().
    glutDisplayFunc(display);            // call display function

    //glEnable(GL_DEPTH_TEST);

    glutTimerFunc(10000,update1,0);
    glutMainLoop();
    return 0;                             // run forever
}

