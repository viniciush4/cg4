class Placar {
public:
	int quantidade_bases;
	int quantidade_bases_destruidas;
	int quantidade_bases_restantes;
	float raio_arena;

	Placar();
	Placar(float raio_arena, int quantidade_bases);
	void desenharPreenchido();
	void desenharMensagemFinal();
	void incrementarBasesDestruidas();
	void resetarPlacar();

	virtual ~Placar();
};
