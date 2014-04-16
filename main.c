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
char** tokens;
int tokens_n = 0;

void saveToken(char* token){
	tokens_n++;
	if(tokens==NULL)
		tokens = (char**)malloc(sizeof(char*)*tokens_n);
	else
		tokens = (char**)realloc(tokens,sizeof(char*)*tokens_n);
	tokens[tokens_n-1] = token;
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

void checkIdentifier(char* token, int br){
	int i;
	if(token!=NULL){
		for(i=0;i<strlen(token);i++){
			if(!isLowercase(token[i]) && !isNumber(token[i])){
				printf("LINHA %d: %s\n",br,token);
				break;
			}
		}
		saveToken(token);
	}
}


void checkSymbol(char c,int br){
	int i;
	bool found = false;
	if(c!=NULL){
		for(i=0;i<20 && !found;i++){
			if((strlen(r_simb[i])==1) && (r_simb[i][0]==c)){
				found = true;
			}
		}
		if(!found)
			//printf("LINHA %d: %c\n",br,c);
			printf("LINHA %d: ?\n",br);
	}
}

void checkToken(char* token, int br){
	int i,l;
	bool found = false;
	if(token!=NULL){
		for(l=0;l<15 && !found;l++){
			for(i=0;i<strlen(token) && strlen(token)==strlen(r_words[l]);i++){
				if(r_words[l][i]!=token[i]){
					l++;
					break;
				}
			}
			if(i==strlen(token) && i==strlen(r_words[l])){
				found = true;
			}
			i=0;
		}
		if(!found)
			printf("LINHA %i: %s\n",br,token);
		saveToken(token);
	}
}

char* append(char* token, char c) {

    char * new_str;

    if(token==NULL){
    	token = (char*)malloc(sizeof(char)*2);
    	token[0] = c;
    	token[1] = '\0';
    }
    else{
    	int i;
    	new_str = token;
    	token = (char*)malloc(sizeof(char)*(strlen(new_str)+2));
    	for(i=0; i< strlen(new_str); i++)
    		token[i] = new_str[i];
    	token[i] = c;
    	i++;
    	token[i] = '\0';
    	//new_str = NULL;
    	//free(new_str);
    }

    return token;
}

void checkNumber(char* token, int br){
	int i, left=0,right=0;
	bool comma = false;
	int commalocation = 0;
	if(token!=NULL){
		for(i=0;i<strlen(token);i++){
			if(!isNumber(token[i]) && token[i]!=',' && !comma){
				printf("LINHA %d: %s\n",br,token);
				break;
			}
			if(!comma){
				if(token[i]==','){
					comma = true;
					commalocation = i;
				}
				else
					left++;
			}
			else{
				if(token[i]==','){
					printf("LINHA %d: %s\n",br,token);
					break;
				}
				else
					right++;
			}
		}
		if(left>10 || right>10)
			printf("LINHA %d: %s\n",br,token);

		if(comma){

			for(i = 0;i<strlen(token);i++){ //caso pro lado direito
				if(!isNumber(token[i])){
					if(i>commalocation+1){
						char* aux=NULL;
						char c;
						for(i = commalocation+2;i<strlen(token);i++){// ver o porque de somar +2
							c = token[i];
							aux = append(aux,c);
						}
						printf("LINHA %d: %s\n",br,aux);
						printf("LINHA %d: %s\n",br,"?");//virgula
						aux = NULL;
						for(i =0;i<commalocation;i++){//salva o lado esquerdo no token
							c = token[i];
							aux = append(aux,c);
						}
						token = aux;
						commalocation = 0;
					}
				}
			}
		}

		saveToken(token);
	}
}

void validateAlphanumeric(FILE* f, char* token,char* ptr_c, int* ptr_br,int* ptr_index, bool* ptr_comment){
	if(isUppercase((*ptr_c))){
		token = append(token,(*ptr_c));
		(*ptr_index) = (*ptr_index) + 1;
		fseek(f,*ptr_index,SEEK_SET);
		fscanf(f,"%c",ptr_c);
		//printf("index:%d char:%c\n",(*ptr_index),c);
		while(isAlphanumeric((*ptr_c)) && !feof(f)){
			token = append(token,(*ptr_c));
			*ptr_index = *ptr_index + 1;
			fseek(f,*ptr_index,SEEK_SET);
			fscanf(f,"%c",ptr_c);
			//printf("index:%d char:%c\n",(*ptr_index),c);
		}
		checkToken(token,(*ptr_br));
	}
	else if(isLowercase((*ptr_c))){
		token = append(token,(*ptr_c));
		*ptr_index = *ptr_index + 1;
		fseek(f,*ptr_index,SEEK_SET);
		fscanf(f,"%c",ptr_c);
		//printf("index:%d char:%c\n",(*ptr_index),c);
		while(isAlphanumeric((*ptr_c)) && !feof(f)){
			token = append(token,(*ptr_c));
			*ptr_index = *ptr_index + 1;
			fseek(f,*ptr_index,SEEK_SET);
			fscanf(f,"%c",ptr_c);
			//printf("index:%d char:%c\n",(*ptr_index),c);
		}
		checkIdentifier(token,(*ptr_br));
	}
	else if(isNumber((*ptr_c))){
		bool hascomma=false;
		//INTEIRO: 1 ou mais caracteres numéricos (0-9) até um limite de 10 caracteres.
		//REAL: 1 ou mais caracteres numéricos até um limite de 10 caracteres seguidos por
		//uma vígula, seguida por 1 ou mais caracteres numéricos até um limite de 10 caracteres.
		token = append(token,(*ptr_c));
		*ptr_index = *ptr_index + 1;
		fseek(f,*ptr_index,SEEK_SET);
		fscanf(f,"%c",ptr_c);
		//printf("index:%d char:%c\n",(*ptr_index),c);
		while((isAlphanumeric((*ptr_c)) || ((*ptr_c)==',' && !hascomma)) && !feof(f)){
			if((*ptr_c)==',')
				hascomma = true;
			if((isUppercase(*ptr_c))||isLowercase(*ptr_c)) // indentifica logo se o erro estiver antes da virgula
				hascomma =true;
			token = append(token,(*ptr_c));
			*ptr_index = *ptr_index + 1;
			fseek(f,*ptr_index,SEEK_SET);
			fscanf(f,"%c",ptr_c);
			//printf("index:%d char:%c\n",(*ptr_index),c);
		}
		hascomma = false;
		checkNumber(token,(*ptr_br));

	}
	else{
		//error
	}
	if((*ptr_c)=='\n'){
		(*ptr_br)++;
		*ptr_comment = false;
	}
	else if((*ptr_c)==' '){
		//do nothing
	}
	else if((*ptr_c)=='@'){
		*ptr_comment = true;
	}
	else if(*ptr_comment){
		//do nothing
	}
	else if(isSymbol((*ptr_c))){
		checkSymbol((*ptr_c),(*ptr_br));
	}

}

int main(int argc, char **argv) {

	if(argc > 0){
		//printf("%s\n",argv[1]);
		FILE *f;
		f = fopen(argv[1],"r");
		if(f!=NULL){
			char c;
			int index = 0,br=1;
			char *token = NULL;
			bool comment = false;
			fseek(f, 0L, SEEK_END);
			int file_size = ftell(f);
			fseek(f,index,SEEK_SET);
			while(index < file_size){
				fscanf(f,"%c",&c);
				//printf("index:%d char:%c\n",index,c);
				if(c=='\n'){
					br++;
					comment = false;
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
						char prev = c;
						index = index + 1;
						fseek(f,index,SEEK_SET);
						fscanf(f,"%c",&c);
						//printf("index:%d char:%c\n",index,c);
						if(isAlphanumeric(c)){
							validateAlphanumeric(f,token,&c,&br,&index,&comment);
						}else{
							checkSymbol(c,br);
						}
					}
					else{
						checkSymbol(c,br);
					}
				}
				else{
					validateAlphanumeric(f,token,&c,&br,&index,&comment);
				}
				index = index + 1;
				fseek(f,index,SEEK_SET);
				//printf("index:%d\n",index);
			}
			br--;
			//printf("br: %i\n",br);
			fclose(f);
			int j;
			for(j=0;j<tokens_n;j++){
				//printf("%s\n",tokens[j]);
			}
		}
		else{
			printf("Arquivo nao encontrado\n");
			exit(1);
		}
	}

	return 0;
}

