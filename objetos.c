/*
  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__

  AO PREENCHER ESSE CABEÇALHO COM O MEU NOME E O MEU NÚMERO USP, 
  DECLARO QUE SOU O ÚNICO AUTOR E RESPONSÁVEL POR ESSE PROGRAMA. 
  TODAS AS PARTES ORIGINAIS DESSE EXERCÍCIO PROGRAMA (EP) FORAM 
  DESENVOLVIDAS E IMPLEMENTADAS POR MIM SEGUINDO AS INSTRUÇÕES DESSE EP
  E QUE PORTANTO NÃO CONSTITUEM PLÁGIO. DECLARO TAMBÉM QUE SOU RESPONSÁVEL
  POR TODAS AS CÓPIAS DESSE PROGRAMA E QUE EU NÃO DISTRIBUI OU FACILITEI A
  SUA DISTRIBUIÇÃO. ESTOU CIENTE QUE OS CASOS DE PLÁGIO SÃO PUNIDOS COM 
  REPROVAÇÃO DIRETA NA DISCIPLINA.

  Nome:

  objetos.c
  Pitao II

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.

  Exemplo:
  - função mallocc retirada de: 

  http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/* interface para esse modulo */
#include "objetos.h"  /* CelObjeto, freeObjeto(), freeListaObjetos() 
                         mostreObjeto(), mostreListaObjetos() */ 

#include <stdlib.h>  /* exit(), malloc(), EXIT_FAILURE */ 
#include <stdio.h>   /* printf(), stdout */
#include "util.h"       /* AVISO(), FILA_ITENS */
#include "categorias.h" /* MAX_CATEGORIAS, OPER_*,  */

/*-----------------------------------------------------------*/
/* OBSERVACAO
 *
 * A ordem  dos strings a  seguir e'  compativel com a  ordem das
 * tabelas  de  operadores  (lexer.c)  e  com  a  enumeracao  das
 * categorias (categorias.h).
 *
 * A tabela a seguir e usada para exibir uma descricao da
 * categoria dos itens lexicos e valores.
 */
/*----------------------------------------------------------*/
/* a 'tabela' operador esta definida em lexer.c             */
extern char *operador[MAX_OPERADORES];

/*----------------------------------------------------------*/
/* a 'tabela' booleans a la Python definida em lexer.c      */
extern char *boolean[2];

static char *nomeCategoria[MAX_CATEGORIAS] =
{
    /* 4 operadores relacionais com 2 simbolos */
    /* "==", 0 */  "operador relacional igual"
    /* "!=", 1 */ ,"operador relacional diferente"
    /* ">=", 2 */ ,"operador relacional maior ou igual"
    /* "<=", 3 */ ,"operador relacional menor ou igual"
 
    /* 2 operadores aritmetico com 2 simbolos */
    /* "**", 4  */ ,"operador aritmetico exponenciacao" 
    /* "//", 5  */ ,"operador aritmetico divisao inteira"
 
    /* 2 operadores relacionais com 1 simbolo */
    /* ">" , 6  */ ,"operador relacional maior"
    /* "<" , 7  */ ,"operador relacional menor"

    /* 6 operadores aritmeticos com 1 simbolo */
    /* "%" , 8  */ ,"operador aritmetico resto de divisao"
    /* "*" , 9  */ ,"operador aritmetico multiplicacao"
    /* "/" ,10  */ ,"operador aritmetico divisao"
    /* "+" ,11  */ ,"operador aritmetico adicao"
    /* "-" ,12  */ ,"operador aritmetico subtracao"
    /* "_" ,13  */ ,"operador aritmetico \"menos unario\""

    /* 3 operadores logicos */
    /* "and",14 */ ,"operador logico e" 
    /* "or",15 */ ,"operador logico ou"
    /* "not" ,16 */ ,"operador logico negacao"

    /* operador de indexacao */
    /* "$" ,17 */ ,"operador indexacao" /*IGNORAR*/

    /* atribuicao */ 
    /* "=" ,18 */  ,"operador de atribuicao"  /*EP4*/

    /* EP4: parenteses */ 
    /* "(" ,19 */  ,"abre parenteses"     /*EP4*/
    /* ")" ,20 */  ,"fecha parenteses"    /*EP4*/

    /* colchetes para listas */
    /* "[" ,21 */  ,"abre colchetes"      /*IGNORAR*/
    /* "]" ,22 */  ,"fecha colchetes"     /*IGNORAR*/

    /* constante */
    /* 23 */      ,"string representando bool"
    /* 24 */      ,"string representando float"
    /* 25 */      ,"string representando int"

    /* Valores */
    /* 26 */      ,"valor str"
    /* 27 */      ,"valor bool"
    /* 28 */      ,"valor float"
    /* 29 */      ,"valor int"

    /* identificador */
    /* 30 */ ,"nome de um identificador"  /*EP4*/
    /* 31 */ ,"indefinida" 
};

/*-------------------------------------------------------------
 * freeObjeto
 *
 * Recebe um ponteiro POBJ para uma celula do tipo CelObjeto 
 * e libera a area alocada por esta celula. 
 *
 * Se o campo valor contem um ponteiro para um string, entao 
 * a area aloca para o string tambem e' liberada. 
 * Isso deve ser feito apenas quando a categoria do objeto for
 * FLOAT_STR, INT_STR, STR (IGNORAR) e ID (EP4!).
 *
 * Se o campo valor contem um ponteiro para um PiObjeto, entao 
 * a area aloca para o PiObjeto tambem e' liberada.
 *
 * ATENCAO:
 * 
 *     Para chamar essa funcao e importante que o campo categoria
 *     da celula tenho sido inicializada apropriadamente.
 * 
 *     Isso e importante para liberar a memoria de objetos que
 *     nao sao inteiramente alocadas na CelObjeto. No caso deste
 *     EP somente strings nao sao inteiramente alocados na
 *     CelObjeto (FLOAT_STR, INT_STR, ...).
 *  
 * EP4: deve liberar os string referentes a variaveis (categoria ID).
 * 
 */
void
freeObjeto(CelObjeto *pObj)
{
    if((pObj->categoria >= 23 && pObj->categoria <= 25) || pObj->categoria == 30){
      free(pObj->valor.pStr);
    }
    free(pObj);
}

/*-------------------------------------------------------------
 * freeListaObjetos(iniLista)
 *
 * Recebe uma lista INILISTA de celulas do tipo CelObjeto e 
 * libera toda a memoria alocada para lista. 
 *
 * Esta funcao deve utilizar a funcao freeObjeto().
 *
 */
void
freeListaObjetos(CelObjeto *iniLista)
{
	CelObjeto *aux;
    aux = iniLista->prox;

	while(aux!=NULL){
		freeObjeto(iniLista);
		iniLista = aux;
		aux = aux->prox;
	}
	freeObjeto(iniLista);
}

/*-------------------------------------------------------------
 * mostreValor
 *
 * Recebe um ponteiro POBJ para uma celula do tipo CelObjeto e
 * imprime o conteudo do seu campo VALOR.
 *
 * PRE-CONDICAO: 
 * 
 *     Aa funcao supoe que o campo valor contem uma constante.
 *     No caso deste EP podemos supor que a categoria de uma
 *     constante e' FLOAT (representada por um double).  Nesse
 *     caso a funcao e praticamente um mero printf.
 *
 * OBSERVACAO:
 * 
 *     Em geral, uma constante poderia ser da categoria FLOAT,
 *     INT (ignorada), BOOL (ignorada) ou STR (ignorada).
 */
void
mostreValor(CelObjeto *pValor)
{
    printf("%lf", pValor->valor.vFloat);
}

/*-------------------------------------------------------------
 * mostreObjeto
 *
 * Recebe um ponteiro POBJ para uma celula do tipo CelObjeto 
 * e um inteiro TIPOLISTA. 
 * 
 * A funcao  imprime o conteudo do  campo VALOR da celula  pOBJ e
 * uma descricao de sua categoria.  A descricao da categoria esta
 * na tabela nomeCategoria (definida acima).
 *
 * O que deve ser impresso depende do valor de TIPOLISTA, que
 * pode ser ITENS ou VALORES (util.h):
 *
 *   - tipoLista == ITENS
 *
 *     O campo valor.pStr da celula contem um string com um 
 *     item lexico.
 *
 *     Exemplos de impressoes feitas pela funcao nesse caso:
 *
 *         "2.0"  (string representando float)
 *         "+"  (operador aritmetico adicao)
 *         "**"  (operador aritmetico exponenciacao)
 *
 *   - tipoLista == VALORES
 *      
 *     Se a categoria for de um OPER_*, o campo valor.vInt contem
 *     um inteiro representando a precedencia (eval.c) do
 *     operador.
 *
 *     Se a categoria for FLOAT, o campo valor.vFloat contem 
 *     um double.
 *      
 *     Exemplos de impressoes feitas pela funcao nesse caso: 
 *
 *         2.000000  (valor float)
 *         3.140000  (valor float)
 *         4.000000  (valor float)
 *         prec=6  (operador aritmetico adicao)
 *         prec=8  (operador aritmetico exponenciacao)
 *
 * EP4:- tipoLista == POSFIXA 
 *      
 *     Se a categoria for de um OPER_*, o campo categoria contem o 
 *     indice na tabela OPERADOR onde esta o string que representa
 *     o operador que devera ser impresso.
 *
 *     Se a categoria for FLOAT, o campo valor.vFloat contem 
 *     um double que devera ser impresso
 *      
 *     Se a categoria for ID, o campo valor.pStr contem 
 *     o nome da variavel que devera ser impresso.
 *
 *     Exemplos de impressoes feitas pela funcao nesse caso: 
 *
 *     >>> mp = (p1 + 2*p2 + 2*p3)/3
 *     Expressao posfixa: mp p1 2 p2 * + 2 p3 * + 3 / = 
 *
 *     Esta funcao imprime apenas um item por vez. Assim, no exemplo
 *     acima foi feira uma chamada para imprimir 'mp' outra 
 *     para imprimir 'p1' outra para imprimir '2' outra para 
 *     imprimir 'p2',...
 *
 * A descricao da categoria da celula esta em  
 * 
 *     nomeCategoria[pObj->categoria])
 * 
 * OBSERVACAO. 
 * 
 * Em geral, essa funcao consideraria outras categorias que estao
 * sendo ignoradas no EP.
 *
 */
void
mostreObjeto(CelObjeto *pObj, int tipoLista)
{
    if(tipoLista == ITENS){
      printf(" '%s'  (%s)\n", pObj->valor.pStr, nomeCategoria[pObj->categoria]);
    }
    if(tipoLista == VALORES){
      if(pObj->categoria == 29 || pObj->categoria == 28)
        printf(" %lf  (%s)\n", pObj->valor.vFloat, nomeCategoria[pObj->categoria]);
      
      else if(pObj->categoria <= 22)
        printf(" prec=%d  (%s)\n", pObj->valor.vInt, nomeCategoria[pObj->categoria]);
    }
    if(tipoLista == POSFIXA){
        if(pObj->categoria <=18)
            printf(" %s", operador[pObj->categoria]);
        else if(pObj->categoria == 28)
            printf(" %lf", pObj->valor.vFloat);
        else if(pObj->categoria == 30)
            printf(" %s", pObj->valor.pStr);
        
    }
}

/*-------------------------------------------------------------
 *  mostreListaObjetos
 *
 * Recebe uma lista ligada com cabeca INILISTA de celulas do tipo
 * CelObjeto e um inteiro TIPOLISTA.
 *
 * A funcao imprime um cabecalho e o conteudo do campo VALOR de
 * cada celula da lista, junto com uma descricao desse valor.
 *
 * O parametro TIPOLISTA indicado o conteudo das celulas.
 * Temos a seguinte relacao "TIPOLISTA x conteudo da celula"
 *
 *   tipo          categoria     campo VALOR contem
 *   ----------------------------------------------  
 *   ITENS         todas         string 
 *   VALORES       OPER_*        int representando a precedencia   
 *   VALORES       FLOAT         double  
 *   VALORES       INT           int  (IGNORAR)
 *   VALORES       BOOL          int representando True e False (IGNORAR)
 *   VALORES       STR           string (IGNORAR)
 *
 * Exemplos de impressao da funcao para TIPOLISTA == ITENS
 * 
 *     ==========================
 *      Fila de itens lexicos  
 *      item  (categoria)
 *     . . . . . . . . . . . . .
 *     "True"  (string representando bool)
 *     "1"  (string representando int)
 *     "+"  (operador aritmetico adicao)
 *     "2.0"  (string representando float)
 * 
 * Exemplos de impressao da funcao para TIPOLISTA == VALORES
 *
 *     ==========================
 *       Fila de valores   
 *       valor  (categoria)
 *     . . . . . . . . . . . . . .
 *     2.000000  (valor float)
 *     3.140000  (valor float)
 *     4.000000  (valor float)
 *     prec=6  (operador aritmetico adicao)
 *     prec=8  (operador aritmetico exponenciacao)
 *
 * EP4:
 * Exemplos do resultado de duas chamada da funcao com TIPOLISTA == POSFIXA
 *
 *     >>> mp = (p1 + 2*p2 + 2*p3)/3
 *     Expressao posfixa: mp p1 2 p2 * + 2 p3 * + 3 / = 
 *
 *     >>> p1 = (p2 =3 * p3) + 5
 *     Expressao posfixa: p1 p2 3 p3 * = 5 + = 
 *     
 */ 
void 
mostreListaObjetos (CelObjeto *iniLista, int tipoLista)
{
	CelObjeto *aux;
	aux = iniLista;

	printf("\n==========================");

	if(tipoLista == ITENS){
		printf("\n   Fila de itens lexicos");
		printf("\n   item   (categoria");
		printf("\n. . . . . . . . . . . . . .\n");
	}
	if(tipoLista == VALORES){
		printf("\n   Fila de valores");
		printf("\n   valor   (categoria");
		printf("\n. . . . . . . . . . . . . .\n");	
	}
	if(tipoLista == POSFIXA){
        printf("\n   Expressao porsfixa:");
    }
	while(aux != NULL){
			mostreObjeto(aux, tipoLista);
			aux = aux->prox;			
	}
	free(aux);
}