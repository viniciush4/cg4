class Configuracao {
public:
	const char* arquivo_arena_nome;
	const char* arquivo_arena_tipo;
	const char* arquivo_arena_caminho;

	float jogador_velocidade;
	float tiro_velocidade;

	float inimigo_velocidade;
	float inimigo_velocidade_tiro;
	float inimigo_frequencia_tiro;

	Configuracao();
	bool validarConfiguracao();
	virtual ~Configuracao();
};
