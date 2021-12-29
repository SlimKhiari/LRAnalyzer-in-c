// Auteurs : Slim Khiari et Chahed Benslama
#ifndef AST
#define AST

#include <stdio.h>
#include <stdlib.h>
#include "lranalyzer.h"
#include "LRGrammar.h"
#include "read_file.h"
#include "ast.h"
#define MAX 1000

int taille_ast(char *ast);
int index_non_terminal(char* ast, int taille_regle);
int indice_crochet_ouvrant(char* ast, int indice_crochet_fermant_recup);
int indice_crochet_fermant(char* ast, int indice_debut);
void afficher_ast(char *ast);
char* ast_decalage(char *ast, char caractere_a_regarder, int verif_ast_vide);
char* ast_reduction(char* ast, int taille_regle, file_read fichier_lu,int regle);
#endif

