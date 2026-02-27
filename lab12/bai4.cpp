#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265358979324

// Độ phân giải của lưới
#define M  50  
#define N  20

/* * Để tạo hình nón cụt như ảnh minh họa, bán kính phải thay đổi theo chiều cao j.
 * j = 0 (đáy dưới): Bán kính lớn.
 * j = N (đáy trên): Bán kính nhỏ hơn.
 */
#define RADIUS(j) (1.2 - 0.6 * (float)j/N)

#define FNX(i,j) ( RADIUS(j) * cos( ( (float) i/M ) * 2 * PI) )
#define FNY(i,j) ( RADIUS(j) * sin( ( (float) i/M ) * 2 * PI) ) 
#define FNZ(i,j) ( 3.0 * (float) j/N - 1.5 ) // Dịch chuyển để vật thể nằm trung tâm

// Vector pháp tuyến cho bề mặt nghiêng
#define NORMX(i,j) ( cos( ( (float) i/M ) * 2 * PI) )
#define NORMY(i,j) ( sin( ( (float) i/M ) * 2 * PI) )
#define NORMZ(i,j) ( 0.2 ) 

static GLfloat vdata[3*(M+1)*(N+1)];
static GLfloat ndata[3*(M+1)*(N+1)];

static int Xangle = 30, Yangle = 0, Zangle = 0;

void init (void) 
{
    int i, j, k, l;

    // Thiết lập ánh sáng
    GLfloat ambient[] = { 0.4, 0.4, 0.4, 1.0 };
    GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat position[] = { 0.0, 3.0, 3.0, 1.0 };    
    GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

    glClearColor (0.0, 0.0, 0.0, 1.0);

    k = 0; l = 0;
    for (j = 0; j <= N; j++) {
        for (i = 0; i <= M; i++) {
            vdata[k++] = FNX(i,j); 
            vdata[k++] = FNY(i,j);
            vdata[k++] = FNZ(i,j); 

            ndata[l++] = NORMX(i,j); 
            ndata[l++] = NORMY(i,j); 
            ndata[l++] = NORMZ(i,j); 
        }
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vdata);
    glNormalPointer(GL_FLOAT, 0, ndata);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST); 
}

void display(void)
{
    int i, j;
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    
    GLfloat mat_diffuse_red[] = { 0.9, 0.1, 0.1, 1.0 };
    GLfloat mat_diffuse_green[] = { 0.1, 0.9, 0.1, 1.0 };

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt (0.0, 4.0, 7.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glRotatef ((GLfloat) Xangle, 1.0, 0.0, 0.0);
    glRotatef ((GLfloat) Yangle, 0.0, 1.0, 0.0);
    glRotatef ((GLfloat) Zangle, 0.0, 0.0, 1.0);

    // Sử dụng GL_CW vì thứ tự vẽ trong vòng lặp dưới là Clockwise
    glFrontFace(GL_CW);

    for(j = 0; j < N; j++) {
        if(j % 2 == 0)
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse_red);
        else
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse_green);
        
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

        glBegin(GL_TRIANGLE_STRIP);
        for(i = 0; i <= M; i++) {
            glArrayElement( j*(M+1) + i );
            glArrayElement( (j+1)*(M+1) + i );
        }
        glEnd();
    }
    glutSwapBuffers();
}

void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(45.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard (unsigned char key, int x, int y)
{
    switch (key) {
        case 'x': Xangle = (Xangle + 5) % 360; break;
        case 'X': Xangle = (Xangle - 5) % 360; break;
        case 'y': Yangle = (Yangle + 5) % 360; break;
        case 'Y': Yangle = (Yangle - 5) % 360; break;
        case 'z': Zangle = (Zangle + 5) % 360; break;
        case 'Z': Zangle = (Zangle - 5) % 360; break;
        case 27: exit(0); break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (500, 500); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Lighted Frustum - Red and Green");
    init ();
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;  
}