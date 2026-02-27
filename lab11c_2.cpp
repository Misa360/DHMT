#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Gl/glut.h"


#define MAX 100
#define MAX2 10000

float Px[MAX] = {1,2,3,4,5,6};
float Py[MAX] = {1,2,1,2,1,2};
float Pz[MAX] = {1,2,3,4,5,6};

int Pn = 6; // So dinh dieu khien
int degre = 3;

/* echantillons de points ˆ tracer */
float echX[MAX2], echY[MAX2], echZ[MAX2]; /* points trouvŽs par le calcul */
int nbEch = 5; 

// Ham tinh giai thua
int fact(int n)
{
    int r = 1;
    for (int i=0; i<n; i++) r = r * (n-i);
    return r;
}


float basicBsplineUniform(int m, int i, float t)
{
    int k;
    float s = 0;
    
    for (k=0; k<=(m-i); k++)
        s = s + (float)pow(-1,k)*((float)(pow(t+m-i-k,m)) / (float)(fact(k) * fact(m-k+1)));
    
    return (m+1) * s;
}


void calcBspline(float Px[MAX], float Py [MAX], float Pz [MAX], int m, int k, float t, float *x, float *y, float *z)
{
    int i;
    float coeff;
        
    //Khoi tao diem
    (*x) = 0;
    (*y) = 0;
    (*z) = 0;
    
    for (i=0; i<=m; i++)
    {
        // Tinh toan cac gia tri cua ham Bernstein theo tham so u
        coeff = basicBsplineUniform(m, i, t);
        
        // Them vào danh sach cac diem be mat
        (*x) = (*x) + (coeff * Px[k+i]);
        (*y) = (*y) + (coeff * Py[k+i]);
        (*z) = (*z) + (coeff * Pz[k+i]);
    }
}


void pointsBspline (float Px[MAX], float Py[MAX], float Pz[MAX], int Pn, int m, float echX[MAX2], float echY[MAX2], float echZ[MAX2], int nb)
{
    int i;
    float x, y, z;
    int k;
    
    for (k=0; k<=Pn-m-1; k++)
    {
        for (i=0; i<(nb+1); i++)
        {
            calcBspline(Px, Py, Pz, m, k, ((float)i)/((float)nb), &x, &y, &z);
            echX[i+k*(nb+1)] = x;
            echY[i+k*(nb+1)] = y;
            echZ[i+k*(nb+1)] = z;
        }
    }
}

void drawBsplinePolygon()
{
    int i;
    
    if (Pn != 0)
    {
        glBegin (GL_LINE_STRIP);
            for (i = 0; i < Pn; i++) glVertex3d (Px[i], Py[i], Pz[i]);
        glEnd ();
    }
}

void drawBsplineCurve (float echX[MAX2], float echY[MAX2], float echZ[MAX2], int nb, int k)
{
    int i;
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_LINE_STRIP);
        for (i=0; i<(nb+1)*k; i++) glVertex3d(echX[i], echY[i], echZ[i]);
    glEnd();
}


void init(void) 
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);     
	glShadeModel(GL_FLAT);
}

void drawAxis()
{
    float h = 7.0f;
    glLineWidth(1);
    
    glBegin(GL_LINES);
        glColor3d(1, 0.0f, 0.0f); //Red Ox
        glVertex3f(0, 0.0f, 0.0f);
        glVertex3f(h, 0.0f, 0.0f);
        
        glColor3d(0,1, 0.0f); //Green Oy
        glVertex3f(0, 0.0f, 0.0f);
        glVertex3f(0,h, 0.0f);
        
        glColor3d(0, 0.0f,1); //Blue Oz
        glVertex3f(0, 0.0f, 0.0f);
        glVertex3f(0, 0.0f,h);
     glEnd();
     
}

float speed = 1.0f;
bool mouseLeftDown = false, mouseRightDown = false ;
float mouseX = 0, mouseY = 0;
float cameraAngleX =0, cameraAngleY = 0;
float cameraDistance = 6.0f;
static float angleX = 0.0, angleY = 0.0, angleZ = 0.0;

void display(void)
{
	// xoa color buffer va depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix(); // luu lai ma tran hien hanh   
    
  	glRotatef((GLfloat) angleY, 0.0, 1.0, 0.0);
	glRotatef((GLfloat) angleX, 1.0, 0.0, 0.0);
	glRotatef((GLfloat) angleZ, 0.0, 0.0, 1.0);

	glRotatef(cameraAngleY*speed, 0, 0, 1);
    glRotatef(cameraAngleX*speed, 1, 0, 0);
    
	drawAxis();	
	
	drawBsplinePolygon();
    pointsBspline(Px, Py, Pz, Pn, degre, echX, echY, echZ, nbEch);
    drawBsplineCurve(echX, echY, echZ, nbEch, Pn-degre);
        
	glPopMatrix(); 
	glutSwapBuffers();

}

void reshape(int w, int h)
{
	glViewport(0, 0,(GLsizei) w,(GLsizei) h); 
	
	glMatrixMode(GL_PROJECTION);   
	glLoadIdentity();
	//gluPerspective(60.0,(GLfloat)w/(GLfloat)h, 1.0, 20.0);
	gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 128.0);

	glMatrixMode(GL_MODELVIEW);   
	glLoadIdentity();

	gluLookAt(8.0, 8.0, 8.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	//gluLookAt(eyex, eyey, eyez, atx, aty, atz, upx, upy, upz);
	
}//reshape

#define KEY_ESCAPE 27
void keyboard(unsigned char key, int x, int y)
{   
	switch(key) {  

		case 'x':  
			angleX += 1.0;           
			glutPostRedisplay();         
			break; 
		case 'X':  
			angleX += -1.0;           
			glutPostRedisplay();         
			break; 
		case 'y':  
			angleY += 1.0;           
			glutPostRedisplay();         
			break; 
		case 'Y':  
			angleY += -1.0;           
			glutPostRedisplay();         
			break; 
		case 'z':  
			angleZ += 1.0;           
			glutPostRedisplay();         
			break; 			
		case 'Z':  
			angleZ += -1.0;           
			glutPostRedisplay();         
			break; 

		case KEY_ESCAPE:
            //exit( 0 );  
			mouseX = 0; mouseY = 0;
			cameraAngleX = 0; cameraAngleY = 0;
			cameraDistance = 6.0f;
			angleY = 0.0; angleX = 0.0; angleZ = 0.0;	
		default:         
		break;
	}

}//keyboard

void timer(int millisec)
{
    glutTimerFunc(millisec, timer, millisec);
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    mouseX = x;
    mouseY = y;

    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            mouseLeftDown = true;
        }
        else if (state == GLUT_UP)
            mouseLeftDown = false;
    }

    else if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            mouseRightDown = true;
        }
        else if (state == GLUT_UP)
            mouseRightDown = false;
    }
    glutPostRedisplay();
}


void mouseMotion(int x, int y)
{
    if (mouseLeftDown)
    {
        cameraAngleY +=(x - mouseX);
        cameraAngleX +=(y - mouseY);
        mouseX = x;
        mouseY = y;
    }
    if (mouseRightDown)
    {
        cameraDistance -=(y - mouseY) * 0.2f;
        mouseY = y;
    }
}


int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);   
	glutInitWindowSize(500, 500);    
	glutInitWindowPosition(100, 100);   
	glutCreateWindow("sv102240070 sv102240079 sv102240101");   
	
	init();
	
	glutDisplayFunc(display);    
	
	glutReshapeFunc(reshape);   
	glutKeyboardFunc(keyboard);  

	glutTimerFunc(33, timer, 33);             // redraw only every given millisec
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
		 
	glutMainLoop();   
	return 0;

}//main
