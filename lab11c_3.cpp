#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Gl/glut.h"


#define MAX1 100
#define MAX2 1000


typedef struct
	{
		double x;
		double y;
    	double z;
	} POINT3D;
	
POINT3D Point[MAX1][MAX1];
int Nu = 4; 			// So dinh polygon control
int Nv = 4;
int degreU = 3;
int degreV = 3;

POINT3D ech[MAX2][MAX2]; 	// Cac diem trên B-spline surface
int nbEch = 5; 			// 

void polygonPoints(void)
{
    Point[0][0].x = -3; Point[0][0].y = -3; Point[0][0].z = 0;
    Point[1][0].x = -1; Point[1][0].y = -3; Point[1][0].z = 1;
    Point[2][0].x = 1; Point[2][0].y = -3; Point[2][0].z = 1;
    Point[3][0].x = 3; Point[3][0].y = -3; Point[3][0].z = 0;
    
    Point[0][1].x = -3; Point[0][1].y = -1; Point[0][1].z = 1;
    Point[1][1].x = -1; Point[1][1].y = -1; Point[1][1].z = 2;
    Point[2][1].x = 1; Point[2][1].y = -1; Point[2][1].z = 2;
    Point[3][1].x = 3; Point[3][1].y = -1; Point[3][1].z = 1;
    
    Point[0][2].x = -3; Point[0][2].y = 1; Point[0][2].z = 1;
    Point[1][2].x = -1; Point[1][2].y = 1; Point[1][2].z = 2;
    Point[2][2].x = 1; Point[2][2].y = 1; Point[2][2].z = 2;
    Point[3][2].x = 3; Point[3][2].y = 1; Point[3][2].z = 1;

    Point[0][3].x = -3; Point[0][3].y = 3; Point[0][3].z = 0;
    Point[1][3].x = -1; Point[1][3].y = 3; Point[1][3].z = 1;
    Point[2][3].x = 1; Point[2][3].y = 3; Point[2][3].z = 1;
    Point[3][3].x = 3; Point[3][3].y = 3; Point[3][3].z = 0;
}

// Ham tinh giai thua
int fact(int n)
{
    int r = 1;
    for (int i=0; i<n; i++) r = r * (n-i);
    return r;
}


float basicBSplineUniform(int m, int i, float t)
{
    int k;
    float s = 0;
    
    for (k=0; k<=(m-i); k++)
        s = s + (float)pow(-1,k)*((float)(pow(t+m-i-k,m)) / (float)(fact(k) * fact(m-k+1)));
    
    return (m+1) * s;
}


void bsplinePoint(POINT3D Point[MAX1][MAX1], int n, int m, int k1, int k2, float u, float v, POINT3D *P)
{
    int i,j;
    float coeff1, coeff2;
        
   // Khoi tao gia tri
    (*P).x = 0;
    (*P).y = 0;
    (*P).z = 0;
    
    for (i=0; i<=n; i++)
    {
        // Tinh theo da thuc Bernstein theo tham so u */
        coeff1 = basicBSplineUniform(n, i, u);
        for (j = 0; j <= m; j++)
        {
            /* on calcule la valeur du polynome de Bernstein au parametre v */
            coeff2 = basicBSplineUniform(m, j, v);
            
            /* on ajoute un point de controle (fois) la valeur du polynome */
            (*P).x = (*P).x + (coeff1 * coeff2 * Point[k1+i][k2+j].x);
            (*P).y = (*P).y + (coeff1 * coeff2 * Point[k1+i][k2+j].y);
            (*P).z = (*P).z + (coeff1 * coeff2 * Point[k1+i][k2+j].z);
        }
    }
}


void calcBSplineSurface(POINT3D Point[MAX1][MAX1], int Nu, int Nv, int n, int m, POINT3D ech[MAX2][MAX2], int nb)
{
    int i,j;
    POINT3D p;
    int k1, k2;
    
    for (k1 = 0; k1 <= Nu-n-1; k1++)
        for (k2 = 0; k2 <= Nv-m-1; k2++)
        {
            /* pour chaque point de l'echantillon sur le carreau k1k2 de B-spline */
            for (i=0; i<(nb+1); i++)
                for (j=0; j<(nb+1); j++)
                {
                    /* on ajoute le point de parametres u = i/nb et v = j/nb */
                    bsplinePoint(Point, n, m, k1, k2, ((float)i)/((float)nb), ((float)j)/((float)nb), &p);
                    ech[i+k1*(nb+1)][j+k2*(nb+1)].x = p.x;
                    ech[i+k1*(nb+1)][j+k2*(nb+1)].y = p.y;
                    ech[i+k1*(nb+1)][j+k2*(nb+1)].z = p.z;
                }//for j
        }//for k2
}

void drawBSplinePolygon()
{
    int i,j;
    if ((Nu != 0) && (Nv != 0))
    {
        for (i = 0; i < Nu; i++)
        {
            glBegin (GL_LINE_STRIP);
                for (j = 0; j < Nv; j++) glVertex3d (Point[i][j].x, Point[i][j].y, Point[i][j].z);
            glEnd ();
        }
        
        for (j = 0; j < Nv; j++)
        {
            glBegin (GL_LINE_STRIP);
                for (i = 0; i < Nu; i++) glVertex3d (Point[i][j].x, Point[i][j].y, Point[i][j].z);
            glEnd ();
        }
    }	
}

void drawBSplineSurface(POINT3D ech[MAX2][MAX2], int nb, int k1, int k2)
{
	int i,j;
        
    glColor3f(0.0, 0.0, 1.0); //Green
    
    for (i=0; i<(nb+1)*k1 - 1; i++)
        for (j=0; j<(nb+1)*k2 - 1; j++)
        {
            glBegin(GL_QUADS);
                glVertex3d(ech[i][j].x, ech[i][j].y, ech[i][j].z);
                glVertex3d(ech[i+1][j].x, ech[i+1][j].y, ech[i+1][j].z);
                glVertex3d(ech[i+1][j+1].x, ech[i+1][j+1].y, ech[i+1][j+1].z);
                glVertex3d(ech[i][j+1].x, ech[i][j+1].y, ech[i][j+1].z);
            glEnd();
        }
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

	glRotatef(cameraAngleY, 0, 0, 1);
    glRotatef(cameraAngleX, 1, 0, 0);
    
	drawAxis();	
	
	polygonPoints();
	drawBSplinePolygon();
    calcBSplineSurface(Point, Nu, Nv, degreU, degreV, ech, nbEch);
    drawBSplineSurface(ech, nbEch, Nu-degreU, Nv-degreV);
        
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
