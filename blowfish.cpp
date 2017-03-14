/**************************************/
/*      Criptosistema BLOWFISH        */
/*      per a C++                     */
/*                                    */
/*      Autor: Albert Nadal Garriga   */
/*      Data:  08/12/2003             */
/**************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>
#include <string.h>
#include <time.h>
#include "blowfish.h"
#include "blowfish_dades.h"

blowfish::blowfish()
{
	NumRondes = 16;            //Aquest valor no s'ha de modificar!
	clau[0] = '\0';
	LongClau = 0;
	LongTextDesxifrat = 0;
	LongTextXifrat = 0;
};

blowfish::~blowfish()
{
	if(LongTextDesxifrat!=0){EliminarTextDesxifrat();}
	if(LongTextXifrat!=0){EliminarTextXifrat();}
};

void blowfish::EliminarTextDesxifrat()
{
	LongTextDesxifrat = 0;
	delete [] TextDesxifrat;
};

void blowfish::EliminarTextXifrat()
{
	LongTextXifrat = 0;
	delete [] TextXifrat;
};

char* blowfish::ObtenirTextXifrat()
{
	return TextXifrat;
};

int blowfish::ObtenirLongTextXifrat()
{
	return LongTextXifrat;
};

char* blowfish::ObtenirTextDesxifrat()
{
	return TextDesxifrat;
};

int blowfish::ObtenirLongTextDesxifrat()
{
	return LongTextDesxifrat;
};

void blowfish::MostrarTextXifrat()
{
	for(int i=0; i < LongTextXifrat; i++)
		std::cout << TextXifrat[i];
	std::cout << '\n';
};

void blowfish::MostrarTextDesxifrat()
{
	for(int i=0; i < LongTextDesxifrat; i++)
		std::cout << TextDesxifrat[i];
	std::cout << '\n';
};

int blowfish::ObtenirLongGuarnicioTextDesxifrat(int LongText)
{
	int i, j, suma=0, trobat=0, pos;
	unsigned char bloc[8];
	ObtenirSeguentBlocPerDesxifrar(bloc, TextDesxifrat, LongText-8);   //Obté l'últim bloc desxifrat

	for(i=7; ((i>0) && (!trobat)); i--)
	{
		for(j=0;j<i;j++)
			suma=suma+bloc[j];       //suma tots els valors del bloc menys l'últim
		suma=suma%255;
	
		if(suma==bloc[i])
		{
			trobat=1;
			pos=i;
		}
		suma=0;
	}
	if(trobat){return 8-pos;}
	else{return -1;}
};

void blowfish::ObtenirSeguentBlocPerDesxifrar(unsigned char bloc[], char text[], int j)
{
	int e=0;
	while(e < 8)
	{
		bloc[e]=text[j+e];
		e++;
	}
	j = j + 8;
};

void blowfish::ObtenirSeguentBlocPerXifrar(unsigned char bloc[], char text[], int j, int LongText)
{
	int e=0,i,suma=0,aleatori;
	while(e < 8)
	{
		if(j+e < LongText)
			bloc[e]=text[j+e];
		else if(j+e == LongText) 
		{						//S'insereix una marca de final
			for(i=0;i<e;i++)
				suma=suma+bloc[i];
			suma=suma%255;
			bloc[e]=suma;
			suma=0;
		}
		else
		{						//S'insereix guarnició aleatòria
			for(i=0;i<e;i++)
				suma=suma+bloc[i];
			aleatori=suma=suma%255;
			while(aleatori==suma)
				aleatori=(rand()%(255));
			bloc[e]=aleatori;
		}
		e++;
	}
	j = j + 8;
};

unsigned long blowfish::ObtenirPartEsquerra(unsigned char bloc[])
{
	unsigned long aux = 0x00000000;
	for(int k=0; k<4; k++)
	{
		//Omple els 4 bytes amb els 4 bytes esquerres del bloc
		aux=(aux << 8)| bloc[k];
	}
	return aux;
};

unsigned long blowfish::ObtenirPartDreta(unsigned char bloc[])
{
	unsigned long aux = 0x00000000;
	for(int k=0; k<4; k++)
	{
		//Omple els 4 bytes amb els 4 bytes drets del bloc
		aux=(aux << 8)| bloc[k+4];
	}
	return aux;
};

void blowfish::AjuntarParts(unsigned char bloc[], unsigned long Xl, unsigned long Xr)
{
	bloc[0]= (Xl & 0xFF000000) >> 24;
	bloc[1]= (Xl & 0x00FF0000) >> 16;
	bloc[2]= (Xl & 0x0000FF00) >> 8;
	bloc[3]= (Xl & 0x000000FF);
	bloc[4]= (Xr & 0xFF000000) >> 24;
	bloc[5]= (Xr & 0x00FF0000) >> 16;
	bloc[6]= (Xr & 0x0000FF00) >> 8;
	bloc[7]= (Xr & 0x000000FF);
};

void blowfish::GuardarBloc(unsigned char bloc[], char *Text, int j)
{
	int e=0;
	while(e < 8)
	{
		Text[j+e]=bloc[e];
		e++;
	}
};

unsigned long blowfish::F(unsigned long Xl)
{
	unsigned long a, b, c;

	a = S[0][((Xl & 0xFF000000) >> 24)] + S[1][((Xl & 0x00FF0000) >> 16)];
	b = S[2][((Xl & 0x0000FF00) >> 8)];
	c = a ^ b;
	return (c + S[3][(Xl & 0x000000FF)]);
};

void blowfish::Intercanviar(unsigned long *Xl, unsigned long *Xr)
{
	unsigned long aux = *Xl;
	*Xl = *Xr;
	*Xr = aux;
};

void blowfish::XifrarBloc(unsigned char bloc[])
{
	unsigned long Xl = ObtenirPartEsquerra(bloc);
	unsigned long Xr = ObtenirPartDreta(bloc);
	for(int i=0; i<NumRondes; i++)
	{
		Xl = Xl ^ P[i];
		Xr = F(Xl) ^ Xr;
		Intercanviar(&Xl, &Xr);
	}
	Intercanviar(&Xl, &Xr);
	Xr = Xr ^ P[16];
	Xl = Xl ^ P[17];
	AjuntarParts(bloc, Xl, Xr);
};

void blowfish::DesxifrarBloc(unsigned char bloc[])
{
	unsigned long Xl = ObtenirPartEsquerra(bloc);
	unsigned long Xr = ObtenirPartDreta(bloc);
	for(int i=NumRondes + 1; i>1; i--)
	{
		Xl = Xl ^ P[i];
		Xr = F(Xl) ^ Xr;
		Intercanviar(&Xl, &Xr);
	}
	Intercanviar(&Xl, &Xr);
	Xr = Xr ^ P[1];
	Xl = Xl ^ P[0];
	AjuntarParts(bloc, Xl, Xr);
};

void blowfish::XifrarText(char text[], int longitud)
{
	int j=0;
	unsigned char bloc[8];
	int reste = longitud%8;
	if(LongTextXifrat!=0){EliminarTextXifrat();}
	if(reste != 0){LongTextXifrat = longitud + (8 - reste);}
	else {LongTextXifrat = longitud;}
	TextXifrat = new char[LongTextXifrat];
	srand((unsigned)time( NULL ));          //Genera una llavor d'aleatorització

	while(j<LongTextXifrat)
	{
		ObtenirSeguentBlocPerXifrar(bloc, text, j, longitud);
		XifrarBloc(bloc);
		GuardarBloc(bloc, TextXifrat, j);
		j=j+8;
	}
};

void blowfish::DesxifrarText(char text[], int longitud, int NoHiHaGuarnicio)
{
	int LongGuarnicio, i, j=0;
	if(LongTextDesxifrat!=0){EliminarTextDesxifrat();}
	TextDesxifrat = new char[longitud];
	unsigned char bloc[8];

	while(j<longitud)
	{
		ObtenirSeguentBlocPerDesxifrar(bloc, text, j);
		DesxifrarBloc(bloc);
		GuardarBloc(bloc, TextDesxifrat, j);
		j=j+8;
	}

	if(NoHiHaGuarnicio){LongGuarnicio=-1;}
	else{LongGuarnicio = ObtenirLongGuarnicioTextDesxifrat(longitud);}

	if(LongGuarnicio!=-1){LongTextDesxifrat = longitud - LongGuarnicio;}  //S'elimina la guarnició
	else{LongTextDesxifrat = longitud;}
};

void blowfish::ObtenirFragment(FILE* fitxer, char fragment[], int *longitud)
{
	int i=0;
	char aux;

	while((i<8000)&&(!feof(fitxer)))
	{
		fread(&aux,1,1,fitxer);
		if(!feof(fitxer))
		{
			fragment[i]=aux;
			i++;
		}
	}
	*longitud = i;
};

void blowfish::GuardarFragment(FILE* fitxer, char fragment[], int longitud)
{
	int i=0;
	while(i<longitud)
	{
		fwrite(&fragment[i],1,1,fitxer);
		i++;
	}
};

void blowfish::XifrarFitxer(char fitxer_e[], char fitxer_s[])
{
	FILE *input, *output;
	int longitud, tamany_fitxer;
	long j=0, f=0;
	char fragment[8000];
	
	tamany_fitxer=ObtenirTamanyFitxer(fitxer_e);
	input = fopen(fitxer_e,"rb");
	output = fopen(fitxer_s,"w+b");
	std::cout<<"[ 0%";
	while(!feof(input))
	{
		ObtenirFragment(input, fragment, &longitud);
		RepresentarFraccioGraficEvolucio(j, &f, tamany_fitxer);
		XifrarText(fragment, longitud);
		GuardarFragment(output, TextXifrat, LongTextXifrat);
		j++;
	}
	std::cout<<"100% ]\nFitxer xifrat!\n";
	fclose(input);
	fflush(output);
	fclose(output);
	EliminarTextXifrat();
};

void blowfish::DesxifrarFitxer(char fitxer_e[], char fitxer_s[])
{
	FILE *input, *output;
	int longitud, tamany_fitxer;
	long j=0, f=0;
	char fragment[8000];

	tamany_fitxer=ObtenirTamanyFitxer(fitxer_e);
	input = fopen(fitxer_e,"rb");
	output = fopen(fitxer_s,"wb");
	std::cout<<"[ 0%";
	while(!feof(input))
	{
		ObtenirFragment(input, fragment, &longitud);
		RepresentarFraccioGraficEvolucio(j, &f, tamany_fitxer);
		DesxifrarText(fragment, longitud, !feof(input));
		GuardarFragment(output, TextDesxifrat, LongTextDesxifrat);
		j++;
	}
	std::cout<<"100% ]\nFitxer desxifrat!\n";
	fclose(input);
	fflush(output);
	fclose(output);
	EliminarTextDesxifrat();
};

void blowfish::GenerarSubclaus()
{
	int i, k;
	unsigned long aux;
	for(i=0; i<18; i++)
	{
		aux = 0;
		for(k=0; k<4; k++)
		{
			//Omple els 4 bytes amb bytes de la clau
			aux=(aux << 8)| clau[(i*4 + k) % LongClau];
		}
		//XOR entre les subclaus i la clau
		P[i]=subclaus[i] ^ aux;
	}
};

void blowfish::EntrarClau(char k[])
{
	LongClau = strlen(k);
	if(LongClau > 72) 
	{
		std::cout << "Clau no acceptada! Ha de tenir una longitud màxima de 72 caràcters.\n";
		LongClau = 0;
	}
	else
	{
		for(int i=0; i<LongClau; i++){	clau[i] = k[i];	}
		GenerarSubclaus();
	}
};

int blowfish::ObtenirTamanyFitxer(char fitxer[])
{
	int tamany;
	FILE* f = fopen(fitxer, "r");
		fseek(f,0,SEEK_END);
		tamany = ftell(f);
	fclose(f);
	return tamany;
};

void blowfish::RepresentarFraccioGraficEvolucio(long j, long* f, int tamany_fitxer)
{
	int fraccions=tamany_fitxer/50;
	if((tamany_fitxer>8000)&&(fraccions>8000)&&(j*8000 > *f))
	{
		*f = *f + fraccions;
		if(*f==fraccions*12){std::cout << "25%";}
		else if(*f==fraccions*25){std::cout << "50%";}
		else if(*f==fraccions*37){std::cout << "75%";}
		std::cout << ".";
		fflush(NULL);
	}
	else if((tamany_fitxer<=8000)||(fraccions<=8000))
	{
		std::cout << ".";
		fflush(NULL);
	}
//	[ 0%.....25%.....50%.....75%.....100% ]
};

