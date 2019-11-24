// Cabeçalhos
#include "pch.h"
#include "utils.h"
#include "unidade.h"
#include "raid.h"

// Bibliotecas
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <filesystem>

namespace fsys = std::filesystem;
using namespace std;

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

// Retornar todas as unidades físicas salvas em .temp
vector <string> unidadesFisicasEmTemp(string temp) {
	ifstream arquivo(temp);
	vector <string> unidades;

	for (string linha; getline(arquivo, linha); unidades.push_back(linha));

	return unidades;
}

// Configurar todas as unidades físicas de .temp
bool configUnidadesFisicas(string caminhoTemp) {
	vector <string> unidades = unidadesFisicasEmTemp(caminhoTemp);

	for (int i = 0; i < unidades.size(); i++)
		if (Unidade::config(unidades[i]))
			println("[log] Unidade fisica " + unidades[i] + " configurada");
		else
			println("[warning] Unidade fisica " + unidades[i] + " ja estava configurada");
	
	return true;
}

// TODO: implementar a possibilidade de adicionar uma nova unidade física depois do Raid configurado
// Configurar Raid
bool Raid::config(string unidadeLogica, vector <string> unidadesFisicas, string caminhoBase, string barra) {
	string caminhoPasta = caminhoBase + barra + "temp";
	string caminhoArquivo = caminhoPasta + barra + "mymfs.temp";

	string unidadesFisicasStr;
	for (int i = 0; i < unidadesFisicas.size(); i++)
		unidadesFisicasStr += unidadesFisicas[i] + ' ';

	if (!fsys::exists(caminhoPasta))
		fsys::create_directory(caminhoPasta);

	if (!fsys::exists(caminhoArquivo)) {
		if (adicionarUnidadesFisicas(caminhoArquivo, unidadesFisicas)) {
			if (configUnidadesFisicas(caminhoArquivo)) {
				cout << "O MyMFS configurado com sucesso!" << endl;
				cout << "- Unidade logica: " + unidadeLogica << endl;
				cout << "- Unidades fisicas: " + unidadesFisicasStr << endl;
				return true;
			}
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