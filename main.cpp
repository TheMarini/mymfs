#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <windows.h>

using namespace std;

void config (string caminhoComando);
void importarArquivo(string caminhoComando, string caminhoArquivoImport);
void exportarArquivo(string caminhoComando, string caminhoArquivoExport, string caminhoDiretorioExport);
void listAll(string caminhoComando);

int main(int argc, char **argv){
    cout << "Bem vindo ao Mymfs!" << endl;

    //Deve possuir no minimo 3 argumentos (nome do programa - passado automaticamente,
    //caminho da unidade X, comando a ser executado)
    if(argc >= 3){
        string caminhoComando = argv[1]; //Caminho de onde o Mymfs deve ser executado
        string comando = argv[2];        //Comando do Mymfs que deve ser executado

        cout << "Comando solicitado: " << comando << endl;

        if(comando == "config"){
            config(caminhoComando);
        }

        else if(comando == "import"){
            string caminhoArquivoImport = argv[3];    //Caminho do arquivo a ser importado para o diret�rio especficiado

            importarArquivo(caminhoComando, caminhoArquivoImport);
        }

        else if(comando == "export"){
            string caminhoArquivoExport = argv[3];  //Caminho do arquivo a ser exportado para o diret�rio especficiado
            string caminhoDiretorioExport = argv[4];  //Caminho do diret�rio para onde o arquivo deve ser exportado

            exportarArquivo(caminhoComando, caminhoArquivoExport, caminhoDiretorioExport);
        }
        else if(comando == "listall"){
            listAll(caminhoComando);
        }
    }
    else{
        cout << "Por favor, informe os argumentos necessarios" << endl;
    }

    return 0;
}

void config (string caminhoComando){
    string caminhoConfig = caminhoComando + "/config.txt";
    cout << "Caminho do arquivo Config: " << caminhoConfig << endl;

    std::ifstream arquivoConfigExiste( caminhoConfig );

    if(arquivoConfigExiste.good()){              //Verifica se o arquivo Config ja existe no caminho especificado
        cout << "O Mymfs ja esta configurado nesta unidade." << endl;
        arquivoConfigExiste.close();
    }
    else{
        arquivoConfigExiste.close();
        std::ofstream arquivoConfig( caminhoConfig );       //Cria o arquivo config, configurando o Mymfs na unidade especificada
        arquivoConfig.close();
        cout << "O Mymfs foi configurado nesta unidade com sucesso." << endl;
    }
}

void importarArquivo(string caminhoComando, string caminhoArquivoImport){
    streampos begin, end, pos;
    std::ifstream arqConfigExiste (caminhoComando + "/config.txt"); // Verifica se o arquivo Config existe
    if(arqConfigExiste.good()){
        arqConfigExiste.seekg (0);              //Busca o fim do arquivo para verificar o tamanho do mesmo
        arqConfigExiste.seekg (0, ios::end);
        end = arqConfigExiste.tellg();
        arqConfigExiste.close();
    } else {
        cout << "O Mymfs nao esta configurado na unidade informada." << endl; //Caso nao exista, nao permite a importacao
        return;
    }
    if(end<50000){                          //Apenas permite a importa��o se o arquivo de config for menor que 50KB
        if(!caminhoArquivoImport.empty()){
            std::ifstream infile (caminhoArquivoImport, std::ifstream::binary);
            begin = infile.tellg();
            infile.seekg (0, ios::end);
            end = infile.tellg();
            infile.seekg (0);
            long sizeMax = 512000;          //Define o tamanho maximo dos arquivos como 500KB

            int numArquivos = ceil((end-begin)/512000.0); // Verifica quantos arquivos de 500 KB ou menos ser�o criados
            cout << "Quantidade de arquivos arquivos: " << numArquivos<< " arquivos. Considerando tamanho max de 500 KB por arquivo. \n";

            char* buffer;
            infile.seekg(0);

            string nomeDiretorio = caminhoArquivoImport.substr(0, caminhoArquivoImport.find(".")); //Obtem o nome do diret�rio a ser criado para o arquivo atraves do seu nome
            string caminhoDiretorioString = caminhoComando + "/" + nomeDiretorio;
            char * caminhoDiretorio = new char [caminhoDiretorioString.length()+1];
            strcpy(caminhoDiretorio, caminhoDiretorioString.c_str()); //Realiza convers�es necess�rias para utilizar funcao de criar diret�rio

            cout << "Caminho diretorio: " << caminhoDiretorio << endl;

            int erro = CreateDirectory(caminhoDiretorio, NULL);   //Cria o diret�rio para o arquivo a ser importado. Caso ocorra algum erro ou o diret�rio ja exista, retorna zero

            if(erro != 0){           //Caso nao ocorram erros, cria os arquivos de 500KB ou menos
                for(int i=0; i<numArquivos; i++){
                    auto s = std::to_string(i);
                    cout << "\nArquivo dividido: " << caminhoDiretorioString + "/" + s + ".txt" << " \n";

                    std::ofstream outfile (caminhoDiretorioString + "/" + s + ".txt",std::ofstream::binary);

                    if(i!= numArquivos-1){  //Efetua a divisao do arquivo de importa��o em arquivos de 500KB ou menos
                        infile.seekg(pos);
                        buffer = new char[sizeMax];
                        infile.read (buffer,sizeMax);
                        pos = infile.tellg();
                        outfile.write (buffer,sizeMax);
                        delete[] buffer;
                    }
                    else{
                        infile.seekg(pos);
                        int finalSize = (end-pos);
                        buffer = new char[finalSize];
                        infile.read (buffer,finalSize);
                        pos = infile.tellg();
                        outfile.write (buffer,finalSize);
                        delete[] buffer;
                    }
                    outfile.close();
                }

                std::ofstream arqConfig (caminhoComando + "/config.txt", std::ios_base::app | std::ios_base::out);
                string linhaConfig = nomeDiretorio + ";" + std::to_string(numArquivos) + "\n";
                arqConfig << linhaConfig; //Adiciona o arquivo importado no arquivo de configura��o (nomeArquivo;quantidadeArquivos)
                arqConfig.close();
                cout << "O arquivo foi importado para o Mymfs com sucesso." << endl;
            }
            else{
                cout << "Ocorreu um erro! Um arquivo com esse nome ja existe no Mymfs." << endl;
            }

            infile.close();
        }
        else{
            cout << "Arquivo nao importado no Mymfs pois o caminho informado esta vazio ou ambiente ainda nao foi configurado." << endl;
        }
    }
    else{
        cout << "Nao foi possivel importar o arquivo. Arquivo config.txt est� cheio - Mymfs." << endl;
    }
}

void exportarArquivo(string caminhoComando, string caminhoArquivoExport, string caminhoDiretorioExport){
    std::ifstream arqConfigExiste(caminhoComando + "/config.txt");
    std::ifstream arqExportExiste(caminhoArquivoExport);
    //Verifica se o arquivo de configura��o e se o arquivo a ser exportado existem
    if(arqConfigExiste.good() &&  arqExportExiste.good() && !caminhoArquivoExport.empty()&& !caminhoDiretorioExport.empty()){
        arqExportExiste.close();
        string nomeDiretorioBuscado = caminhoArquivoExport.substr(0, caminhoArquivoExport.find("."));   //Obtem o nome do diretorio atraves do nome do arquivo
        string nomeDiretorioEncontrado;
        string qtdArquivosEncontrado;

        do{
            //Percorre o arquivo config at� o final ou at� encontrar o arquivo a ser exportado
            string linhaConfig;
            std::getline(arqConfigExiste, linhaConfig);
            nomeDiretorioEncontrado = linhaConfig.substr(0, linhaConfig.find(";"));
            qtdArquivosEncontrado = linhaConfig.substr(linhaConfig.find(";") + 1, (linhaConfig.size()-linhaConfig.find(";")));
        }
        while (strcmp(nomeDiretorioEncontrado.c_str(), nomeDiretorioBuscado.c_str()) != 0 &&
                !arqConfigExiste.eof() && !nomeDiretorioEncontrado.empty());

        //Verifica se encontrou o diretorio do arquivo a ser exportado
        if(strcmp(nomeDiretorioEncontrado.c_str(), nomeDiretorioBuscado.c_str()) == 0 &&
                !nomeDiretorioEncontrado.empty() && !qtdArquivosEncontrado.empty()){
            int numArquivos = stoi(qtdArquivosEncontrado);
            cout << "Arquivo encontrado! Exportando..." << endl;
            std::ifstream arquivoJaExiste(caminhoDiretorioExport);
            if(arquivoJaExiste.good()){ //Verifica se o arquivo a ser exportado existe
                arquivoJaExiste.close();
                cout << "O arquivo a ser exportado ja existe na pasta destino (" << caminhoDiretorioExport << "), por favor indique outro destino." << endl; //EDITADO TALITA
            }
            else{
                //Caso exista, cria um arquivo no diretorio informado concatenando todos os arquivos de 500KB
                std::ofstream combined_file(caminhoDiretorioExport);
                for(int i=0; i < numArquivos; i++){
                    auto s = std::to_string(i);
                    cout << "\nArquivo concatenado: " << s + ".txt" << " \n";
                    s = s+".txt";
                    //Percorre os arquivos de 0 a numArquivos concatenando-os no arquivo exportado
                    std::ifstream srce_file(caminhoComando + "/" + nomeDiretorioEncontrado + "/" + s) ;
                    if(srce_file){
                        combined_file << srce_file.rdbuf() ;
                        if(combined_file){
                            std::cout << "Arquivo " + s + " concatenado\n" ;
                        }
                    }
                    else{
                        std::cerr << "Ocorreu um erro. O arquivo nao pode ser aberto " << s << '\n' ;
                    }
                    srce_file.close();
                }
                combined_file.close();
                cout << "O arquivo foi exportado do Mymfs com sucesso." << endl;
            }

        }
        else{
            cout << "O arquivo nao foi encontrado no Mymfs, portanto nao foi exportado" << endl;
        }
    }
    else{
        arqExportExiste.close();
        cout << "Arquivo nao exportado do Mymfs pois o caminho informado esta vazio ou ambiente ainda nao foi configurado." << endl;
    }
}

void listAll(string caminhoComando){
    //Valida se o arquivo config existe no diretorio especificado
    std::ifstream arqConfigExiste(caminhoComando + "/config.txt");
    if(arqConfigExiste.good()){
        //Caso exista, percorre o arquivo buscando os nomes dos diretorios/arquivos e listando-os
        string nomeDiretorioEncontrado = "x";
        string qtdArquivosEncontrado;
        string linhaConfig;
        std::getline(arqConfigExiste, linhaConfig);
        if(linhaConfig.length() > 0){
            //Caso existam registros no arquivo config, eles ser�o buscados e exibidos
            cout << "Arquivos encontrados: \n" << endl;
            nomeDiretorioEncontrado = linhaConfig.substr(0, linhaConfig.find(";"));
            while (!arqConfigExiste.eof() && !nomeDiretorioEncontrado.empty()){
                //Exibe nome do diret�rio/arquivo
                cout << nomeDiretorioEncontrado + ".txt" << endl;
                string linhaConfig;
                std::getline(arqConfigExiste, linhaConfig);
                nomeDiretorioEncontrado = linhaConfig.substr(0, linhaConfig.find(";"));
            }
            cout << "\nFim da lista de arquivos encontrados. \n" << endl;
        }
        else{
            cout << "Nao ha arquivos salvos pelo Mymfs!" << endl;
        }
    }
    else{
        cout << "Arquivos nao listados do Mymfs pois o caminho informado esta vazio ou ambiente ainda nao foi configurado." << endl;
    }
}
