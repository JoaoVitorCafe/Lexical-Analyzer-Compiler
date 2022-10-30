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

int findReservadas(char reservadas[][20] , char string[]){
    for(int i = 0 ; i < 20 ; i++){
        if(strcmp(reservadas[i] , string) == 0){
          return i;
        }
    }

    return -1;
}


TOKEN AnaLex(FILE *fd) { 

char reservadas[20][20] = {"class" , "data" , "code" , "intern" , "void" , "char", "int", "float" , "bool" , "if" , "else" , "while" , "for" , "return" , "delete" , "main" , "new"};
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

            else if (c == '\"') {  // Início de uma string(literal)
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
            }

            else if(c == '<'){ 
                estado = 23;
            }

            else if(c == '{'){ 
              estado = 26;
              t.cat = SN; 
              t.codigo = ABRE_CHAVE; 
              return t;
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
              t.codigo = ABRE_CHAVE; 
              return t;
            }

            else if (c == '&') {   
              estado = 32; 
            } 

            else if (c == '|') {   
              estado = 34; ;
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
              t.codigo = VIRGULA; 
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
            } 

            else if (c == '=') {   
              estado = 17; 
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

          if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')  || (c >= '0' && c <= '9') || (c == '_')) {   
              estado = 1; 
              lexema[tamL] = c;   // acumula caracteres lidos em lexema 
              lexema[++tamL] = '\0'; 
          }

          else {           // transicao OUTRO* do estado 1 do AFD 
              estado = 2;      // monta token identificador e retorna 
              ungetc(c, fd); 
              
              if((findReservadas(reservadas , lexema) != -1)){
                t.cat = PR;
              }  else {
                t.cat = ID;
              }
               
              strcpy(t.lexema, lexema); 
              return t; 
          } 

          break; 

        case 3:

          if (c >= '0' && c <= '9') { 
              estado = 3; 
              digitos[tamD] = c;    // acumula digitos lidos na variaavel digitos 
              digitos[++tamD] = '\0'; 
            }

          else if (c == '.'){
            estado = 4;
            digitos[tamD] = c;    // acumula digitos lidos na variaavel digitos 
            digitos[++tamD] = '\0';
          } 

          else {            // transicao OUTRO* do estado 10 do AFD 
              estado = 7;       // monta token constante inteira e retorna 
              ungetc(c, fd); 
              t.cat = CT_I; 
              t.valInt = atoi(digitos); 
              return t; 
            }

          break;

        case 4:

          if (c >= '0' && c <= '9') { 
              estado = 8; 
              digitos[tamD] = c;    // acumula digitos lidos na variaavel digitos 
              digitos[++tamD] = '\0'; 
          } 

          else {
            estado = 0;
            tamD = 0;
            digitos[tamD] = '\0'; // limpa os digitos           
            error("Caracter invalido na expressao!");
            ungetc(c, fd);
          }

          break;

        case 8:
          
          if (c >= '0' && c <= '9') { 
              estado = 8; 
              digitos[tamD] = c;    // acumula digitos lidos na variaavel digitos 
              digitos[++tamD] = '\0'; 
            }

          else {
            estado = 9;       // monta token constante inteira e retorna  
            t.cat = CT_F; 
            t.valFloat = atof(digitos); 
            ungetc(c, fd);
            return t; 
          }

          break;

        case 6:
          
          if((isprint(c) != 0) && (c != '\"') && (c!= '\n')){
            estado = 6;
            lexema[tamL] = c;    
            lexema[++tamL] = '\0'; 
          }

          else if(c == '\"'){
            estado = 10;
            lexema[tamL] = c;    
            lexema[++tamL] = '\0';
            t.cat = LT; 
            strcpy(t.lexema, lexema); 
            return t; 
          } 

          else {
            estado = 0;
            tamL = 0;
            lexema[tamL] = '\0'; // limpa o lexema
            error("Caracter invalido na expressao!");
            ungetc(c, fd);
          }
          
          break;

        case 11:
          if((isprint(c) != 0) && (c != '\\') && (c != '\'')){
            estado = 12;
            lexema[tamL] = c;    
            lexema[++tamL] = '\0'; 
          }

          else if(c == '\\'){
            estado = 43;
            lexema[tamL] = c;    
            lexema[++tamL] = '\0';
          }

          else if(c == '\''){
            estado = 13;
            lexema[tamL] = c;    
            lexema[++tamL] = '\0';
            t.cat = CT_C; 
            strcpy(t.lexema, lexema);
            return t; 
          }

          else {
            estado = 0;
            tamL = 0;
            lexema[tamL] = '\0'; // limpa o lexema
            error("Caracter invalido na expressao!");
            ungetc(c, fd);
          }

          break;

        case 12:

          if(c == '\''){
            estado = 13;
            lexema[tamL] = c;     
            lexema[++tamL] = '\0';
            t.cat = CT_C; 
            strcpy(t.lexema, lexema); 
            return t; 
          }

          else {
            estado = 0;
            tamL = 0;
            lexema[tamL] = '\0'; // limpa o lexema
            error("Caracter invalido na expressao!");
            ungetc(c, fd);
          }

          break;
        
        case 43:

          if(c == 'n'){
            estado = 44;
            lexema[tamL] = c;    
            lexema[++tamL] = '\0';
          }

          else if(c == '0'){
            estado = 45;
            lexema[tamL] = c;    
            lexema[++tamL] = '\0';
          }

          else {
            estado = 0;
            tamL = 0;
            lexema[tamL] = '\0'; // limpa o lexema
            error("Caracter invalido na expressao!");
            ungetc(c, fd);
          }

          break;
        
        case 44:
          
          if(c == '\''){
            estado = 13;
            lexema[tamL] = c;     
            lexema[++tamL] = '\0';
            t.cat = CT_C; 
            strcpy(t.lexema, lexema); 
            return t; 
          }

          else {
            estado = 0;
            tamL = 0;
            lexema[tamL] = '\0'; // limpa o lexema
            error("Caracter invalido na expressao!");
            ungetc(c, fd);
          }

          break;

        case 45:
          
          if(c == '\''){
            estado = 13;
            lexema[tamL] = c;   
            lexema[++tamL] = '\0';
            t.cat = CT_C; 
            strcpy(t.lexema, lexema); 
            return t; 
          }

          else {
            estado = 0;
            tamL = 0;
            lexema[tamL] = '\0'; // limpa o lexema
            error("Caracter invalido na expressao!");
            ungetc(c, fd);
          }

          break;

        case 20:
          
          if(c == '='){
            estado = 21;
            t.cat = SN; 
            t.codigo = MAIORIGUAL;
            
            return t; 
          }

          else {
            estado = 22;
            ungetc(c, fd); 
            t.cat = SN; 
            t.codigo = MAIOR ;
            return t; 
          }

          break;

        case 23:
          
          if(c == '='){
            estado = 24;
            t.cat = SN; 
            t.codigo = MENORIGUAL;
            return t; 
          }

          else {
            estado = 25;
            ungetc(c, fd); 
            t.cat = SN; 
            t.codigo = MENOR ;
            return t; 
          }

          break;

        case 32:

          if(c == '&'){
            estado == 33;
            t.cat = SN;
            t.codigo = AND;
            return t; 
          }

          else {
            estado = 49;
            ungetc(c, fd); 
            t.cat = SN; 
            t.codigo = ENDERECO;
            return t; 
          }

          break;

        case 34:

          if(c == '|'){
            estado = 35;
            t.cat = SN; 
            t.codigo = OR;
            return t; 
          }

          
          else {
            error("Caracter invalido na expressao!");
            estado = 0;
            ungetc(c , fd); 
          }
          

          break; 

        case 51:
          
          if(c == ':'){
            estado = 52;
            t.cat = SN; 
            t.codigo = ESCOPO;
            return t; 
          }

          else {
            estado = 53;
            ungetc(c, fd); 
            t.cat = SN; 
            t.codigo = DOISPONTOS;
            return t;
          }

          break;

        case 14 :

          if(c == '='){
            estado = 16;
            t.cat = SN; 
            t.codigo = DIFERENTE;
            return t; 
          }

          else {
            estado = 15;
            ungetc(c, fd); 
            t.cat = SN; 
            t.codigo = NEGACAO;
            return t;
          }

          break;

        case 17:

          if(c == '='){
            estado = 18;
            t.cat = SN; 
            t.codigo = COMPARACAO;
            return t; 
          }

          else {
            estado = 19;
            ungetc(c, fd); 
            t.cat = SN; 
            t.codigo = ATRIB;
            return t;
          }

          break;
          
        case 30:
          
          if(c == '*'){
            estado = 38;
            lexema[tamL] = c; 
            lexema[++tamL] = '\0';
          }

          else {
            estado = 37;
            ungetc(c, fd); 
            t.cat = SN;
            t.codigo = DIVISAO;
            return t;
          }

          break;

        case 38:
          if((isprint(c) != 0) && c != '*'){
            estado =  38;
            lexema[tamL] = c;    
            lexema[++tamL] = '\0'; 
          }

          else if(c == '*'){
            estado = 39;
            lexema[tamL] = c;    
            lexema[++tamL] = '\0';
          }

          else {
            estado = 0;
            tamL = 0;
            lexema[tamL] = '\0'; // limpa o lexema
            error("Caracter invalido na expressao!");
            ungetc(c, fd); 
          }

          break;

        case 39:
          if(c == '/'){
            estado = 40;
            lexema[tamL] = c;    
            lexema[++tamL] = '\0';
            strcpy(t.lexema, lexema);
            t.cat = COMMENT;
            return t;
          }

          else if(c == '*'){
            estado = 39;
            lexema[tamL] = c;    
            lexema[++tamL] = '\0';
          }

          else {
            estado = 0;
            tamL = 0;
            lexema[tamL] = '\0'; // limpa o lexema
            error("Caracter invalido na expressao!");
            ungetc(c, fd); 
          }
          
          break;
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

        case LT:
            printf("<LT, %s> ", tk.lexema); 
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

                case COMPARACAO: 
                  printf("<SN, COMPARACAO> "); 
                  break; 

                case ABRE_CHAVE: 
                  printf("<SN, ABRE_CHAVE> "); 
                  break; 

                case FECHA_CHAVE: 
                  printf("<SN, FECHA_CHAVE> "); 
                  break; 

                case ABRE_COL: 
                  printf("<SN, ABRE_COL> "); 
                  break; 

                case FECHA_COL: 
                  printf("<SN, FECHA_COL> "); 
                  break;

                case NEGACAO: 
                  printf("<SN, NEGACAO> "); 
                  break;

                case DIFERENTE:
                  printf("<SN, DIFERENTE> "); 
                  break;

                case CIRCUN:
                  printf("<SN, CIRCUN> "); 
                  break;

                case PONTO_VIRG:
                  printf("<SN, PONTO_VIG> "); 
                  break;

                case VIRGULA:
                  printf("<SN, VIRGULA> "); 
                  break;
                
                case PONTO:
                  printf("<SN, PONTO> "); 
                  break;

                case ESCOPO:
                  printf("<SN, ESCOPO> "); 
                  break;

                case DOISPONTOS:
                  printf("<SN, DOISPONTOS> "); 
                  break;

                case MAIOR:
                  printf("<SN, MAIOR> "); 
                  break;
                
                case MENOR:
                  printf("<SN, MENOR> "); 
                  break;
                
                case MAIORIGUAL:
                  printf("<SN, MAIORIGUAL> "); 
                  break;
                
                case MENORIGUAL:
                  printf("<SN, MENORIGUAL> "); 
                  break;
                
                case AND:
                  printf("<SN, AND> "); 
                  break;

                case OR:
                  printf("<SN, OR> "); 
                  break;

                case ENDERECO:
                  printf("<SN, ENDERECO> "); 
                  break;
            } 

            break; 

        case CT_I: 
            printf("<CT_I, %d> ", tk.valInt); 
            break;

        case CT_F:
            printf("<CT_F, %f> ", tk.valFloat);
            break;

        case CT_C:
            printf("<CT_C, %s> ", tk.lexema);
            break;

        case COMMENT:
            printf("<COMMENT, %s> ", tk.lexema);
            break;
        
        case PR:
            printf("<PALAVRA RESERVADA, %s> ", tk.lexema);
            break;

        case FIM_ARQ: 
            printf("\nFim da expressao encontrado.\n"); 
            break;
    } 
    
    if (tk.cat == FIM_ARQ) 
        break; 
  }

  fclose(fd); 
  return 0; 

}