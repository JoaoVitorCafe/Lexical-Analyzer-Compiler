#ifndef ANALEX
#define ANALEX
#define TAM_MAX_LEXEMA 31

enum TOKEN_CAT
{
    ID = 1,
    PR,
    SN,
    CT_I,
    CT_F,
    CT_C,
    LT,
    COMMENT,
    FIM_ARQ
};

/* Onde: ID: Identificador, SN: Sinal; CT_I: Constante numérica inteira ; LT: cadeia de caracter */

enum PAL_RESERV {CLASS=1, DATA , CODE , INTERN , VOID , CHAR , INT , FLOAT , BOOL , IF , ELSE , WHILE , FOR , RETURN , DELETE , MAIN , NEW};

enum SINAIS 
{
    ATRIB = 1,
    SOMA,
    SUBTRACAO,
    MULTIPLIC,
    DIVISAO,
    COMPARACAO,
    ABRE_PAR,
    FECHA_PAR,
    ABRE_CHAVE,
    FECHA_CHAVE,
    ABRE_COL,
    FECHA_COL,
    NEGACAO,
    DIFERENTE,
    CIRCUN,
    PONTO_VIRG,
    VIRGULA,
    PONTO,
    ESCOPO,
    DOISPONTOS,
    MAIOR,
    MENOR,
    MAIORIGUAL,
    MENORIGUAL,
    AND,
    OR,
    ENDERECO
}; // Sinais válidos da linguagem

typedef struct
{
    enum TOKEN_CAT cat; // deve receber uma das constantes de enum TOKEN_CAT
    union
    {                                // parte variável do registro
        int codigo;                  // para tokens das categorias SN
        int indice;                  // indica o índice da tabela onde o Analex armazenou um literal(cadeira de caracter) para tokens da cat
        char lexema[TAM_MAX_LEXEMA]; // cadeia de caractares que corresponde o nome do token da cat. ID
        int valInt;                  // valor da constante inteira em tokens da cat. CT_I
        float valFloat;              // valor da constante flutuante em tokens da cat. CT_F
        char caracter;               // caracter do token da cat. CT_C
    };
} TOKEN; // Tipo TOKEN

#endif
