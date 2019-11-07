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

Tiro::Tiro(float x, float y, float r, float jogador_angulo, float canhao_angulo, float canhao_angulo_arena, float cor_r, float cor_g, float cor_b, float velocidade){
    this->x = x;
    this->y = y;
    this->r = r;
    this->jogador_angulo = jogador_angulo;
    this->canhao_angulo = canhao_angulo;
    this->canhao_angulo_arena = canhao_angulo_arena;
    this->cor_r = cor_r;
	this->cor_g = cor_g;
	this->cor_b = cor_b;
    this->velocidade = velocidade;
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
            glColor3f(cor_r, cor_g, cor_b);
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

void Tiro::mover(float coeficiente_ajuste){
    y += velocidade*coeficiente_ajuste*sin(grausParaRadianos(canhao_angulo_arena));
    x += velocidade*coeficiente_ajuste*cos(grausParaRadianos(canhao_angulo_arena));
}

Tiro::~Tiro(){
}
