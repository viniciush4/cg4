#include <iostream>
#include "tiro.h"
#include <math.h>
#include <GL/glut.h>

using namespace std;

#define grausParaRadianos(g) g*(M_PI/180)
#define radianosParaGraus(r) r*(180/M_PI)

Tiro::Tiro(){
    this->x = 0;
    this->y = 0;
    this->jogador_angulo = 0;
}

Tiro::Tiro(float x, float y, float r, float jogador_angulo, float canhao_angulo, float canhao_angulo_arena){
    this->x = x;
    this->y = y;
    this->r = r;
    this->jogador_angulo = jogador_angulo;
    this->canhao_angulo = canhao_angulo;
    this->canhao_angulo_arena = canhao_angulo_arena;
}

void Tiro::desenharPreenchido(){
    glPushMatrix();

        GLfloat x_aux, y_aux;
        int i;

        glTranslatef(x, y, 0);

        glRotatef(jogador_angulo, 0, 0, 1);

        glPushMatrix();
            glTranslatef(r, 0, 0);
            glRotatef(canhao_angulo, 0, 0, 1);
            glTranslatef(10, 0, 0);
            glScalef(0.08, 0.08, 1);
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

void Tiro::mover(float velocidade){
    this->y += velocidade*sin(grausParaRadianos(canhao_angulo_arena));
    this->x += velocidade*cos(grausParaRadianos(canhao_angulo_arena));
}

Tiro::~Tiro(){
}
