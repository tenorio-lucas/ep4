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

  eval.c
  Pitao II

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:
  - função mallocc retirada de: http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/*
  NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ESQUELETO
*/

/*------------------------------------------------------------*/
/* interface para as funcoes deste modulo */
#include "eval.h" 

/*------------------------------------------------------------*/
#include <stdlib.h>  /* atoi(), atof(), strtol() */
#include <string.h>  /* strncmp(), strlen(), strncpy(), strcpy(), strcmp() */
#include <math.h>    /* pow() */

/*------------------------------------------------------------*/
#include "categorias.h" /* Categoria, MAX_OPERADORES */
#include "util.h"       /* ERRO(), AVISO(), mallocSafe() */
#include "objetos.h"    /* CelObjeto, mostreObjeto(), freeObjeto()
                           mostreListaObjetos(), 
                           freeListaObjetos() */
#include "stack.h"      /* funcoes que manipulam uma pilha */ 
                        /* essa pilha sera usada para implementar 
                           a pilha de execucao */
#include "st.h"         /* getValorST(), setValorST() */    
    

/*------------------------------------------------------------*/
/* Protipos de funcoes auxiliares */

/*------------------------------------------------------------*/
/* Tabela com uma representacao da precedencia dos operadores
 * atraves de valores inteiros. 
 * Quanto maior o valor, maior o valor, maior a precedencia.
 *
 *  http://www.ime.usp.br/~pf/algoritmos/apend/precedence.html
 */
static const int precedencia[MAX_OPERADORES] =
{
    /* 4 operadores relacionais com 2 simbolos  */
    4   /* "==" */ /* da esquerda para a direita */
    ,4   /* "!=" */ /* da esquerda para a direita */
    ,5   /* ">=" */ /* da esquerda para a direita */
    ,5   /* "<=" */ /* da esquerda para a direita */
         
    /* 2 operadores aritmeticos com 2 simbolos */
    ,8 /* ,"**" */ /* da direita para a esquerda */
    ,7 /* ,"//" */ /* da esquerda para a direita */
 
    /* 2 operadores relacionais com 1 simbolo */
    ,5  /* ">"  */ /* da esquerda para a direita */
    ,5  /* "<"  */ /* da esquerda para a direita */ 
    
    /* 6 operadores aritmeticos */
    ,7  /* "%" */ /* da esquerda para a direita */
    ,7  /* "*" */ /* da esquerda para a direita */
    ,7  /* "/" */ /* da esquerda para a direita */
    ,6  /* "+" */ /* da esquerda para a direita */
    ,6  /* "-" */ /* da esquerda para a direita */
    ,8  /* "_" */ /* da direita para a esquerda */
    
    /* 3 operadores logicos  */
    ,3  /* "and" */ /* da esquerda para a direita */ 
    ,2  /* "or" */ /* da esquerda para a direita */
    ,8  /* "not"  */ /* da direita para a esquerda */
    
    /* operador de indexacao */
    ,9  /* "$"  */ /* da esquerda para a direita (IGNORAR)*/

    /* atribuicao */ 
    ,1  /* "=" */ /* da direita para a esquerda EP4 */ 
}; 

/*-------------------------------------------------------------
 *  itensParaValores
 *  
 *  RECEBE uma lista ligada com cabeca INIFILAITENS representando uma
 *  fila de itens lexicos. Inicialmente, o campo VALOR de cada celula
 *  contem um string com o item lexico.
 *
 *  A funca CONVERTE o campo VALOR de cada celula para um inteiro 
 *  ou double, como descrito a seguir. 
 *
 *  IMPLEMENTACAO
 *
 *  O campo VALOR de uma CelObjeto tem 3 subcampos:
 *
 *      - valor.vInt   (int)
 *      - valor.vFloat (float)
 *      - valor.pStr   (string)
 *
 *  Nessa conversao, o campo VALOR de cada celula recebe um valor 
 *  da seguinte maneira.
 *
 *     - Se o campo CATEGORIA da celula indica que o item e um
 *       string representando um int (INT_STR) entao o campo 
 *       valor.vFloat deve receber esse numero inteiro. 
 *
 *       Nesse caso a CATEGORIA do item deve ser alterada para 
 *       FLOAT.
 *
 *     - se o campo CATEGORIA da celula indica que o item e um
 *       string representando um float (FLOAT_STR) entao o campo 
 *       valor.vFloat deve receber esse float;
 *
 *       Nesse caso a CATEGORIA do item deve ser alterada para 
 *       FLOAT.
 *
 *     - se o campo CATEGORIA da celula indica que o item e um
 *       string representando um Bool (BOOL_STR) entao o campo 
 *       valor.vFloat deve receber o inteiro correspondente 
 *       (False = 0, True = 1);
 *
 *       Nesse caso a CATEGORIA do item deve ser alterada para 
 *       FLOAT.
 *
 *     - se o campo CATEGORIA da celula indica que o item e um
 *       string representando um operador (OPER_*) entao o campo 
 *       valor.vInt deve receber o inteiro correspondente 
 *       a precedencia desse operador. 
 *       
 *       Para isto utilize o vetor PRECEDENCIA declarado antes 
 *       desta funcao. 
 *
 * Nesta funcao (e nas outras) voce pode utilizar qualquer funcao da
 * biblioteca string do C, como, por exemplo, atoi(), atof().
 *
 * Esta funcao (e todas as outras) devem 'dar' free nas estruturas que
 * deixarem de ser necessarias.
 *
 * Esse e o caso das dos strings dos itens das categorias 
 * FLOAT_STR e INT_STR.  
 *
 * Os campos strings de objetos OPER_* e BOOL_STR apontam para
 * strings em tabelas definidas no modulo lexer.h. Nesse
 * caso, tentar liberar essa memoria e' um erro.
 *
 */

void
itensParaValores(CelObjeto *iniFilaItens)
{
  CelObjeto *aux;
  aux = iniFilaItens;

  while(aux != NULL){
    if(aux->categoria == 25){
      aux->categoria = 28;
      aux->valor.vFloat = atof(aux->valor.pStr);
    }
    if(aux->categoria == 24){
      aux->categoria = 28;
      aux->valor.vFloat = atof(aux->valor.pStr);
    }
    if(aux->categoria == 23){
      aux->categoria = 28;
      aux->valor.vFloat = atof(aux->valor.pStr);
    }
    if(aux->categoria < 19){
      aux->valor.vInt = atoi(aux->valor.pStr);
    }
    aux = aux->prox;
  }
}

/*-------------------------------------------------------------
 * eval
 * 
 * RECEBE uma lista ligada como cabeca INIPOSFIXA que representa
 * uma expressao em notacao posfixa. 
 *
 * RETORNA o endereco de uma celula com o valor da expressao.
 *
 * PRE-CONDICAO: a funcao supoe que a expressao esta sintaticamente
 *               correta.
 *
 * IMPLEMENTACAO
 *
 * Para o calculo do valor da expressao deve ser usada uma 
 * pilha. O endereco retornado sera o da celula no topo no
 * topo dessa  "pilha de execucao".
 * 
 * A funcao percorre a expressao calculando os valores resultantes.
 * Para isto e utilizada uma pilha de execucao. 
 * 
 * A implementacao das funcoes que manipulam a pilham devem ser
 * escritas no modulo stack.c.
 * 
 * O arquivo stack.h contem a interface para essas funcoes. 
 * A pilha de execucao so deve ser acessada atraves dessa interface
 * (em caso contrario nao chamariamos stack.h de interface).
 * 
 * O parametro mostrePilhaExecucao indica se os valores
 * na pilha de execucao devem ser exibidos depois de qualquer 
 * alteracao.
 * 
 * Esta funcao deve "dar free" nas estruturas que deixarem de ser 
 * necessarias.
 *
 * EP4: funcao deve ser adaptada para tratar do operador de atribuicao
 *      '=' e variaveis. A maneira de proceder esta descrita no 
 *      enunciado na secao "Interpretacao da expressao posfixa".
 */
CelObjeto *
eval (CelObjeto *iniPosfixa, Bool mostrePilhaExecucao)
{
  int a, valor_aux;
  Item item, valor1, valor2;
  CelObjeto *aux, *pilha;
  aux = iniPosfixa->prox;
  
  pilha = stackInit();
  
  while(aux != NULL){
    item.valor.vFloat = aux->valor.vFloat;
    item.valor.vInt = aux->valor.vInt;
    item.valor.pStr = aux->valor.pStr;
    item.categoria = aux->categoria;
    a = aux->categoria;


    switch (a){
      case 28:
      stackPush(pilha, item);
      break;
      case 0:
          valor1 = stackPop(pilha);
        valor2 = stackPop(pilha);
        if(valor1.valor.vFloat - valor2.valor.vFloat == 0)
          item.valor.vFloat = TRUE;
        else
          item.valor.vFloat = FALSE;
        stackPush(pilha, item);
      break;
      case 1:
          valor1 = stackPop(pilha);
          valor2 = stackPop(pilha);
        if(valor1.valor.vFloat - valor2.valor.vFloat != 0)
          item.valor.vFloat = TRUE;
        else
          item.valor.vFloat = FALSE;
        stackPush(pilha, item);
      break;
      case 2:
        valor1 = stackPop(pilha);
        valor2 = stackPop(pilha);
        if(valor1.valor.vFloat - valor2.valor.vFloat <= 0)
          item.valor.vFloat = TRUE;
        else
          item.valor.vFloat = FALSE;
        stackPush(pilha, item);
      break;
      case 3:
        valor1 = stackPop(pilha);
        valor2 = stackPop(pilha);
        if(valor1.valor.vFloat - valor2.valor.vFloat >= 0)
          item.valor.vFloat = TRUE;
        else
          item.valor.vFloat = FALSE;
        stackPush(pilha, item);
      break;
      case 4:/*potencia*/
        valor1 = stackPop(pilha);
        valor2 = stackPop(pilha);
        item.valor.vFloat = pow(valor1.valor.vFloat , valor2.valor.vFloat);
        stackPush(pilha, item); 
        break;
      case 5:/*divisao inteira*/
        valor1 = stackPop(pilha);
        valor2 = stackPop(pilha);
        item.valor.vFloat = valor2.valor.vFloat / valor1.valor.vFloat;
        item.valor.vFloat = (int)item.valor.vFloat;
        stackPush(pilha, item);
      break;
      case 6:/*maior*/
        valor1 = stackPop(pilha);
        valor2 = stackPop(pilha);
        if(valor1.valor.vFloat - valor2.valor.vFloat < 0)
          item.valor.vFloat = TRUE;
        else
          item.valor.vFloat = FALSE;
        stackPush(pilha, item);
      break;
      case 7:/*menor*/
        valor1 = stackPop(pilha);
        valor2 = stackPop(pilha);
        if(valor1.valor.vFloat - valor2.valor.vFloat > 0)
          item.valor.vFloat = TRUE;
        else
          item.valor.vFloat = FALSE;
        stackPush(pilha, item);
      break;
      case 8:/*resto da divisao*/
        valor1 = stackPop(pilha);
        valor2 = stackPop(pilha);
        item.valor.vFloat = (int)valor2.valor.vFloat % (int)valor1.valor.vFloat;
        stackPush(pilha, item);
      break;
      case 9:/*multiplicacao*/
        valor1 = stackPop(pilha);
        valor2 = stackPop(pilha);
        item.valor.vFloat = valor1.valor.vFloat * valor2.valor.vFloat;
        stackPush(pilha, item);
      break;
      case 10:/*divisao*/
        valor1 = stackPop(pilha);
        valor2 = stackPop(pilha);
        item.valor.vFloat = valor2.valor.vFloat / valor1.valor.vFloat;
        stackPush(pilha, item);
      break;
      case  11:/*soma*/
        valor1 = stackPop(pilha);
        valor2 = stackPop(pilha);
        item.valor.vFloat = valor1.valor.vFloat + valor2.valor.vFloat;
        stackPush(pilha, item);
      break;
      case 12:/*subtracao*/
        valor1 = stackPop(pilha);
        valor2 = stackPop(pilha);
        item.valor.vFloat = valor1.valor.vFloat - valor2.valor.vFloat;
        stackPush(pilha, item);
      break;
      case 13:/*menos unario*/
        valor1 = stackPop(pilha);
        item.valor.vFloat = -valor1.valor.vFloat;
        stackPush(pilha, item);
      break;
      case 14:/*and*/
        valor1 = stackPop(pilha);
        valor2 = stackPop(pilha);
        if(valor1.valor.vFloat * valor2.valor.vFloat == 0)
          item.valor.vFloat = 0;
        else
          item.valor.vFloat = valor1.valor.vFloat;
        stackPush(pilha, item);
      break;
      case 15:/*or*/
        valor1 = stackPop(pilha);
        valor2 = stackPop(pilha);
        if(valor1.valor.vFloat == 0){
          if(valor2.valor.vFloat == 0)
            item.valor.vFloat = 0;
          else
            item.valor.vFloat = valor2.valor.vFloat;  
        }
        else
          if(valor2.valor.vFloat == 0)
            item.valor.vFloat = valor1.valor.vFloat;
          else
            item.valor.vFloat = valor2.valor.vFloat;
        stackPush(pilha, item);
      break;
      case 16:/*negacao*/
        valor1 = stackPop(pilha);
        valor2 = stackPop(pilha);
        if(valor1.valor.vFloat * valor2.valor.vFloat == 0)
          item.valor.vFloat = 0;
        else
          item.valor.vFloat = valor1.valor.vFloat;
        stackPush(pilha, item);
      break;
      case 18:/*atribuicao*/
        valor1 = stackPop(pilha);
        valor2 = stackPop(pilha);
        item.valor.vFloat = valor2.valor.vFloat;
        stackPush(pilha, item);
      break;
      case 19:/*abre parenteses*/
        
      break;
      case 31:
        printf(" indefinida\n");
      break;

     }
    aux = aux->prox;
  }

    pilha = pilha->prox;
    return pilha; 
}

