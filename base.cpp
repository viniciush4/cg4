#include <iostream>
#include "base.h"
#include <math.h>
#include <GL/glut.h>

using namespace std;

#define grausParaRadianos(g) g*(M_PI/180)
#define radianosParaGraus(r) r*(180/M_PI)

Base::Base(){
	this->x = 0.;
	this->y = 0.;
	this->r = 0.;
    this->angulo = rand() % 100;
    this->escala = 1;
    this->angulo_canhao = 0;
    this->angulo_canhao_arena = 0;
    this->angulo_helices = 0;
}

Base::Base(float x, float y, float r, float cor_r, float cor_g, float cor_b, char* fill, int id){
	this->x = x;
	this->y = y;
	this->r = r;
    this->r_inicial = r;
    this->cor_r = cor_r;
    this->cor_g = cor_g;
    this->cor_b = cor_b;
    this->fill = fill;
    this->id = id;
    this->angulo = rand() % 100;
    this->escala = 1;
    this->angulo_canhao = 0;
    this->angulo_canhao_arena = 0;
    this->angulo_helices = 0;
}

void Base::desenharPreenchido(){
    glPushMatrix();

        glTranslatef(x, y, 0);

        glRotatef(angulo, 0, 0, 1);

        glScalef(escala, escala, 1);

        GLfloat x_aux, y_aux;
        int i;

        // Desenha um círculo model
        glPushMatrix();
            glColor3f(cor_r, cor_g, cor_b);
            glBegin(GL_POLYGON);
            for(i=0; i<360; i+=20){
                x_aux = 1 * cos(M_PI*i/180);
                y_aux = 1 * sin(M_PI*i/180);
                glVertex3f(x_aux, y_aux, 0);
            }
            glEnd();
        glPopMatrix();

        // Desenha um círculo model
        glPushMatrix();
            glColor3f(1, 1, 1);
            glBegin(GL_POLYGON);
            for(i=0; i<360; i+=20){
                x_aux = 0.7 * cos(M_PI*i/180);
                y_aux = 0.7 * sin(M_PI*i/180);
                glVertex3f(x_aux, y_aux, 0);
            }
            glEnd();
        glPopMatrix();

        // Desenha um círculo model
        glPushMatrix();
            glColor3f(cor_r, cor_g, cor_b);
            glBegin(GL_POLYGON);
            for(i=0; i<360; i+=20){
                x_aux = 0.4 * cos(M_PI*i/180);
                y_aux = 0.4 * sin(M_PI*i/180);
                glVertex3f(x_aux, y_aux, 0);
            }
            glEnd();
        glPopMatrix();

        // Desenha um círculo model
        glPushMatrix();
            glColor3f(1, 1, 1);
            glBegin(GL_POLYGON);
            for(i=0; i<360; i+=20){
                x_aux = 0.1 * cos(M_PI*i/180);
                y_aux = 0.1 * sin(M_PI*i/180);
                glVertex3f(x_aux, y_aux, 0);
            }
            glEnd();
        glPopMatrix();

    glPopMatrix();
}

Base::~Base(){
}
