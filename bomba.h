class Bomba {
public:
	float x;
	float y;
	float r;
	float r_inicial;
	float angulo;

	Bomba();
	Bomba(float x, float y, float r, float angulo);

	void desenharPreenchido();
	
	void mover(float velocidade);
	void decrementarRaio(float velocidade);
	
	virtual ~Bomba();
};
