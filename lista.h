#ifndef LISTA_H_
#define LISTA_H_

#include <stddef.h>

typedef struct Lista Lista;

/*vida util*/
Lista *lista_create(void);
void lista_destroy(Lista *l);

/*acceso*/
size_t lista_size(const Lista *l);
int lista_empty(const Lista *l);
int lista_front(const Lista *l, int *valor);
int lista_back(const Lista *l, int *valor);
int lista_get(const Lista *l, size_t pos, int *valor);

/*insert*/
int lista_push_front(Lista *l, int valor);
int lista_push_back(Lista *l, int valor);
int lista_insert_at(Lista *l, size_t pos, int valor);

/*eliminar*/
int lista_pop_front(Lista *l, int *out);
int lista_pop_back(Lista *l, int *out);
int lista_remove(Lista *l, int valor);

/*modificar*/
int lista_reverse(Lista *l);

#endif