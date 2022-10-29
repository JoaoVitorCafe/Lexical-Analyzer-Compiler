#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h> 
#include "AnalexCalcula.h" 
#define TAM_LEXEMA 50 
#define TAM_NUM 20 

void error(char msg[]) { 
    printf("%s\n", msg); 
    exit(1); 
} 

TOKEN AnaLex(FILE *fd) { 

int estado; 

char lexema[TAM_LEXEMA] = ""; 
int tamL = 0; 
char digitos[TAM_NUM] = ""; 
int tamD = 0; 
 
TOKEN t; 

estado = 0; 

while (1) { 

    char c = fgetc(fd); 
    switch (estado) { 
        case 0: 
        
            if (c == ' ' || c == '\t'){
                estado = 0; 
            }

            else if (c >= 'a' && c <= 'z') { // inicio de identificador - inicializa lexema 
                estado = 1; 
                lexema[tamL] = c; 
                lexema[++tamL] = '\0'; 
            }

            else if (c >= '1' && c <= '9') { // inicio de constante inteira - inicializa digitos 
              estado = 10; 
              digitos[tamD] = c;  
              digitos[++tamD] = '\0';     
            } 

            else if (c == '+') {  // sinal de adicao - monta e devolve token 
              estado = 3; 
              t.cat = SN; 
              t.codigo = ADICAO; 
              return t; 
            } 

            else if (c == '-') {  // sinal de subtracao - monta e devolve token 
              estado = 4; 
              t.cat = SN; 
              t.codigo = SUBTRACAO; 
              return t; 
            } 

            else if (c == '*') {  // sinal de multiplicacao - monta e devolve token 
              estado = 6; 
              t.cat = SN; 
              t.codigo = MULTIPLIC; 
              return t; 
            } 

            else if (c == '/') {  // sinal de divisao - monta e devolve token 
              estado = 5; 
              t.cat = SN; 
              t.codigo = DIVISAO; 
              return t; 
            } 

            else if (c == '=') {  // sinal de atribuicao - monta e devolve token 
              estado = 7; 
              t.cat = SN; 
              t.codigo = ATRIB; 
              return t; 
            } 

            else if (c == '(') {  // sinal de abre parenteses - monta e devolve token 
              estado = 8; 
              t.cat = SN; 
              t.codigo = ABRE_PAR; 
              return t; 
            } 

            else if (c == ')') {  // sinal de fecha parenteses - monta e devolve token 
              estado = 9; 
              t.cat = SN; 
              t.codigo = FECHA_PAR; 
              return t; 
            } 
            else if (c == EOF) {  // fim da expressao encontrado 
               t.cat = FIM_ARQ; 
               return t; 
            } 

            else{
              error("Caracter invalido na expressao!");  // sem transicao valida no AFD 
            }  

            break; 

        case 1: 

          if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c == '_')) {   
              estado = 1; 
              lexema[tamL] = c;   // acumula caracteres lidos em lexema 
              lexema[++tamL] = '\0'; 
            } 

          else {           // transicao OUTRO* do estado 1 do AFD 
              estado = 2;      // monta token identificador e retorna 
              ungetc(c, fd); 
              t.cat = ID; 
              strcpy(t.lexema, lexema); 
              return t; 
            } 

          break; 

        case 10:

          if (c >= '0' && c <= '9') { 
              estado = 10; 
              digitos[tamD] = c;    // acumula digitos lidos na variaavel digitos 
              digitos[++tamD] = '\0'; 
            } 

            else {            // transicao OUTRO* do estado 10 do AFD 
              estado = 11;       // monta token constante inteira e retorna 
              ungetc(c, fd); 
              t.cat = CT_I; 
              t.valInt = atoi(digitos); 
              return t; 
            } 
        }           
    } 
} 

int main() { 

FILE *fd; 
TOKEN tk; 

if ((fd=fopen("expressao.dat", "r")) == NULL) 
    error("Arquivo de entrada da expressao nao encontrado!"); 

while (1) { 
    tk = AnaLex(fd); 
    switch (tk.cat) { 
        case ID: 
            printf("<ID, %s> ", tk.lexema); 
            break; 
        case SN: 
            switch (tk.codigo) { 
                case ADICAO: 
                    printf("<SN, ADICAO> "); 
                    break; 
        
                case SUBTRACAO: 
                    printf("<SN, SUBTRACAO> "); 
                    break;

                case MULTIPLIC: 
                    printf("<SN, MULTIPLICACAO> "); 
                    break;

                case DIVISAO: 
                    printf("<SN, DIVISAO> "); 
                    break; 

                case ATRIB: 
                    printf("<SN, ATRIBUICAO> "); 
                    break; 

                case ABRE_PAR: 
                    printf("<SN, ABRE_PARENTESES> "); 
                    break; 

                case FECHA_PAR: 
                    printf("<SN, FECHA_PARENTESES> "); 
                    break; 
            } 

            break; 

        case CT_I: 
            printf("<CT_I, %d> ", tk.valInt); 
            break; 

        case FIM_ARQ: 
            printf("\nFim da expressao encontrado.\n"); 
    } 
    
    if (tk.cat == FIM_ARQ) 
        break; 
  }

  fclose(fd); 
  return 0; 

} 