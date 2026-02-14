#include <stdio.h>
#include <cstring>
#include <cmath>
#include <GL/glut.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
double *m_cv4 = NULL;
int numpoint = 0;

void myTranslatef(float x, float y, float z);
void myLookAt(double eyex, double eyey, double eyez,
              double centerx, double centery, double centerz,
              double upx, double upy, double upz);
void myOrtho(double l, double r, double b, double t, double n, double f);
void myPerspective(double fovy, double aspect, double zNear, double zFar);
void myFrustum(double l, double r, double b, double t, double n, double f);
struct Vector4 {
    float x, y, z, w;
};

// Hàm nhân ma trận 4x4 với Vector4 (P_out = M * P_in)
Vector4 multiplyMatrixVector(GLfloat M[16], Vector4 p) {
    Vector4 res;
    res.x = M[0] * p.x + M[4] * p.y + M[8] * p.z + M[12] * p.w;
    res.y = M[1] * p.x + M[5] * p.y + M[9] * p.z + M[13] * p.w;
    res.z = M[2] * p.x + M[6] * p.y + M[10] * p.z + M[14] * p.w;
    res.w = M[3] * p.x + M[7] * p.y + M[11] * p.z + M[15] * p.w;
    return res;
}
void printfAffine3DMatrix(int model, const char str[]) {
    GLfloat M[16];
    int row, col, index;

    if (model == 1) {
        glGetFloatv(GL_MODELVIEW_MATRIX, M);
        printf("\n[%s] GL_MODELVIEW_MATRIX\n", str);
    }
    if (model == 2) {
        glGetFloatv(GL_PROJECTION_MATRIX, M);
        printf("\n[%s] GL_PROJECTION_MATRIX\n", str);
    }

    for (row = 0; row < 4; row++) {
        for (col = 0, index = row; col < 4; col++, index += 4) {
            printf("%8.3f ", M[index]);
        }
        printf("\n");
    }
}
void myTranslatef(float x, float y, float z) {
    GLfloat T[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1
    };
    glMultMatrixf(T);
}

void myLookAt(double eyex, double eyey, double eyez,
              double centerx, double centery, double centerz,
              double upx, double upy, double upz) {
    // Forward vector (f)
    double fx = centerx - eyex;
    double fy = centery - eyey;
    double fz = centerz - eyez;
    double fl = sqrt(fx*fx + fy*fy + fz*fz);
    fx /= fl; fy /= fl; fz /= fl;

    // Up vector (normalize)
    double ul = sqrt(upx*upx + upy*upy + upz*upz);
    upx /= ul; upy /= ul; upz /= ul;

    // Side vector s = f x up
    double sx = fy*upz - fz*upy;
    double sy = fz*upx - fx*upz;
    double sz = fx*upy - fy*upx;

    // Recompute up = s x f
    double ux = sy*fz - sz*fy;
    double uy = sz*fx - sx*fz;
    double uz = sx*fy - sy*fx;

    GLfloat M[16] = {
        (GLfloat)sx, (GLfloat)ux, (GLfloat)-fx, 0,
        (GLfloat)sy, (GLfloat)uy, (GLfloat)-fy, 0,
        (GLfloat)sz, (GLfloat)uz, (GLfloat)-fz, 0,
        0,           0,           0,            1
    };

    glMultMatrixf(M);
    myTranslatef(-eyex, -eyey, -eyez);
}

void myOrtho(double l, double r, double b, double t, double n, double f) {
    GLfloat M[16] = {
        (GLfloat)(2.0 / (r - l)), 0.0f, 0.0f, 0.0f,
        0.0f, (GLfloat)(2.0 / (t - b)), 0.0f, 0.0f,
        0.0f, 0.0f, (GLfloat)(-2.0 / (f - n)), 0.0f,
        (GLfloat)(-(r + l) / (r - l)),
        (GLfloat)(-(t + b) / (t - b)),
        (GLfloat)(-(f + n) / (f - n)),
        1.0f
    };
    glMultMatrixf(M);
}


void myPerspective(double fovy, double aspect, double zNear, double zFar) {
    double rad = fovy * M_PI / 180.0;
    double tanHalf = tan(rad / 2.0);

    double t = zNear * tanHalf;
    double b = -t;
    double r = t * aspect;
    double l = -r;

    // reuse frustum
    myFrustum(l, r, b, t, zNear, zFar);
}

// Frustum matrix (same as OpenGL glFrustum)
void myFrustum(double l, double r, double b, double t, double n, double f) {
    GLfloat M[16] = {
        (GLfloat)(2*n/(r-l)), 0, 0, 0,
        0, (GLfloat)(2*n/(t-b)), 0, 0,
        (GLfloat)((r+l)/(r-l)), (GLfloat)((t+b)/(t-b)), (GLfloat)(-(f+n)/(f-n)), -1,
        0, 0, (GLfloat)(-2*f*n/(f-n)), 0
    };
    glMultMatrixf(M);
}
void myDrawModel() {
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
        glVertex3f(-2, 0, 0);   // P0
        glVertex3f(0, 0, 1.5);  // P1
        glVertex3f(0, 2.5, 0);  // P2
    glEnd();
}

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    myTranslatef(1.0f, 2.0f, 3.0f);
    myLookAt(10, 0, 0,   0, 0, 0,   0, 0, 1);
    myDrawModel();

    printfAffine3DMatrix(1, "ModelView matrix");
    glFlush();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    myFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);

    printfAffine3DMatrix(2, "Projection matrix");

    glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Affine 3D - Custom Matrices");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}
