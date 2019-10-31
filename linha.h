class Linha {
public:
	float x1;
	float y1;
	float x2;
	float y2;
	float cor_r;
	float cor_g;
	float cor_b;
	const char* style;
	int id;

	Linha();
	Linha(float x1, float y1, float x2, float y2);
	void desenhar();
	virtual ~Linha();
};
