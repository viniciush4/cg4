#include "linha.h"
#include <math.h>
#include <GL/glut.h>

using namespace std;

Linha::Linha(){
	this->x1 = 0.;
	this->y1 = 0.;
    this->x2 = 0.;
	this->y2 = 0.;
	this->cor_r = 0.;
    this->cor_g = 0.;
    this->cor_b = 0.;
    this->style = "";
    this->id = 0;
}

Linha::Linha(float x1, float y1, float x2, float y2){
	this->x1 = x1;
	this->y1 = y1;
    this->x2 = x2;
	this->y2 = y2;
}

void Linha::desenhar(){
    glColor3f(this->cor_r, this->cor_g, this->cor_b);
	glBegin(GL_LINES);
        glVertex3f(x1, y1, 0.0);
        glVertex3f(x2, y2, 0.0);
    glEnd();
}

Linha::~Linha(){
}
