#include <stdio.h>
#include <stdlib.h>
#include "lranalyzer.h"
#include "read_file.h"
#include "LRGrammar.h"

int main(int argc, char* argv[])
{	
	size_t w,j,l; // des compteurs
	int k,i,d,q,o; // des compteurs
	int nbr_carac_terminal=0,nbr_carac_non_terminal=0;
	file_read fichier_lu = read_file(argv[1]);	
	int nbr_lignes = fichier_lu.t.nblines-1;  // nombre de lignes de la table 2D de la grammaire
	int nbr_colonnes = nbr_colonnes_table(fichier_lu.G,&nbr_carac_terminal,&nbr_carac_non_terminal)+1; // nombre de colonnes de la table 2D de la grammaire
	char *les_terminaux_et_non = (char*) malloc(nbr_colonnes*sizeof(char*)); // les terminaux et les non terminaux de la grmmaire dans un tableau 1D pour récupérer la bonne transformation
	File *flot = malloc(sizeof(*flot)); flot->premier=NULL;
	Pile *pile=malloc(sizeof(*pile)); pile->premier=NULL;
	Table **table = NULL; // la table 2D de la grammaire
	FILE *f=fopen(argv[1], "r");
	char caractere_a_regarder; // pour récupérer la bonne transformation nécessaire pour l'analyse synthaxique
	int numero_de_ligne_a_regarder; // pour récupérer la bonne transformation nécessaire pour l'analyse synthaxique
	int regle = 0;
	
	// allocation dynamique de la table 2D de la grammaire (car plus pratique à manipuler) + initialisation + affichage
	table = (Table**)malloc(nbr_lignes*sizeof(Table*));
	for(int c=0;c<nbr_lignes;c++)
	{
		table[c] = (Table*)malloc(nbr_colonnes*sizeof(Table));
	}
	for(int i=0; i<nbr_lignes; i++)
	{
		for(int j=0; j<nbr_colonnes; j++)
		{
			table[i][j].numero_red_dec = 0; 
			table[i][j].red_dec = ' ';
		}
	}
	
	
	// initialisation du flot en utilisant une file 
	while(argv[2][w] != '\0')
	{
		enfiler(flot,argv[2][w]);
		w++;
	}
	// initialisation de la pile
	empiler(pile,0);
	
	// récupération des terminaux et non terminaux
	*les_terminaux_et_non = recuperation_des_terminaux_et_non(f,les_terminaux_et_non);
  	
	// remplissage de la table
	**table = remplissage_table(table,fichier_lu,les_terminaux_et_non,nbr_carac_terminal,nbr_colonnes);
		
	// affichage de la table
	printf("La table : \n\n");print_table(fichier_lu.t,fichier_lu.G);
		
	// analyse synthaxique	
	printf("\nL'analyse synthaxique du %s sur %s : \n",argv[1], argv[2]);
	printf("\n-------------------------------------------------------------------------------\n");					
	printf("\nFlot: ");
	afficher_file(flot); 
	printf("\nPile: ");
	afficher_pile(pile);
	printf("\n-------------------------------------------------------------------------------\n");					
	while(1<2)
	{
		caractere_a_regarder = recup_premier_element_file(flot);
		numero_de_ligne_a_regarder = recup_premier_element_pile(pile);
		i=0;
		while(i<nbr_colonnes)
		{
				// traiter le cas ou le flot est vide (c'est-à-dire il ne contient que le "$") 
				if(flot->premier == NULL)
				{
					// chercher le rang du "$" dans le tableau les terminaux_et_non pour pourvoir la bonne transformation qui correspond
					for(o=0;o<nbr_colonnes;o++)
					{
						if(les_terminaux_et_non[o] ==  '$')
						{
							// traiter les réductions dans la pile
							if(table[numero_de_ligne_a_regarder][o].red_dec == 'r')
							{
								regle = table[numero_de_ligne_a_regarder][o].numero_red_dec;
														printf("  (réduction %c%d)\n",table[numero_de_ligne_a_regarder][o].red_dec,table[numero_de_ligne_a_regarder][o].numero_red_dec-1);
								j=0;
								// savoir jusqu'à quand on va s'arreter à supprimer dans la pile, ensuite on supprime pour remplacer avec la bonne regle
								while(fichier_lu.G.rules[regle-1].rhs[j]!='\0'){
								      j++; 
								  }
								
								for(d=0; d<2*j; d++)
								{
									depiler(pile);
								}
								
								// remplacement de ce qui a été supprimé par le non terminal de la regle correpondante
								numero_de_ligne_a_regarder = recup_premier_element_pile(pile);
								empiler(pile,fichier_lu.G.rules[regle-1].lhs);
								caractere_a_regarder = fichier_lu.G.rules[regle-1].lhs;
								q=0;
								while(q<nbr_colonnes)
								{
										if(caractere_a_regarder == les_terminaux_et_non[q])
										{
											empiler(pile,table[numero_de_ligne_a_regarder][q].numero_red_dec);
										}
										q++;
									
								}
								break;
							}
							// traiter l'"accept" dans la pile
							else if(table[numero_de_ligne_a_regarder][o].red_dec == 'a')
							{
								printf("Accept\n"); return 0;
							}
							else 
							{
								printf("Non acceptance\n"); return 0;
							}
							
						}
					}
					break;
				}	
				// traiter le cas ou le flot n'est pas vide (c'est-à-dire il contient les caracteres + "$") 
				// chercher le rang du "caractere_a_regarder" dans le tableau les terminaux_et_non pour pourvoir la bonne transformation qui correspond
				else if(caractere_a_regarder == les_terminaux_et_non[i])
				{
					// traiter le décalage dans la pile
					if(table[numero_de_ligne_a_regarder][i].red_dec == 'd')
					{
						printf("  (décalage %c%d)\n",table[numero_de_ligne_a_regarder][i].red_dec,table[numero_de_ligne_a_regarder][i].numero_red_dec);
						// ajout du symbole "d" et le numéro qui correspond
						empiler(pile,caractere_a_regarder);
						empiler(pile,table[numero_de_ligne_a_regarder][i].numero_red_dec);
						defiler(flot);
						break;
					}
					// traiter les réductions dans la pile
					else if(table[numero_de_ligne_a_regarder][i].red_dec == 'r')
					{
						regle = table[numero_de_ligne_a_regarder][i].numero_red_dec;
												printf(" (réduction %c%d)\n",table[numero_de_ligne_a_regarder][i].red_dec,table[numero_de_ligne_a_regarder][i].numero_red_dec-1);
						j=0;
						// savoir jusqu'à quand on va s'arreter à supprimer dans la pile, ensuite on supprime pour remplacer avec la bonne regle
						while(fichier_lu.G.rules[regle-1].rhs[j]!='\0'){
						      j++; 
						  }
						for(d=0; d<2*j; d++)
						{
							depiler(pile);
						}
						
						// remplacement de ce qui a été supprimé par le non terminal de la regle correpondante
						numero_de_ligne_a_regarder = recup_premier_element_pile(pile);
						empiler(pile,fichier_lu.G.rules[regle-1].lhs);
						caractere_a_regarder = fichier_lu.G.rules[regle-1].lhs;
						q=0;
						while(q<nbr_colonnes)
						{
								if(caractere_a_regarder == les_terminaux_et_non[q])
								{
									empiler(pile,table[numero_de_ligne_a_regarder][q].numero_red_dec);
								}
								q++;
							
						}
						break;
					}
					// traiter l'"accept" dans la pile
					else if(table[numero_de_ligne_a_regarder][o].red_dec == 'a')
					{
						printf("Accept\n"); return 0;
					}
					else 
					{
						printf("Non acceptance\n"); return 0;
					}
				}
				
				else {
					i++;
				}
			
		}
		printf("\nFlot: ");
		afficher_file(flot); 
		printf("\nPile: ");
		afficher_pile(pile);
		printf("\n-------------------------------------------------------------------------------\n");					
		k++;
	}
	printf("\n-------------------------------------------------------------------------------\n");					
						
	// desallocation de la table
	for(i=0; i <nbr_colonnes; i++)
	{
		free(table[i]);
	}
	free(table);
	free(les_terminaux_et_non);
	
	return 0;
}
