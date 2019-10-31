all: tinyxml2 configuracao circulo jogador linha tiro bomba
	g++ main.cpp -o trabalhocg tinyxml2.o configuracao.o circulo.o jogador.o linha.o tiro.o bomba.o -lglut -lGLU -lGL -std=c++11
	@echo "Done."

tinyxml2: tinyxml2.cpp
	g++ -c tinyxml2.cpp -std=c++11

configuracao: configuracao.cpp
	g++ -c configuracao.cpp -std=c++11

circulo: circulo.cpp
	g++ -c circulo.cpp -std=c++11

jogador: jogador.cpp
	g++ -c jogador.cpp -std=c++11

linha: linha.cpp
	g++ -c linha.cpp -std=c++11

tiro: tiro.cpp
	g++ -c tiro.cpp -std=c++11

bomba: bomba.cpp
	g++ -c bomba.cpp -std=c++11

clean:
	@rm -f *.o trabalhocg