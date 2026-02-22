#include <GL/glut.h>

float angle = 0.0f; // Góc xoay

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Đặt camera
    gluLookAt(0.0, 0.0, 5.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0);
    
    glRotatef(angle, 1.0f, 1.0f, 0.0f); // Xoay khối quanh trục x và y

    glBegin(GL_QUADS);
        // Mặt trước (Đỏ)
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(-1.0, -1.0,  1.0); glVertex3f( 1.0, -1.0,  1.0);
        glVertex3f( 1.0,  1.0,  1.0); glVertex3f(-1.0,  1.0,  1.0);
        // Mặt sau (Xanh lá)
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(-1.0, -1.0, -1.0); glVertex3f(-1.0,  1.0, -1.0);
        glVertex3f( 1.0,  1.0, -1.0); glVertex3f( 1.0, -1.0, -1.0);
        // Các mặt khác tương tự...
    glEnd();

    glutSwapBuffers();
}

void timer(int) {
    angle += 0.8f; // Cập nhật góc xoay
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("3D Cube Vibe");
    glEnable(GL_DEPTH_TEST); // Bật kiểm tra độ sâu để thấy khối 3D
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}