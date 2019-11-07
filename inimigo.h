#define NUMERO_SEGMENTOS 50

class Inimigo {
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
	float somatorio_incremento_angulo;
	bool incrementar_angulo;
	float tempo_desde_ultimo_tiro;
	float velocidade;

	Inimigo();
	Inimigo(float x, float y, float r, float cor_r, float cor_g, float cor_b, char* fill, int id, float velocidade);
	void desenharPreenchido();
	void alterarAngulo(float incremento);
	void andar(float coeficiente_ajuste);
	void girarHelices(float coeficiente_ajuste);
	
	virtual ~Inimigo();
};
