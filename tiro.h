class Tiro {
public:
	float x;
	float y;
	float r;
	float jogador_angulo;
	float canhao_angulo;
	float canhao_angulo_arena;
	float cor_r;
	float cor_g;
	float cor_b;

	Tiro();
	Tiro(float x, float y, float r, float jogador_angulo, float canhao_angulo, float canhao_angulo_arena, float cor_r, float cor_g, float cor_b);

	void desenharPreenchido();
	
	void mover(float velocidade);
	
	virtual ~Tiro();
};
