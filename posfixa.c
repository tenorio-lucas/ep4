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

  posfixa.c
  Pitao II

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:

  - função mallocc retirada de: 

  http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/*
  NAO EDITE OU MODIFIQUE NADA QUE ESTA ESCRITO NESTE ESQUELETO
*/

/*------------------------------------------------------------*/
/* iterface para o uso da funcao deste módulo */
#include "posfixa.h"

/*------------------------------------------------------------*/
#include "categorias.h" /* Categoria, MAX_OPERADORES, INDEFINIDA, 
                           ABRE_PARENTESES, ... */
#include "objetos.h" /* tipo CelObjeto, freeObjeto(), freeListaObjetos() */
#include "stack.h"   /* stackInit(), stackFree(), stackPop() 
                        stackPush(), stackTop() */

/*-------------------------------------------------------------
 *  infixaParaPosfixa
 * 
 *  Recebe uma lista ligada com cabeca INIINFIXA representando uma
 *  fila de itens de uma expressao infixa e RETORNA uma lista ligada
 *  com cabeca contendo a fila com a representacao da correspondente
 *  expressao em notacao posfixa.
 */
 /*  As celulas da notacao infixa que nao forem utilizadas na notacao 
  *  posfixa (abre e fecha parenteses) devem ser liberadas 
  *  (freeObjeto()).
  */


CelObjeto *
infixaParaPosfixa(CelObjeto *iniInfixa)
{
  CelObjeto *iniPosf, *pilha, *fimPosf;
  
  
  /*criando lista posfixa */
  iniPosf = mallocSafe(sizeof(CelObjeto));
  iniPosf->prox = NULL;
  fimPosf = iniPosf

  Item item;

  pilha = stackInit();
  
  while(iniInfixa != NULL){
    
    switch(iniInfixa->categoria){

      item.valor.vFloat = iniInfixa->valor.vFloat;
      item.valor.vInt = iniInfixa->valor.vInt;
      item.valor.pStr = iniInfixa->valor.pStr;
      item.categoria = iniInfixa->categoria;
      
      case OPER_ATRIBUICAO: 
      
      break;
      case OPER_LOGICO_OR:

      break;
      case OPER_LOGICO_AND:

      break;
      case OPER_IGUAL:
      case OPER_DIFERENTE:

      break;
      case OPER_MAIOR:
      case OPER_MENOR:
      case OPER_MAIOR_IGUAL:
      case 
      case OPER_ADICAO:
      case OOPER_MENOR_IGUAL:

      break;PER_SUBTRACAO:

      break;
      case OPER_MULTIPLICACAO:
      case OPER_DIVISAO:
      case OPER_RESTO_DIVISAO:
      case OPER_DIVISAO_INT:

      break:
      case OPER_LOGICO_NOT:
      case OPER_MENOS_UNARIO:
      case OPER_EXPONENCIACAO:
      
      break;
      case ABRE_PARENTESES:
        stackPush(pilha, item);
      break;
      case FECHA_PARENTESES:
        item = stackPop(iniInfixa);
        while(item != ABRE_PARENTESES){
        
        CelObjeto *nova;/*criando uma nova celula na fila */
        nova = mallocSafe(sizeof(CelObjeto));
        nova.valor.vFloat = item.valor.vFloat;
        nova.valor.vInt =  item.valor.vInt;
        nova.valor.pStr = item.valor.pStr;
        nova.categoria = item.categoria;
        nova.prox->NULL;
        fimPosf->prox = nova;
        fimPosf = nova;
        item = stackPop(iniInfixa);
        }
      break;
    }
    
  }

  return NULL; 
}
