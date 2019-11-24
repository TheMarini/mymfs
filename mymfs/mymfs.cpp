// Cabeçalhos
#include "pch.h"
#include "raid.h"

// Bibliotecas
#include <iostream>
#include <string>
#include <filesystem>

namespace fsys = std::filesystem;
using namespace std;

class MyMFS {
	public:
		// Tamanho dos argumanetos
		int argc;
		// Vetor de argumentos
		char** argv;

		// Unidade lógica
		string unidadeLogica;

		// Construtor
		MyMFS(int argc, char** argv) {
			// Argumentos
			this->argc = argc;
			this->argv = argv;

			// Caminho de execução
			MyMFS::Ambiente::caminhoCompleto = argv[0];
		
			// Nome da unidade lógica
			//this->unidadeLogica = argv[1];
		
			//this->executarComando(argv[2]);
		}

		// Executar comando
		bool executarComando(string comando) {
			if (comando == "config") {
				vector<string> unidadesFisicas(argv, argv + argc - 3);

				for (int index = 3; index < argc; index++)
					unidadesFisicas[index - 3] = argv[index];

				return config(MyMFS::Ambiente::caminhoCompleto, unidadeLogica, unidadesFisicas);
			}

			return false;
		}

	private: 
		static class Ambiente {
			public:
				// Caminho completo da execução
				static string caminhoCompleto;
				// Nome do executável
				static string nomeExecutavel;
				// Tipo de barra
				static string barra;

				Ambiente() {

				}
		};

		// Unidades físicas
		static vector <string> unidadesFisicas;

		// Configurar MyMFS
		static bool config(string caminho, string unidadeLogica, vector <string> unidadesFisicas) {
			// TODO: muito provavelmente a ocorrência de não achar a substring não retorna um erro
			try {
				// Windows
				MyMFS::Ambiente::nomeExecutavel = caminho.substr(caminho.find_last_of("\\"));
				MyMFS::Ambiente::barra = "\\";
			}
			catch (const std::exception&) {
				// Linux
				MyMFS::Ambiente::nomeExecutavel = caminho.substr(caminho.find_last_of("/"));
				MyMFS::Ambiente::barra = "/";
			}

			// Caminho base
			string caminhoBase = caminho;
			// Remover o nome do executável para pegar somente o caminho base
			caminhoBase.erase(caminhoBase.find(MyMFS::Ambiente::nomeExecutavel), MyMFS::Ambiente::nomeExecutavel.length());

			return Raid::config(unidadeLogica, unidadesFisicas, caminhoBase, MyMFS::Ambiente::barra);
		}
};

int main(int argc, char **argv) {
	// Deve possuir no mínimo 3 argumentos: <nome do executável> <nome da unidade lógica> <comando>
	if (argc >= 3) {
		// Início
		MyMFS* API = new MyMFS(argc, argv);

		// Fim
		delete API;
	}
}