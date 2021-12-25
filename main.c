#include <stdio.h>
#include <stdlib.h>
#include "lranalyzer.h"
#include "read_file.h"
#include "LRGrammar.h"
#include "ast.h"

int get_index_next_closing_parenthese(char* string, int indexStart)
{
    int indexClosingParenthese = indexStart;
    
    while( (string[indexClosingParenthese] != ']') && (indexClosingParenthese > 0) )
    {
        indexClosingParenthese--;
    }
    
    return indexClosingParenthese;
}

int get_index_matching_opening_parenthese(char* string, int indexClosingParenthese)
{
    int indexOpeningParenthese = indexClosingParenthese, counterParentheses = 1;
    char charString = string[indexOpeningParenthese];

    while (counterParentheses > 0)
    {
        indexOpeningParenthese--;
        charString = string[indexOpeningParenthese];

        if (charString == -1)
            counterParentheses++;
        else if (charString == '[')
            counterParentheses--;
    }

    return indexOpeningParenthese;
}

int get_index_insert_reduction(char* string, int nbCharsRule)
{
    int i, indexReduction = strlen(string)-1;

    for(i = 0; i < nbCharsRule; i++)
    {
        indexReduction = get_index_matching_opening_parenthese(string, indexReduction);
        if (i < nbCharsRule-1) 
            indexReduction = get_index_next_closing_parenthese(string, indexReduction);
    }

    return indexReduction-1; //-1 because the current index is for the opening parenthese, not the non-terminal character
}

int main(int argc, char* argv[])
{	
	size_t l,q,k,d,w,o,i,j,tmp,u,t; // des compteurs
	int a;
	int verif_ast_vide=0;
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
	Table **table = NULL; // la table 2D de la grammaire
	FILE *f=fopen(argv[1], "r");
	char caractere_a_regarder; // pour récupérer la bonne transformation nécessaire pour l'analyse synthaxique
	size_t numero_de_ligne_a_regarder; // pour récupérer la bonne transformation nécessaire pour l'analyse synthaxique
	
	//Les variables utilisées pour construire l'ast 
	size_t *taille_regles = (size_t*)malloc(fichier_lu.G.size*sizeof(size_t*)); // utilisé pour connaitre la taille de la regle
	char *ast = (char*) malloc(MAX*sizeof(char*)); // utilisé pour enregistrer l'ast
	char ast_tmp_dep[MAX] = {0};// utilisé pour l'ast pour effectuer le shift
	int taille_ast=0;// utilisé pour l'ast
	int index_parenthese_ouvrante=0;
	int mettre_non_terminal=0;
	int indexReduction=0;
	
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
	for(int i=0; i<nbr_lignes; i++)
	{
		for(int j=0; j<nbr_colonnes; j++)
		{
			printf(" %c%d ",table[i][j].red_dec,table[i][j].numero_red_dec);
		}
		printf("\n");
	}
	// analyse synthaxique	
	printf("\nL'analyse synthaxique du (%s) sur (%s) : \n\n",argv[1], argv[2]);					
	printf("flot: "); afficher_file(flot); printf(" | "); printf("pile: ");afficher_pile(pile);
	
	numero_de_ligne_a_regarder = recup_premier_element_pile(pile);
	caractere_a_regarder = recup_premier_element_file(flot);
	
	for(o=0;o<nbr_colonnes;o++)
	{
		if(caractere_a_regarder == les_terminaux_et_non[o])	
		{
			verif=1;break;
		}			
	}
	
	if(verif==0)
	{
		printf(">> Caractere non reconnu par la grammaire\n");
		
	}
	
	tmp=0; o=0;
	
	while(table[numero_de_ligne_a_regarder][o].red_dec != 'a' && verif==1)
	{
		o=0;
		verif=0;
		caractere_a_regarder = recup_premier_element_file(flot);
		numero_de_ligne_a_regarder = recup_premier_element_pile(pile);
		i=0;
		taille_ast=0;
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
								regle = table[numero_de_ligne_a_regarder][o].numero_red_dec;
								printf("\n\n> %c%d: \n",table[numero_de_ligne_a_regarder][o].red_dec,table[numero_de_ligne_a_regarder][o].numero_red_dec-1);
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
								
								if(j == 0)
								{
									taille_ast = taille_arbre(ast);
									ast[taille_ast] =fichier_lu.G.rules[regle-1].lhs ;
									ast[taille_ast+1] ='[';
									ast[taille_ast+2] =']';
									break;
								}
								else if(j != 0)
								{
									
									indexReduction = taille_arbre(ast) ;
									index_parenthese_ouvrante = get_index_matching_opening_parenthese(ast,get_index_next_closing_parenthese(ast,indexReduction-1));
									mettre_non_terminal = get_index_insert_reduction(ast,j);
									u=0;
									taille_ast = taille_arbre(ast);
									for(a=mettre_non_terminal; a<taille_ast+1; a++)
									{
										ast_tmp_dep[u] = ast[a]; u++;
									}
									u=0;
									ast[mettre_non_terminal] = fichier_lu.G.rules[regle-1].lhs;
									ast[mettre_non_terminal+1] = '[';
															
									for(a=mettre_non_terminal+2; a<MAX; a++)
									{
										ast[a] = ast_tmp_dep[u];u++;
									}
									taille_ast = taille_arbre(ast); 
									ast[taille_ast] = ']';	
									for(a=0; a<MAX; a++)
									{
										ast_tmp_dep[a] = 0;
									}						
									break;
								 }
				
							}
							// traiter l'"accept" dans la pile
							if(table[numero_de_ligne_a_regarder][o].red_dec == 'a')
							{
								printf("\n\n>> Accept\n");
								for(a=0; a<MAX;a++)
								{
									if(ast[a] == '[')
									{
										printf("(");
									}
									else if(ast[a] == ']')
									{
										printf(")");
									}
									else
									{
										printf(" %c",ast[a]);
									}	
								}
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
						printf("\n\n> %c%d: \n",table[numero_de_ligne_a_regarder][i].red_dec,table[numero_de_ligne_a_regarder][i].numero_red_dec);
						// ajout du symbole "d" et le numéro qui correspond
						empiler(pile,caractere_a_regarder);
						empiler(pile,table[numero_de_ligne_a_regarder][i].numero_red_dec);
						
						//gérer le décalage dans l'AST
						for(a=MAX; a>=0;a--)
						{
							if(ast[a] != 0)
							{
								verif_ast_vide = 1;
							}
						}
						if(verif_ast_vide == 0)
						{
							ast[0] = caractere_a_regarder;
							ast[1] = '[';
							ast[2] = ']';
						}
						else
						{
							for(a=MAX; a>=0;a--)
							{
								if(ast[a] != 0)
								{
									ast[a+1] = caractere_a_regarder;
									ast[a+2] = '[';
									ast[a+3] = ']';
									break;
								}
							}	
						}
						for(a=MAX; a>=0; a--)
						{
							if(ast[a] == 0)
							{
								taille_ast++;
							}
							else
							{
								break;
							}
						}

						taille_ast = taille_arbre(ast);
						
						defiler(flot);
						
						break;
					}
					// traiter les réductions dans la pile
					else if(table[numero_de_ligne_a_regarder][i].red_dec == 'r')
					{
						regle = table[numero_de_ligne_a_regarder][i].numero_red_dec;
						printf("\n\n> %c%d: \n",table[numero_de_ligne_a_regarder][i].red_dec,table[numero_de_ligne_a_regarder][i].numero_red_dec-1);
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
						while(q<=nbr_colonnes)
						{
								if(caractere_a_regarder == les_terminaux_et_non[q])
								{
									
									empiler(pile,table[numero_de_ligne_a_regarder][q].numero_red_dec);
								}
								q++;
						}
					
						if(j == 0)
						{
							taille_ast = taille_arbre(ast);
							ast[taille_ast] =fichier_lu.G.rules[regle-1].lhs ;
							ast[taille_ast+1] ='[';				
							ast[taille_ast+2] =']';
							break;
						}
						else if(j != 0)
						{
							indexReduction = taille_arbre(ast) ;
							index_parenthese_ouvrante = get_index_matching_opening_parenthese(ast,get_index_next_closing_parenthese(ast,indexReduction-1));
							mettre_non_terminal = get_index_insert_reduction(ast,j);
							u=0;
							taille_ast = taille_arbre(ast);
							for(a=mettre_non_terminal; a<taille_ast+1; a++)
							{
								ast_tmp_dep[u] = ast[a]; u++;
							}
							u=0;
							ast[mettre_non_terminal] = fichier_lu.G.rules[regle-1].lhs;
							ast[mettre_non_terminal+1] = '[';
												
							for(a=mettre_non_terminal+2; a<MAX; a++)
							{
								ast[a] = ast_tmp_dep[u];u++;
							}
							taille_ast = taille_arbre(ast); 
							ast[taille_ast] = ']';	
													
							for(a=0; a<MAX; a++)
							{
								ast_tmp_dep[a] = 0;
							}						
							break;
						}
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
			printf(">> Caractere non reconnu par la grammaire");
		}
	}

	printf("\n");
	for(a=0; a<MAX;a++)
	{
		if(ast[a] == '[')
		{
			printf("(");
		}
		else if(ast[a] == ']')
		{
			printf(")");
		}
		else
		{
			printf(" %c",ast[a]);
		}	
	}
	for(i=0; i <nbr_colonnes; i++)
	{
		free(table[i]);
	}
	free(table);
	free(les_terminaux_et_non);
	free(ast);
	return 0;
}
