#ifndef LRANALYZER
#define LRANALYZER

#include <stdio.h>
#include <stdlib.h>
#include "lranalyzer.h"
#include "LRGrammar.h"
#include "read_file.h"
#include "ast.h"

typedef struct Element Element;
struct Element
{
	char lettre_nombre;
	Element *suivant;
};

typedef struct Pile Pile;
struct Pile
{
	Element *premier;
};

typedef struct File File;
struct File
{
	Element *premier;
};

typedef struct Table
{
	char red_dec;
	int numero_red_dec;
}Table;

void empiler(Pile *pile, int nvLettre_Nombre);
char depiler(Pile *pile);
void afficher_pile(Pile *pile);
int recup_premier_element_pile(Pile *pile);
void enfiler(File *file, int nvLettre_Nombre);
char defiler(File *file);
void afficher_file(File *file);
char recup_premier_element_file(File *file);
int nbr_colonnes_table(grammar G,int *nbr_carac_terminal, int *nbr_carac_non_terminal);
Table remplissage_table(Table** table, file_read fichier_lu,char* les_terminaux_et_non,int nbr_carac_terminal,int nbr_colonnes);
char recuperation_des_terminaux_et_non(FILE *f,char *les_terminaux_et_non);

#endif

