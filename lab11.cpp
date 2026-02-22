#include <math.h>
#include <GL/glut.h>

#define MAX1 100
#define MAX2 100*100

float Px[MAX1], Py[MAX1]; // Toa do cac dinh
int num = 0;								// num: Tong so dinh dieu khien

float echCastelX[MAX2], echCastelY[MAX2];	// Cac diem tinh theo giai thuat De Casteljau
int nbEchCastel = 40;						// So diem tinh toan ve duong cong bang gia thuat De Casteljau

int idxInsertion = 0;
float ptCurrentX, ptCurrentY;

int edition_on = 0; 
double distance = 5;

void casteljau (float Px[MAX1], float Py [MAX1], int Pn, float t, 
				float castelX[MAX1][MAX1], float castelY[MAX1][MAX1])   // Chua gia tri tinh theo giai thuat De Castejau
{
    int i,j;          
    for (j=0; j<Pn; j++)  // Khoi tao dong dau tien trong mang
    {
        castelX[0][j] = Px[j];
        castelY[0][j] = Py[j];
    }
       
    for (i=1; i<Pn; i++)
    for (j=0; j<(Pn-i); j++)
    {     
        castelX[i][j] = (1-t) * castelX[i-1][j] + t * castelX[i-1][j+1];
        castelY[i][j] = (1-t) * castelY[i-1][j] + t * castelY[i-1][j+1];
    }
}

void pointsDeCasteljau (float Px[MAX1], float Py [MAX1], int Pn, float echX[MAX2], float echY[MAX2], int nb)
{
    float castelX[MAX1][MAX1];
    float castelY[MAX1][MAX1];

    for (int i=0; i<(nb+1); i++) 
    {
        // Tinh giai thuat De Casteljau theo tham so i/nb
        casteljau(Px, Py, Pn, ((float)i)/((float)nb), castelX, castelY);
        
        // Them diem cuong cung vao danh sach
        echX[i] = castelX[Pn - 1][0];
        echY[i] = castelY[Pn - 1][0];
    }
}

// Ve duong cong Bezier theo giai thuat De Castejau
void showBezier (float echX[MAX2], float echY[MAX2], int nb)
{
    glColor3f(1.0, 0.0, 1.0); 
    glBegin(GL_LINE_STRIP);
	for (int i=0; i<(nb+1); i++) glVertex2d(echX[i], echY[i]);            
    glEnd();
}


void initList (void)  // Khoi tao danh sach
{      
    for (int i=0; i<MAX1; i++)
    {
        Px[i] = 0;
        Py[i] = 0;
    }
    num = 0;
    idxInsertion = 0;
}

void showList (GLenum mode)
{      
    if (num != 0)
    {
        glBegin (mode);
        for (int i = 0; i < num; i++) glVertex2d (Px[i], Py[i]);
        glEnd ();
    }
}

double distancePoints (int p1X, int p1Y, int p2X, int p2Y)	// Tinh khoang cach giua hai diem
{
    return sqrt	(((p2X - p1X) * (p2X - p1X)) + ((p2Y - p1Y) * (p2Y - p1Y)));
}

int findPoint (int pX, int pY, double dist) // Tim kiem diem dau tien dua theo khoang cach
{
    int i = num - 1;
    while ((i >= 0) && (distancePoints (pX, pY, Px[i], Py[i]) >= dist))i--;
    return i;
}

void moveRight (int indice) // Dich chuyen phai
{
    for (int i = num - 1; i >= indice; i--) {
    	Px[i+1] = Px[i];
    	Py[i+1] = Py[i];
    
    }
    num++;
}

int insertPoint (int i, int pX, int pY)
{
    if (num >= MAX1) return -1;
    else
    {
        moveRight(i);	
	    Px[i] = pX;
	    Py[i] = pY;
    }    
    return i;
}

void display(void)
{    
    glClear ( GL_COLOR_BUFFER_BIT);     
    glPointSize (3.0);				
    glColor3f (0.0, 0.0, 0.0);		
    showList (GL_LINE_STRIP);	
    glColor3f (1.0, 0.0, 0.0);		
    showList (GL_POINTS);
   
    if (num>=2)
    {
        pointsDeCasteljau (Px, Py, num, echCastelX, echCastelY, nbEchCastel);
        showBezier (echCastelX, echCastelY, nbEchCastel);
    }
  
    glFlush();
}

void keyboard ( unsigned char key, int x, int y )
{    
    switch ( key )
    {	
		case 27: exit (0); break; //Nhan phim ESC                    
        case 'e': initList(); break;                                  
    }    
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{

    int pX = (float) x;
    int pY = (float) (glutGet(GLUT_WINDOW_HEIGHT) - y);
    
    switch(button)
    {
		case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
            {
                edition_on = findPoint (pX, pY, distance);
                ptCurrentX = pX;
                ptCurrentY = pY;                
            }
			
            else if (state == GLUT_UP)
            {                
                if (edition_on == -1)
                {
                    if (insertPoint(idxInsertion, ptCurrentX, ptCurrentY) != -1) idxInsertion++;
                }                
            } 
            break;
	}
    glutPostRedisplay();
}

void redisplay(int width, int height)
{
    glViewport(0, 0, width, height);  
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
	glutInitWindowSize(640, 480);
  glutInitWindowPosition(400, 50);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display); 
	glutMouseFunc(mouse); 
	glutKeyboardFunc(keyboard); 
	glutReshapeFunc(redisplay);   
    glClearColor (1.0, 1.0, 1.0, 1.0);
	glutMainLoop();   
    return 0; 
}
 
