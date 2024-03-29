#include <iostream>
#include "inimigo.h"
#include <math.h>
#include <GL/glut.h>

using namespace std;

#define grausParaRadianos(g) g*(M_PI/180)
#define radianosParaGraus(r) r*(180/M_PI)

Inimigo::Inimigo(){
	this->x = 0.;
	this->y = 0.;
	this->r = 0.;
    this->angulo = rand() % 360;
    this->escala = 1;
    this->angulo_canhao = 0;
    this->angulo_canhao_arena = 0;
    this->angulo_helices = 0;
    this->somatorio_incremento_angulo = 0;
    this->incrementar_angulo = true;
    this->tempo_desde_ultimo_tiro = 0;
    this->velocidade = 0;
}

Inimigo::Inimigo(float x, float y, float r, float cor_r, float cor_g, float cor_b, char* fill, int id, float velocidade){
	this->x = x;
	this->y = y;
	this->r = r;
    this->r_inicial = r;
    this->cor_r = cor_r;
    this->cor_g = cor_g;
    this->cor_b = cor_b;
    this->fill = fill;
    this->id = id;
    this->angulo = rand() % 360;
    this->escala = 1;
    this->angulo_canhao = 0;
    this->angulo_canhao_arena = 0;
    this->angulo_helices = 0;
    this->somatorio_incremento_angulo = 0;
    this->incrementar_angulo = true;
    this->tempo_desde_ultimo_tiro = 0;
    this->velocidade = velocidade;
}

void Inimigo::desenharPreenchido(){
    glPushMatrix();

        // Círculo base
        // float theta = 0.0;
        // glColor3f(1, 1, 1);
        // glBegin(GL_POLYGON);
        // for(int i=0; i < NUMERO_SEGMENTOS; i++){
        //     glVertex3f(x + r * cos(theta), y + r * sin(theta), 0.0);
        //     theta += 2*3.1415926f /NUMERO_SEGMENTOS;
        // }
        // glEnd();

        glTranslatef(x, y, 0);

        glRotatef(angulo, 0, 0, 1);
        
        glScalef(escala, escala, 1);


        GLfloat x_aux, y_aux;
        int i;

        // Desenha o canhão
        glPushMatrix();
            glTranslatef(1, 0, 0);
            glRotatef(angulo_canhao, 0, 0, 1);
            glColor3f(0, 0, 0);
            glBegin(GL_POLYGON);
                glVertex3f(0, 0.07, 0);
                glVertex3f(0.5, 0.07, 0);
                glVertex3f(0.5, -0.07, 0);
                glVertex3f(0, -0.07, 0);
            glEnd();
        glPopMatrix();

        // Suporte de hélice 1
        glPushMatrix();
            glTranslatef(0, -0.7, 0);
            glColor3f(0, 0, 0);
            glBegin(GL_POLYGON);
                glVertex3f(0, 0.05, 0);
                glVertex3f(0.5, 0.05, 0);
                glVertex3f(0.5, -0.05, 0);
                glVertex3f(0, -0.05, 0);
            glEnd();
            glTranslatef(0.5, 0, 0);
            glRotatef(angulo_helices, 1, 0, 0);
            glColor3f(1, 1, 0);
            glBegin(GL_POLYGON);
                glVertex3f(0, 0, 0);
                glVertex3f(-0.1, -0.2, 0);
                glVertex3f(0.1, -0.2, 0);
                glVertex3f(0, 0, 0);
                glVertex3f(0.1, 0.2, 0);
                glVertex3f(-0.1, 0.2, 0);
            glEnd();

        glPopMatrix();

        // Suporte de hélice 2
        glPushMatrix();
            glTranslatef(0, 0.7, 0);
            glColor3f(0, 0, 0);
            glBegin(GL_POLYGON);
                glVertex3f(0, 0.05, 0);
                glVertex3f(0.5, 0.05, 0);
                glVertex3f(0.5, -0.05, 0);
                glVertex3f(0, -0.05, 0);
            glEnd();
            glTranslatef(0.5, 0, 0);
            glRotatef(angulo_helices, 1, 0, 0);
            glColor3f(1, 1, 0);
            glBegin(GL_POLYGON);
                glVertex3f(0, 0, 0);
                glVertex3f(-0.1, -0.2, 0);
                glVertex3f(0.1, -0.2, 0);
                glVertex3f(0, 0, 0);
                glVertex3f(0.1, 0.2, 0);
                glVertex3f(-0.1, 0.2, 0);
            glEnd();
        glPopMatrix();

        // Desenha as asas
        glPushMatrix();
            glTranslatef(0.5, 0, 0);
            glColor3f(0, 0, 0);
            glBegin(GL_POLYGON);
                glVertex3f(0, 0, 0);
                glVertex3f(-0.4, 1, 0);
                glVertex3f(-0.7, 1, 0);
                glVertex3f(-0.7, -1, 0);
                glVertex3f(-0.4, -1, 0);
            glEnd();
        glPopMatrix();

        // Desenha o corpo do avião
        glPushMatrix();
            glScalef(1, 0.28, 1);
            glColor3f(cor_r, cor_g, cor_b);
            glBegin(GL_POLYGON);
            for(i=0; i<360; i+=20){
                x_aux = 1 * cos(M_PI*i/180);
                y_aux = 1 * sin(M_PI*i/180);
                glVertex3f(x_aux, y_aux, 0);
            }
            glEnd();
        glPopMatrix();

        // Desenha a cabine
        glPushMatrix();
            glScalef(0.3, 0.15, 1);
            glTranslatef(1.7, 0, 0);
            glColor3f(0, 0, 0);
            glBegin(GL_POLYGON);
            for(i=0; i<360; i+=20){
                x_aux = 1 * cos(M_PI*i/180);
                y_aux = 1 * sin(M_PI*i/180);
                glVertex3f(x_aux, y_aux, 0);
            }
            glEnd();
        glPopMatrix();

        // Desenha o rabo
        glPushMatrix();
        glTranslatef(-0.7, 0, 0);
        glColor3f(0, 0, 0);
        glBegin(GL_POLYGON);
            glVertex3f(0, 0, 0);
            glVertex3f(-0.1, 0.4, 0);
            glVertex3f(-0.3, 0.4, 0);
            glVertex3f(-0.3, -0.4, 0);
            glVertex3f(-0.1, -0.4, 0);
        glEnd();
        glPopMatrix();

    glPopMatrix();
}

void Inimigo::alterarAngulo(float incremento){
    angulo += incremento;
    angulo_canhao_arena += incremento;
}

void Inimigo::andar(float coeficiente_ajuste){
    y += sin(grausParaRadianos(angulo)) * velocidade * coeficiente_ajuste;
    x += cos(grausParaRadianos(angulo)) * velocidade * coeficiente_ajuste;
}

void Inimigo::girarHelices(float coeficiente_ajuste){
    angulo_helices += velocidade * coeficiente_ajuste * 2;
}

Inimigo::~Inimigo(){
}
