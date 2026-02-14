#include <stdio.h>
#include <cstring>
#include <cmath>
#include <GL/glut.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Biến lưu chế độ chiếu: 1: Song song/Trực giao, 2: Xiên, 3: Phối cảnh (Frustum), 4: Phối cảnh (Perspective)
int projectionMode = 1; 

// Cấu trúc điểm 4D phục vụ tính toán tọa độ Clip Space
typedef struct { double x, y, z, w; } Point4d;
typedef struct { double x, y, z; } Vector3;

// --- CÁC HÀM TOÁN HỌC BỔ TRỢ ---
Vector3 normalize(Vector3 v) {
    double len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return { v.x / len, v.y / len, v.z / len };
}

Vector3 cross(Vector3 a, Vector3 b) {
    return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}

Point4d multiplyM4xP4(GLfloat M[16], Point4d p) {
    Point4d res;
    // OpenGL matrix là column-major
    res.x = M[0]*p.x + M[4]*p.y + M[8]*p.z + M[12]*p.w;
    res.y = M[1]*p.x + M[5]*p.y + M[9]*p.z + M[13]*p.w;
    res.z = M[2]*p.x + M[6]*p.y + M[10]*p.z + M[14]*p.w;
    res.w = M[3]*p.x + M[7]*p.y + M[11]*p.z + M[15]*p.w;
    return res;
}

// --- CÁC HÀM DO BẠN ĐỊNH NGHĨA (Đã sửa lỗi) ---

void myTranslatef(float x, float y, float z) {
    GLfloat T[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x, y, z, 1 };
    glMultMatrixf(T);
}

void myLookAt(double eyex, double eyey, double eyez,
              double centerx, double centery, double centerz,
              double upx, double upy, double upz) {
    Vector3 f = normalize({centerx - eyex, centery - eyey, centerz - eyez});
    Vector3 up = normalize({upx, upy, upz});
    Vector3 s = cross(f, up);
    Vector3 u = cross(s, f);

    GLfloat M[16] = {
        (GLfloat)s.x,  (GLfloat)u.x, (GLfloat)-f.x, 0,
        (GLfloat)s.y,  (GLfloat)u.y, (GLfloat)-f.y, 0,
        (GLfloat)s.z,  (GLfloat)u.z, (GLfloat)-f.z, 0,
        0, 0, 0, 1
    };
    glMultMatrixf(M);
    myTranslatef(-eyex, -eyey, -eyez);
}

void myOrtho(double l, double r, double b, double t, double n, double f) {
    GLfloat M[16] = {
        (GLfloat)(2.0/(r-l)), 0, 0, 0,
        0, (GLfloat)(2.0/(t-b)), 0, 0,
        0, 0, (GLfloat)(-2.0/(f-n)), 0,
        (GLfloat)(-(r+l)/(r-l)), (GLfloat)(-(t+b)/(t-b)), (GLfloat)(-(f+n)/(f-n)), 1
    };
    glMultMatrixf(M);
}

void myFrustum(double l, double r, double b, double t, double n, double f) {
    GLfloat M[16] = {
        (GLfloat)(2*n/(r-l)), 0, 0, 0,
        0, (GLfloat)(2*n/(t-b)), 0, 0,
        (GLfloat)((r+l)/(r-l)), (GLfloat)((t+b)/(t-b)), (GLfloat)(-(f+n)/(f-n)), -1,
        0, 0, (GLfloat)(-2*f*n/(f-n)), 0
    };
    glMultMatrixf(M);
}

void myPerspective(double fovy, double aspect, double zNear, double zFar) {
    double rad = fovy * M_PI / 180.0;
    double t = zNear * tan(rad / 2.0);
    double r = t * aspect;
    myFrustum(-r, r, -t, t, zNear, zFar);
}

// Câu 3b: Phép chiếu xiên (Oblique)
void myOblique(double l, double r, double b, double t, double n, double f) {
    float alpha = 45.0 * M_PI / 180.0; // Góc xiên
    float L = 0.5; // Cabinet projection
    GLfloat S[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        (GLfloat)(-L*cos(alpha)), (GLfloat)(-L*sin(alpha)), 1, 0,
        0, 0, 0, 1
    };
    glMultMatrixf(S);
    myOrtho(l, r, b, t, n, f);
}

void printfAffine3DMatrix(int model, const char str[]) {
    GLfloat M[16];
    glGetFloatv(model == 1 ? GL_MODELVIEW_MATRIX : GL_PROJECTION_MATRIX, M);
    printf("\n[%s]\n", str);
    for (int row = 0; row < 4; row++) {
        for (int col = 0, index = row; col < 4; col++, index += 4) {
            printf("%8.3f ", M[index]);
        }
        printf("\n");
    }
}

// --- HÀM TÍNH TOÁN VÀ ĐỐI CHIẾU ---

void calculateAndShowPoints() {
    Point4d P_world = {-2.0, 0.0, 0.0, 1.0}; // Điểm P0
    GLfloat MV[16], P[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, MV);
    glGetFloatv(GL_PROJECTION_MATRIX, P);

    // Camera Space
    Point4d Pc = multiplyM4xP4(MV, P_world);
    // Clip Space
    Point4d Pclip = multiplyM4xP4(P, Pc);

    printf("\n--- DOI CHIEU KET QUA ---");
    printf("\nPoint P_world : (%.2f, %.2f, %.2f)", P_world.x, P_world.y, P_world.z);
    printf("\nPoint P_camera: (%.3f, %.3f, %.3f)", Pc.x, Pc.y, Pc.z);
    printf("\nPoint P_clip  : (%.3f, %.3f, %.3f, %.3f)\n", Pclip.x, Pclip.y, Pclip.z, Pclip.w);
}

void myDrawModel() {
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
        glVertex3f(-2, 0, 0);   // P0
        glVertex3f(0, 0, 1.5);  // P1
        glVertex3f(0, 2.5, 0);  // P2
    glEnd();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Câu 2a: Đặt camera tại (10, 0, 0), nhìn vào gốc (0,0,0), hướng lên (0,0,1)
    // t=(10,0,0), c=(0,0,0), k=(0,0,1)
    myLookAt(10, 0, 0, 0, 0, 0, 0, 0, 1);
    
    myDrawModel();
    calculateAndShowPoints();
    
    glFlush();
}
void myParallel(double l, double r, double b, double t, double n, double f, float dx, float dy, float dz) {
    // Ma trận chiếu song song tổng quát được xây dựng bằng cách làm biến dạng 
    // không gian sao cho hướng chiếu trở nên vuông góc, sau đó dùng Ortho.
    float shx = -dx / dz;
    float shy = -dy / dz;

    GLfloat M_parallel[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        shx, shy, 1, 0,
        0, 0, 0, 1
    };
    glMultMatrixf(M_parallel);
    myOrtho(l, r, b, t, n, f);
}
void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    switch(projectionMode) {
        case 1: // 3c. Trực giao
            myOrtho(-5, 5, -5, 5, 1, 20); 
            printf("\n[3c. Phep chieu Truc giao]"); 
            break;
        case 2: // 3b. Xiên (Oblique)
            myOblique(-5, 5, -5, 5, 1, 20); 
            printf("\n[3b. Phep chieu Xien]"); 
            break;
        case 3: // 3d. Phối cảnh (Frustum)
            myFrustum(-1, 1, -1, 1, 1.5, 20); 
            printf("\n[3d. Phep chieu Phoi canh]"); 
            break;
        case 5: // 3a. Song song tổng quát
            myParallel(-5, 5, -5, 5, 1, 20, 0.5, 0.5, -1.0); 
            printf("\n[3a. Phep chieu Song song tong quat]"); 
            break;
    }
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    if (key >= '1' && key <= '5') { // Tăng lên 5 chế độ
        projectionMode = key - '0';
        reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Kiem tra phep chieu - Bam 1,2,3,4");
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    
    glutMainLoop();
    return 0;
}