#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include <map>
#include <vector>
#include <iostream>

using namespace std;

/* Vertices cho demo */
const int VERTICES_SIZE = 4;
float vertices[VERTICES_SIZE][3] = {
    {0.0, 1.0, 0.0},
    {-1.0, -1.0, 1.0},
    {1.0, -1.0, 1.0},
    {0.0, -1.0, -1.0},
};

// Mảng lưu trữ vector pháp tuyến của đỉnh
float vertex_normals[VERTICES_SIZE][3];

/* Các mặt (Polygons) - Tetrahedon (Hình tứ diện) */
const int POLYGON_SIZE = 4;
unsigned int polygon[POLYGON_SIZE][3] = {
    {0, 1, 2},
    {0, 2, 3},
    {0, 3, 1},
    {1, 3, 2},
};

float av_point[3] = {0, 0, 0};

void averagePoint(float verts[][3], int length, float result[3]) {
    result[0] = result[1] = result[2] = 0;
    for(int i = 0; i < length; i++) {
        result[0] += verts[i][0];
        result[1] += verts[i][1];
        result[2] += verts[i][2];
    }
    result[0] /= length;
    result[1] /= length;
    result[2] /= length;
}

void normalise(float v[3]) {
    float length = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (length > 0) {
        v[0] /= length; v[1] /= length; v[2] /= length;
    }
}

void crossProduct(float u[], float v[], float result[]) {
    result[0] = u[1] * v[2] - u[2] * v[1];
    result[1] = u[2] * v[0] - u[0] * v[2];
    result[2] = u[0] * v[1] - u[1] * v[0];
}

void calculate_normal(unsigned int p[3], float verts[][3], float normal[3]) {
    float u[3], v[3];
    u[0] = verts[p[1]][0] - verts[p[0]][0];
    u[1] = verts[p[1]][1] - verts[p[0]][1];
    u[2] = verts[p[1]][2] - verts[p[0]][2];

    v[0] = verts[p[2]][0] - verts[p[0]][0];
    v[1] = verts[p[2]][1] - verts[p[0]][1];
    v[2] = verts[p[2]][2] - verts[p[0]][2];

    crossProduct(u, v, normal);
    normalise(normal);
}

void calculate_vertex_normals() {
    vector<vector<int>> vertex_to_faces(VERTICES_SIZE);
    vector<vector<float>> faces_to_normal(POLYGON_SIZE, vector<float>(3));

    for (int i = 0; i < POLYGON_SIZE; i++) {
        calculate_normal(polygon[i], vertices, &faces_to_normal[i][0]);
        for (int j = 0; j < 3; j++) {
            vertex_to_faces[polygon[i][j]].push_back(i);
        }
    }

    for (int v = 0; v < VERTICES_SIZE; v++) {
        float sum_n[3] = {0, 0, 0}; // QUAN TRỌNG: Khởi tạo bằng 0
        for (int face_idx : vertex_to_faces[v]) {
            sum_n[0] += faces_to_normal[face_idx][0];
            sum_n[1] += faces_to_normal[face_idx][1];
            sum_n[2] += faces_to_normal[face_idx][2];
        }
        normalise(sum_n);
        memcpy(vertex_normals[v], sum_n, sizeof(sum_n));
    }
}

void draw_polygon() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT, 0, vertex_normals);

    // Vẽ các tam giác sử dụng mảng chỉ số
    glDrawElements(GL_TRIANGLES, POLYGON_SIZE * 3, GL_UNSIGNED_INT, polygon);

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void init_geometry() {
    averagePoint(vertices, VERTICES_SIZE, av_point);
    calculate_vertex_normals();
}

void setup_illumination() {
    GLfloat light_pos[] = {2.0, 2.0, 5.0, 1.0};
    GLfloat light_ka[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat light_kd[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_ks[] = {1.0, 1.0, 1.0, 1.0};

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ka);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_kd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_ks);

    GLfloat material_ka[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat material_kd[] = {0.4, 0.6, 0.9, 1.0};
    GLfloat material_ks[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat material_se[] = {50.0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  material_ka);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  material_kd);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_ks);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_se);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void display() {
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    float aspect = (float)width / (float)height;

    glViewport(0, 0, width, height);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Đưa camera lùi xa hơn để nhìn thấy vật thể
    gluLookAt(0, 2, 5, av_point[0], av_point[1], av_point[2], 0, 1, 0);

    setup_illumination();
    glEnable(GL_DEPTH_TEST);

    // Xoay vật thể một chút để dễ quan sát khối 3D
    static float angle = 0;
    glRotatef(angle, 0, 1, 0);
    angle += 0.5f;

    draw_polygon();
    
    glutSwapBuffers();
    glutPostRedisplay(); // Gọi lại để tạo hiệu ứng xoay
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Lighting & Vertex Normals");

    init_geometry();
    glutDisplayFunc(display);
    
    glutMainLoop();
    return 0;
}