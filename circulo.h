#define NUMERO_SEGMENTOS 50

class Circulo {
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

	Circulo();
	Circulo(float x, float y, float r, float cor_r, float cor_g, float cor_b, char* fill, int id);
	void desenhar();
	void desenharPreenchido();

	
	void moverParaCima(float velocidade);
	void moverParaBaixo(float velocidade);
	void moverParaEsquerda(float velocidade);
	void moverParaDireita(float velocidade);
	
	
	virtual ~Circulo();
};
