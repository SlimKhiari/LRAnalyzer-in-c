#ifndef AST
#define AST

#include <stdio.h>
#include <stdlib.h>
#include "lranalyzer.h"
#include "LRGrammar.h"
#include "read_file.h"
#include "ast.h"
#define MAX 1000

int taille_arbre(char *ast);
int nombre_de_carac_a_decaler(char  *ast,file_read fichier_lu,int nbr_de_caractere_a_decaler,int taille_ast, int regle);
char* decalage_des_caracteres(char ast_tmp_dep[1000], int taille_ast, int nbr_de_caractere_a_decaler, char* ast, int indice_de_ast_pour_mettre_lhs, int position_ast_parenthese_fermante, file_read fichier_lu, int regle);
char* affichage_ast_reduction(char ast_tmp_dep[1000], int taille_ast, int nbr_de_caractere_a_decaler, char* ast, int indice_de_ast_pour_mettre_lhs, int position_ast_parenthese_fermante, file_read fichier_lu, int regle);
char* affichage_ast_decalage(char* ast, char caractere_a_regarder);

#endif

