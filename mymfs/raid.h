#ifndef RAID_H
#define RAID_H

#include <string>
#include <filesystem>
using namespace std;

bool configRaid (string unidadeLogica, vector <string> unidadesFisicas, string caminhoBase, string barra);

#endif //RAID.H
