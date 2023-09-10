
/***************************************************
 *                                                  *
 * Vinicius Silva de Paula                          *
 * 2017.0743.043-8                                  *
 * Implementação 3                                  *
 * Disciplina: Estruturas de Dados e Programação I  *
 * Professor: Ronaldo Fiorilo                       *
 *                                                  *
 ***************************************************/

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 80
#define TAMANHO_TABELA 2000

struct chave {
  char chave[MAX];
  struct chave *prox;
  struct chave *sinonimo;
};
typedef struct chave lista;
/* Definição da estrutura da tabela hash */
struct hash {
  lista *baldes[TAMANHO_TABELA];
};

typedef struct hash TabelaHash;

int funcaoHash(char *chave) {
  int t, i, hash;
  t = strlen(chave);
  hash = 0;
  for (i = 0; i < t; i++) {
    hash = hash + (chave[i] * (i + 1));
  }
  return hash % TAMANHO_TABELA;
}

lista *aloca(char palavra[]) {
  lista *novo;
  novo = (lista *)malloc(sizeof(lista));
  if (novo == NULL) {
    fprintf(stderr, "Erro ao alocar memória!\n");
    exit(1);
  }
  strcpy(novo->chave, palavra);
  novo->sinonimo = NULL;
  novo->prox = NULL;
  return novo;
}
void libera_lista(lista **lst, int op) {
  lista *anterior;
  while (*lst != NULL) {
    anterior = *lst;
    *lst = (*lst)->prox;
    if (op == 1) {
      libera_lista(&anterior->sinonimo, 2);
    }
    free(anterior);
  }
}
void mostra(lista *lst, int op) {
  lista *pt;
  pt = lst;
  while (pt->prox != NULL) {
    printf("%s\n", pt->chave);
    if (op < 2) {
      mostra(pt->sinonimo, 2);
    }
    pt = pt->prox;
  }
  printf("%s\n", pt->chave);
}
void insere(lista **lst, char *palavra, char *palavra2, int op) {
  lista *pt, *novo, *anterior = NULL;
  novo = aloca(palavra);
  if (*lst == NULL) /* lista vazia */
  {
    *lst = novo;
    if (op == 1) {
      insere(&novo->sinonimo, palavra2, palavra, 2);
    }
  } else {
    pt = *lst;
    while (pt->prox != NULL && (strcmp(novo->chave, pt->chave) > 0)) {
      anterior = pt;
      pt = pt->prox;
    }
    if (strcmp(novo->chave, pt->chave) == 0) {
      if (op == 1) {
        insere(&pt->sinonimo, palavra2, palavra, 2);
      }
      free(novo);
    } else if (strcmp(novo->chave, pt->chave) > 0) {
      pt->prox = novo;
      if (op == 1) {
        insere(&novo->sinonimo, palavra2, palavra, 2);
      }
    } else {
      novo->prox = pt;
      if (anterior != NULL) {
        anterior->prox = novo;
      } else {

        *lst = novo;
      }
      if (op < 2) {
        insere(&novo->sinonimo, palavra2, palavra, 2);
      }
    }
  }
}
void inserirHash(TabelaHash *tabelaHash, char *chave, char *chave2) {
  int indice;
  indice = funcaoHash(chave);
  insere(&tabelaHash->baldes[indice], chave, chave2, 1);
}
void buscaHash(TabelaHash *tabelaHash, char *chave) {
  int indice;
  lista *noAtual;
  indice = funcaoHash(chave);
  noAtual = tabelaHash->baldes[indice];
  if (noAtual != NULL) {
    mostra(noAtual->sinonimo, 2);
  } else {
    printf("hein?\n");
  }
}
void remover_lista1(lista **lst, char *valor) {
  lista *aux, *remover;
  aux = *lst;
  remover = NULL;
  if (aux != NULL) {
    if (strcmp(aux->chave, valor) == 0) {
      remover = aux;
      aux = remover->prox;
    } else {
      while (aux->prox != NULL && strcmp(aux->prox->chave, valor) == 0) {
        aux = aux->prox;
      }
      if (aux->prox != NULL) {
        remover = aux->prox;
        aux->prox = remover->prox;
      }
    }
    libera_lista(&remover->sinonimo, 2);
    free(remover);
  }
}
void removerHash(TabelaHash *tabelaHash, char *chave, char *chave2) {
  int indice;
  indice = funcaoHash(chave);
  if (tabelaHash->baldes[indice] == NULL) {
    printf("Chave nao existe!");
  } else {
    remover_lista1(&tabelaHash->baldes[indice], chave);
  }
}
void mostra_hash(TabelaHash tabela, int op) {
  int i;
  for (i = 0; i < TAMANHO_TABELA; i++) {
    if (tabela.baldes[i] != NULL) {
      mostra(tabela.baldes[i], op);
    }
  }
}
void inicializa_hash(TabelaHash *tabela) {
  int i;
  /* Inicializa todos os baldes com NULL */
  for (i = 0; i < TAMANHO_TABELA; i++) {
    (*tabela).baldes[i] = NULL;
  }
}
void libera_hash(TabelaHash *tabela) {
  int i;
  for (i = 0; i < TAMANHO_TABELA; i++) {
    if ((*tabela).baldes[i] != NULL) {
      libera_lista(&(*tabela).baldes[i], 1);
    }
  }
}
void comandos(TabelaHash *tabela, char *chamada, char *valor, char *valor2) {
  scanf("%s", chamada);
  while (strcmp(chamada, "fim") != 0) {
    scanf("%s", valor);
    if (strcmp(chamada, "insere") == 0) {
      scanf("%s", valor2);
      inserirHash(tabela, valor, valor2);
      inserirHash(tabela, valor2, valor);
    } else if (strcmp(chamada, "busca") == 0) {
      buscaHash(tabela, valor);
    } else if (strcmp(chamada, "remove") == 0) {
      removerHash(tabela, valor, NULL);
    }
    scanf("%s", chamada);
  }
}
int main() {
  TabelaHash tabela;
  char chamada[MAX], valor[MAX], valor2[MAX];

  setlocale(LC_ALL, "Portuguese");
  inicializa_hash(&tabela);
  comandos(&tabela, chamada, valor, valor2);
  libera_hash(&tabela);

  return 0;
}