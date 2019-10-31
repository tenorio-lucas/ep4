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

  stack.c
  Pitao II

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.

  Exemplo:
  - função mallocc retirada de: 
 
  http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

/* interface para o uso de uma pilha */
#include "stack.h" 
#include <stdio.h>
#include <stdlib.h>
#include "objetos.h"

CelObjeto *stackInit(){ /* retorna o enderecoda cabeca da lista */
  CelObjeto *ini;
  
  ini = mallocSafe(sizeof (CelObjeto));
  ini->prox = NULL;
  
  return ini;
}

int stackEmpty(CelObjeto *s){
  if(s->prox == NULL)
    return 0;
  else 
    return 1;
}

void stackPush(CelObjeto *s, Item item){
  CelObjeto *nova = mallocSafe(sizeof *nova);

  nova->valor = item.valor;
  nova->categoria = item.categoria;
  nova->prox = s->prox;
  s->prox = nova;
}

Item stackPop(CelObjeto *s){
  Item conteudo;
  CelObjeto *p;
  p = s->prox;

  if(p != NULL){
  conteudo.valor.vFloat = p->valor.vFloat;
  conteudo.valor.vInt = p->valor.vInt;
  conteudo.valor.pStr = p->valor.pStr;
  conteudo.categoria = p->categoria;
  freeObjeto(p);
    s->prox = p->prox;
  }
  else{
  printf("pilha vazia");
  exit (0);
  }
   

  return conteudo;
}


Item stackTop(CelObjeto *s){
  Item item;
  item.valor = (s->prox)->prox->valor;
  item.valor.vFloat = s->prox->valor.vFloat;
  item.valor.vInt = s->prox->valor.vInt;
  item.valor.pStr = s->prox->valor.pStr;
  item.categoria = s->prox->categoria;
  


  return item;
}

void stackFree(CelObjeto *s){

  while(s != NULL){
    CelObjeto *p;
    p = s->prox;
    s->prox = p->prox;
    freeObjeto(p);
  }
  freeObjeto(s);
}

void stackDump(CelObjeto *s){
  
}

/* 
 * 
 * STACK.c: IMPLEMENTACAO DE UMA PILHA 
 *
 * TAREFA EP3
 *
 * Faca aqui a implementacao de uma pilha atraves de uma 
 * __lista encadeada com cabeca__. 
 * 
 * A implementacao deve respeitar a interface que voce declarar em 
 * stack.h. 
 *
 * TAREFA EP4: copiar a implementacao feira para o EP3.
 *     E possivel que sua implementacao para o EP3 deve ser 
 *     adaptada, ja que agora a pilha de execucao pode conter
 *     nomes de variaveis e nao apenas valores double.
 *
 */

