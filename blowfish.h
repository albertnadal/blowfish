/**************************************/
/*      Criptosistema BLOWFISH        */
/*      per a C++                     */
/*                                    */
/*      Autor: Albert Nadal Garriga   */
/*      Data:  08/12/2003             */
/**************************************/

#ifndef _BLOWFISH_H
#define _BLOWFISH_H

#include <stdio.h>

class blowfish
{
	private:
	int NumRondes;
	unsigned long P[18];
	char *TextXifrat;
	char *TextDesxifrat;
	char clau[72];
	int LongClau;
	int LongTextDesxifrat;
	int LongTextXifrat;

	public:
	blowfish();
	~blowfish();
	void EntrarClau(char k[]);
	void XifrarText(char text[], int longitud);
	void DesxifrarText(char text[], int longitud, int NoHiHaGuarnicio);
	void XifrarFitxer(char fitxer_e[], char fitxer_s[]);
	void DesxifrarFitxer(char fitxer_e[], char fitxer_s[]);
	void MostrarTextXifrat();
	void MostrarTextDesxifrat();
	char* ObtenirTextXifrat();
	char* ObtenirTextDesxifrat();
	int ObtenirLongTextXifrat();
	int ObtenirLongTextDesxifrat();

	private:
	void GenerarSubclaus();
	void AjuntarParts(unsigned char bloc[], unsigned long Xl, unsigned long Xr);
	unsigned long ObtenirPartEsquerra(unsigned char bloc[]);
	unsigned long ObtenirPartDreta(unsigned char bloc[]);
	unsigned long F(unsigned long c);
	void XifrarBloc(unsigned char bloc[]);
	void DesxifrarBloc(unsigned char bloc[]);
	void Intercanviar(unsigned long *Xl, unsigned long *Xr);
	void ObtenirSeguentBlocPerXifrar(unsigned char bloc[], char text[], int j, int LongText);
	void ObtenirSeguentBlocPerDesxifrar(unsigned char bloc[], char text[], int j);
	void ObtenirFragment(FILE* fitxer, char fragment[], int *longitud);
	void GuardarFragment(FILE* fitxer, char fragment[], int longitud);
	void GuardarBloc(unsigned char bloc[], char *Text, int j);
	void EliminarTextDesxifrat();
	void EliminarTextXifrat();
	void RepresentarFraccioGraficEvolucio(long j, long* f, int tamany_fitxer);
	int ObtenirLongGuarnicioTextDesxifrat(int LongText);
	int ObtenirTamanyFitxer(char fitxer[]);
};
#endif
