#define NUMERO_SEGMENTOS 50

class Placar {
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
	float largura;

	Placar();
	Placar(float largura);
	void desenhar();
	void desenharPreenchido();

	void moverParaCima(float velocidade);
	void moverParaBaixo(float velocidade);
	void moverParaEsquerda(float velocidade);
	void moverParaDireita(float velocidade);

	void alterarAngulo(float velocidade);
	void alterarAnguloCanhao(float velocidade);
	void alterarEscala(float velocidade);

	void andar(float velocidade);
	void girarHelices(float velocidade);
	
	virtual ~Placar();
};
