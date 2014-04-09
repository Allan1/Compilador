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
char **r_simb;

void checkSymbol(char c,int br){
	int i;
	bool found = false;
	if(c!=NULL){
		for(i=0;i<20 && !found;i++){
			//printf("i:%d %s\n",i,r_simb[i]);
			if((strlen(r_simb[i])==1) && (r_simb[i][0]==c)){
				found = true;
			}
		}
		if(!found)
			printf("LINHA %i: %c\n",br,c);
	}
}

void checkToken(char* token, int br){//tem que corrigir
	int i,l;
	bool found = false;
	if(token!=NULL){
		printf("strlen %s %i\n",token,strlen(token));
		for(l=0;l<15 && !found;l++){
			for(i=0;i<strlen(token) && i<strlen(r_words[l]);i++){
				if(r_words[l][i]!=token[i]){
					l++;
					break;
				}
			}
			if(i==sizeof(token) && r_words[l][i]=='\0'){
				found = true;
			}
		}
		if(!found)
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
	if(!isAlphanumeric(c))
		return true;
	return false;
}

char* append(char* token, char c) {
    char* src = &c;
    char * new_str;

    if(token==NULL){
    	token = (char*)malloc(sizeof(char));
    	token[0] = c;
    }
    else{
    	/*printf("%s\n",token);
    	new_str = malloc(sizeof(token)+sizeof(src)+1); // aloca o espaco pra concatencao
		if(new_str != NULL){
			new_str[0] = '\0';
			strcat(new_str,token);
			strcat(new_str,src);
		}
		token = new_str;
		new_str = NULL;
		free(new_str);*/
    	int i;
    	new_str = token;
    	token = (char*)malloc(sizeof(char)*(strlen(new_str)+2));
    	for(i=0; i< strlen(new_str); i++)
    		token[i] = new_str[i];
    	token[i] = c;
    	i++;
    	token[i] = '\0';

    	//free(new_str);
    }

	if(token!=NULL)
		printf("token:%s\n",token);

    return token;
}

void validateAlphanumeric(FILE* f, char* token,char c, int* ptr_br,int* ptr_index){
	//puts("validate");
	if(isUppercase(c)){
		token = append(token,c);
		(*ptr_index) = (*ptr_index) + 1;
		fseek(f,*ptr_index,SEEK_SET);
		fscanf(f,"%c",&c);
		printf("index:%d char:%c\n",(*ptr_index),c);
		while(isAlphanumeric(c) && !feof(f)){
			//printf("seek_cur: %d\n",ftell(f));
			token = append(token,c);
			*ptr_index = *ptr_index + 1;
			fseek(f,*ptr_index,SEEK_SET);
			fscanf(f,"%c",&c);
			printf("index:%d char:%c\n",(*ptr_index),c);
		}
		checkToken(token,(*ptr_br));
	}
	else if(isLowercase(c)){
		token = append(token,c);
		puts("antes");
		*ptr_index = *ptr_index + 1;
		fseek(f,*ptr_index,SEEK_SET);
		fscanf(f,"%c",&c);
		puts("depois");
		while(isAlphanumeric(c) && !feof(f)){
			printf("seek_cur: %d\n",ftell(f));
			token = append(token,c);
			*ptr_index = *ptr_index + 1;
			fseek(f,*ptr_index,SEEK_SET);
			fscanf(f,"%c",&c);
			printf("isalpha:%c\n",c);
		}
		checkToken(token,(*ptr_br));
	}
	else if(isNumber(c)){
		token = append(token,c);
		*ptr_index = *ptr_index + 1;
		fseek(f,*ptr_index,SEEK_SET);
		fscanf(f,"%c",&c);
		while(isAlphanumeric(c) && !feof(f)){
			token = append(token,c);
			*ptr_index = *ptr_index + 1;
			fseek(f,*ptr_index,SEEK_SET);
			fscanf(f,"%c",&c);
		}
		checkToken(token,(*ptr_br));

	}
	else{
		//erro
	}
	if(c=='\n'){
		(*ptr_br)++;
		printf("LINHA:%d\n",(*ptr_br));
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

	r_simb = (char**)malloc(sizeof(char*)*20);
	r_simb[0] = "+";
	r_simb[1] = "-";
	r_simb[2] = "*";
	r_simb[3] = "/";
	r_simb[4] = ">";
	r_simb[5] = "<";
	r_simb[6] = "[";
	r_simb[7] = "]";
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
	r_simb[18] = ">=";
	r_simb[19] = "<=";

	if(argc > 0){
		//printf("%s\n",argv[1]);
		FILE *f;
		f = fopen("Debug/exemplo","r");
		if(f!=NULL){
			char c;
			int index = 0;
			int br=0,i=0;
			char *token = NULL;
			fseek(f,index,SEEK_SET);
			printf("LINHA:%d\n",br);
			while(!feof(f)){
				//printf("seek_cur: %d\n",ftell(f));
				fscanf(f,"%c",&c);
				printf("index:%d char:%c\n",index,c);
				if(c=='\n'){
					br++;
					printf("LINHA:%d\n",br);
				}
				else if(isSymbol(c)){
					//puts("symbol");
					//Alterar para a analise sintatica
					if(c=='>' || c=='<'){
						index = index + 1;
						fseek(f,index,SEEK_SET);
						fscanf(f,"%c",&c);
						printf("index:%d char:%c\n",index,c);
						if(isAlphanumeric(c)){
							continue;
						}else{
							checkSymbol(c,br);
						}
					}
					else{
						checkSymbol(c,br);
					}
				}
				else{
					//puts("chamada validate");
					validateAlphanumeric(f,token,c,&br,&index);
				}
				index = index + 1;
				fseek(f,index,SEEK_SET);
			}
			br--;
			//printf("br: %i\n",br);
			fclose(f);

		}
		else{
			printf("Arquivo nao encontrado\n");
			exit(1);
		}
	}

	return 0;
}

