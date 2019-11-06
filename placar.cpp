#include <iostream>
#include "placar.h"
#include <math.h>
#include <GL/glut.h>

using namespace std;

Placar::Placar(){
	this->raio_arena = 0;
    this->quantidade_bases = 0;
    this->quantidade_bases_destruidas = 0;
    this->quantidade_bases_restantes = 0;
}

Placar::Placar(float raio_arena, int quantidade_bases){
	this->raio_arena = raio_arena;
    this->quantidade_bases = quantidade_bases;
    this->quantidade_bases_destruidas = 0;
    this->quantidade_bases_restantes = quantidade_bases;
}

void Placar::desenharPreenchido(){

    char buffer_bases_destruidas [20];
    sprintf (buffer_bases_destruidas, "%d bases destruidas", this->quantidade_bases_destruidas);
    
    glPushMatrix();
        glTranslatef(this->raio_arena-(20*8), this->raio_arena-15, 0);
        glPushMatrix();
            glColor3f(1, 1, 1);
            glRasterPos3f(0, 0, 0);
            for (size_t i = 0; i < 19; i++) {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, buffer_bases_destruidas[i]);
            }
        glPopMatrix();
    glPopMatrix();

    char buffer_bases_restantes [13];
    sprintf (buffer_bases_restantes, "%d restantes", this->quantidade_bases_restantes);

    glPushMatrix();
        glTranslatef(this->raio_arena-(13*8), this->raio_arena-32, 0);
        glPushMatrix();
            glColor3f(1, 1, 1);
            glRasterPos3f(0, 0, 0);
            for (size_t i = 0; i < 12; i++) {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, buffer_bases_restantes[i]);
            }
        glPopMatrix();
    glPopMatrix();
}

void Placar::desenharMensagemFinal(){
    
    if(quantidade_bases_restantes == 0){
        char buffer [23];
        sprintf (buffer, "PARABENS, VOCE GANHOU!");

        // Quadrado
        glPushMatrix();
            glTranslatef(0, 0, 0);
            glColor3f(0, 1, 0);
            glBegin(GL_POLYGON);
                glVertex3f(-90, -5, 0);
                glVertex3f(88, -5, 0);
                glVertex3f(88, 13, 0);
                glVertex3f(-90, 13, 0);
            glEnd();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-88, 0, 0);
            glPushMatrix();
                glColor3f(0, 0, 0);
                glRasterPos3f(0, 0, 0);
                for (size_t i = 0; i < 22; i++) {
                    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, buffer[i]);
                }
            glPopMatrix();
        glPopMatrix();

    } else {
        
        char buffer [23];
        sprintf (buffer, "QUE PENA, VOCE PERDEU!");

        // Quadrado
        glPushMatrix();
            glTranslatef(0, 0, 0);
            glColor3f(1, 0, 0);
            glBegin(GL_POLYGON);
                glVertex3f(-90, -5, 0);
                glVertex3f(88, -5, 0);
                glVertex3f(88, 13, 0);
                glVertex3f(-90, 13, 0);
            glEnd();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-88, 0, 0);
            glPushMatrix();
                glColor3f(1, 1, 1);
                glRasterPos3f(0, 0, 0);
                for (size_t i = 0; i < 22; i++) {
                    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, buffer[i]);
                }
            glPopMatrix();
        glPopMatrix();
    }
}

void Placar::incrementarBasesDestruidas(){
    this->quantidade_bases_destruidas++;
    this->quantidade_bases_restantes--;
}

void Placar::resetarPlacar(){
    this->quantidade_bases_destruidas = 0;
    this->quantidade_bases_restantes = this->quantidade_bases;
}

Placar::~Placar(){
}
