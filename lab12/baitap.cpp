#include <GL/glut.h>
#include <math.h>
#include <iostream>

// Biến điều khiển chế độ tô bóng
// 1: Flat (Hằng), 2: Gouraud (Mượt), 3: Phong (Giả lập)
int shadeMode = 1; 
float angle = 0.0f;

void init() {
    // Bật kiểm tra độ sâu để các mặt không chồng chéo sai
    glEnable(GL_DEPTH_TEST);
    
    // Bật hệ thống chiếu sáng
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Thiết lập vị trí và thông số đèn
    GLfloat light_pos[] = { 5.0, 5.0, 5.0, 1.0 }; // Đèn ở góc trên bên phải, phía trước
    GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

    // Màu nền xám tối để dễ quan sát vật thể
    glClearColor(0.1, 0.1, 0.1, 1.0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Thiết lập Camera nhìn vào tâm hệ tọa độ từ khoảng cách 5 đơn vị
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();
    glRotatef(angle, 0, 1, 0); // Tự động xoay để thấy bóng đổ thay đổi
    glRotatef(30, 1, 0, 0);

    // Cấu hình vật liệu (Material)
    GLfloat mat_diffuse[] = { 0.8, 0.2, 0.2, 1.0 };  // Màu đỏ chủ đạo
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 }; // Phản chiếu trắng
    GLfloat shininess[] = { 50.0 };                  // Độ bóng

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    // --- LOGIC TÔ BÓNG ---
    if (shadeMode == 1) {
        glShadeModel(GL_FLAT);   // Tô bóng hằng
        glutSolidSphere(1.2, 16, 16); // Lưới thưa để thấy rõ các mặt phẳng
    } 
    else if (shadeMode == 2) {
        glShadeModel(GL_SMOOTH); // Tô bóng Gouraud
        glutSolidSphere(1.2, 16, 16); // Lưới thưa nhưng bề mặt sẽ mượt hơn
    } 
    else if (shadeMode == 3) {
        glShadeModel(GL_SMOOTH); 
        // Giả lập Phong bằng cách tăng mật độ lưới cực cao (Slices/Stacks)
        // Khi các đa giác cực nhỏ, kết quả nội suy Gouraud sẽ tiệm cận Phong
        glutSolidSphere(1.2, 120, 120); 
    }

    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Thiết lập hình chóp cụt quan sát (Frustum)
    gluPerspective(45.0, (float)w / (float)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '1': shadeMode = 1; std::cout << "Mode: Flat Shading\n"; break;
        case '2': shadeMode = 2; std::cout << "Mode: Gouraud Shading\n"; break;
        case '3': shadeMode = 3; std::cout << "Mode: Phong (Simulated)\n"; break;
        case 27:  exit(0); break; // ESC để thoát
    }
    glutPostRedisplay();
}

void idle() {
    angle += 0.2f; // Tốc độ xoay
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    // Sử dụng Double Buffer để chuyển động mượt mà
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Shading Comparison: 1-Flat, 2-Gouraud, 3-Phong");

    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    std::cout << "Nhan phim 1, 2, 3 de chuyen che do to bong.\n";
    glutMainLoop();
    return 0;
}