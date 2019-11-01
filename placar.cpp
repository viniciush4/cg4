#include <iostream>
#include "placar.h"
#include <math.h>
#include <GL/glut.h>

using namespace std;

#define grausParaRadianos(g) g*(M_PI/180)
#define radianosParaGraus(r) r*(180/M_PI)

Placar::Placar(){
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
}

Placar::Placar(float largura){
	this->x = 0;
	this->y = 0;
	this->largura = largura;
}

void Placar::desenharPreenchido(){
    glPushMatrix();

        glTranslatef(x, y, 0);

        // Desenha o placar
        glPushMatrix();
            glTranslatef(x, y, 0);
            glColor3f(1, 1, 1);
            glBegin(GL_POLYGON);
                glVertex3f(0, 0, 0);
                glVertex3f(largura, 0, 0);
                glVertex3f(largura, largura, 0);
                glVertex3f(0, largura, 0);
            glEnd();
        glPopMatrix();

    glPopMatrix();
}

void Placar::moverParaCima(float velocidade){
    this->y = this->y + velocidade;
}

void Placar::moverParaBaixo(float velocidade){
    this->y = this->y - velocidade;
}

void Placar::moverParaEsquerda(float velocidade){
    this->x = this->x - velocidade;
}

void Placar::moverParaDireita(float velocidade){
    this->x = this->x + velocidade;
}

void Placar::alterarAngulo(float velocidade){
    this->angulo += velocidade;
    this->angulo_canhao_arena += velocidade;
}

void Placar::alterarAnguloCanhao(float velocidade){
    if(this->angulo_canhao + velocidade <= 45 && this->angulo_canhao + velocidade >= -45){
        this->angulo_canhao += velocidade;
        this->angulo_canhao_arena += velocidade;
    }
}

void Placar::alterarEscala(float velocidade){
    this->escala += velocidade;
    this->r += velocidade;
}

void Placar::andar(float velocidade){
    this->y += sin(grausParaRadianos(angulo)) * velocidade;
    this->x += cos(grausParaRadianos(angulo)) * velocidade;
}

void Placar::girarHelices(float velocidade){
    this->angulo_helices += velocidade;
}

Placar::~Placar(){
}
