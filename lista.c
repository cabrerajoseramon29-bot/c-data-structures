#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

typedef struct Nodo{
    int data;
    struct Nodo *sig;
} Nodo;

struct Lista{
    Nodo *cabeza;
    Nodo *cola;
    size_t size;
};

/*vida util*/
Lista *lista_create(void){
    Lista *l = malloc(sizeof(Lista));
    if(!l)return NULL;

    l->cabeza = NULL;
    l->cola = NULL;
    l->size = 0;

    return l;
}

void lista_destroy(Lista *l){
    if(!l)return;

    Nodo *actual = l->cabeza;

    while (actual) {
        Nodo *next = actual->sig;
        free(actual);
        actual = next;
    }
    free(l);
}

/*acceso*/
size_t lista_size(const Lista *l){
    return l ? l->size : 0;
}

int lista_empty(const Lista *l){
    return (!l || !l->cabeza) ? 1 : 0;
}

int lista_front(const Lista *l, int *valor){
    if(!l || !l->cabeza || !valor)return -1;
    *valor = l->cabeza->data;
    return 0;
}

int lista_back(const Lista *l, int *valor){
    if(!l || !l->cabeza || !valor)return -1;
    *valor = l->cola->data;
    return 0;
}

int lista_get(const Lista *l, size_t pos, int *valor){
    if(!l || pos >= l->size || !valor)return -1;

    Nodo *actual = l->cabeza;

    for (size_t i = 0; i < pos; i++) {
        actual = actual->sig;
    }

    *valor = actual->data;

    return 0;
}

/*insert*/
int lista_push_front(Lista *l, int valor){
    if(!l)return -1;
    
    Nodo *nuevo = malloc(sizeof(Nodo));
    if(!nuevo)return -1;


    nuevo->data = valor;
    nuevo->sig = l->cabeza;
    l->cabeza = nuevo;
    
    if(!l->size){
        l->cola = nuevo;
    }

    l->size++;
    return 0;
}

int lista_push_back(Lista *l, int valor){
    if(!l)return -1;

    Nodo *nuevo = malloc(sizeof(Nodo));
    if(!nuevo)return -1;


    nuevo->data = valor;
    nuevo->sig = NULL;

    if (!l->cabeza) {
        l->cabeza = nuevo;
        l->cola = nuevo;
        l->size++;
        return 0;
    }

    l->cola->sig = nuevo;
    l->cola = nuevo;
    l->size++;

    return 0;
}

int lista_insert_at(Lista *l, size_t pos, int valor){
    if(!l || pos > l->size)return -1;
    
    /* 1- interceptar casos frontera*/
    if (pos == 0) {
        lista_push_front(l, valor);
        return 0;
    }

    if (pos == l->size) {
        lista_push_back(l, valor);
        return 0;
    }

    /* 2- la insercion debe ser en el medio O(n)*/
    Nodo *nuevo = malloc(sizeof(Nodo));
    if(!nuevo)return -1;

    nuevo->data = valor;

    /* 3- buscar el nodo anterior a la posicion deseada*/
    Nodo *anterior = l->cabeza;

    for (size_t i = 0; i < pos -1; i++) {
        anterior = anterior->sig;
    }

    /* 4- cirujia de punteros*/
    nuevo->sig = anterior->sig;
    anterior->sig = nuevo;

    l->size++;

    return 0;
}   

/*eliminar*/
int lista_pop_front(Lista *l, int *out){
    if(!l || !l->cabeza || !out)return -1;

    Nodo *temp = l->cabeza;
    *out = l->cabeza->data;
    l->cabeza = temp->sig;
    free(temp);

    if (!l->cabeza) {
        l->cola = NULL;
    }

    l->size--;

    return 0;
}

int lista_pop_back(Lista *l, int *out){
    if(!l || !l->cabeza || !out)return -1;

    /*un solo nodo*/
    if (!l->cabeza->sig) {
        *out = l->cabeza->data;
        free(l->cabeza);
        l->cabeza = NULL;
        l->cola = NULL;
        l->size--;
        return 0;
    }

    /*mas de un nodo. Buscar el penultimo*/
    Nodo *actual = l->cabeza;
    
    while (actual->sig != l->cola) {
        actual = actual->sig;    
    }

    /*actual es ahora el penultimo nodo*/
    *out = l->cola->data;
    free(l->cola);
    l->cola = actual;
    l->cola->sig = NULL;
    l->size--;

    return 0;
}

int lista_remove(Lista *l, int valor){
    if(!l || !l->cabeza)return -1;

    int temp;

    if (l->cabeza->data == valor) {
        return (lista_pop_front(l, &temp) == 0)? 0 : -1;
    }

    Nodo *anterior = l->cabeza;
    Nodo *actual = l->cabeza->sig;

    while (actual) {
        if (actual->data == valor) {
            anterior->sig = actual->sig;

            if (actual == l->cola) {
                l->cola = anterior;
            }

            free(actual);
            l->size--;

            return 0;
        }

        anterior = actual;
        actual = actual->sig;
    }
    return -1;
}

/*modificar*/
int lista_reverse(Lista *l){
    if(!l || l->size < 2)return -1;

    Nodo *anterior = NULL;
    Nodo *actual = l->cabeza;
    Nodo *next = NULL;

    /*guardar antigua cabeza porque al final se convertira en la nueva cola*/
    Nodo *nueva_cabeza = l->cabeza;

    while (actual) {
        /* 1- guardar el futuro, no se puede perder el resto de la lista*/
        next = actual->sig;

        /* 2- Invertir enlace. Ahora apuntar hacia atras*/
        actual->sig = anterior;

        /* 3- avanzar el tren, mover el puntero un paso*/
        anterior = actual;
        actual = next;
    }

    /*al salir del bucle anterior quedo parado en el ultimo nodo de la lista*/
    l->cabeza = anterior;   
    l->cola = nueva_cabeza;

    return 0;
}