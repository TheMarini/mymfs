// Cabeçalhos
#include "pch.h"
#include "utils.h"

//#include "unidade.h"

// Bibliotecas
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <filesystem>

namespace fsys = std::filesystem;
using namespace std;

bool arquivoExiste (string arquivo) {
	if (arquivo.empty())
		return false;
	return fsys::exists(arquivo);
}

// Adiciona múltiplas unidades físicas ao .temp
bool adicionarUnidadesFisicas (string caminhoArquivo, vector <string> unidadesFisicas) {
	try
	{
		ofstream arquivo(caminhoArquivo, ios_base::app | ios_base::out);

		string linha = "";
		for (int i = 0; i < unidadesFisicas.size(); i++)
			linha += unidadesFisicas[i] + "\n";

		arquivo << linha;
		arquivo.close();
	}
	catch (const std::exception & ex)
	{
		cout << ex.what() << endl;
		return false;
	}
	
	return true;
}

// TODO: implementar a possibilidade de adicionar uma nova unidade física depois do Raid configurado
// Configurar Raid
bool configRaid(string unidadeLogica, vector <string> unidadesFisicas, string caminhoBase, string barra) {
	string caminhoPasta = caminhoBase + barra + "temp";
	string caminhoArquivo = caminhoPasta + barra + "mymfs.temp";

	string unidadesFisicasStr;
	for (int i = 0; i < unidadesFisicas.size(); i++)
		unidadesFisicasStr += unidadesFisicas[i] + ' ';

	if (!fsys::exists(caminhoPasta))
		fsys::create_directory(caminhoPasta);

	if (!fsys::exists(caminhoArquivo)) {
		if (adicionarUnidadesFisicas(caminhoArquivo, unidadesFisicas)) {
			cout << "O MyMFS configurado com sucesso!" << endl;
			cout << "- Unidade logica: " + unidadeLogica << endl;
			cout << "- Unidades fisicas: " + unidadesFisicasStr << endl;
			return true;
		}

		return false;
	}
	else {
		cout << "Ja existe um MyMFS com estas configuracoes" << endl;
		cout << "- Unidade logica: " + unidadeLogica << endl;
		cout << "- Unidades fisicas: " + unidadesFisicasStr << endl;
		return false;
	}
}