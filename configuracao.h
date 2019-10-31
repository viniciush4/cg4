class Configuracao {
public:
	const char* arquivo_arena_nome;
	const char* arquivo_arena_tipo;
	const char* arquivo_arena_caminho;

	float jogador_velocidade;
	float tiro_velocidade;

	Configuracao();
	bool validarConfiguracao();
	virtual ~Configuracao();
};
