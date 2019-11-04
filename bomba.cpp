#include <iostream>
#include "bomba.h"
#include <math.h>
#include <GL/glut.h>

using namespace std;

#define grausParaRadianos(g) g*(M_PI/180)
#define radianosParaGraus(r) r*(180/M_PI)

Bomba::Bomba(){
    this->x = 0;
    this->y = 0;
    this->angulo = 0;
}

Bomba::Bomba(float x, float y, float r, float angulo, float velocidade){
    this->x = x;
    this->y = y;
    this->r = r;
    this->r_inicial = r;
    this->angulo = angulo;
    this->velocidade = velocidade;
}

void Bomba::desenharPreenchido(){
    glPushMatrix();

        GLfloat x_aux, y_aux;
        int i;

        glTranslatef(x, y, 0);

        glPushMatrix();
            glScalef(0.28, 0.28, 1);
            glColor3f(1, 1, 0);
            glBegin(GL_POLYGON);
            for(i=0; i<360; i+=20){
                x_aux = r * cos(grausParaRadianos(i));
                y_aux = r * sin(grausParaRadianos(i));
                glVertex3f(x_aux, y_aux, 0);
            }
            glEnd();
        glPopMatrix();

    glPopMatrix();

}

void Bomba::mover(float timeDiference){
    this->y += this->velocidade*timeDiference*sin(grausParaRadianos(angulo));
    this->x += this->velocidade*timeDiference*cos(grausParaRadianos(angulo));
}

void Bomba::decrementarRaio(float velocidade){
    this->r -= velocidade;
}

Bomba::~Bomba(){
}
