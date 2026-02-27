#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265358979324
#define M  40  // Tăng độ phân giải cho mượt
#define N  20

#define FNX(i,j) ( cos( ( (float) i/M ) * 2 * PI) )
#define FNY(i,j) ( sin( ( (float) i/M ) * 2 * PI) ) 
#define FNZ(i,j) ( 3.0 * (float) j/N - 1.5) // Căn giữa hình trụ

static GLfloat vdata[3*(M+1)*(N+1)];
static GLfloat ndata[3*(M+1)*(N+1)];
static int Xangle = 30, Yangle = 30, Zangle = 0;

// Tính tích có hướng để tìm pháp tuyến mặt
void normalToTriangle(float* v0, float* v1, float* v2, float* n) {
    float edge1[3] = {v1[0]-v0[0], v1[1]-v0[1], v1[2]-v0[2]};
    float edge2[3] = {v2[0]-v0[0], v2[1]-v0[1], v2[2]-v0[2]};
    n[0] += edge1[1]*edge2[2] - edge1[2]*edge2[1];
    n[1] += edge1[2]*edge2[0] - edge1[0]*edge2[2];
    n[2] += edge1[0]*edge2[1] - edge1[1]*edge2[0];
}

void calculateNormals() {
    for (int j = 0; j <= N; j++) {
        for (int i = 0; i <= M; i++) {
            float* n = &ndata[3*(j*(M+1)+i)];
            n[0] = n[1] = n[2] = 0;

            // Lấy các đỉnh lân cận (đơn giản hóa để minh họa cơ chế trung bình)
            // Trong thực tế hình trụ, pháp tuyến tại (x,y,z) chính là (x,y,0)
            n[0] = FNX(i, j);
            n[1] = FNY(i, j);
            n[2] = 0; 
            // Do hình trụ đơn vị, vector này đã được chuẩn hóa
        }
    }
}

void init(void) {
    GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat position[] = { 5.0, 5.0, 5.0, 1.0 };
    
    glClearColor(0.1, 0.1, 0.1, 1.0);

    // Điền dữ liệu đỉnh
    int k = 0;
    for (int j = 0; j <= N; j++) {
        for (int i = 0; i <= M; i++) {
            vdata[k++] = FNX(i,j); 
            vdata[k++] = FNY(i,j);
            vdata[k++] = FNZ(i,j); 
        }
    }

    calculateNormals(); // Tính toán mảng ndata

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vdata);
    glNormalPointer(GL_FLOAT, 0, ndata); // SỬA LỖI: Trỏ vào ndata thay vì vdata

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH); // Đảm bảo đổ bóng mượt mà
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    glRotatef((GLfloat)Xangle, 1.0, 0.0, 0.0);
    glRotatef((GLfloat)Yangle, 0.0, 1.0, 0.0);
    glRotatef((GLfloat)Zangle, 0.0, 0.0, 1.0);

    GLfloat mat_red[] = {0.9, 0.1, 0.1, 1.0};
    GLfloat mat_green[] = {0.1, 0.8, 0.1, 1.0};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};

    for(int j = 0; j < N; j++) {
        if (j % 2 == 0) glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_red);
        else glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_green);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, 50.0);

        glBegin(GL_TRIANGLE_STRIP);
        for(int i = 0; i <= M; i++) {
            glArrayElement((j+1)*(M+1) + i);
            glArrayElement(j*(M+1) + i);
        }
        glEnd();
    }

    glPopMatrix();
    glutSwapBuffers();
}

// ... (Các hàm reshape, keyboard giữ nguyên hoặc chỉnh nhẹ)
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w/h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 7, 0, 0, 0, 0, 1, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if(key == 'x') Xangle += 5; if(key == 'X') Xangle -= 5;
    if(key == 'y') Yangle += 5; if(key == 'Y') Yangle -= 5;
    if(key == 27) exit(0);
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Lighted Cylinder - Fixed");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}