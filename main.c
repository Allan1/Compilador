/*
 * sem título.c
 *
 *
 */

#include <stdlib.h>
#include <stdio.h>

void checkToken(char* token, int br){
	printf("LINHA %i: %s\n",br,token);
}

int main(int argc, char **argv) {
	void **r_words = (void*)malloc(sizeof(void)*15);
	r_words[0] = "BINARIO";
	r_words[1] = "CONTINUAR";
	r_words[2] = "E";
	r_words[3] = "ENQUANTO";
	r_words[4] = "INTEIRO";
	r_words[5] = "IMPRIMIR";
	r_words[6] = "LER";
	r_words[7] = "NAO";
	r_words[8] = "OU";
	r_words[9] = "PARAR";
	r_words[10] = "REAL";
	r_words[11] = "RETORNAR";
	r_words[12] = "SE";
	r_words[13] = "SENAO";
	r_words[14] = "SIM";

	void **r_simb = (void*)malloc(sizeof(void)*20);
		r_simb[0] = "+";
		r_simb[1] = "-";
		r_simb[2] = "*";
		r_simb[3] = "/";
		r_simb[4] = ">";
		r_simb[5] = "<";
		r_simb[6] = ">=";
		r_simb[7] = "<=";
		r_simb[8] = "=";
		r_simb[9] = "<>";
		r_simb[10] = "<<";
		r_simb[11] = ".";
		r_simb[12] = ";";
		r_simb[13] = "@";
		r_simb[14] = "(";
		r_simb[15] = ")";
		r_simb[16] = "{";
		r_simb[17] = "}";
		r_simb[18] = "[";
		r_simb[19] = "]";
	if(argc > 1){
		printf("%s\n",argv[1]);
		FILE *f;
		f = fopen(argv[1],"r");
		if(f!=NULL){
			int br=0,i=0;
			char c;
			char* token = (char*)malloc(sizeof(char));
			fseek(f,0,SEEK_SET);
			while(!feof(f)){
				fscanf(f,"%c",&c);
				printf("\%c",c);
				if((c>='A' || c<='Z')||(c>='a' || c<='z')||(c>='0' || c<='9')){//token
					token = (char*)realloc(token,sizeof(char));
					token[i] = c;
					i++;
				}
				else{
					checkToken(token,br);
					token = NULL;
					i = 0;
					if(c=='\n'){
						br++;
					}
					else if(c==' '){

					}
				}
				printf("\%c\n",c);
			}
			br--;
			printf("br: %i\n",br);
			fclose(f);
		}
		else{
			printf("Arquivo nao encontrado\n");
			exit(1);
		}
	}

	return 0;
}

