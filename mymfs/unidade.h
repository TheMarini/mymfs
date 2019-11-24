#ifndef COMANDOS.H
#include <string>

class Unidade {
	public:
		static bool config(std::string caminhoComando);
		static void importarArquivo(std::string caminhoComando, std::string caminhoArquivoImport);
		static void exportarArquivo(std::string caminhoComando, std::string caminhoArquivoExport, std::string caminhoDiretorioExport);
		static void listAll(std::string caminhoComando);
		static void remove(std::string caminhoComando, std::string nomeArquivo);
		static void removeAll(std::string caminhoComando);
		static void procuraPalavra(std::string caminhoComando, std::string palavra, std::string caminhoArquivoToRead);
		static void primeiras100Linhas(std::string caminhoComando, std::string caminhoArquivoToRead);
		static void ultimas100Linhas(std::string caminhoComando, std::string caminhoArquivoToRead);
};


#endif //COMANDOS.H
