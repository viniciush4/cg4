#include <iostream>
#include <math.h>
#include "configuracao.h"
#include "circulo.h"
#include "jogador.h"
#include "inimigo.h"
#include "base.h"
#include "linha.h"
#include "placar.h"
#include "tiro.h"
#include "bomba.h"
#include "tinyxml2.h"
#include <GL/glut.h>
#include <vector>

#define grausParaRadianos(g) g*(M_PI/180)
#define radianosParaGraus(r) r*(180/M_PI)

using namespace tinyxml2;
using namespace std;

/*
 * MAPEAMENTO DAS TECLAS
 */
int teclas[256];

/*
 * VARIÁVEIS DO JOGO
 */
Configuracao configuracao;
vector<Inimigo> inimigos_aereos;
vector<Inimigo> inimigos_aereos_base;
vector<Base> inimigos_terrestres;
vector<Base> inimigos_terrestres_base;
vector<Tiro> tiros;
vector<Tiro> tiros_inimigos;
vector<Bomba> bombas;
Circulo arena;
Jogador jogador;
Jogador jogador_base;
Linha pista;
Placar placar;
int estado = 0;
int mouse_ultima_posicao_x = 0;
bool incrementar_angulo_inimigo = true;

/*
 * VARIÁVEIS PARA AJUSTE DO TEMPO
 */
static GLdouble previousTime = 0;
GLdouble currentTime;
GLdouble timeDiference;

/*
 * REGRAS DE NEGÓCIO
 */
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

void teletransportarInimigo(Inimigo& inimigo){
	double m = tanf(grausParaRadianos(inimigo.angulo));
	double E = inimigo.y - 0;
	double A = 1+pow(m,2);
	double B = -2*pow(m,2)*inimigo.x + 2*E*m;
	double C = pow(E,2) - 2*E*m*inimigo.x + pow(m,2)*pow(inimigo.x,2) - pow(arena.r,2);

	double x1 = (-B + sqrt(pow(B,2) - 4*A*C))/(2*A);
	double x2 = (-B - sqrt(pow(B,2) - 4*A*C))/(2*A);

	double y1 = inimigo.y + m*(x1-inimigo.x);
	double y2 = inimigo.y + m*(x2-inimigo.x);

	if(fabs(inimigo.x - x1) < fabs(inimigo.x - x2)){
		inimigo.x = x2;
		inimigo.y = y2;
	}else{
		inimigo.x = x1;
		inimigo.y = y1;
	}

}

void reiniciarJogo(){
	jogador = jogador_base;
	inimigos_terrestres = inimigos_terrestres_base;
	inimigos_aereos = inimigos_aereos_base;
	tiros.clear();
	tiros_inimigos.clear();
	bombas.clear();
	placar.resetarPlacar();
	estado = 0;
}

void tratarColisaoJogador(){

	// Colisão com inimigos voadores
	for(auto it=inimigos_aereos.begin(); it!=inimigos_aereos.end();++it){
		Inimigo &inimigo = *it;
		float distancia = sqrt(pow(inimigo.x - jogador.x,2)+pow(inimigo.y - jogador.y,2));
		if(distancia < jogador.r + inimigo.r)
			estado = 3;
	}

	// Colisão com inimigos voadores
	// for(int i=0; i < inimigos_aereos.size(); i++){
	// 	float distancia = sqrt(pow(inimigos_aereos.at(i).x - jogador.x,2)+pow(inimigos_aereos.at(i).y - jogador.y,2));
	// 	if(distancia < jogador.r + inimigos_aereos.at(i).r)
	// 		estado = 3;
	// }

	// Colisão com a arena
	float distancia = sqrt(pow(jogador.x,2)+pow(jogador.y,2));
	if(distancia > arena.r)
		teletransportarJogador();
}

void atualizarEstadoInimigos(){

	for(auto it=inimigos_aereos.begin(); it!=inimigos_aereos.end();++it){
		Inimigo &inimigo = *it;
		inimigo.girarHelices(timeDiference/1000);
		inimigo.andar((timeDiference/1000) * configuracao.inimigo_velocidade);
		// Colisão com a arena
		float distancia = sqrt(pow(inimigo.x,2)+pow(inimigo.y,2));
		if(distancia > arena.r) {
			teletransportarInimigo(inimigo);
		}
		// Muda o ângulo (180 graus em 5 segundos)
		float incremento_angulo = 180 / (5 / (timeDiference/1000));
		incremento_angulo = inimigo.incrementar_angulo ? incremento_angulo : -incremento_angulo;
		inimigo.somatorio_incremento_angulo += incremento_angulo;
		if(inimigo.incrementar_angulo && inimigo.somatorio_incremento_angulo > 180){
			inimigo.somatorio_incremento_angulo = 0;
			inimigo.incrementar_angulo = !inimigo.incrementar_angulo;
		}
		if(!inimigo.incrementar_angulo && inimigo.somatorio_incremento_angulo < -180){
			inimigo.somatorio_incremento_angulo = 0;
			inimigo.incrementar_angulo = !inimigo.incrementar_angulo;
		}
		inimigo.alterarAngulo(incremento_angulo);
	}

	// Atualiza os inimigos aéreos
	// for(int i=0; i < inimigos_aereos.size(); i++){
	// 	inimigos_aereos.at(i).girarHelices(timeDiference/1000);
	// 	inimigos_aereos.at(i).andar((timeDiference/1000) * configuracao.inimigo_velocidade);
	// 	// Colisão com a arena
	// 	float distancia = sqrt(pow(inimigos_aereos.at(i).x,2)+pow(inimigos_aereos.at(i).y,2));
	// 	if(distancia > arena.r) {
	// 		teletransportarInimigo(inimigos_aereos.at(i));
	// 	}
	// 	// Muda o ângulo (180 graus em 5 segundos)
	// 	float incremento_angulo = 180 / (5 / (timeDiference/1000));
	// 	incremento_angulo = inimigos_aereos.at(i).incrementar_angulo ? incremento_angulo : -incremento_angulo;
	// 	inimigos_aereos.at(i).somatorio_incremento_angulo += incremento_angulo;
	// 	if(inimigos_aereos.at(i).incrementar_angulo && inimigos_aereos.at(i).somatorio_incremento_angulo > 180){
	// 		inimigos_aereos.at(i).somatorio_incremento_angulo = 0;
	// 		inimigos_aereos.at(i).incrementar_angulo = !inimigos_aereos.at(i).incrementar_angulo;
	// 	}
	// 	if(!inimigos_aereos.at(i).incrementar_angulo && inimigos_aereos.at(i).somatorio_incremento_angulo < -180){
	// 		inimigos_aereos.at(i).somatorio_incremento_angulo = 0;
	// 		inimigos_aereos.at(i).incrementar_angulo = !inimigos_aereos.at(i).incrementar_angulo;
	// 	}
	// 	inimigos_aereos.at(i).alterarAngulo(incremento_angulo);
	// }
}

void atualizarEstadoTirosInimigos(){
	// Atualiza estado dos tiros inimigos
	for(auto it=tiros_inimigos.begin(); it!=tiros_inimigos.end();){
		Tiro &tiro_inimigo = *it;
		// Posição
		tiro_inimigo.mover((timeDiference/1000) * configuracao.inimigo_velocidade_tiro);
		// Condição de remoção do tiro
		float distancia = sqrt(pow(tiro_inimigo.x,2)+pow(tiro_inimigo.y,2));
		if(distancia > arena.r){
			it = tiros_inimigos.erase(it);
		} 
		else 
		{
			// Colisão com o jogador
			float distancia_jogador = sqrtf(powf(tiro_inimigo.x - jogador.x,2)+powf(tiro_inimigo.y - jogador.y,2));
			if(distancia_jogador < jogador.r + tiro_inimigo.r){
				it = tiros_inimigos.erase(it);
				estado = 3;
			}
			else
			{
				++it;
			}
		}
	}

	// Atualiza estado dos tiros inimigos
	// for(int i=0; i < tiros_inimigos.size(); i++){
	// 	// Posição
	// 	tiros_inimigos.at(i).mover((timeDiference/1000) * configuracao.inimigo_velocidade_tiro);
	// 	// Condição de remoção do tiro
	// 	float distancia = sqrt(pow(tiros_inimigos.at(i).x,2)+pow(tiros_inimigos.at(i).y,2));
	// 	if(distancia > arena.r){
	// 		tiros_inimigos.erase(tiros_inimigos.begin()+i);
	// 	} 
	// 	else 
	// 	{
	// 		// Colisão com o jogador
	// 		float distancia_jogador = sqrt(pow(tiros_inimigos.at(i).x - jogador.x,2)+pow(tiros_inimigos.at(i).y - jogador.y,2));
	// 		// cout << jogador.escala << "..." << jogador.r << "............["<< i <<"] Distancia: " << distancia_jogador << endl;
	// 		if(distancia_jogador < jogador.r + tiros_inimigos.at(i).r){
	// 			estado = 3;
	// 		}
	// 	}
	// }
}

void atualizarEstadoTirosJogador(){

	for(auto it=tiros.begin(); it!=tiros.end();){
		Tiro &tiro = *it;
		// Posição
		tiro.mover((timeDiference/1000) * configuracao.tiro_velocidade);
		// Condição de remoção do tiro
		float distancia = sqrt(pow(tiro.x,2)+pow(tiro.y,2));
		if(distancia > arena.r){
			it = tiros.erase(it);
		} 
		else 
		{
			// Colisão com algum inimigo
			float houve_colisao = false;
			for(auto it2=inimigos_aereos.begin(); it2!=inimigos_aereos.end();){
				Inimigo &inimigo = *it2;
				float distancia_inimigo = sqrt(pow(tiro.x - inimigo.x,2)+pow(tiro.y - inimigo.y,2));
				if(distancia_inimigo < inimigo.r + tiro.r){
					it2 = inimigos_aereos.erase(it2);
					it = tiros.erase(it);
					houve_colisao = true;
					break;
				} else {
					++it2;
				}
			}

			if(!houve_colisao){
				++it;
			}
		}
	}

	// Atualiza estado dos tiros
	// for(int i=0; i < tiros.size(); i++){
	// 	// Posição
	// 	tiros.at(i).mover((timeDiference/1000) * configuracao.tiro_velocidade);
	// 	// Condição de remoção do tiro
	// 	float distancia = sqrt(pow(tiros.at(i).x,2)+pow(tiros.at(i).y,2));
	// 	if(distancia > arena.r){
	// 		tiros.erase(tiros.begin()+i);
	// 	} 
	// 	else 
	// 	{
	// 		// Colisão com algum inimigo
	// 		for(int j=0; j < inimigos_aereos.size(); j++){
	// 			float distancia_inimigo = sqrt(pow(tiros.at(i).x - inimigos_aereos.at(j).x,2)+pow(tiros.at(i).y - inimigos_aereos.at(j).y,2));
	// 			if(distancia_inimigo < inimigos_aereos.at(j).r + tiros.at(i).r){
	// 				inimigos_aereos.erase(inimigos_aereos.begin()+j);
	// 				tiros.erase(tiros.begin()+i);
	// 			}
	// 		}
	// 	}
	// }
}

void atualizarEstadoBombas(){
	
	for(auto it=bombas.begin(); it!=bombas.end();){
		Bomba &bomba = *it;
		// Posição
		bomba.mover(timeDiference/1000);
		// Escala
		if(bomba.r > bomba.r_inicial / 2){
			float decremento_raio = (bomba.r_inicial / 2) / (2 / (timeDiference/1000));
			bomba.decrementarRaio(decremento_raio);
		}
		// Condições de remoção da bomba (saiu da arena ou chegou ao raio mínimo)
		float distancia = sqrt(pow(bomba.x,2)+pow(bomba.y,2));
		if(distancia > arena.r || bomba.r <= bomba.r_inicial / 2){
			
			// Se a bomba está em cima de alguma base inimiga, deleta a base
			for(auto it2=inimigos_terrestres.begin(); it2!=inimigos_terrestres.end();){
				Base &base = *it2;
				float distancia_base = sqrt(pow(bomba.x - base.x,2)+pow(bomba.y - base.y,2));
				if(distancia_base < (bomba.r + base.r)){
					placar.incrementarBasesDestruidas();
					it2 = inimigos_terrestres.erase(it2);
					if(placar.quantidade_bases_restantes == 0){
						estado = 3;
					}
				} else {
					++it2;
				}
			}
			it = bombas.erase(it);
		} else {
			++it;
		}
	}

	// Atualiza estado das bombas
	// for(int i=0; i < bombas.size(); i++){
	// 	// Posição
	// 	bombas.at(i).mover(timeDiference/1000);
	// 	// Escala
	// 	if(bombas.at(i).r > bombas.at(i).r_inicial / 2){
	// 		float decremento_raio = (bombas.at(i).r_inicial / 2) / (2 / (timeDiference/1000));
	// 		bombas.at(i).decrementarRaio(decremento_raio);
	// 	}
	// 	// Condições de remoção da bomba (saiu da arena ou chegou ao raio mínimo)
	// 	float distancia = sqrt(pow(bombas.at(i).x,2)+pow(bombas.at(i).y,2));
	// 	if(distancia > arena.r || bombas.at(i).r <= bombas.at(i).r_inicial / 2){
			
	// 		// Se a bomba está em cima de alguma base inimiga, deleta a base
	// 		for(int j=0; j < inimigos_terrestres.size(); j++){
	// 			float distancia_base = sqrt(pow(bombas.at(i).x - inimigos_terrestres.at(j).x,2)+pow(bombas.at(i).y - inimigos_terrestres.at(j).y,2));
	// 			if(distancia_base < (bombas.at(i).r + inimigos_terrestres.at(j).r)){
	// 				placar.incrementarBasesDestruidas();
	// 				inimigos_terrestres.erase(inimigos_terrestres.begin()+j);
	// 				if(placar.quantidade_bases_restantes == 0){
	// 					estado = 3;
	// 				}
	// 			}
	// 		}
			
	// 		bombas.erase(bombas.begin()+i);
	// 	}
	// }
}

void criarTirosInimigos(){
	// Faz os inimigos aéreos atirar
	for(auto it=inimigos_aereos.begin(); it!=inimigos_aereos.end();++it){
		Inimigo &inimigo = *it;
		// Atira
		if(inimigo.tempo_desde_ultimo_tiro > 1000/configuracao.inimigo_frequencia_tiro){
			Tiro tiro = Tiro(
				inimigo.x, 
				inimigo.y,
				inimigo.r,
				inimigo.angulo, 	// angulo jogador
				inimigo.escala,
				0, 								// angulo canhão
				inimigo.angulo, 	// angulo canhão arena
				1,
				0.5,
				0,
				inimigo.velocidade
			);
			tiros_inimigos.push_back(tiro);
			inimigo.tempo_desde_ultimo_tiro = 0;
		} else {
			inimigo.tempo_desde_ultimo_tiro += timeDiference;
		}
	}

	// for(int i=0; i < inimigos_aereos.size(); i++){
	// 	// Atira
	// 	if(inimigos_aereos.at(i).tempo_desde_ultimo_tiro > configuracao.inimigo_frequencia_tiro*1000){
	// 		Tiro tiro = Tiro(
	// 			inimigos_aereos.at(i).x, 
	// 			inimigos_aereos.at(i).y,
	// 			inimigos_aereos.at(i).r,
	// 			inimigos_aereos.at(i).angulo, 	// angulo jogador
	// 			inimigos_aereos.at(i).escala,
	// 			0, 								// angulo canhão
	// 			inimigos_aereos.at(i).angulo, 	// angulo canhão arena
	// 			1,
	// 			0.5,
	// 			0,
	// 			inimigos_aereos.at(i).velocidade
	// 		);
	// 		tiros_inimigos.push_back(tiro);
	// 		inimigos_aereos.at(i).tempo_desde_ultimo_tiro = 0;
	// 	} else {
	// 		inimigos_aereos.at(i).tempo_desde_ultimo_tiro += timeDiference;
	// 	}
	// }
}

/*
 * CALLBACK'S
 */

void display(void){

	glClear(GL_COLOR_BUFFER_BIT);
	
	arena.desenharPreenchido();

	pista.desenhar();

	for(auto it=inimigos_terrestres.begin(); it!=inimigos_terrestres.end();++it){
		Base &base = *it;
		base.desenharPreenchido();
	}

	for(auto it=bombas.begin(); it!=bombas.end();++it){
		Bomba &bomba = *it;
		bomba.desenharPreenchido();
	}

	for(auto it=tiros.begin(); it!=tiros.end();++it){
		Tiro &tiro = *it;
		tiro.desenharPreenchido();
	}

	for(auto it=tiros_inimigos.begin(); it!=tiros_inimigos.end();++it){
		Tiro &tiro_inimigo = *it;
		tiro_inimigo.desenharPreenchido();
	}


	// for(int i=0; i < inimigos_terrestres.size(); i++){
	// 	inimigos_terrestres.at(i).desenharPreenchido();
	// }

	// for(int i=0; i < bombas.size(); i++){
	// 	bombas.at(i).desenharPreenchido();
	// }

	// for(int i=0; i < tiros.size(); i++){
	// 	tiros.at(i).desenharPreenchido();
	// }

	// for(int i=0; i < tiros_inimigos.size(); i++){
	// 	tiros_inimigos.at(i).desenharPreenchido();
	// }	

	jogador.desenharPreenchido();

	for(auto it=inimigos_aereos.begin(); it!=inimigos_aereos.end();++it){
		Inimigo &inimigo = *it;
		inimigo.desenharPreenchido();
	}

	// for(int i=0; i < inimigos_aereos.size(); i++){
	// 	inimigos_aereos.at(i).desenharPreenchido();
	// }

	placar.desenharPreenchido();
	
	if(estado == 3){
		placar.desenharMensagemFinal();
	}
	
	glutSwapBuffers();
}

void idle(void){
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	timeDiference = currentTime - previousTime;
	previousTime = currentTime;

	// Decolando
	if(estado == 1){
		
		float distancia 		= sqrt(pow(pista.y2-pista.y1,2) + pow(pista.x2-pista.x1,2));
		float distancia_jogador = sqrt(pow(jogador.y-pista.y1,2) + pow(jogador.x-pista.x1,2));
		jogador.velocidade = jogador.velocidade + (distancia / 8) * (timeDiference/1000);

		jogador.girarHelices(timeDiference/1000);
		jogador.andar(timeDiference/1000);

		if(distancia_jogador >= distancia / 2)
			jogador.alterarEscala((timeDiference/1000) * jogador.velocidade * (jogador.r_inicial / (distancia / 2)));			

		if(distancia_jogador >= distancia)
			estado = 2;

	}

	// Jogando
	else if(estado == 2){

		jogador.girarHelices(timeDiference/1000);
		jogador.andar((timeDiference/1000) * configuracao.jogador_velocidade);

		tratarColisaoJogador();

		atualizarEstadoTirosInimigos();

		atualizarEstadoTirosJogador();

		atualizarEstadoBombas();

		criarTirosInimigos();		

		if(teclas['a'] == 1) {
			jogador.alterarAngulo(+1 * timeDiference/1000);
		}
		if(teclas['d'] == 1) {
			jogador.alterarAngulo(-1 * timeDiference/1000);
		}
	}

	if(estado != 3){
		atualizarEstadoInimigos();
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
			jogador.velocidade += (estado == 2) ? 1 : 0;
			break;
		case '-':
			jogador.velocidade -= (estado == 2) ? ((jogador.velocidade > 50) ? 1 : 0) : 0;
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
			jogador.escala,
			jogador.angulo_canhao,
			jogador.angulo_canhao_arena,
			jogador.cor_r,
			jogador.cor_g,
			jogador.cor_b,
			jogador.velocidade
		);
		tiros.push_back(tiro);
	}
	if(button == 2 && state == 0 && estado == 2){
		Bomba bomba = Bomba(
			jogador.x, 
			jogador.y,
			jogador.r,
			jogador.angulo,
			jogador.velocidade
		);
		bombas.push_back(bomba);
	}

}

/*
 * FUNÇÃO PRINCIPAL
 */

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

		XMLElement* xml_inimigo 			= doc_configuracoes.FirstChildElement()->FirstChildElement( "inimigo" );
		xml_inimigo->QueryFloatAttribute( "vel", &configuracao.inimigo_velocidade );
		xml_inimigo->QueryFloatAttribute( "velTiro", &configuracao.inimigo_velocidade_tiro );
		xml_inimigo->QueryFloatAttribute( "freqTiro", &configuracao.inimigo_frequencia_tiro );


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
				Inimigo i = Inimigo();
				i.cor_r = 1;
				i.cor_g = 0;
				i.cor_b = 0;
				i.x = circulo.x;
				i.y = circulo.y;
				i.r = circulo.r;
				i.r_inicial = circulo.r;
				i.fill = circulo.fill;
				i.id = circulo.id;
				i.escala = circulo.r;
				inimigos_aereos.push_back(i);
				inimigos_aereos_base.push_back(i);
			}
			if(!strcmp(circulo.fill,"orange")){
				Base b = Base();
				b.cor_r = 1;
				b.cor_g = 0.5;
				b.cor_b = 0;
				b.x = circulo.x;
				b.y = circulo.y;
				b.r = circulo.r;
				b.r_inicial = circulo.r;
				b.fill = circulo.fill;
				b.id = circulo.id;
				b.escala = circulo.r;
				inimigos_terrestres.push_back(b);
				inimigos_terrestres_base.push_back(b);
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

		// Calcula a velocidade dos inimigos
		float distancia 					= sqrt(pow(pista.y2-pista.y1,2) + pow(pista.x2-pista.x1,2));
		float velocidade_decolagem_final 	= (distancia / 8) * 4;
		
		// Guarda instância Jogador
		jogador_base = jogador;
		// Guarda instâncias Inimigos aéreos
		for(int i=0; i < inimigos_aereos.size(); i++){
			inimigos_aereos.at(i).velocidade = velocidade_decolagem_final;
        	inimigos_aereos_base.push_back(inimigos_aereos.at(i));
		}
		// Guarda instâncias Inimigos terrestres
		for(int i=0; i < inimigos_terrestres.size(); i++){
			inimigos_terrestres_base.push_back(inimigos_terrestres.at(i));
		}

		// Cria o placar
		placar = Placar(arena.r, inimigos_terrestres.size());

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(arena.r*2, arena.r*2);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Trabalho 4");
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
