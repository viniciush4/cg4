#define NUMERO_SEGMENTOS 50

class Jogador {
public:
	float x;
	float y;
	float r;
	float r_inicial;
	float cor_r;
	float cor_g;
	float cor_b;
	const char* fill;
	int id;
	float angulo;
	float escala;
	float angulo_canhao;
	float angulo_canhao_arena;
	float angulo_helices;
	float velocidade;

	Jogador();
	Jogador(float x, float y, float r, float cor_r, float cor_g, float cor_b, char* fill, int id, float velocidade);
	void desenharPreenchido();
	void alterarAngulo(float coeficiente_ajuste);
	void alterarAnguloCanhao(float incremento);
	void alterarEscala(float incremento);
	void andar(float coeficiente_ajuste);
	void girarHelices(float coeficiente_ajuste);
	
	virtual ~Jogador();
};
