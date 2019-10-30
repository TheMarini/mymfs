// Cabeçalhos pré-compilados
#include "pch.h"
// Cabeçalhos
#include "raid.h"

// Bibliotecas
#include <iostream>
#include <string>
#include <filesystem>

namespace fsys = std::filesystem;
using namespace std;

void config(string caminho, string unidadeLogica, vector <string> unidadesFisicas) {
	// Nome do executável
	string nomeExe;
	// Tipo de barra
	string barra;

	// TODO: muito provavelmente a ocorrência de não achar a substring não retorna um erro
	try {
		// Windows
		nomeExe = caminho.substr(caminho.find_last_of("\\"));
		barra = "\\";
	}
	catch (const std::exception&) {
		// Linux
		nomeExe = caminho.substr(caminho.find_last_of("/"));
		barra = "/";
	}

	// Caminho base
	string caminhoBase = caminho;
	// Remover o nome do executável para pegar somente o caminho base
	caminhoBase.erase(caminhoBase.find(nomeExe), nomeExe.length());

	configRaid(unidadeLogica, unidadesFisicas, caminhoBase, barra);
}

int main(int argc, char **argv) {

	// Deve possuir no mínimo 3 argumentos: <nome do executável> <nome da unidade lógica> <comando>
	if (argc >= 3) {
		// Caminho do executável
		string caminho = argv[0];

		// Nome da unidade lógica
		string unidadeLogica = argv[1];

		// Comando do MyMFS que deve ser executado
		string comando = argv[2];

		if (comando == "config") {
			vector<string> unidadesFisicas(argv, argv + argc - 3);

			for (int index = 3; index < argc; index++)
				unidadesFisicas[index - 3] = argv[index];

			config(caminho, unidadeLogica, unidadesFisicas);
		}
	}
}