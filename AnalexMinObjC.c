#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h> 
#include "AnalexMinObjC.h"

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

            else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) { // inicio de ID - inicializa lexema 
                estado = 1; 
                lexema[tamL] = c; 
                lexema[++tamL] = '\0'; 
            }

            else if (c >= '0' && c <= '9') { // inicio de constante inteira - inicializa digitos 
              estado = 3; 
              digitos[tamD] = c;  
              digitos[++tamD] = '\0';     
            } 

            else if (c == '"') {  // Início de uma string(literal)
              estado = 6; 
              lexema[tamL] = c; 
              lexema[++tamL] = '\0'; 
            } 

            else if(c == '\''){ // aspas simples
              estado = 11;
              lexema[tamL] = c; 
              lexema[++tamL] = '\0'; 
            }

            else if(c == '>'){ 
                estado = 20;
                lexema[tamL] = c; 
                lexema[++tamL] = '\0';
            }

            else if(c == '<'){ 
                estado = 23;
                lexema[tamL] = c; 
                lexema[++tamL] = '\0';
            }

            else if(c == '{'){ 
                estado = 26;
                t.cat = SN; 
                t.codigo = FECHA_CHAVE; 
            }

             else if(c == '}'){ 
              estado = 27;
              t.cat = SN; 
              t.codigo = FECHA_CHAVE; 
              return t;
            }

             else if(c == '{'){ 
              estado = 26;
              t.cat = SN; 
              t.codigo = FECHA_CHAVE; 
              return t;
            }

            else if (c == '&') {   
              estado = 32; 
              lexema[tamL] = c; 
              lexema[++tamL] = '\0';
            } 

            else if (c == '|') {   
              estado = 34; 
              lexema[tamL] = c; 
              lexema[++tamL] = '\0';
            } 

            else if (c == '(') { 
              estado = 42; 
              t.cat = SN; 
              t.codigo = ABRE_PAR; 
              return t; 
            } 

              else if (c == ')') {   
              estado = 41; 
              t.cat = SN; 
              t.codigo = FECHA_PAR; 
              return t; 
            } 

             else if (c == ':') {   
              estado = 51; 
              lexema[tamL] = c; 
              lexema[++tamL] = '\0';
            } 

             else if (c == '.') {   
              estado = 50; 
              t.cat = SN; 
              t.codigo = PONTO; 
              return t;
            } 

            else if (c == ',') {   
              estado = 48; 
              t.cat = SN; 
              t.codigo = PONTO; 
              return t;
            } 

             else if (c == ';') {   
              estado = 36; 
              t.cat = SN; 
              t.codigo = PONTO_VIRG; 
              return t;
            }

            else if (c == '[') {   
              estado = 46; 
              t.cat = SN; 
              t.codigo = ABRE_COL; 
              return t; 
            } 

            else if (c == ']') {   
              estado = 47; 
              t.cat = SN; 
              t.codigo = FECHA_COL; 
              return t; 
            } 

            else if (c == '^') {   
              estado = 5; 
              t.cat = SN; 
              t.codigo = CIRCUN; 
              return t; 
            } 

            else if (c == '/') {  
              estado = 30; 
              lexema[tamL] = c; 
              lexema[++tamL] = '\0';
            } 

            else if (c == '*') {  
              estado = 31; 
              t.cat = SN; 
              t.codigo = MULTIPLIC; 
              return t; 
            } 

            else if (c == '-') {   
              estado = 29; 
              t.cat = SN; 
              t.codigo = SUBTRACAO; 
              return t; 
            } 

            else if (c == '+') {   
              estado = 29; 
              t.cat = SN; 
              t.codigo = SOMA; 
              return t; 
            } 

             else if (c == '!') {   
              estado = 14; 
              t.cat = SN; 
              t.codigo = NEGACAO; 
              return t; 
            } 

             else if (c == '=') {   
              estado = 18; 
              t.cat = SN; 
              t.codigo = COMPARA; 
              return t; 
            } 

            else if (c == '=') {   
              estado = 18; 
              lexema[tamL] = c; 
              lexema[++tamL] = '\0'; 
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
                case SOMA: 
                    printf("<SN, SOMA> "); 
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