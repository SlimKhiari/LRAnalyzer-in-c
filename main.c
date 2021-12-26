#include <stdio.h>
#include <stdlib.h>
#include "lranalyzer.h"
#include "read_file.h"
#include "LRGrammar.h"
#include "ast.h"


int main(int argc, char* argv[])
{	
	size_t taille_regles=0,tmp=0; // varibles inutiles
	size_t o,i,j; // des compteurs
	int verif_ast_vide=0; // = 0 si vide sinon 1
	
	//Les variables utilisées pour construire l'analyseur
	size_t regle = 0;
	int nbr_carac_terminal=0,nbr_carac_non_terminal=0;
	size_t verif=0; // la variable verif permet de verifier si un caractere appartient ou non à la grammaire
	file_read fichier_lu = read_file(argv[1]);	
	size_t nbr_lignes = fichier_lu.t.nblines-1;  // nombre de lignes de la table 2D de la grammaire
	size_t nbr_colonnes = nbr_colonnes_table(fichier_lu.G,&nbr_carac_terminal,&nbr_carac_non_terminal)+2; // nombre de colonnes de la table 2D de la grammaire
	char *les_terminaux_et_non = (char*) malloc(nbr_colonnes*sizeof(char*)); // les terminaux et les non terminaux de la grmmaire dans un tableau 1D pour récupérer la bonne transformation
	File *flot = malloc(sizeof(*flot)); flot->premier=NULL;
	Pile *pile=malloc(sizeof(*pile)); pile->premier=NULL;
	FILE *f=fopen(argv[1], "r");
	char caractere_a_regarder; // pour récupérer la bonne transformation nécessaire pour l'analyse synthaxique
	size_t numero_de_ligne_a_regarder; // pour récupérer la bonne transformation nécessaire pour l'analyse synthaxique
	
	//Les variables utilisées pour construire l'ast 
	char *ast = (char*) malloc(MAX*sizeof(char*)); // utilisé pour enregistrer l'ast
	char ast_tmp_dep[MAX] = {0};// utilisé pour l'ast pour effectuer le shift
	int taille_arbre=0;
	int index_parenthese_ouvrante=0;
	int mettre_non_terminal=0;
	int indexReduction=0;
	
	// allocation dynamique de la table 2D de la grammaire (car plus pratique à manipuler) + initialisation
	Table **table = NULL; 
	table = (Table**)malloc(nbr_lignes*sizeof(Table*));
	for(i=0;i<nbr_lignes;i++)
	{
		table[i] = (Table*)malloc(nbr_colonnes*sizeof(Table));
	}
	for(i=0; i<nbr_lignes; i++)
	{
		for(int j=0; j<nbr_colonnes; j++)
		{
			table[i][j].numero_red_dec = 0; 
			table[i][j].red_dec = ' ';
		}
	}
	
	// initialisation du flot en utilisant une file 
	i=0;
	while(argv[2][i] != '\0')
	{
		enfiler(flot,argv[2][i]);
		i++;
	}
	
	// initialisation de la pile
	empiler(pile,0);
	
	// récupération des terminaux et non terminaux
	*les_terminaux_et_non = recuperation_des_terminaux_et_non(f,les_terminaux_et_non);
  	
	// remplissage de la table
	**table = remplissage_table(table,fichier_lu,les_terminaux_et_non,nbr_carac_terminal,nbr_colonnes);
	
	// analyse synthaxique	
	printf("L'analyse synthaxique du %s sur %s : \n\n",argv[1], argv[2]);					
	printf("flot: "); afficher_file(flot); printf(" | "); printf("pile: ");afficher_pile(pile);
	
	numero_de_ligne_a_regarder = recup_premier_element_pile(pile);
	caractere_a_regarder = recup_premier_element_file(flot);
	
	for(o=0;o<nbr_colonnes;o++)
	{
		if(caractere_a_regarder == les_terminaux_et_non[o])	
		{
			verif=1;
			break;
		}			
	}
	if(verif==0)
	{
		printf("\n>> Caractere non reconnu par la grammaire\n");
		
	}
	
	o=0;
	
	while(table[numero_de_ligne_a_regarder][o].red_dec != 'a' && verif==1)
	{
		o=0;
		verif=0;
		caractere_a_regarder = recup_premier_element_file(flot);
		numero_de_ligne_a_regarder = recup_premier_element_pile(pile);
		i=0;
		taille_arbre=0;
		while(i<nbr_colonnes)
		{
				// traiter le cas ou le flot est vide (c'est-à-dire il ne contient que le "$") 
				if(flot->premier == NULL || flot->premier->lettre_nombre == '$')
				{
					// chercher le rang du "$" dans le tableau les terminaux_et_non pour pourvoir trouver la bonne transformation qui correspond
					for(o=0;o<nbr_colonnes;o++)
					{
						if(les_terminaux_et_non[o] ==  '$')
						{
							verif=1;	
							// traiter les réductions dans la pile
							if(table[numero_de_ligne_a_regarder][o].red_dec == 'r')
							{
								*pile = pile_reduction(fichier_lu,pile,les_terminaux_et_non,caractere_a_regarder,o,
								numero_de_ligne_a_regarder,nbr_colonnes,table, ast);
								
							}
							
							// traiter l'"accept" dans la pile
							else if(table[numero_de_ligne_a_regarder][o].red_dec == 'a')
							{
								printf("\n>> Accept\n");
								afficher_ast(ast);
								for(i=0; i <nbr_colonnes; i++)
								{
									free(table[i]);
								}
								free(table);
								free(les_terminaux_et_non);
								free(ast);
								return 0;
							}
							else 
							{
								printf("\n>> Non acceptance\n"); 
								for(i=0; i <nbr_colonnes; i++)
								{
									free(table[i]);
								}
								free(table);
								free(les_terminaux_et_non);
								free(ast);
								return 0;
							}
							
						}
					}
					break;
				}	
			
				// traiter le cas ou le flot n'est pas vide (c'est-à-dire il contient les caracteres + "$") 
				// chercher le rang du "caractere_a_regarder" dans le tableau les terminaux_et_non pour pourvoir trouver la bonne transformation qui correspond
				else if(caractere_a_regarder == les_terminaux_et_non[i])
				{
					verif=1;
					// traiter le décalage dans la pile
					if(table[numero_de_ligne_a_regarder][i].red_dec == 'd')
					{
						printf("\n%c%d > ",table[numero_de_ligne_a_regarder][i].red_dec,table[numero_de_ligne_a_regarder][i].numero_red_dec);
						// ajout du symbole "d" et le numéro qui correspond
						empiler(pile,caractere_a_regarder);
						empiler(pile,table[numero_de_ligne_a_regarder][i].numero_red_dec);
						
						// gérer le décalage dans l'ast 
						ast = ast_decalage(ast,caractere_a_regarder,verif_ast_vide);
						
						//taille_arbre = taille_ast(ast);
						
						defiler(flot);
						
						break;
					}
					// traiter les réductions dans la pile
					else if(table[numero_de_ligne_a_regarder][i].red_dec == 'r')
					{
						regle = table[numero_de_ligne_a_regarder][i].numero_red_dec;
						printf("\n%c%d > ",table[numero_de_ligne_a_regarder][i].red_dec,table[numero_de_ligne_a_regarder][i].numero_red_dec-1);
						j=0;
						// savoir jusqu'à quand on va s'arreter à supprimer dans la pile, ensuite on supprime pour remplacer avec la bonne regle
						while(fichier_lu.G.rules[regle-1].rhs[j]!='\0'){
						      j++; 
						  }
						
						for(i=0; i<2*j; i++)
						{
							depiler(pile);
						}
						
						// remplacement de ce qui a été supprimé par le non terminal de la regle correpondante
						numero_de_ligne_a_regarder = recup_premier_element_pile(pile);
						empiler(pile,fichier_lu.G.rules[regle-1].lhs);
						caractere_a_regarder = fichier_lu.G.rules[regle-1].lhs;
					
						i=0;
						while(i<=nbr_colonnes)
						{
								if(caractere_a_regarder == les_terminaux_et_non[i])
								{
									empiler(pile,table[numero_de_ligne_a_regarder][i].numero_red_dec);
								}
								i++;
						}
						
						// gérer la réduction dans l'ast 
						ast = ast_reduction(ast,j,fichier_lu,regle);
					}
					else 
					{
						printf("\n\n>> Non acceptance\n"); 
						// desallocation de la table
						for(i=0; i <nbr_colonnes; i++)
						{
							free(table[i]);
						}
						free(table);
						free(les_terminaux_et_non);
						free(ast);
						return 0;
					}
				}

				else {
					verif=0;
					i++;
				}	
		}
		printf("flot: "); afficher_file(flot); printf(" | "); printf("pile: ");afficher_pile(pile);
		if(verif==0)
		{
			printf("\n>> Caractere non reconnu par la grammaire\n");
			for(i=0; i <nbr_colonnes; i++)
			{
				free(table[i]);
			}
			free(table);
			free(les_terminaux_et_non);
			free(ast);
			return 0;
		}	
	}
}
