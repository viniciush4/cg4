class Bomba {
public:
	float x;
	float y;
	float r;
	float r_inicial;
	float angulo;
	float velocidade;

	Bomba();
	Bomba(float x, float y, float r, float angulo, float velocidade);

	void desenharPreenchido();
	
	void mover(float coeficiente_ajuste);
	void decrementarRaio(float velocidade);
	
	virtual ~Bomba();
};
