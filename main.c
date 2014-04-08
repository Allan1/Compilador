/*
 * MATA61 - Compiladores
 * Alunos: Allan dos Santos Oliveira, Marcio Vicente, Welbert Serra
 * main.c
 *
 *
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char **r_words;
void **r_simb;

void checkSymbol(char* token, char c){

}

void checkToken(char* token, int br){
	int i,l;
	bool found = false;
	if(token!=NULL){
		//puts("!null");
		/*for(l=0;l<15 && !found;l++){
			printf("strlen %s %i\n",token,strlen(token));
			for(i=0;i<strlen(token);i++){
				if(r_words[l][i]!=token[i]){
					l++;
					break;
				}
			}
			if(i==sizeof(token) && r_words[l][i]=='\0'){
				found = true;
			}
		}*/
		printf("LINHA %i: %s\n",br,token);
	}
}

bool isNumber(char c){
	if(c>='0' && c<='9')
		return true;
	return false;
}

bool isUppercase(char c){
	if(c>='A' && c<='Z')
		return true;
	return false;
}

bool isLowercase(char c){
	if(c>='a' && c<='z')
		return true;
	return false;
}

bool isAlphanumeric(char c){
	if(isNumber(c) || isUppercase(c) || isLowercase(c))
		return true;
	return false;
}

bool isSymbol(char c){
	printf("%c\n",c);
	if(!isAlphanumeric(c))
		return true;
	return false;
}

void append(char* token,char c){

	char src[1];
	src[0] = c;


   strcat(token, src);

   printf("%s",token);
}

void validateAlphanumeric(FILE* f, char* token,char c, int br){
	puts("validate");
	if(isUppercase(c)){
		append(token,c);
		fseek(f,4,SEEK_CUR);
		fscanf(f,"%c",&c);
		while(isAlphanumeric(c) && !feof(f)){
			append(token,c);
			fseek(f,4,SEEK_CUR);
			fscanf(f,"%c",&c);
		}
		checkToken(token,br);
	}
	else if(isLowercase(c)){
		append(token,c);
		puts("antes");
		fseek(f,4,SEEK_CUR);
		fscanf(f,"%c",&c);
		puts("depois");
		while(isAlphanumeric(c) && !feof(f)){
			append(token,c);
			fseek(f,4,SEEK_CUR);
			fscanf(f,"%c",&c);
			printf("isalpha:%c\n",c);
		}
		checkToken(token,br);
	}
	else if(isNumber(c)){
		append(token,c);
		fseek(f,4,SEEK_CUR);
		fscanf(f,"%c",&c);
		while(isAlphanumeric(c) && !feof(f)){
			append(&token,c);
			fseek(f,4,SEEK_CUR);
			fscanf(f,"%c",&c);
		}
		checkToken(token,br);

	}
	if(c=='\n'){
		br++;
	}

}

int main(int argc, char **argv) {
	r_words = (void*)malloc(sizeof(void)*15);
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

	r_simb = (void*)malloc(sizeof(void)*20);
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

	if(argc > 0){
		//printf("%s\n",argv[1]);
		FILE *f;
		f = fopen("Debug/teste","r");
		if(f!=NULL){
			char c;

			int br=0,i=0;
			char *token = NULL;
			fseek(f,0,SEEK_SET);

			while(!feof(f)){
				puts("while");
				fscanf(f,"%c",&c);
				printf("%c\n",c);
				if(c=='\n'){
					puts("br");
					br++;
					continue;
				}
				else if(isSymbol(c)){
					//puts("symbol");
					//Alterar para a analise sintatica
					if(c=='>' || c=='<'){
						fseek(f,4,SEEK_CUR);
						fscanf(f,"%c",&c);

						if(isAlphanumeric(c)){
							continue;
						}else{
							checkSymbol(token,c);
						}
					}
				}
				else{
					//puts("chamada validate");
					validateAlphanumeric(f,token,c,br);
				}
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

