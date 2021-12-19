/* Author : Devan SOHIER, 2021 */

#include "read_file.h"

file_read read_file(const char *file_name){
  file_read fr;
  FILE *f;
  signed char c;
  unsigned char col[256];
  size_t i;
  f=fopen(file_name, "r");
  if(!f){
    fprintf(stderr, "Unable to open file %s.\n", file_name);
    printf("test1\n");
    exit(1);
  }
  fr.G.size=0;
  fr.G.rules=(rule *)malloc(MAX_SIZE*sizeof(rule));
  if(!fr.G.rules){fprintf(stderr, "Unable to allocate memory.\n"); exit(1);}
  while(1){
    if(fr.G.size>MAX_SIZE){
      fprintf(stderr, "Too many grammar rules.\n");
      exit(1);
    }
    c=fgetc(f);
    if(c=='\t'){
      break;
    }
    fr.G.rules[fr.G.size].lhs=c;
    c=fgetc(f);
    c=fgetc(f);
    fr.G.rules[fr.G.size].rhs=(signed char *)malloc((MAX_LEN+1)*sizeof(signed char));
    if(!fr.G.rules[fr.G.size].rhs){fprintf(stderr, "Unable to allocate memory.\n");exit(1);}
    i=0;
    while(c!='\n'&&!feof(f)){
      if(i>MAX_LEN){
        fprintf(stderr, "Rule too long.\n");
        exit(1);
      }
      if(c=='$'){
        c=fgetc(f);
        fr.G.rules[fr.G.size].rhs[i]=-c; 
      }
      else{
        fr.G.rules[fr.G.size].rhs[i]=c;
      }
      c=fgetc(f);
      i++;
    }
    fr.G.rules[fr.G.size].rhs[i]='\0';
    fr.G.rules[fr.G.size].rhs=(signed char *)realloc(fr.G.rules[fr.G.size].rhs, strlen((char *)fr.G.rules[fr.G.size].rhs)+1);
    fr.G.size++;
  }
  fr.G.rules=(rule *)realloc(fr.G.rules, fr.G.size*sizeof(rule));
  fr.G.start=fr.G.rules[0].lhs;
  c=fgetc(f);
  i=0;
  while(c!='$'){
    col[i]=c;
    i++;
    c=fgetc(f);
    if(c=='\t'){
      c=fgetc(f); 
    }
  }
  col[i]=0;
  i++;
  c=fgetc(f);
  if(c=='\t'){
    c=fgetc(f);
  }
  while(c!='\n'){
    col[i]=-c; 
    i++;
    c=fgetc(f); 
    if(c=='\t'){
      c=fgetc(f);  
    }
  }
  fr.t.trans=(signed char *)calloc(256*MAX_SIZE, sizeof(char));
  if(!fr.t.trans){fprintf(stderr, "Unable to allocate memory.\n");exit(1);}
  fr.t.nblines=0;
  while(!feof(f)){
    c=fgetc(f);
    while(!feof(f)&&c!='\t'){
      c=fgetc(f);
    }
    i=0;
    c=fgetc(f);
    while(c!='\n'){
      if(fr.t.nblines>MAX_SIZE){
        fprintf(stderr, "Table too large (too many states).\n");
        exit(1);
      }
      if(c=='\t'){
			i++;
			c=fgetc(f);
      }
      else if(c=='d'){
			c=fgetc(f);
			fr.t.trans[256*fr.t.nblines+col[i]]=0;
			while('0'<=c&&c<='9'){
				fr.t.trans[256*fr.t.nblines+col[i]]=10*fr.t.trans[256*fr.t.nblines+col[i]]+c-'0';
				c=fgetc(f);
			}
      }
      else if(c=='r'){
			c=fgetc(f);
			fr.t.trans[256*fr.t.nblines+col[i]]=0;
			while('0'<=c&&c<='9'){
				fr.t.trans[256*fr.t.nblines+col[i]]=10*fr.t.trans[256*fr.t.nblines+col[i]]-c+'0';
				c=fgetc(f);
			}
      }
      else if(c=='a'){
			fr.t.trans[256*fr.t.nblines+col[i]]=-127;
			c=fgetc(f);
      }
      else if('0'<=c&&c<='9'){
			fr.t.trans[256*fr.t.nblines+col[i]]=c-'0';
			c=fgetc(f);
			while('0'<=c&&c<='9'){
			  fr.t.trans[256*fr.t.nblines+col[i]]=10*fr.t.trans[256*fr.t.nblines+col[i]]+c-'0';
			  c=fgetc(f);
			}
      }
      else{
			if(feof(f))
			  break;
			else{
			  fprintf(stderr, "Unknown symbol %c in table.\n", c);
			  exit(1);
			}
      }
    }
    fr.t.nblines++;
  }
  fr.t.trans=(signed char*)realloc(fr.t.trans, 256*fr.t.nblines*sizeof(signed char));
  return fr;
}
