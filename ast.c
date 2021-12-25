#include <stdio.h>
#include <stdlib.h>
#include "lranalyzer.h"
#include "LRGrammar.h"
#include "read_file.h"
#include "ast.h"

int taille_arbre(char *ast)
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
 
int nombre_de_carac_a_decaler(char  *ast,file_read fichier_lu,int nbr_de_caractere_a_decaler,int taille_ast, int regle )
{
	int a=0;
	nbr_de_caractere_a_decaler=0;
	for(a = taille_ast; a>=0; a--)
	{
		if(ast[a] != 0){if(ast[a] == fichier_lu.G.rules[regle-1].rhs[0] ||
		ast[a] == -fichier_lu.G.rules[regle-1].rhs[0])
		{
			nbr_de_caractere_a_decaler++;
			break;
		}
		else{nbr_de_caractere_a_decaler++;}}
	}
	return nbr_de_caractere_a_decaler;
}

char* decalage_des_caracteres(char ast_tmp_dep[1000], int taille_ast, int nbr_de_caractere_a_decaler, char* ast, int indice_de_ast_pour_mettre_lhs, int position_ast_parenthese_fermante, file_read fichier_lu, int regle)
{
	int u=0, a=0;
	// décalage des caracteres + mettre le nom terminal
	for(a=taille_ast-nbr_de_caractere_a_decaler; a<taille_ast; a++)
	{
		ast_tmp_dep[u] = ast[a]; u++;
	}
	ast[indice_de_ast_pour_mettre_lhs] = fichier_lu.G.rules[regle-1].lhs;
	ast[indice_de_ast_pour_mettre_lhs+1] = '[';
	position_ast_parenthese_fermante = indice_de_ast_pour_mettre_lhs;					
	for(a=0; a<nbr_de_caractere_a_decaler; a++)
	{
							
		ast[indice_de_ast_pour_mettre_lhs+2] = ast_tmp_dep[a];
		indice_de_ast_pour_mettre_lhs++;
	}
	for(a=position_ast_parenthese_fermante; a<=taille_ast;a++)
	{
		position_ast_parenthese_fermante++;
	}
	ast[position_ast_parenthese_fermante+1] = ']';
	
	return ast;						
}	

char* affichage_ast_reduction(char ast_tmp_dep[1000], int taille_ast, int nbr_de_caractere_a_decaler, char* ast, int indice_de_ast_pour_mettre_lhs, int position_ast_parenthese_fermante, file_read fichier_lu, int regle)
{
	int a,u;
	u=0;
	// pour connaitre la taille du tableau (sans les 0)
	taille_ast = taille_arbre(ast);
						
	// pour connaitre le nbr de caracatere a decaler
						         nbr_de_caractere_a_decaler=nombre_de_carac_a_decaler(ast,fichier_lu,nbr_de_caractere_a_decaler,taille_ast,regle);
			
	indice_de_ast_pour_mettre_lhs = taille_ast-nbr_de_caractere_a_decaler;
						
	ast = decalage_des_caracteres(ast_tmp_dep,taille_ast,nbr_de_caractere_a_decaler,ast, indice_de_ast_pour_mettre_lhs,position_ast_parenthese_fermante,fichier_lu,regle);
						
	//affichage de l'ast
	for(a=0; a<MAX;a++)
	{
			printf("%c",ast[a]);
	}
	return ast;
}

char* affichage_ast_decalage(char* ast, char caractere_a_regarder)
{
	int a;
	int taille_ast=0;
	
	for(a=0; a<MAX;a++)
	{
		if(ast[a] == 0)
		{
				ast[a] = caractere_a_regarder;
				ast[a+1] = '[';
				ast[a+2] = ']';
				break;
		}
	}
	for(a=0; a<MAX;a++)
	{
		if(ast[a] != 0)
		{
			taille_ast++;
		}
	}
	for(a=0; a<MAX;a++)
	{
		printf("%c",ast[a]);
	}
	return ast;
}					
						
