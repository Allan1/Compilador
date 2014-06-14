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

typedef struct Stack {
    int state;
    char *str;
    struct Stack* prev;
} Stack;

static char* r_words[] = {"BINARIO","CONTINUAR","E","ENQUANTO","INTEIRO","IMPRIMIR","LER","NAO","OU","PARAR","REAL","RETORNAR","SE","SENAO","SIM"};
static char* r_simb[] = {"+","-","*","/",">","<","[","]","=","<>","<<",".",";","@","(",")","{","}",">=","<="};
char** tokens;
int tokens_counter = 0;
bool lexic_ok = true;
Stack *states_stack = NULL;
Stack *tokens_stack = NULL;
char* table[150][72];
char* vocabulary[72];

void saveToken(char* token){
	tokens_counter++;
	if(tokens==NULL)
		tokens = (char**)malloc(sizeof(char*)*tokens_counter);
	else
		tokens = (char**)realloc(tokens,sizeof(char*)*tokens_counter);
	tokens[tokens_counter-1] = token;
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

bool isInt(char* str){
	int i = 0;
	for(i=0;i<strlen(str);i++){
		if(!isNumber(str[i]))
			return false;
	}
	return true;
}

bool isIdentifier(char* token){
	int i;
	if(token!=NULL){
		if(!isLowercase(token[0]))
			return false;
		for(i=1;i<strlen(token);i++){
			if(!isLowercase(token[i]) && !isNumber(token[i])){
				return false;
			}
		}
	}
	return true;
}

void checkIdentifier(char* token, int br){
	int i;
	if(token!=NULL){
		for(i=0;i<strlen(token);i++){
			if(!isLowercase(token[i]) && !isNumber(token[i])){
				printf("LINHA %d: %s\n",br,token);
				lexic_ok = false;
				break;
			}
		}
		saveToken(token);
	}
}


void checkSymbol(char c,int br){
	int i;
	bool found = false;
	for(i=0;i<20 && !found;i++){
		if((strlen(r_simb[i])==1) && (r_simb[i][0]==c)){
			found = true;
		}
	}
	if(!found){
		//printf("LINHA %d: %c\n",br,c);
		printf("LINHA %d: ?\n",br);
		lexic_ok = false;
	}
	else{
		char *aux = (char*)malloc(sizeof(char)*2);
		aux[0] = c;
		aux[1] = '\0';
		saveToken(aux);
	}
}

bool isReservedWord(char* token){
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
	}
	return found;
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
		if(!found){
			printf("LINHA %i: %s\n",br,token);
			lexic_ok = false;
		}
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
    	new_str = NULL;
    	free(new_str);
    }

    return token;
}

void checkNumber(char* token, int br){
	int i, left=0,right=0;
	bool comma = false;
	if(token!=NULL){
		for(i=0;i<strlen(token);i++){
			if(!isNumber(token[i]) && token[i]!=','){
				printf("LINHA %d: %s\n",br,token);
				lexic_ok = false;
				break;
			}
			if(!comma){
				if(token[i]==',')
					comma = true;
				else
					left++;
			}
			else{
				if(token[i]==','){
					printf("LINHA %d: %s\n",br,token);
					lexic_ok = false;
					break;
				}
				else
					right++;
			}
		}
		if(left>10 || right>10){
			printf("LINHA %d: %s\n",br,token);
			lexic_ok = false;
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
	    bool prev_comma = false;
	    int count_comma = 0;
	    int i = 0,j,next;
	    char** n_tokens = (char**)malloc(sizeof(char*));
	    n_tokens[i] = NULL;
	    n_tokens[i] = append(n_tokens[i],(*ptr_c));

	    *ptr_index = *ptr_index + 1;
	    fseek(f,*ptr_index,SEEK_SET);
	    fscanf(f,"%c",ptr_c);

	    while((isAlphanumeric((*ptr_c)) || ((*ptr_c)==',')) && !feof(f)){
	        if((*ptr_c)==','){
	            count_comma++;
	            prev_comma = true;
	        }
	        else{
	            if(prev_comma){
	                i++;
	                n_tokens = (char**)realloc(n_tokens,sizeof(char*)*(i+1));
	                n_tokens[i] = NULL;
	                prev_comma = false;
	            }
	            n_tokens[i] = append(n_tokens[i],(*ptr_c));
	        }

	        *ptr_index = *ptr_index + 1;
	        fseek(f,*ptr_index,SEEK_SET);
	        fscanf(f,"%c",ptr_c);
	    }

	    for (j = 0; j <= i; j++) {
	        next = j+1;
	        if (next <= i){ //Não é o último
	            if (isInt(n_tokens[j]) && isInt(n_tokens[next])){ //00,00 | 0000000000000,00 | 00,0000000000000
	            	char* float_number = append(n_tokens[j],',');
	            	int k;
	            	for(k=0;k<strlen(n_tokens[next]);k++)
	            		float_number = append(float_number,n_tokens[next][k]);
	                checkNumber(float_number,(*ptr_br));
	                j=j+2; //pula o próximo
	                if(j<=i){
	                	printf("LINHA %d: ?\n",(*ptr_br));
	                	lexic_ok = false;
	                }
	                break;
	            }
	            else if(isInt(n_tokens[j])){ // 0,asd | 0,0a | 0,REAL | 00000000000000,a
	                checkNumber(n_tokens[j],(*ptr_br));
	                printf("LINHA %d: ?\n",(*ptr_br));
	                lexic_ok = false;
	            }
	            else if(!isIdentifier(n_tokens[j]) && !isReservedWord(n_tokens[j])){ //Não é variavel, função ou palavra reservada
	            	printf("LINHA %d: %s\n",(*ptr_br),n_tokens[j]);
	                printf("LINHA %d: ?\n",(*ptr_br));
	                lexic_ok = false;
	            }
	        }
	        else{ //É o último
	            if(isInt(n_tokens[j])){ // 00000 | 0000000000000000
	                checkNumber(n_tokens[j],(*ptr_br));
	            }
	            else if(!isIdentifier(n_tokens[j]) && !isReservedWord(n_tokens[j])){ //Não é variavel, função ou palavra reservada
	                printf("LINHA %d: %s\n",(*ptr_br),n_tokens[j]);
	                lexic_ok = false;
	            }
	        }
	    }
	    if (count_comma > i){ // último caracter lido antes de ler um divisor('\br',' ',simbolos) foi uma virgula(',')
	    	printf("LINHA %d: ?\n",(*ptr_br));
	    	lexic_ok = false;
	    }
	    n_tokens = NULL;
	    free(n_tokens);
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

void push(Stack **stack,int state, char* str){
	Stack *aux= (Stack*)malloc(sizeof(Stack));
	aux->state = state;
	aux->prev = *stack;
	if(*stack !=NULL){
		printf("prev:%s\n",(*stack)->str);
	}
	aux->str = str;
	printf("push %d\n",state);
	*stack = aux;
	//free(aux);
}

Stack* pop(Stack *stack){
	if(stack!=NULL){
		Stack *top = stack;
		printf("pop %d\n",top->state);
		stack = top->prev;
		return top;
	}
	return NULL;
}

Stack* get(Stack *stack){
	if(stack!=NULL){
		Stack *top = stack;
		printf("get %d\n",top->state);
		return top;
	}
	return NULL;
}

int posInVocabulary(char* token){
	int i,l;
	int pos = -1;
	if(token!=NULL){
		for(l=0;l<15 && pos==-1;l++){
			for(i=0;i<strlen(token) && strlen(token)==strlen(vocabulary[l]);i++){
				if(vocabulary[l][i]!=token[i]){
					l++;
					break;
				}
			}
			if(i==strlen(token) && i==strlen(vocabulary[l])){
				pos = l;
			}
			i=0;
		}
	}
	return pos;
}

bool isTokenNumber(char* token){
	int i, left=0,right=0;
	bool comma = false;
	bool lexic_ok = false;
	if(token!=NULL){
		for(i=0;i<strlen(token);i++){
			if(!isNumber(token[i]) && token[i]!=','){
				lexic_ok = false;
				break;
			}
			if(!comma){
				if(token[i]==',')
					comma = true;
				else
					left++;
			}
			else{
				if(token[i]==','){
					lexic_ok = false;
					break;
				}
				else
					right++;
			}
		}
		if(left>10 || right>10){
			lexic_ok = false;
		}
	}

	return lexic_ok;

}

void loadreductions(){
        FILE *file_reductions;
        file_reductions = fopen("Debug/reducoes.txt","r");
        bool falha = false;
        if(file_reductions != NULL){
                char c;
                int number_reduction = 0;
                fseek(file_reductions, 0L, SEEK_END);
                long int file_size = ftell(file_reductions);
                fseek(file_reductions,0,SEEK_SET);
                fscanf(file_reductions,"%c",&c);
                char* str;

                //str = append(NULL,c);

                while(c!=EOF && ftell(file_reductions) < file_size){
                        str = NULL;
                        while (c!='#' && ftell(file_reductions) < file_size){
                                fscanf(file_reductions,"%c",&c);
                        }
                        while (c!='\n' && ftell(file_reductions) < file_size){
                                str = append(str,c);
                                fscanf(file_reductions,"%c",&c);
                                if(c=='\r'){
                                        fscanf(file_reductions,"%c",&c);
                                }
                        }
                        reductions[number_reduction] = str;
                        number_reduction++;
                }
        }else{
                printf("Falha ao importar o arquivo de reduções.");
                exit(-1);
        }


}

int main(int argc, char **argv) {

	if(argc >= 1){
		//printf("%d\n",argc);
		FILE *f;
		f = fopen("Debug/exemplos/exemplo3.c141","r");
		//f = fopen(argv[1],"r");

		/*
		 * Inicio da analise lexica
		 * */
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
						//char prev = c;
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
			for(j=0;j<tokens_counter;j++){
				//printf("%s\n",tokens[j]);
			}
		}
		else{
			lexic_ok = false;
			printf("Arquivo nao encontrado\n");
			exit(1);
		}
		/*Fim da analise lexica
		 * */


		/*Inicio analise sintatica
		 * */
		if(lexic_ok){
			FILE *fv;
			//abre tabela.txt
			fv = fopen("Debug/tabela.txt","r");
			if(fv!=NULL){
				char c;
				int v_counter = 0,i;
				fseek(fv, 0L, SEEK_END);
				long int file_size = ftell(fv);
				fseek(fv,0,SEEK_SET);
				fscanf(fv,"%c",&c);
				char* str;
				str = append(NULL,c);
				//carrega vocabulario
				while(c!='\n'){
					fscanf(fv,"%c",&c);
					if(c == ' '){
						//adiciona str no vocabulario
						//vocabulary[v_counter] = (char*)malloc(sizeof(char));
						vocabulary[v_counter] = str;
						v_counter++;
						str = NULL;
					}
					else{
						str = append(str,c);
					}
				}
				if(c=='\n'){
					//vocabulary[v_counter] = str;
					//v_counter++;
				}
				for(i=0;i<72;i++){
					//printf("%s\n",vocabulary[i]);
				}
				str = NULL;
				fscanf(fv,"%c",&c);
				while(c !=' ')
					fscanf(fv,"%c",&c);//e0
				fscanf(fv,"%c",&c);
				int row = 0, col = 0;

				while(c != EOF && col <= 72 && ftell(fv) < file_size){

					while(c!='\n' && c!= EOF && col <= 72 && ftell(fv) < file_size){
						//printf("%c\n",c);
						//printf("%s ,",str);
						str = append(str,c);
						//printf("%s ,",str);
						fscanf(fv,"%c",&c);
						//printf("c2:%c\n",c);
						while(c!=' '  && ftell(fv) < file_size){
							str = append(str,c);
							fscanf(fv,"%c",&c);
							//printf("c3:%c\n",c);
							//printf("%lu %lu\n",ftell(fv),file_size);

						}
						//printf("%s ,",str);
						while(c==' '){
							fscanf(fv,"%c",&c);
							//printf("c2:%c\n",c);
						}

						if(c=='\r'){
							fscanf(fv,"%c",&c);
							//printf("c2:%c\n",c);
						}
						table[row][col] = str;
						//printf("s:%s ",table[row][col]);
						str = NULL;
						col++;
					}

					if(c=='\n'){
						row++;
						col = 0;
						fscanf(fv,"%c",&c);
						//printf("c2:%c\n",c);
						while(c !=' ' && c!= '\n'){
							fscanf(fv,"%c",&c);//en
							//printf("c4:%c\n",c);
						}

							//printf("\n");
					}
					fscanf(fv,"%c",&c);
					//printf("c1:%c\n",c);
				}
				fscanf(fv,"%c",&c);
				table[149][71] = append(table[149][71],c);
				//printf("%s\n",table[149][71]);
				//printf("fclose\n");
				fclose(fv);

				int j = 0;
				/*
				for(i=0;i<150;i++){
					for(j=0;j<72;j++){
						printf("%s ",table[i][j]);
					}
					j=0;
					printf("\n");
				}
				*/
				
				loadreductions();
				
				states_stack = (Stack*)malloc(sizeof(Stack));
				states_stack->state = 0;
				states_stack->str = NULL;
				states_stack->prev = NULL;

				tokens_stack = (Stack*)malloc(sizeof(Stack));
				tokens_stack->state = 0;
				tokens_stack->str = NULL;
				tokens_stack->prev = NULL;

				push(&tokens_stack,-1,"$");
				for(j=tokens_counter-1;j>=0;j--){
					printf("t:%s\n",tokens[j]);
					push(&tokens_stack,-1,tokens[j]);
				}
				bool accept = false;
				bool error = false;
				Stack *top = get(tokens_stack);
				while (top != NULL){
					printf("print %s\n",top->str);
					top = top->prev;
				}

				/*
				 * Algoritmo sintatico LR
				 * */
				int current_state = 0;
				Stack *current_token = pop(tokens_stack);
				int vocabulary_pos = -1;
				while(!accept && !error){
					char *type;
					if(isReservedWord(current_token->str)){
						vocabulary_pos = posInVocabulary(current_token->str);
						type = current_token->str;
					}
					else if(isTokenNumber(current_token->str)){
						vocabulary_pos = posInVocabulary("CONSTANTE");
						type = "CONSTANTE";
					}
					else if(isIdentifier(current_token->str) && current_token->prev->str[0] == '('){
						vocabulary_pos = posInVocabulary("FUNCAO");
						type = "FUNCAO";
					}
					else if(isIdentifier(current_token->str) && current_token->prev->str[0] != '('){
						vocabulary_pos = posInVocabulary("VARIAVEL");
						type = "VARIAVEL";
					}
					else{
						vocabulary_pos = posInVocabulary(current_token->str);
						type = current_token->str;
					}
					printf("pos:%d\n",vocabulary_pos);
					if(vocabulary_pos!=-1){
						printf("table:%s\n",table[current_state][vocabulary_pos]);
						char* cell = table[current_state][vocabulary_pos];
						if (cell[0] == 'X'){
							error = true;
						}
						else if(cell[0] == 'e'){
							push(&states_stack,current_state,type);
							char* state_name;
							for(j=1;j<strlen(cell);j++){
								state_name = append(state_name,cell[j]);
							}
							current_state = atoi(state_name);
						}
						else if(cell[0] == 'r'){
							char* redu_name;
							for(j=1;j<strlen(cell);j++){
								redu_name = append(redu_name,cell[j]);
							}
							int reduction = atoi(redu_name);

							//TODO: importar tabela de reduções
						}
						else if(cell[0] == 'a'){
							accept = true;
						}
					}
					//TODO: desempilhar token
				}
			}
			else{
				printf("tabela.txt not found\n");
			}



		}
	}

	return 0;
}
