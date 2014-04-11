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

static char* r_words[] = {"BINARIO","CONTINUAR","E","ENQUANTO","INTEIRO","IMPRIMIR","LER","NAO","OU","PARAR","REAL","RETORNAR","SE","SENAO","SIM"};
static char* r_simb[] = {"+","-","*","/",">","<","[","]","=","<>","<<",".",";","@","(",")","{","}",">=","<="};

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

void checkIdentifier(char* token, int br){
	int i;
	if(token!=NULL){
		for(i=0;i<strlen(token);i++){
			if(isUppercase(token[i])){
				printf("LINHA %d: %s\n",br,token);
				break;
			}
		}
	}
}
void checkNumber(char* token, int br){
	int i, left=0,right=0;
	bool comma = false;
	if(token!=NULL){
		for(i=0;i<strlen(token);i++){
			if(!isNumber(token[i]) && token[i]!=','){
				printf("LINHA %d: %s\n",br,token);
				break;
			}
			if(!comma){
				if(token[i]==',')
					comma = true;
				else
					left++;
			}
			else{
				if(token[i]==',')
					printf("LINHA %d: %s\n",br,token);
				else
					right++;
			}
		}
		if(left>10 || right>10)
			printf("LINHA %d: %s\n",br,token);
	}
}

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
			printf("LINHA %d: %c\n",br,c);
	}
}

void checkToken(char* token, int br){//tem que corrigir
	int i,l;
	bool found = false;
	//printf("%s\n",r_words[5]);
	if(token!=NULL){
		//printf("strlen %s %i\n",token,strlen(token));
		for(l=0;l<15 && !found;l++){
			//printf("%d\n",l);
			//printf("compare %s\n",r_words[l]);
			for(i=0;i<strlen(token) && strlen(token)==strlen(r_words[l]);i++){
				if(r_words[l][i]!=token[i]){
					l++;
					break;
				}
			}
			//printf("%d %d\n",strlen(token),strlen(r_words[l]));
			if(i==strlen(token) && i==strlen(r_words[l])){
				found = true;
			}
			i=0;
		}
		if(!found)
			printf("LINHA %i: %s\n",br,token);
	}
}


char* append(char* token, char c) {

    char * new_str;

    if(token==NULL){
    	token = (char*)malloc(sizeof(char));
    	token[0] = c;
    }
    else{
    	/*
    	char* src = &c;
    	printf("%s\n",token);
    	new_str = malloc(sizeof(token)+sizeof(src)+1); // aloca o espaco pra concatencao
		if(new_str != NULL){
			new_str[0] = '\0';
			strcat(new_str,token);
			strcat(new_str,src);
		}
		token = new_str;
		new_str = NULL;
		free(new_str);*/
    	//free(new_str);
    	int i;
    	new_str = token;
    	token = (char*)malloc(sizeof(char)*(strlen(new_str)+2));
    	for(i=0; i< strlen(new_str); i++)
    		token[i] = new_str[i];
    	token[i] = c;
    	i++;
    	token[i] = '\0';
    }

	//if(token!=NULL)
		//printf("token:%s\n",token);

    return token;
}

void validateAlphanumeric(FILE* f, char* token,char c, int* ptr_br,int* ptr_index, bool* ptr_comment){
	if(isUppercase(c)){
		token = append(token,c);
		(*ptr_index) = (*ptr_index) + 1;
		fseek(f,*ptr_index,SEEK_SET);
		fscanf(f,"%c",&c);
		//printf("index:%d char:%c\n",(*ptr_index),c);
		while(isAlphanumeric(c) && !feof(f)){
			token = append(token,c);
			*ptr_index = *ptr_index + 1;
			fseek(f,*ptr_index,SEEK_SET);
			fscanf(f,"%c",&c);
			//printf("index:%d char:%c\n",(*ptr_index),c);
		}
		checkToken(token,(*ptr_br));
	}
	else if(isLowercase(c)){
		token = append(token,c);
		*ptr_index = *ptr_index + 1;
		fseek(f,*ptr_index,SEEK_SET);
		fscanf(f,"%c",&c);
		//printf("index:%d char:%c\n",(*ptr_index),c);
		while(isAlphanumeric(c) && !feof(f)){
			token = append(token,c);
			*ptr_index = *ptr_index + 1;
			fseek(f,*ptr_index,SEEK_SET);
			fscanf(f,"%c",&c);
			//printf("index:%d char:%c\n",(*ptr_index),c);
		}
		checkIdentifier(token,(*ptr_br));
	}
	else if(isNumber(c)){
		//INTEIRO: 1 ou mais caracteres numéricos (0-9) até um limite de 10 caracteres.
		//REAL: 1 ou mais caracteres numéricos até um limite de 10 caracteres seguidos por
		//uma vígula, seguida por 1 ou mais caracteres numéricos até um limite de 10 caracteres.
		token = append(token,c);
		*ptr_index = *ptr_index + 1;
		fseek(f,*ptr_index,SEEK_SET);
		fscanf(f,"%c",&c);
		//printf("index:%d char:%c\n",(*ptr_index),c);
		while(isAlphanumeric(c) && !feof(f)){
			token = append(token,c);
			*ptr_index = *ptr_index + 1;
			fseek(f,*ptr_index,SEEK_SET);
			fscanf(f,"%c",&c);
			//printf("index:%d char:%c\n",(*ptr_index),c);
		}
		checkNumber(token,(*ptr_br));

	}
	else{
		//error
	}
	if(c=='\n'){
		(*ptr_br)++;
		*ptr_comment = false;
		//printf("LINHA:%d\n",(*ptr_br));
	}
	else if(c==' '){
		//do nothing
	}
	else if(c=='@'){
		*ptr_comment = true;
	}
	else if(*ptr_comment){
		//do nothing
	}
	else if(isSymbol(c)){
		checkSymbol(c,(*ptr_br));
	}

}

int main(int argc, char **argv) {

	if(argc > 0){
		//printf("%s\n",argv[1]);
		FILE *f;
		f = fopen("Debug/exemplo","r");
		if(f!=NULL){
			char c;
			int index = 0;
			int br=1,i=0;
			char *token = NULL;
			bool comment = false;
			fseek(f, 0L, SEEK_END);
			int file_size = ftell(f);
			//printf("filesize %d\n",file_size);
			fseek(f,index,SEEK_SET);
			//printf("LINHA:%d\n",br);
			while(index < file_size){
				//printf("seek_cur: %d\n",ftell(f));
				fscanf(f,"%c",&c);
				//printf("index:%d char:%c\n",index,c);
				if(c=='\n'){
					br++;
					comment = false;
					//printf("LINHA:%d\n",br);
				}
				else if(c==' ' || c=='\t'){
					//do nothing
				}
				else if(c=='@'){
					comment = true;
				}
				else if(comment){
					//do nothing
				}
				else if(isSymbol(c)){
					//Alterar para a analise sintatica
					if(c=='>' || c=='<'){
						index = index + 1;
						fseek(f,index,SEEK_SET);
						fscanf(f,"%c",&c);
						//printf("index:%d char:%c\n",index,c);
						if(isAlphanumeric(c)){
							validateAlphanumeric(f,token,c,&br,&index,&comment);
						}else{
							checkSymbol(c,br);
						}
					}
					else{
						checkSymbol(c,br);
					}
				}
				else{
					validateAlphanumeric(f,token,c,&br,&index,&comment);
				}
				index = index + 1;
				fseek(f,index,SEEK_SET);
				//printf("index:%d\n",index);
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

