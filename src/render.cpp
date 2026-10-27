#define GL_SILENCE_DEPRECATION
#include "..//include/render.hpp"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include <cstdlib>
#include <algorithm>

constexpr unsigned char KEY_RETURN = 13;

Scene global_scene;

float camera_radius = 15.0f;
float camera_yaw = 0.8f;
float camera_pitch = 0.6f;

void draw_axes() {
    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(5.0f, 0.0f, 0.0f);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 5.0f, 0.0f);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 5.0f);
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
}

void draw_triangle(const Triangle3& t) {
    Vector3 n = t.normal();
    glNormal3f(n.x(), n.y(), n.z());
    glVertex3f(t.a().x(), t.a().y(), t.a().z());
    glVertex3f(t.b().x(), t.b().y(), t.b().z());
    glVertex3f(t.c().x(), t.c().y(), t.c().z());
}

void draw_polygon(const Polygon3& p) {
    if (p.dots_amount() < 3) return;
    // Используем геттер dot_list() с круглыми скобками
    Vector3 v1 = p.dot_list()[1] - p.dot_list()[0];
    Vector3 v2 = p.dot_list()[2] - p.dot_list()[0];
    Vector3 n = v1.cross(v2).normalized();

    glNormal3f(n.x(), n.y(), n.z());
    for (unsigned int i = 0; i < p.dots_amount(); ++i) {
        glVertex3f(p.dot_list()[i].x(), p.dot_list()[i].y(), p.dot_list()[i].z());
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float camX = camera_radius * std::cos(camera_pitch) * std::sin(camera_yaw);
    float camY = camera_radius * std::sin(camera_pitch);
    float camZ = camera_radius * std::cos(camera_pitch) * std::cos(camera_yaw);

    gluLookAt(camX, camY, camZ,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    draw_axes();

    // Draw triangles (blue)
    GLfloat blue[] = {0.2f, 0.2f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
    glBegin(GL_TRIANGLES);
    for (const auto& t : global_scene.triangles) {
        draw_triangle(t);
    }
    glEnd();

    // Draw coplanar intersections (red polygons)
    GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(-2.0, -2.0);

    for (const auto& p : global_scene.intersections) {
        glBegin(GL_POLYGON);
        draw_polygon(p);
        glEnd();
    }
    glDisable(GL_POLYGON_OFFSET_FILL);

    // Draw non-coplanar intersection segments (thick yellow lines)
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.0f);
    glLineWidth(4.0f);
    glBegin(GL_LINES);
    for (const auto& seg : global_scene.intersection_segments) {
        glVertex3f(seg.a().x(), seg.a().y(), seg.a().z());
        glVertex3f(seg.b().x(), seg.b().y(), seg.b().z());
    }
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int, int) {
    if (key == KEY_RETURN) {
        exit(0);
    } else if (key == 'w' || key == 'W') {
        camera_pitch = std::min(1.5f, camera_pitch + 0.1f);
    } else if (key == 's' || key == 'S') {
        camera_pitch = std::max(-1.5f, camera_pitch - 0.1f);
    } else if (key == 'a' || key == 'A') {
        camera_yaw -= 0.1f;
    } else if (key == 'd' || key == 'D') {
        camera_yaw += 0.1f;
    } else if (key == '+' || key == '=') {
        camera_radius = std::max(2.0f, camera_radius - 1.0f);
    } else if (key == '-' || key == '_') {
        camera_radius += 1.0f;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int, int) {
    switch (key) {
        case GLUT_KEY_LEFT: camera_yaw -= 0.1f; break;
        case GLUT_KEY_RIGHT: camera_yaw += 0.1f; break;
        case GLUT_KEY_UP: camera_pitch = std::min(1.5f, camera_pitch + 0.1f); break;
        case GLUT_KEY_DOWN: camera_pitch = std::max(-1.5f, camera_pitch - 0.1f); break;
    }
    glutPostRedisplay();
}

void initGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
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
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutMainLoop();
}
