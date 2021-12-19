/* Author: Devan SOHIER, 2021 */

#include "LRGrammar.h"

void print_grammar(grammar G){
  size_t i, j;
  for(i=0; i<G.size; i++){
    printf("%c -> ", G.rules[i].lhs);
    j=0;
    while(G.rules[i].rhs[j]!='\0'){
      if(G.rules[i].rhs[j]>0){
			printf("%c", G.rules[i].rhs[j]);
      }
      else{
			printf("#%c", -G.rules[i].rhs[j]);
      }
      j++;
    }
    printf("\n");
  }
}

void print_table(table t, grammar G){
  int i, j, k;
  for(i=0; i<t.nblines; i++){ 
    if(t.trans[256*i]){
      if(t.trans[256*i]==-127){
        printf("%d, $ : a\n", i);
      }
      else{
        printf("%d, $ : r%d (", i, -t.trans[256*i]);
        printf("%c -> ", G.rules[-1-t.trans[256*i]].lhs);
        k=0;
        while(G.rules[-1-t.trans[256*i]].rhs[k]!='\0'){
          if(G.rules[-1-t.trans[256*i]].rhs[k]>0){
		 	   printf("%c", G.rules[-1-t.trans[256*i]].rhs[k]);
          }
          else{
		      printf("#%c", -G.rules[-1-t.trans[256*i]].rhs[k]);
          }
          k++;
       }
       printf(")\n");
     }
    }
    for(j=1; j<128; j++){
      if(t.trans[256*i+j]){
        if(t.trans[256*i+j]>0){
	       printf("%d, %c : d%d\n", i, j, t.trans[256*i+j]);
	     }
	     else{
	       printf("%d, %c : r%d (", i, j, -t.trans[256*i+j]);
	       printf("%c -> ", G.rules[-1-t.trans[256*i+j]].lhs);
          k=0;
          while(G.rules[-1-t.trans[256*i+j]].rhs[k]!='\0'){
            if(G.rules[-1-t.trans[256*i+j]].rhs[k]>0){
		 	     printf("%c", G.rules[-1-t.trans[256*i+j]].rhs[k]);
            }
            else{
		        printf("#%c", -G.rules[-1-t.trans[256*i+j]].rhs[k]);
            }
            k++;
          }
          printf(")\n");
	     }
      }
    }
    for(j=128; j<256; j++){
      if(t.trans[256*i+j]){
			printf("%d, %c : %d\n", i, -j, t.trans[256*i+j]);
      }
    }
  }
}
