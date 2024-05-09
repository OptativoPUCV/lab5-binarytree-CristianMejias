#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"
#include "stdbool.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    //reservar memoria arbolito
    TreeMap *arbolito = (TreeMap *) malloc(sizeof(TreeMap));
    //validar reserva de memoria
    if (arbolito == NULL)
        return NULL;
    //Asignar datos
    arbolito->root = NULL;
    arbolito->current = NULL;
    arbolito->lower_than = lower_than;
    return arbolito;
}


void insertTreeMap(TreeMap * arbolito, void* key, void * value) {
    
    //Creamos noso para almacenar datos
    TreeNode *nuevoNodo = createTreeNode(key, value);
    TreeNode *nodoPosicion = arbolito->root;
    
    //Si el arbol no tiene datos solo tengo que agregar los datos
    if (nodoPosicion == NULL) {
        //raiz y actual apuntaran al nodo creado
        arbolito->root = nuevoNodo;
        arbolito->current = nuevoNodo;
        return;
    }
    //Si el arbol no es nulo buscamos posicion a insertar
    while (true) {
         Pair *pair = nodoPosicion->pair;

        //Caso clave nodo es igual a clave insertar
        if (is_equal(arbolito, pair->key, key))
            return;
        //Caso izquierda o derecha sin hijos
        if (nodoPosicion->left == NULL || nodoPosicion->right == NULL)
            break;
        //Caso clave nodo es menor o mayor avanzamos
        if (arbolito->lower_than(pair->key, key))
            nodoPosicion = nodoPosicion->right;
        else
            nodoPosicion = nodoPosicion->left;
    }
    if (nodoPosicion->left == NULL)
        nodoPosicion->left = nuevoNodo;
    else
        nodoPosicion->right = nuevoNodo;
        
    //asignamos a posicion encontrada
    nuevoNodo->parent = nodoPosicion;
    arbolito->current = nuevoNodo;
}

TreeNode * minimum(TreeNode * x){
    //Nodo inicial a recorrer
    TreeNode *nodoActual = x;
    //recorrer siempre a la izquierda hasta llegar a null
    while (nodoActual->left != NULL)
        nodoActual = nodoActual->left;
    return nodoActual;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    TreeNode *nodoEliminar = node;
    TreeNode *nodoPadre = nodoEliminar->parent;

    //caso nodo eliminar es el primero
    if (nodoPadre == NULL) {
        tree->root = NULL;
        return;
    }
    //caso el nodo eliminar no tiene hijos
    if (nodoEliminar->left == NULL && nodoEliminar->right == NULL) {
        if (nodoPadre->left == nodoEliminar)
            nodoPadre->left = NULL;
        else
            nodoPadre->right = NULL;
    }
    /*
    //caso el nodo eliminar tiene 1 hijo
    else if (nodoEliminar->left == NULL || nodoEliminar->right == NULL) {
        TreeNode *nodoHijo = NULL;
        
        //determinar si es derecha o izquierda
        if (nodoEliminar->left == NULL)
            nodoHijo = nodoEliminar->right;
        else
            nodoHijo = nodoEliminar->left;
        
        //caso anterior pero asignando el hijo
        if (nodoPadre->left == nodoEliminar)
            nodoPadre->left = nodoHijo;
        else
            nodoPadre->right = nodoHijo;

        //actualizar padre
        nodoHijo->parent = nodoPadre;
    }
    //caso el nodo eliminar tiene 2 hijos
    else {
        TreeNode *aux = minimum(nodoEliminar->right);
        removeNode(tree, aux);
        nodoEliminar->pair = aux->pair;
        
    }
    */
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    //iniciamos en el primer valor
    TreeNode *nodoActual = tree->root;
    Pair *pairActual = NULL;
    //recorremos hasta que encontremos un nodo nulo
    while (nodoActual != NULL) {
        pairActual = nodoActual->pair;
        //comprobamos si nodo actual tiene el dato buscado
        //no olvidar actualizar current
        if (is_equal(tree, pairActual->key, key)) {
            tree->current = nodoActual;
            return pairActual;
        }
        //caso que es menor
        else if (tree->lower_than(key, pairActual->key))
            nodoActual = nodoActual->left;
        //caso es mayor
        else
            nodoActual = nodoActual->right;
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode *nodo = minimum(tree->root);
    tree->current = nodo;
    return nodo->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
