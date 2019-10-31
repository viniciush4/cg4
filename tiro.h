class Tiro {
public:
	float x;
	float y;
	float r;
	float jogador_angulo;
	float canhao_angulo;
	float canhao_angulo_arena;

	Tiro();
	Tiro(float x, float y, float r, float jogador_angulo, float canhao_angulo, float canhao_angulo_arena);

	void desenharPreenchido();
	
	void mover(float velocidade);
	
	virtual ~Tiro();
};
