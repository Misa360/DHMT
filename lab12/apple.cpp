#include <GL/glut.h>
#include <cmath>

// Hàm vẽ quả táo
void DrawApple() {
    // 1. Thân quả táo (Màu đỏ)
    GLfloat mat_ambient_red[] = { 0.3f, 0.0f, 0.0f, 1.0f };
    GLfloat mat_diffuse_red[] = { 0.9f, 0.1f, 0.1f, 1.0f };
    GLfloat mat_specular_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat shininess = 80.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_red);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_red);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_white);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    glPushMatrix();
        // Hơi bóp nhẹ hình cầu để giống quả táo hơn
        glScalef(1.0, 0.9, 1.0); 
        glutSolidSphere(3.0, 50, 50);
    glPopMatrix();

    // 2. Cuống quả táo (Màu nâu)
    GLfloat mat_ambient_brown[] = { 0.2f, 0.1f, 0.0f, 1.0f };
    GLfloat mat_diffuse_brown[] = { 0.5f, 0.35f, 0.05f, 1.0f };
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_brown);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_brown);

    glPushMatrix();
        glTranslatef(0.0, 2.5, 0.0); // Đặt trên đỉnh quả táo
        glRotatef(-15, 0, 0, 1);      // Hơi nghiêng cuống
        glScalef(0.2, 1.5, 0.2);     // Làm cho hình trụ dài và gầy
        glutSolidCube(1.0);          // Dùng cube kéo dài làm cuống đơn giản
    glPopMatrix();
}

void RenderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Đặt camera
    gluLookAt(8.0, 8.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Vẽ quả táo
    DrawApple();

    glFlush();
}

void Init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    // --- NGUỒN SÁNG 0: Ánh sáng trắng từ phía trên, bên phải ---
    glEnable(GL_LIGHT0);
    GLfloat light0_pos[] = { 10.0f, 10.0f, 10.0f, 1.0f }; // Vị trí
    GLfloat light0_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Màu trắng
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);

    // --- NGUỒN SÁNG 1: Ánh sáng vàng dịu từ phía bên trái để đổ bóng mềm ---
    glEnable(GL_LIGHT1);
    GLfloat light1_pos[] = { -10.0f, 0.0f, 5.0f, 1.0f };
    GLfloat light1_diffuse[] = { 0.4f, 0.4f, 0.0f, 1.0f }; // Màu vàng nhẹ
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
}

void ReShape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Apple Shading - OpenGL");

    Init();
    glutReshapeFunc(ReShape);
    glutDisplayFunc(RenderScene);

    glutMainLoop();
    return 0;
}