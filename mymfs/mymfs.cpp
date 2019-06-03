#include "pch.h"
#include "comandos.h"
#include <iostream>
#include <string>
#include <filesystem>

namespace fsys = std::filesystem;
using namespace std;

int main(int argc, char **argv) {

	//Deve possuir no minimo 3 argumentos (nome do programa - passado automaticamente,
	//caminho da unidade X, comando a ser executado)
	if (argc >= 3) {
		string caminhoComando = argv[1]; //Caminho de onde o Mymfs deve ser executado
		string comando = argv[2];        //Comando do Mymfs que deve ser executado

		if (comando == "config") {
			config(caminhoComando);
		}

		else if (comando == "import") {
			string caminhoArquivoImport = argv[3];    //Caminho do arquivo a ser importado para o diret�rio especficiado
			
			importarArquivo(caminhoComando, caminhoArquivoImport);
		}
		
		else if (comando == "export") {
			string caminhoArquivoExport = argv[3];    //Caminho do arquivo a ser exportado para o diret�rio especficiado
			string caminhoDiretorioExport = argv[4];  //Caminho do diret�rio para onde o arquivo deve ser exportado

			exportarArquivo(caminhoComando, caminhoArquivoExport, caminhoDiretorioExport);
		}
		
		else if (comando == "listall") {
			listAll(caminhoComando);
		}
		
		else if (comando == "remove") {
			string caminhoArquivoRemove = argv[3];    //Caminho do arquivo a ser removido
			
			remove(caminhoComando, caminhoArquivoRemove);
		}
		
		else if (comando == "removeall") {
			removeAll(caminhoComando);
		}
		
		else if (comando == "grep") {
			string palavra = argv[3];    //Palavra que ser� procurado no arquivo
			string caminhoArquivoToRead = argv[4];  //Caminho do arquivo que ser� pesquisado
			
			procuraPalavra(caminhoComando, palavra, caminhoArquivoToRead);
		}

		else if (comando == "head100") {
			string caminhoArquivoToRead = argv[3];  //Caminho do arquivo que ser� lido

			primeiras100Linhas(caminhoComando, caminhoArquivoToRead);
		}

		else if (comando == "tail100") {
			string caminhoArquivoToRead = argv[3];  //Caminho do arquivo que ser� lido

			ultimas100Linhas(caminhoComando, caminhoArquivoToRead);
		}

	}
	else {
		cout << "Por favor, informe os argumentos necessarios" << endl;
	}

	return 0;
}