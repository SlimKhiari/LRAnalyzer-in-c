// Auteur: Slim Khiari
#include <stdio.h>
#include <stdlib.h>
#include "lranalyzer.h"
#include "LRGrammar.h"
#include "read_file.h"
#include "ast.h"

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
			printf("%c",actuel->lettre_nombre);
		}
		else 
		{
			printf("%d",actuel->lettre_nombre);
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
		printf("%c",actuel->lettre_nombre);
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
		     	while(verif < nbr_colonnes+1)
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
		    	while(verif < nbr_colonnes+1)
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
			        while(verif < nbr_colonnes+1)
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
	// enlevement de la partie de la grammaire du fichier test qui nous interesse pas 
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

Pile pile_reduction_derniere_etape(file_read fichier_lu, Pile *pile,char *les_terminaux_et_non, char caractere_a_regarder, int o,int numero_de_ligne_a_regarder, int nbr_colonnes, Table **table, char* ast)
{
	int regle,j,i;
	
	regle = table[numero_de_ligne_a_regarder][o].numero_red_dec;
	printf("\n%c%d  ",table[numero_de_ligne_a_regarder][o].red_dec,table[numero_de_ligne_a_regarder][o].numero_red_dec-1);
	
	// savoir jusqu'à quand on va s'arreter à supprimer dans la pile, ensuite on supprime pour remplacer avec la bonne regle
	j=0;
	while(fichier_lu.G.rules[regle-1].rhs[j]!='\0')
	{
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
	while(i<nbr_colonnes)
	{
		if(caractere_a_regarder == les_terminaux_et_non[i])
		{
			empiler(pile,table[numero_de_ligne_a_regarder][i].numero_red_dec);
		}
		i++;							
	}
	
	// mettre à jour l'ast
	ast = ast_reduction(ast,j,fichier_lu,regle);
	return *pile;
}

