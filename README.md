Compilador
==========
Alfabeto: {
"+","-","*","/",">","<","[","]","=","<>","<<",".",";","@","(",")","{","}",">=","<=","BINARIO","CONTINUAR","E","ENQUANTO","INTEIRO","IMPRIMIR","LER","NAO","OU","PARAR","REAL","RETORNAR","SE","SENAO","SIM", identificador, numero
}


#NUM : numeros = {0 .. 9}
#ID : identificadores = {funcao, variavel}
#OP : operadores = {"+","-","*","/",">","<"} 
#OA : operandos = {variavel, numero}
#DEL : delimitadores = {}


r0: #SS -> #S $
r1: #S -> #S #DEL #S
r2: #S -> #S #DEL
r3: #S -> #OA #OP #OA #DEL
r4: #S -> 
