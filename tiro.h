class Tiro {
public:
	float x;
	float y;
	float r;
	float jogador_raio;
	float jogador_angulo;
	float jogador_escala;
	float canhao_angulo;
	float canhao_angulo_arena;
	float cor_r;
	float cor_g;
	float cor_b;
	float velocidade;

	Tiro();
	Tiro(float x, float y, float jogador_raio, float jogador_angulo, float jogador_escala, float canhao_angulo, float canhao_angulo_arena, float cor_r, float cor_g, float cor_b, float velocidade);

	void desenharPreenchido();
	
	void mover(float coeficiente_ajuste);
	
	virtual ~Tiro();
};
