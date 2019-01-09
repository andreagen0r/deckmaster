#ifndef PACK_H
#define PACK_H

#include <string.h>

#include "card.h"

typedef struct _pack Pack;

// Retorna um baralho com 30 cartas aleatórios
Pack *pack_init(void);

// Funções para estrutura de dados tipo pilha
Pack *pack_newNode(const Card card);

// Adiciona nova carta na pilha
// Returna "true" se for possível incluir, senão retorna false
bool pack_push(Pack **root, const Card card);

// Remove carta do topo da pilha
// Returna "true" se for possível excluir, senão retorna false
bool pack_pop(Pack **root);

// Returna a carta do topo da pilha
Card pack_top(Pack *root);

// Returna "true" se o baralho estiver vazio, senão retorna false
bool pack_isEmpty(Pack *root);

// Retorna a quantidade de itens na pilha
int pack_size(void);

// Imprime a próxima carta
void pack_print_next_card(Pack *m_pack, int m_mode);

#endif // PACK_H
