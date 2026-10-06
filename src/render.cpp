#define GL_SILENCE_DEPRECATION // убирает warning-и Apple
#include "render.hpp"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include <cstdlib> // Для exit()

Scene global_scene;

void draw_triangle(const Triangle3& t) {
    Vector3 n = t.normal();
    glNormal3f(n.x, n.y, n.z);
    glVertex3f(t.a.x, t.a.y, t.a.z);
    glVertex3f(t.b.x, t.b.y, t.b.z);
    glVertex3f(t.c.x, t.c.y, t.c.z);
}

void draw_polygon(const Polygon3& p) {
    if (p.dots_amount < 3) return;
    // Считаем нормаль полигона пересечения
    Vector3 v1 = p.dot_list[1] - p.dot_list[0];
    Vector3 v2 = p.dot_list[2] - p.dot_list[0];
    Vector3 n = v1.cross(v2).normalized();

    glNormal3f(n.x, n.y, n.z);
    for (unsigned int i = 0; i < p.dots_amount; ++i) {
        glVertex3f(p.dot_list[i].x, p.dot_list[i].y, p.dot_list[i].z);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Фиксированная камера (без вращения)
    gluLookAt(8.0, 6.0, 10.0,  // Позиция камеры
              0.0, 0.0, 0.0,   // Точка обзора (центр)
              0.0, 1.0, 0.0);  // Вектор верха

    // Отрисовка самих треугольников (синие с обеих сторон)
    GLfloat blue[] = {0.2f, 0.2f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
    glBegin(GL_TRIANGLES);
    for (const auto& t : global_scene.triangles) {
        draw_triangle(t);
    }
    glEnd();

    // Отрисовка пересечений (красные)
    GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);

    // Сдвиг глубины чтобы предотвратить Z-fighting с синими треугольниками
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(-2.0, -2.0);

    for (const auto& p : global_scene.intersections) {
        glBegin(GL_POLYGON);
        draw_polygon(p);
        glEnd();
    }
    glDisable(GL_POLYGON_OFFSET_FILL);

    glutSwapBuffers();
}

// Обработчик нажатия клавиш
void keyboard(unsigned char key, int x, int y) {
    // ASCII код 13 соответствует клавише Return (Enter)
    if (key == 13) {
        exit(0);
    }
}

void initGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE); // Важно при масштабировании или произвольных нормалях

    // Включаем двустороннее освещение, чтобы обратные стороны треугольников не были черными
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    GLfloat light_pos[] = {5.0f, 10.0f, 5.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void start_render(int argc, char** argv, const Scene& scene) {
    global_scene = scene;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Triangles Intersection");

    initGL();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // Регистрация функции обработки клавиатуры
    glutKeyboardFunc(keyboard);

    glutMainLoop();
}
