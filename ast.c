// Auteur : Slim Khiari
#include <stdio.h>
#include <stdlib.h>
#include "lranalyzer.h"
#include "LRGrammar.h"
#include "read_file.h"
#include "ast.h"

int taille_ast(char *ast)
{
	int taille_ast = 0;
	int a;
	
	for(a=0; a<MAX;a++)
	{
		if(ast[a] != 0)
		{
			taille_ast++;
		}
	}
	
	return taille_ast;
}

int indice_crochet_fermant(char* ast, int indice_crochet_fermant_recup)
{
 
    while( ast[indice_crochet_fermant_recup] != ']')
    {
    	if(indice_crochet_fermant_recup > 0)
    	{
    		indice_crochet_fermant_recup = indice_crochet_fermant_recup - 1;
    	}
    }

    return indice_crochet_fermant_recup;
}

int indice_crochet_ouvrant(char* ast, int indice_crochet_fermant_recup)
{
    int indice_crochet_ouvrant_recup = indice_crochet_fermant_recup;
    int compteur_des_crochets = 1;

    do 
    {
        indice_crochet_ouvrant_recup = indice_crochet_ouvrant_recup - 1;
        if (ast[indice_crochet_ouvrant_recup]== '[')
        {
            compteur_des_crochets = compteur_des_crochets-1;
        }
        else if (ast[indice_crochet_ouvrant_recup] == ']')
        {
            compteur_des_crochets = compteur_des_crochets+1;
    	}
    }while (compteur_des_crochets > 0);

    return indice_crochet_ouvrant_recup;
}

int indice_non_terminal(char* ast, int taille_regle)
{
    int i=0;
    int  index_crochet_ouvrant_de_non_terminal_a_ajouter = taille_ast(ast)-1;
	
    do
    {
        index_crochet_ouvrant_de_non_terminal_a_ajouter = indice_crochet_ouvrant(ast, index_crochet_ouvrant_de_non_terminal_a_ajouter);
        if (i < taille_regle-1) 
        {
            index_crochet_ouvrant_de_non_terminal_a_ajouter = indice_crochet_fermant(ast,index_crochet_ouvrant_de_non_terminal_a_ajouter);
        }
        i = i + 1;
    }while(i < taille_regle);

    return index_crochet_ouvrant_de_non_terminal_a_ajouter-1;
}

void afficher_ast(char *ast)
{
	printf("\nAST : ");
	for(int a=0; a<MAX; a++)
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
			printf("%c",ast[a]);
		}
	}
	printf("\n");
}

char* ast_decalage(char *ast, char caractere_a_regarder, int verif_ast_vide)
{
	int a;
	
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
	
	return ast;
}

char* ast_reduction(char* ast, int taille_regle, file_read fichier_lu,int regle)
{
	int taille_arbre=0;
	char ast_tmp_dep[MAX] = {0};
	int index_parenthese_ouvrante=0;
	int mettre_non_terminal=0;
	int indice_reduction=0;
	int i,a;
	
	if(taille_regle == 0)
	{
		taille_arbre = taille_ast(ast);
		ast[taille_arbre] = fichier_lu.G.rules[regle-1].lhs ;
		ast[taille_arbre+1] ='[';
		ast[taille_arbre+2] =']';
	}
	else if(taille_regle != 0)
	{				
		indice_reduction = taille_ast(ast);
		index_parenthese_ouvrante = indice_crochet_ouvrant(ast,indice_crochet_fermant(ast,indice_reduction-1));
		mettre_non_terminal = indice_non_terminal(ast,taille_regle);
		i=0;
		taille_arbre = taille_ast(ast);
		
		for(a=mettre_non_terminal; a<taille_arbre+1; a++)
		{
			ast_tmp_dep[i] = ast[a]; i++;
		}
									
		ast[mettre_non_terminal] = fichier_lu.G.rules[regle-1].lhs;
		ast[mettre_non_terminal+1] = '[';
									
		i=0;					
		for(a=mettre_non_terminal+2; a<MAX; a++)
		{
			ast[a] = ast_tmp_dep[i];i++;
		}
		
		taille_arbre = taille_ast(ast); 
		ast[taille_arbre] = ']';	
		for(a=0; a<MAX; a++)
		{
			ast_tmp_dep[a] = 0;
		}						
	}
	
	return ast;			
}

						
