#include <stdio.h>
#include "read_file.h"

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

void empiler(Pile *pile, int nvLettre_Nombre)
{
	Element *nouveau = malloc(sizeof(*nouveau));
	if(pile == NULL || nouveau == NULL)
	{
		exit(EXIT_FAILURE);
	}
	nouveau->lettre_nombre = nvLettre_Nombre;
	nouveau->suivant = pile->premier;
	pile->premier = nouveau;
}

char depiler(Pile *pile)
{
	if(pile == NULL)
	{
		exit(EXIT_FAILURE);
	}
	char nombre_lettre_depile=0;
	Element *elementDepile = pile->premier;
	if(pile != NULL && pile->premier!=NULL)
	{
		nombre_lettre_depile = elementDepile->lettre_nombre;
		pile->premier = elementDepile->suivant;
		free(elementDepile);
	}
	return nombre_lettre_depile;
}

void afficher_pile(Pile *pile)
{
	if(pile == NULL)
	{
		exit(EXIT_FAILURE);
	}
	Element *actuel = pile->premier;
	while(actuel != NULL)
	{
		if(actuel->lettre_nombre > 9)
		{
			printf("%c ",actuel->lettre_nombre);
		}
		else 
		{
			printf("%d ",actuel->lettre_nombre);
		}
		actuel = actuel->suivant;
	}
}

int recup_premier_element_pile(Pile *pile)
{
	if(pile == NULL)
	{
		exit(EXIT_FAILURE);
	}
	int nombre_lettre_depile = 0;
	if(pile->premier != NULL)
	{
		Element *elementDeFile = pile->premier;
		nombre_lettre_depile = elementDeFile->lettre_nombre;
	}
	return nombre_lettre_depile;
}

void enfiler(File *file, int nvLettre_Nombre)
{
	Element *nouveau = malloc(sizeof(*nouveau));
	if(file == NULL || nouveau == NULL)
	{
		exit(EXIT_FAILURE);
	}
	nouveau->lettre_nombre = nvLettre_Nombre;
	nouveau->suivant = NULL;
	if(file->premier != NULL)
	{
		Element *elementActuel = file->premier;
		while(elementActuel->suivant != NULL)
		{
			elementActuel = elementActuel->suivant;
		}
		elementActuel->suivant = nouveau;
	}
	else 
	{
		file->premier = nouveau;
	}
} 

char defiler(File *file)
{
	if(file == NULL)
	{
		exit(EXIT_FAILURE);
	}
	char nombre_lettre_depile = 0;
	if(file->premier != NULL)
	{
		Element *elementDeFile = file->premier;
		nombre_lettre_depile = elementDeFile->lettre_nombre;
		file->premier = elementDeFile->suivant;
		free(elementDeFile);
	}
	return nombre_lettre_depile;
}

void afficher_file(File *file)
{
	if(file == NULL)
	{
		exit(EXIT_FAILURE);
	}
	Element *actuel = file->premier;
	while(actuel != NULL)
	{
		printf("%c ",actuel->lettre_nombre);
		actuel = actuel->suivant;
	}
}

char recup_premier_element_file(File *file)
{
	if(file == NULL)
	{
		exit(EXIT_FAILURE);
	}
	char nombre_lettre_depile = 0;
	if(file->premier != NULL)
	{
		Element *elementDeFile = file->premier;
		nombre_lettre_depile = elementDeFile->lettre_nombre;
	}
	return nombre_lettre_depile;
}

int nbr_colonnes_table(grammar G,int *nbr_carac_terminal, int *nbr_carac_non_terminal)
{
	size_t i,z,v,j,k=0,w=0;
	int inserer=1;
	char nbr_terminal[256] ="o", nbr_non_terminal[256] = "o";
	char x;
	
	for(i=0; i<G.size; i++){
		j=0;
		while(G.rules[i].rhs[j]!='\0'){
			if(G.rules[i].rhs[j]>0){
				z=0;
				while(z<256) 
				{
					if(G.rules[i].rhs[j] == nbr_terminal[z])
					{
						inserer=0; break;
					}
					z++;
				}	
				
				if (inserer != 0) 
				nbr_terminal[k] = G.rules[i].rhs[j]; 
				inserer=1;
				k++;
			}
			else{
				v=0;
				while(v<256) 
				{
					if(-G.rules[i].rhs[j] == nbr_non_terminal[v])
					{
						inserer=0; break;
					}
					v++;
				}	
				if (inserer != 0) nbr_non_terminal[w] = -G.rules[i].rhs[j]; 
				inserer=1;
				w++;
			}
			j++;
		}
	}

	for(int i=0; i<256; i++)
	{
		if (nbr_terminal[i] > 0 && nbr_terminal[i] != 13) *nbr_carac_terminal=*nbr_carac_terminal+1;
		if (nbr_non_terminal[i] > 0 && nbr_non_terminal[i] != 13) *nbr_carac_non_terminal=*nbr_carac_non_terminal+1;
		
	}
	
	return *nbr_carac_terminal+*nbr_carac_non_terminal;
}

Table remplissage_table(Table** table, file_read fichier_lu,char* les_terminaux_et_non,int nbr_carac_terminal,int nbr_colonnes)
{
	int i,j,verif=0;
	char caractere_a_rajouter;
	
	for(i=0; i<fichier_lu.t.nblines; i++){ 
	    if(fichier_lu.t.trans[256*i]){
	      if(fichier_lu.t.trans[256*i]==-127){
		table[i][nbr_carac_terminal].red_dec = 'a';
	      }
	      else{
		table[i][nbr_carac_terminal].red_dec = 'r';
		table[i][nbr_carac_terminal].numero_red_dec = -fichier_lu.t.trans[256*i];
	     }
	    }
	    for(j=1; j<128; j++){
	      if(fichier_lu.t.trans[256*i+j]){
	      	caractere_a_rajouter = j;
		if(fichier_lu.t.trans[256*i+j]>0){
			verif=0;
		     	while(verif < nbr_colonnes)
		     	{
		     		if(les_terminaux_et_non[verif] == caractere_a_rajouter)
		     		{
		     			table[i][verif].red_dec = 'd';
			     		table[i][verif].numero_red_dec = fichier_lu.t.trans[256*i+j];
		     			break;
		     		}
		     		else
		     		{
		     			verif++;
		     		}
		     	}
		     }
		     else{
		    	verif = 0;
		    	while(verif < nbr_colonnes)
		     	{
		     		if(les_terminaux_et_non[verif] == j)
		     		{
		     			table[i][verif].red_dec = 'r';
			     		table[i][verif].numero_red_dec = -fichier_lu.t.trans[256*i+j];
		     			break;
		     		}
		     		else
		     		{
		     			verif++;
		     		}
		     	}
		     }
	      }
	    }
	 
	    for(j=128; j<256; j++){
	      if(fichier_lu.t.trans[256*i+j]){
				verif = 0;
			        while(verif < nbr_colonnes)
				     	{
				     		caractere_a_rajouter = -j;
				     		if(les_terminaux_et_non[verif] == caractere_a_rajouter)
				     		{
				     			table[i][verif].red_dec = ' ';
				     			table[i][verif].numero_red_dec = fichier_lu.t.trans[256*i+j];
				     			break;
				     		}
				     		else
				     		{
				     			verif++;
				     		}
				     	}
	      }
	    }
  	}
	return **table;
}

char recuperation_des_terminaux_et_non(FILE *f,char *les_terminaux_et_non)
{
	signed char c;
	int i=0;
	// enlevement de la partie de la grammaire du fihier test qui nous interesse pas 
	while(1)
	{
		c=fgetc(f);
		if(c=='\t')
		{
			break;
		}
		c=fgetc(f);
		c=fgetc(f);
		i=0;
		while(c!='\n'&&!feof(f))
		{
			if(c=='$')
			{
				c=fgetc(f); 
			}
			c=fgetc(f);
		}
	}
	// début de la récupération des terminaux et non terminaux
	c=fgetc(f);
	i=0;
	les_terminaux_et_non[i] = c;
	while(c!='$')
	{
		i++;
		c=fgetc(f);
		if(c=='\t')
		{
			c=fgetc(f); 
		}
	les_terminaux_et_non[i] = c;
	}
	i++;
	c=fgetc(f);
	if(c=='\t')
	{
		c=fgetc(f);
		les_terminaux_et_non[i] = c; 
	}
	while(c!='\n')
	{
		i++;
		c=fgetc(f); 
		if(c=='\t')
		{
			c=fgetc(f);
			les_terminaux_et_non[i] = c; 
		}
	}
	return *les_terminaux_et_non;
}

int main(int argc, char* argv[])
{	
	size_t w,j,l;
	int k,i,d,q,o;
	int nbr_carac_terminal=0,nbr_carac_non_terminal=0;
	file_read fichier_lu = read_file(argv[1]);	
	int nbr_lignes = fichier_lu.t.nblines-1;
	int nbr_colonnes = nbr_colonnes_table(fichier_lu.G,&nbr_carac_terminal,&nbr_carac_non_terminal)+1;
	char *les_terminaux_et_non = (char*) malloc(nbr_colonnes*sizeof(char*));
	File *flot = malloc(sizeof(*flot)); flot->premier=NULL;
	Pile *pile=malloc(sizeof(*pile)); pile->premier=NULL;
	Table **table = NULL;
	FILE *f=fopen(argv[1], "r");
	char caractere_a_regarder;
	int numero_de_ligne_a_regarder;
	int regle = 0;
	
	// allocation dynamique de la table + initialisation + affichage
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
				if(flot->premier == NULL)
				{
					for(o=0;o<nbr_colonnes;o++)
					{
						if(les_terminaux_et_non[o] ==  '$')
						{
							if(table[numero_de_ligne_a_regarder][o].red_dec == 'r')
							{
								regle = table[numero_de_ligne_a_regarder][o].numero_red_dec;
														printf("  (réduction %c%d)\n",table[numero_de_ligne_a_regarder][o].red_dec,table[numero_de_ligne_a_regarder][o].numero_red_dec-1);
								j=0;
								while(fichier_lu.G.rules[regle-1].rhs[j]!='\0'){
								      j++; 
								  }
								  
								for(d=0; d<2*j; d++)
								{
									depiler(pile);
								}
								
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
							else if(table[numero_de_ligne_a_regarder][o].red_dec == 'a')
							{
								printf("Accept\n"); return 0;
							}
							
						}
					}
					break;
				}	
				else if(caractere_a_regarder == les_terminaux_et_non[i])
				{
					if(table[numero_de_ligne_a_regarder][i].red_dec == 'd')
					{
						printf("  (décalage %c%d)\n",table[numero_de_ligne_a_regarder][i].red_dec,table[numero_de_ligne_a_regarder][i].numero_red_dec);
						empiler(pile,caractere_a_regarder);
						empiler(pile,table[numero_de_ligne_a_regarder][i].numero_red_dec);
						defiler(flot);
						break;
					}
					else if(table[numero_de_ligne_a_regarder][i].red_dec == 'r')
					{
						regle = table[numero_de_ligne_a_regarder][i].numero_red_dec;
												printf(" (réduction %c%d)\n",table[numero_de_ligne_a_regarder][i].red_dec,table[numero_de_ligne_a_regarder][i].numero_red_dec-1);
						j=0;
						while(fichier_lu.G.rules[regle-1].rhs[j]!='\0'){
						      j++; 
						  }
						  
						for(d=0; d<2*j; d++)
						{
							depiler(pile);
						}
						
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
					else if(table[numero_de_ligne_a_regarder][o].red_dec == 'a')
					{
						printf("Accept\n"); return 0;
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
