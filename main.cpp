#include <iostream>
#include <math.h>
#include "configuracao.h"
#include "circulo.h"
#include "jogador.h"
#include "linha.h"
#include "tiro.h"
#include "bomba.h"
#include "tinyxml2.h"
#include <GL/glut.h>
#include <vector>

#define grausParaRadianos(g) g*(M_PI/180)
#define radianosParaGraus(r) r*(180/M_PI)

using namespace tinyxml2;
using namespace std;

int teclas[256];

Configuracao configuracao;
vector<Circulo> inimigos_aereos;
vector<Circulo> inimigos_terrestres;
vector<Tiro> tiros;
vector<Bomba> bombas;
Circulo arena;
Jogador jogador;
Jogador jogador_base;
Linha pista;
int estado = 0;
float velocidade_decolagem = 0;
int mouse_ultima_posicao_x = 0;

static GLdouble previousTime = 0;
GLdouble currentTime;
GLdouble timeDiference;

void teletransportarJogador(){
	double m = tanf(grausParaRadianos(jogador.angulo));
	double E = jogador.y - 0;
	double A = 1+pow(m,2);
	double B = -2*pow(m,2)*jogador.x + 2*E*m;
	double C = pow(E,2) - 2*E*m*jogador.x + pow(m,2)*pow(jogador.x,2) - pow(arena.r,2);

	double x1 = (-B + sqrt(pow(B,2) - 4*A*C))/(2*A);
	double x2 = (-B - sqrt(pow(B,2) - 4*A*C))/(2*A);

	double y1 = jogador.y + m*(x1-jogador.x);
	double y2 = jogador.y + m*(x2-jogador.x);

	if(fabs(jogador.x - x1) < fabs(jogador.x - x2)){
		jogador.x = x2;
		jogador.y = y2;
	}else{
		jogador.x = x1;
		jogador.y = y1;
	}

}

void reiniciarJogo(){
	jogador = jogador_base;
	velocidade_decolagem = 0;
	tiros.clear();
	bombas.clear();
	estado = 0;
}

void verificarColisao(){

	// Colisão com inimigos voadores
	for(int i=0; i < inimigos_aereos.size(); i++){
		float distancia = sqrt(pow(inimigos_aereos.at(i).x - jogador.x,2)+pow(inimigos_aereos.at(i).y - jogador.y,2));
		if(distancia < jogador.r + inimigos_aereos.at(i).r)
			estado = 3;
	}

	// Colisão com a arena
	float distancia = sqrt(pow(jogador.x,2)+pow(jogador.y,2));
	if(distancia > arena.r)
		teletransportarJogador();
}

void display(void){

	glClear(GL_COLOR_BUFFER_BIT);
	
	arena.desenharPreenchido();

	pista.desenhar();

	for(int i=0; i < inimigos_terrestres.size(); i++){
		inimigos_terrestres.at(i).desenharPreenchido();
	}

	for(int i=0; i < tiros.size(); i++){
		tiros.at(i).desenharPreenchido();
	}	

	for(int i=0; i < bombas.size(); i++){
		bombas.at(i).desenharPreenchido();
	}

	for(int i=0; i < inimigos_aereos.size(); i++){
		inimigos_aereos.at(i).desenharPreenchido();
	}

	jogador.desenharPreenchido();

	glutSwapBuffers();
}

void idle(void){
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	timeDiference = currentTime - previousTime;
	previousTime = currentTime;

	if(estado == 1){
		
		float distancia 		= sqrt(pow(pista.y2-pista.y1,2) + pow(pista.x2-pista.x1,2));
		float distancia_jogador = sqrt(pow(jogador.y-pista.y1,2) + pow(jogador.x-pista.x1,2));
		velocidade_decolagem 	= velocidade_decolagem + (distancia / 8) * (timeDiference/1000); 

		jogador.girarHelices(velocidade_decolagem * (timeDiference/1000));
		jogador.andar(velocidade_decolagem * (timeDiference/1000));

		if(distancia_jogador >= distancia / 2)
			jogador.alterarEscala((timeDiference/1000) * velocidade_decolagem * (jogador.r_inicial / (distancia / 2)));			

		if(distancia_jogador >= distancia)
			estado = 2;

	}
	else if(estado == 2){

		jogador.girarHelices(velocidade_decolagem * 2 * (timeDiference/1000));
		jogador.andar(velocidade_decolagem * (timeDiference/1000));

		verificarColisao();

		// Atualiza estado dos tiros
		for(int i=0; i < tiros.size(); i++){
			// Posição
			tiros.at(i).mover(velocidade_decolagem * (timeDiference/1000) * configuracao.tiro_velocidade);
			// Condição de remoção do tiro
			float distancia = sqrt(pow(tiros.at(i).x,2)+pow(tiros.at(i).y,2));
			if(distancia > arena.r){
				tiros.erase(tiros.begin()+i);
			}
		}

		// Atualiza estado das bombas
		for(int i=0; i < bombas.size(); i++){
			// Posição
			bombas.at(i).mover(velocidade_decolagem * (timeDiference/1000));
			// Escala
			if(bombas.at(i).r > bombas.at(i).r_inicial / 2){
				float decremento_raio = (bombas.at(i).r_inicial / 2) / (4 / (timeDiference/1000));
				bombas.at(i).decrementarRaio(decremento_raio);
			}
			// Condições de remoção da bomba (saiu da arena ou chegou ao raio mínimo)
			float distancia = sqrt(pow(bombas.at(i).x,2)+pow(bombas.at(i).y,2));
			if(distancia > arena.r || bombas.at(i).r <= bombas.at(i).r_inicial / 2){
				bombas.erase(bombas.begin()+i);
			}
		}

		if(teclas['a'] == 1) {
			jogador.alterarAngulo(+1 * velocidade_decolagem * (timeDiference/1000));
		}
		if(teclas['d'] == 1) {
			jogador.alterarAngulo(-1 * velocidade_decolagem * (timeDiference/1000));
		}
	}

	if(teclas['r'] == 1) {
		reiniciarJogo();
	}

	// Marca a janela atual como precisando ser reexibida
	glutPostRedisplay();
}

void keyPress(unsigned char key, int x, int y) {
	teclas[key] = 1;	
	switch (key)
	{
		case 'u':
			estado = (estado == 0) ? 1 : estado;
			break;
		case '+':
			velocidade_decolagem += (estado == 2) ? 1 : 0;
			break;
		case '-':
			velocidade_decolagem -= (estado == 2) ? ((velocidade_decolagem > 50) ? 1 : 0) : 0;
			break;
	}
}

void keyUp(unsigned char key, int x, int y) {
	teclas[key] = 0;
}

void passiveMotion(int x, int y){
	if(x > mouse_ultima_posicao_x)
		jogador.alterarAnguloCanhao(-3);
	if(x < mouse_ultima_posicao_x)
		jogador.alterarAnguloCanhao(3);

	mouse_ultima_posicao_x = x;
}

void mouse(int button, int state, int x, int y){

	if(button == 0 && state == 0 && estado == 2){
		Tiro tiro = Tiro(
			jogador.x, 
			jogador.y,
			jogador.r,
			jogador.angulo, 
			jogador.angulo_canhao,
			jogador.angulo_canhao_arena
		);
		tiros.push_back(tiro);
	}
	if(button == 2 && state == 0 && estado == 2){
		Bomba bomba = Bomba(
			jogador.x, 
			jogador.y,
			jogador.r,
			jogador.angulo
		);
		bombas.push_back(bomba);
	}

}

int main(int argc, char** argv){

	// ***********************************
	// Realiza a leitura das configurações
	// ***********************************
	XMLDocument doc_configuracoes;
    bool loaded_conf = doc_configuracoes.LoadFile(strcat(argv[1],"config.xml"));
	if(loaded_conf){
		cout << "Erro ao carregar configurações" << endl;
		return 1;
	}

		XMLElement* xml_arquivo_arena 		= doc_configuracoes.FirstChildElement()->FirstChildElement( "arquivoDaArena" );
		configuracao.arquivo_arena_nome 	= xml_arquivo_arena->FirstChildElement( "nome" )->GetText();
		configuracao.arquivo_arena_tipo 	= xml_arquivo_arena->FirstChildElement( "tipo" )->GetText();
		configuracao.arquivo_arena_caminho 	= xml_arquivo_arena->FirstChildElement( "caminho" )->GetText();

		XMLElement* xml_jogador 			= doc_configuracoes.FirstChildElement()->FirstChildElement( "jogador" );
		xml_jogador->QueryFloatAttribute( "vel", &configuracao.jogador_velocidade );
		xml_jogador->QueryFloatAttribute( "velTiro", &configuracao.tiro_velocidade );


	// ***********************************
	// Realiza a leitura da arena
	// ***********************************
	XMLDocument doc_arena;
	char* caminho_arena_svg = strcat(strcat(strcat((char*)configuracao.arquivo_arena_caminho, configuracao.arquivo_arena_nome),"."), configuracao.arquivo_arena_tipo);
	bool loaded_arena = doc_arena.LoadFile(caminho_arena_svg);
	if(loaded_arena){
		cout << "Erro ao carregar arena" << endl;
		return 1;
	}
		XMLElement* xml_linha = doc_arena.FirstChildElement( "svg" )->FirstChildElement( "line" );
		Linha linha = Linha();
		xml_linha->QueryFloatAttribute("x1", &linha.x1);
		xml_linha->QueryFloatAttribute("y1", &linha.y1);
		xml_linha->QueryFloatAttribute("x2", &linha.x2);
		xml_linha->QueryFloatAttribute("y2", &linha.y2);
		pista = linha;

		XMLElement* xml_circulo = doc_arena.FirstChildElement( "svg" )->FirstChildElement( "circle" );
		for(XMLElement* e = xml_circulo; e != NULL; e = e->NextSiblingElement("circle")) {
			
			Circulo circulo = Circulo();
			
			e->QueryFloatAttribute("cx", &circulo.x);
			e->QueryFloatAttribute("cy", &circulo.y);
			e->QueryFloatAttribute("r", &circulo.r);
			e->QueryFloatAttribute("r", &circulo.r_inicial);
			e->QueryStringAttribute("fill", &circulo.fill);
			e->QueryIntAttribute("id", &circulo.id);

			if(!strcmp(circulo.fill,"blue")){
				circulo.cor_r = 0;
				circulo.cor_g = 0;
				circulo.cor_b = 1;
				arena = circulo;
			}
			if(!strcmp(circulo.fill,"red")){
				circulo.cor_r = 1;
				circulo.cor_g = 0;
				circulo.cor_b = 0;
				inimigos_aereos.push_back(circulo);
			}
			if(!strcmp(circulo.fill,"orange")){
				circulo.cor_r = 1;
				circulo.cor_g = 0.5;
				circulo.cor_b = 0;
				inimigos_terrestres.push_back(circulo);
			}
			if(!strcmp(circulo.fill,"green")){
				Jogador j = Jogador();
				j.cor_r = 0;
				j.cor_g = 1;
				j.cor_b = 0;
				j.x = circulo.x;
				j.y = circulo.y;
				j.r = circulo.r;
				j.r_inicial = circulo.r;
				j.fill = circulo.fill;
				j.id = circulo.id;
				j.escala = circulo.r;
				jogador = j;
			}
		}

		int fator_correcao_x = (arena.r - arena.x) - arena.r;
		int fator_correcao_y = (arena.r - arena.y) - arena.r;

		// Normaliza todos os elementos
		arena.x += fator_correcao_x;
		arena.y += fator_correcao_y;
		arena.y *= -1;

		jogador.x += fator_correcao_x;
		jogador.y += fator_correcao_y;
		jogador.y *= -1;

		pista.x1 += fator_correcao_x;
		pista.y1 += fator_correcao_y;
		pista.y1 *= -1;
		pista.x2 += fator_correcao_x;
		pista.y2 += fator_correcao_y;
		pista.y2 *= -1;

		for(int i=0; i < inimigos_aereos.size(); i++){
			inimigos_aereos.at(i).x += fator_correcao_x;
			inimigos_aereos.at(i).y += fator_correcao_y;
			inimigos_aereos.at(i).y *= -1;
		}

		for(int i=0; i < inimigos_terrestres.size(); i++){
			inimigos_terrestres.at(i).x += fator_correcao_x;
			inimigos_terrestres.at(i).y += fator_correcao_y;
			inimigos_terrestres.at(i).y *= -1;
		}

		// Seta valores iniciais para os ângulos
		float angulo = atan2((pista.y2-pista.y1), (pista.x2-pista.x1));
		jogador.angulo = radianosParaGraus(angulo);
		jogador.angulo_canhao_arena = radianosParaGraus(angulo);
		jogador_base = jogador;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(arena.r*2, arena.r*2);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Trabalho 2");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-arena.r, arena.r, -arena.r, arena.r, -1.0, 1.0);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyUp);
	glutPassiveMotionFunc(passiveMotion);
	glutMouseFunc(mouse);
	glutMainLoop();

	return 0;
}
