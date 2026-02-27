#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h> // Thư viện cho hàm exit()

#define PI 3.14159265358979324
#define M  25   // Số lát cắt dọc (quanh trục)
#define N  15   // Số tầng (dọc theo trục Z)

// Tọa độ đỉnh (X, Y, Z)
#define FNX(i,j) ( cos( ( (float) i/M ) * 2 * PI) )
#define FNY(i,j) ( sin( ( (float) i/M ) * 2 * PI) ) 
#define FNZ(i,j) ( 3.0 * (float) j/N - 1.5 ) // Trừ 1.5 để căn giữa hình trụ tại gốc tọa độ

// Vector pháp tuyến (Normal vector) - giúp tính toán ánh sáng
#define NORMX(i,j) ( cos( ( (float) i/M ) * 2 * PI) )
#define NORMY(i,j) ( sin( ( (float) i/M ) * 2 * PI) )
#define NORMZ(i,j) ( 0 )

static GLfloat vdata[3 * (M + 1) * (N + 1)];
static GLfloat ndata[3 * (M + 1) * (N + 1)];

static int Xangle = 0, Yangle = 0, Zangle = 0;

void init(void) {
    int i, j, k = 0, l = 0;

    GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat position[] = { 2.0, 2.0, 5.0, 1.0 }; // Nguồn sáng cố định
    GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

    glClearColor(0.0, 0.0, 0.0, 0.0);

    // Điền dữ liệu vào mảng Vertex và Normal
    for (j = 0; j <= N; j++) {
        for (i = 0; i <= M; i++) {
            vdata[k++] = FNX(i, j);
            vdata[k++] = FNY(i, j);
            vdata[k++] = FNZ(i, j);

            ndata[l++] = NORMX(i, j);
            ndata[l++] = NORMY(i, j);
            ndata[l++] = NORMZ(i, j);
        }
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vdata);
    glNormalPointer(GL_FLOAT, 0, ndata);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Đặt camera
    gluLookAt(0.0, 0.0, 7.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Xoay hình trụ
    glRotatef((GLfloat)Xangle, 1.0, 0.0, 0.0);
    glRotatef((GLfloat)Yangle, 0.0, 1.0, 0.0);
    glRotatef((GLfloat)Zangle, 0.0, 0.0, 1.0);

    GLfloat mat_specular1[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess1[] = { 50.0 };
    GLfloat mat_diffuse1[] = { 0.9, 0.0, 0.0, 1.0 }; // Màu đỏ

    GLfloat mat_specular2[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess2[] = { 50.0 };
    GLfloat mat_diffuse2[] = { 0.0, 0.9, 0.0, 1.0 }; // Màu xanh lá

    for (int j = 0; j < N; j++) {
        if (j % 2 == 0) {
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular1);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess1);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse1);
        } else {
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular2);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess2);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse2);
        }

        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i <= M; i++) {
            glArrayElement((j + 1) * (M + 1) + i);
            glArrayElement(j * (M + 1) + i);
        }
        glEnd();
    }

    glutSwapBuffers(); // Dùng SwapBuffers cho mượt thay vì glFlush
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'x': Xangle += 5; break;
    case 'X': Xangle -= 5; break;
    case 'y': Yangle += 5; break;
    case 'Y': Yangle -= 5; break;
    case 'z': Zangle += 5; break;
    case 'Z': Zangle -= 5; break;
    case 27: exit(0); break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Lighted Cylinder - OpenGL");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}