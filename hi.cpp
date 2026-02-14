#include <stdio.h>
#include <cstring>
#include <cmath>
#include <GL/glut.h>

void init(void) 
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

void myDrawModel()
{
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-2, 0, 0); 
    glVertex3f(0, 0, 1.5);  
    glVertex3f(0, 2.5, 0); 
    glEnd();
}

void printfAffine3DMatrix(int model, const char str[]) {
    GLfloat M[16];
    int row, col, index;
    
    if (model == 1) glGetFloatv(GL_MODELVIEW_MATRIX, M);
    else if (model == 2) glGetFloatv(GL_PROJECTION_MATRIX, M);

    printf("\n[%s]\n", str);
    for (row = 0; row < 4; row++) {
        for (col = 0, index = row; col < 4; col++, index += 4) { 
            printf("%3.3f\t", M[index]); 
        }
        printf("\n");
    }
}

void myTranslatef(float x, float y, float z) {
    float matrix[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1
    };
    glMultMatrixf(matrix);
}

void myFrustum(double l, double r, double b, double t, double n, double f) {
    double matrix[16];
    memset(matrix, 0, sizeof(matrix));

    matrix[0] = (2 * n) / (r - l);
    matrix[5] = (2 * n) / (t - b);
    matrix[8] = (r + l) / (r - l);
    matrix[9] = (t + b) / (t - b);
    matrix[10] = -(f + n) / (f - n);
    matrix[11] = -1;
    matrix[14] = -(2 * f * n) / (f - n);

    glMultMatrixd(matrix);
}

void mygluLookAt(double eyex, double eyey, double eyez, 
                 double centerx, double centery, double centerz, 
                 double upx, double upy, double upz) {
    double f[3] = {centerx - eyex, centery - eyey, centerz - eyez};
    double f_mag = sqrt(f[0]*f[0] + f[1]*f[1] + f[2]*f[2]);
    f[0] /= f_mag; f[1] /= f_mag; f[2] /= f_mag;

    double up_mag = sqrt(upx*upx + upy*upy + upz*upz);
    double up[3] = {upx/up_mag, upy/up_mag, upz/up_mag};

    double s[3] = {f[1]*up[2] - f[2]*up[1], f[2]*up[0] - f[0]*up[2], f[0]*up[1] - f[1]*up[0]};
    double s_mag = sqrt(s[0]*s[0] + s[1]*s[1] + s[2]*s[2]);
    s[0] /= s_mag; s[1] /= s_mag; s[2] /= s_mag;

    double u[3] = {s[1]*f[2] - s[2]*f[1], s[2]*f[0] - s[0]*f[2], s[0]*f[1] - s[1]*f[0]};

    double m[16] = {
        s[0], u[0], -f[0], 0,
        s[1], u[1], -f[1], 0,
        s[2], u[2], -f[2], 0,
        0,    0,    0,     1
    };

    glMultMatrixd(m);
    myTranslatef(-eyex, -eyey, -eyez);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();    

    mygluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
    myTranslatef(0.0f, 0.0f, 0.0f); 

    myDrawModel();   
    glFlush();
}

void reshape(int w, int h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, (GLsizei) w, (GLsizei) h); 
    glMatrixMode(GL_PROJECTION);   
    glLoadIdentity();
    
    myFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
    printfAffine3DMatrix(2, "Projection Matrix");
      
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 800); 
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Matrix Implementation");
    init();
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}