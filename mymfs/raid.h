#ifndef RAID_H
#define RAID_H

#include <string>
#include <filesystem>
using namespace std;

class Raid {
	public:
		static bool config (string unidadeLogica, vector <string> unidadesFisicas, string caminhoBase, string barra);
};

#endif //RAID.H
