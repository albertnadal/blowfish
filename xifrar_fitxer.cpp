#include <iostream>
#include "blowfish.h"

using namespace std;

int main(int argc, char *argv[])
{
	blowfish c;
	int i, LongTextXifrat, LongTextDesxifrat;
	char *TextXifrat;
	char *TextDesxifrat;
	char text[140] = "Tinc 2 taronges i 8 peres. El pallasso em fa riure. El raïm és bò. Sóc massa llest! ò és la lletra o amb accent obert.";
	char clau[70];

	cout << "Xifrador (Blowfish) v1.0\n\n";

	if((argc>1)&&(strcmp(argv[1], "--ajuda")==0))
	{
		cout << "Per xifrar un fitxer usa:           " << argv[0] << " -x <fitxer_clar> <fitxer_xifrat>\n";
		cout << "Per desxifrar un fitxer usa:        " << argv[0] << " -d <fitxer_xifrat> <fitxer_clar>\n";
	}
	else if(argc!=4)
	{
		cout << argv[0] << ":nombre incorrecte d'arguments\nProva <<" << argv[0] << " --ajuda>> per obtenir més informació.\n" ;
		exit(0);
	}
	else
	{
		cout << "Introdueix una clau: ";
		cin >> clau;
		c.EntrarClau(clau);

		if(strcmp(argv[1], "-x")==0){c.XifrarFitxer(argv[2], argv[3]);}
		else if(strcmp(argv[1], "-d")==0){c.DesxifrarFitxer(argv[2], argv[3]);}
		else
		{
			cout << argv[0] << ":argument desconegut\nProva <<" << argv[0] << " --ajuda>> per obtenir més informació.\n" ;
			exit(0);
		}
	}


	//c.EntrarClau("cocoliso");

	//c.XifrarText(text, strlen(text));
	//TextXifrat = c.ObtenirTextXifrat();
	//LongTextXifrat = c.ObtenirLongTextXifrat();
	//c.MostrarTextXifrat();

	//c.DesxifrarText(TextXifrat, LongTextXifrat);
	//TextDesxifrat = c.ObtenirTextDesxifrat();
	//LongTextDesxifrat = c.ObtenirLongTextDesxifrat();
	//c.MostrarTextDesxifrat();


	return 0;
}

