#include "circulo.h"
#include <math.h>
#include <GL/glut.h>

using namespace std;

Circulo::Circulo(){
	this->x = 0.;
	this->y = 0.;
	this->r = 0.;
}

Circulo::Circulo(float x, float y, float r, float cor_r, float cor_g, float cor_b, char* fill, int id){
	this->x = x;
	this->y = y;
	this->r = r;
    this->r_inicial = r;
    this->cor_r = cor_r;
    this->cor_g = cor_g;
    this->cor_b = cor_b;
    this->fill = fill;
    this->id = id;
}

void Circulo::desenhar(){
    float theta = 0.0;
    glColor3f(this->cor_r, this->cor_g, this->cor_b);
	glBegin(GL_LINE_LOOP);
    for(int i=0; i < NUMERO_SEGMENTOS; i++){
        glVertex3f(x + r * cos(theta), y + r * sin(theta), 0.0);
        theta += 2*3.1415926f /NUMERO_SEGMENTOS;
    }
    glEnd();
}

void Circulo::desenharPreenchido(){
    float theta = 0.0;
    glColor3f(this->cor_r, this->cor_g, this->cor_b);
	glBegin(GL_POLYGON);
    for(int i=0; i < NUMERO_SEGMENTOS; i++){
        glVertex3f(x + r * cos(theta), y + r * sin(theta), 0.0);
        theta += 2*3.1415926f /NUMERO_SEGMENTOS;
    }
    glEnd();
}



void Circulo::moverParaCima(float velocidade){
    this->y = this->y + velocidade;
}

void Circulo::moverParaBaixo(float velocidade){
    this->y = this->y - velocidade;
}

void Circulo::moverParaEsquerda(float velocidade){
    this->x = this->x - velocidade;
}

void Circulo::moverParaDireita(float velocidade){
    this->x = this->x + velocidade;
}



Circulo::~Circulo(){
}
