#define NUMERO_SEGMENTOS 50

class Base {
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

	Base();
	Base(float x, float y, float r, float cor_r, float cor_g, float cor_b, char* fill, int id);
	void desenharPreenchido();
	
	virtual ~Base();
};
