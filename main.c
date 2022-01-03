// Auteurs : Slim Khiari et Chahed Benslama
#include <stdio.h>
#include <stdlib.h>
#include "lranalyzer.h"
#include "read_file.h"
#include "LRGrammar.h"
#include "ast.h"


int main(int argc, char* argv[])
{	
	size_t o=0,i=0,j=0; // des compteurs

	//Les variables utilisées pour construire l'analyseur
	size_t regle = 0;
	int nbr_carac_terminal=0,nbr_carac_non_terminal=0;
	size_t verif_caractere_dans_grammaire=0; // cette variable permet de verifier si un caractere appartient ou non à la grammaire
	file_read fichier_lu = read_file(argv[1]);	
	size_t nbr_lignes = fichier_lu.t.nblines-1;  // nombre de lignes de la table 2D de la grammaire
	size_t nbr_colonnes = nbr_colonnes_table(fichier_lu.G,&nbr_carac_terminal,&nbr_carac_non_terminal)+2; // nombre de colonnes de la table 2D de la grammaire
	char *les_terminaux_et_non = (char*) malloc(nbr_colonnes*sizeof(char*)); // les terminaux et les non terminaux de la grammaire dans un tableau 1D pour récupérer la bonne transformation
	File *flot = malloc(sizeof(*flot)); flot->premier=NULL;
	Pile *pile=malloc(sizeof(*pile)); pile->premier=NULL;
	FILE *f=fopen(argv[1], "r");
	char caractere_a_regarder; // pour récupérer la bonne transformation nécessaire pour l'analyse synthaxique depuis la table 2D
	size_t numero_de_ligne_a_regarder; // pour récupérer la bonne transformation nécessaire pour l'analyse synthaxique depuis la table 2D
	int dec=0; // pour l'affichage
	
	//Les variables utilisées pour construire l'ast 
	char *ast = (char*) malloc(MAX*sizeof(char*)); // utilisé pour enregistrer l'ast
	char ast_tmp_dep[MAX] = {0};// utilisé pour l'ast pour effectuer le shift
	int taille_arbre=0;
	int index_parenthese_ouvrante=0;
	int mettre_non_terminal=0;
	int indexReduction=0;
	int verif_ast_vide=0; // = 0 si vide sinon 1
	
	/***********************************************************************************************************************/
	/********************************INITIALISATION DES TABLEAUX / PILES / FILES********************************************/
	/***********************************************************************************************************************/
	
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
	
	// récupération des terminaux et non terminaux depuis la grammaire
	*les_terminaux_et_non = recuperation_des_terminaux_et_non(f,les_terminaux_et_non);
  	
	// remplissage de la table 2D à partir de la table 1D
	**table = remplissage_table(table,fichier_lu,les_terminaux_et_non,nbr_carac_terminal,nbr_colonnes);
	
	/***********************************************************************************************************************/
	/************************************************ANALYSE SYNTHAXIQUE ***************************************************/
	/***********************************************************************************************************************/
	
	//début de l'analyse synthaxique 
	printf("    ");printf("Flot");
	Element *actuel = flot->premier;
	while(actuel != NULL)
	{
		printf(" ");
		actuel = actuel->suivant;
	}	
	printf("Pile\n");
	printf("    ");	
	for(i=0; i<3; i++)
	{
		Element *actuel = flot->premier;
		while(actuel != NULL)
		{
			printf("_");
			actuel = actuel->suivant;
		}
	}
	printf("\n");	
	printf("    ");afficher_file(flot); printf(" | "); afficher_pile(pile);
	
	
	numero_de_ligne_a_regarder = recup_premier_element_pile(pile);
	caractere_a_regarder = recup_premier_element_file(flot);
	for(o=0;o<nbr_colonnes;o++)
	{
		if(caractere_a_regarder == les_terminaux_et_non[o])	
		{
			verif_caractere_dans_grammaire=1;
			break;
		}			
	}
	
	if(verif_caractere_dans_grammaire==0)
	{
		printf("\n>> Caractere non reconnu par la grammaire\n");
		
	}
	
	o=0;
	
	//Suite de l'analyse synthaxique + fin de l'analyse synthaxique
	while(1>0)
	{
		verif_caractere_dans_grammaire=0;
		caractere_a_regarder = recup_premier_element_file(flot);
		numero_de_ligne_a_regarder = recup_premier_element_pile(pile);
		i=0;
		taille_arbre=0;
		
		//On parcourt les colonnes (donc les caracteres de la grammaire)de la table 2D construite precedemment afin de connaitre l'index du 'caractere_a_regarder' dans ce cette table 
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
							verif_caractere_dans_grammaire=1;	
							// traiter les réductions dans la pile ( pas de décalage possible)
							if(table[numero_de_ligne_a_regarder][o].red_dec == 'r')
							{
								*pile = pile_reduction_derniere_etape(fichier_lu,pile,les_terminaux_et_non,caractere_a_regarder,o,
								numero_de_ligne_a_regarder,nbr_colonnes,table, ast);
								
							}
							// traiter l'acceptation dans la pile
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
							// traiter la non acceptation dans la pile
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
					verif_caractere_dans_grammaire=1;
					// traiter le décalage dans la pile
					if(table[numero_de_ligne_a_regarder][i].red_dec == 'd')
					{
						dec++;
						printf("\n%c%d  ",table[numero_de_ligne_a_regarder][i].red_dec,table[numero_de_ligne_a_regarder][i].numero_red_dec);
						// ajout du symbole "d" et le numéro qui correspond
						empiler(pile,caractere_a_regarder);
						empiler(pile,table[numero_de_ligne_a_regarder][i].numero_red_dec);
						// mise à jour de l'ast 
						ast = ast_decalage(ast,caractere_a_regarder,verif_ast_vide);
						//suppression du caractere du flot
						defiler(flot);
						break;
					}
					// traiter la réduction dans la pile
					else if(table[numero_de_ligne_a_regarder][i].red_dec == 'r')
					{
						printf("\n%c%d  ",table[numero_de_ligne_a_regarder][i].red_dec,table[numero_de_ligne_a_regarder][i].numero_red_dec-1);
						// connaitre le numero de la regle à appliquer
						regle = table[numero_de_ligne_a_regarder][i].numero_red_dec;
						// savoir jusqu'à quand on va s'arreter à supprimer dans la pile, ensuite on supprime pour remplacer avec la bonne regle
						j=0;
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
						// mise à jour de l'ast
						ast = ast_reduction(ast,j,fichier_lu,regle);
					}
					else 
					{
						printf("\n\n>> Non acceptance\n"); 
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
				// sinon on continue à chercher l'index du caractere se trouvant dans les colonnes de la table 2D
				else {
					verif_caractere_dans_grammaire=0;
					i++;
				}	
		}
		
		if(verif_caractere_dans_grammaire==0)
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
		
		afficher_file(flot); 
		for(i=0;i<dec;i++) printf(" "); printf(" | "); 
		afficher_pile(pile);
	}
}
