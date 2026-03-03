#include <GL/glut.h>
#include <math.h>

float MatCong[300][300][3];
int N = 20, M = 20; // Tăng độ phân giải để mặt trụ mịn hơn
float gocquayx = 30;
float gocquayy = 30;

// --- 1. BỔ SUNG HÀM TÍNH PHÁP VECTOR ---
void PhapVector(float P1[3], float P2[3], float P3[3], float V[3]) {
    float v1[3], v2[3];
    // Tính 2 vector cạnh
    v1[0] = P2[0] - P1[0]; v1[1] = P2[1] - P1[1]; v1[2] = P2[2] - P1[2];
    v2[0] = P3[0] - P1[0]; v2[1] = P3[1] - P1[1]; v2[2] = P3[2] - P1[2];
    // Tích có hướng để tìm vector pháp tuyến
    V[0] = v1[1] * v2[2] - v1[2] * v2[1];
    V[1] = v1[2] * v2[0] - v1[0] * v2[2];
    V[2] = v1[0] * v2[1] - v1[1] * v2[0];
    // Chuẩn hóa vector (đưa về độ dài bằng 1)
    float d = sqrt(V[0] * V[0] + V[1] * V[1] + V[2] * V[2]);
    if (d != 0) { V[0] /= d; V[1] /= d; V[2] /= d; }
}

// --- 2. BỔ SUNG HÀM KHỞI TẠO ÁNH SÁNG ---
void KhoiTaoMoHinhChieuSang() {
    float light_pos[] = { 100.0, 100.0, 100.0, 1.0 };
    float light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    float light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_NORMALIZE);
}

void KhoiTaoMatTru(void) {
    int R = 70, A = 150;
    float PI = 4.0 * atan(1.0);
    float u, du = 2 * PI / M;
    float v, dv = 1.0 / N;
    float x, y, z;
    int m, n;
    for (n = 0; n <= N; n++) {
        v = n * dv;
        for (m = 0; m <= M; m++) {
            u = m * du;
            x = R * cos(u);
            y = A * (v - 0.5); // Dịch chuyển để tâm mặt trụ ở gốc tọa độ
            z = R * sin(u);
            MatCong[n][m][0] = x;
            MatCong[n][m][1] = y;
            MatCong[n][m][2] = z;
        }
    }
}

void ChieuSangMatCong(void) {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glPushMatrix();
    glRotatef(gocquayx, 1, 0, 0);
    glRotatef(gocquayy, 0, 1, 0);
    
    float mauden[] = { 0.1, 0.1, 0.1, 1.0 };
    float mauvang[] = { 0.8, 0.8, 0.0, 1.0 };
    float V[3];
    int n, m;

    for (n = 0; n <= N - 1; n++) {
        if (n % 2 == 0) {
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mauden);
        } else {
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mauvang);
        }

        for (m = 0; m <= M - 1; m++) {
            glBegin(GL_QUADS);
            PhapVector(MatCong[n][m], MatCong[n + 1][m], MatCong[n + 1][m + 1], V);
            glNormal3fv(V);
            glVertex3fv(MatCong[n][m]);
            glVertex3fv(MatCong[n + 1][m]);
            glVertex3fv(MatCong[n + 1][m + 1]);
            glVertex3fv(MatCong[n][m + 1]);
            glEnd();
        }
    }
    glPopMatrix();
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
}

// --- 3. BỔ SUNG CÁC HÀM ĐIỀU KHIỂN HIỂN THỊ ---
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // Vì glOrtho từ -200 đến 200, ta đặt camera phù hợp
    ChieuSangMatCong();
    glutSwapBuffers();
}

void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_UP) gocquayx += 5;
    if (key == GLUT_KEY_DOWN) gocquayx -= 5;
    if (key == GLUT_KEY_LEFT) gocquayy -= 5;
    if (key == GLUT_KEY_RIGHT) gocquayy += 5;
    glutPostRedisplay();
}

void Init(void) {
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Điều chỉnh tầm nhìn cho phù hợp với kích thước R=70, A=150
    glOrtho(-200.0, 200.0, -200.0, 200.0, -200.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    
    KhoiTaoMatTru();
    KhoiTaoMoHinhChieuSang();
}

// --- 4. HÀM MAIN ---
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Mo hinh chieu sang mat tru");
    Init();
    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutMainLoop();
    return 0;
}