/*!
 * \file Nodes.h
 * \brief Definicja struktury grafu i niezbędnych typów.
 * POPRO - 23Z - Zadanie projektowe.
 * 
 * Nazwa zadania:           Grafy.
 * Grupa projektowa:        Zespół nr 1.
 * Nazwa pliku:             Nodes.h
 * 
 * \see Nodes.c
 */

#ifndef NODES_H
#define NODES_H

#include <stdbool.h>
#include <stdint.h>

/*!
 * \typedef Vertex_t
 * \brief Struktura wierzchołka.
 *
 * \see Vertex
 */
typedef struct Vertex {
    uint64_t id;                                ///< Identyfikator wierzchołka.
    struct Vertex *next;                        ///< Odnośnik do następnego wierzchołka w grafie.
    struct NeighbourVertex *firstNeighbour;     ///< Odnośnik do pierwszego sąsiada wierzchołka.
} Vertex_t;


/*!
 * \typedef NeighbourVertex_t
 * \brief Struktura wierzchołka sąsiadującego z innym.
 * 
 * \see NeighbourVertex
 */
typedef struct NeighbourVertex {
    Vertex_t *data;                             ///< Odnośnik do struktury opisującej wierzchołek sąsiadujący.
    struct NeighbourVertex *next;               ///< Odnośnik do następnego sąsiada wierzchołka opisywanego w data.
} NeighbourVertex_t;


/*!
 * \typedef ChangeStatus_t
 * \brief Opis możliwych zachowań komend składowych programu.
 * 
 * Opisy wykorzystywane do implementacji wewnętrznego wykrywania błędów.
 * 
 * \see ChangeStatus
 */
typedef enum ChangeStatus {
    OK = 1,                                     ///< Status "wszystko w porządku".
    FAIL_NOT_EXIST,                             ///< Status błędu z powodu nieistnienia danej zmiennej / pliku.
    FAIL_EXISTS,                                ///< Status błędu z powodu istnienia ("dana zmienna / plik już istnieje").
    FAIL_OTHER                                  ///< Status błędu z innego powodu.
} ChangeStatus_t;


void listNodes(Vertex_t *first);
unsigned int getGraphLength(Vertex_t *first);
unsigned int getNeighborsLength(Vertex_t *first);

bool seekNeighbour(Vertex_t *node, uint64_t ID);

Vertex_t* seekNode(Vertex_t *first, uint64_t ID);

ChangeStatus_t addNode(Vertex_t **first, uint64_t ID);
ChangeStatus_t removeNode(Vertex_t **first, uint64_t ID);
ChangeStatus_t addNeighbour(Vertex_t *node1, Vertex_t *node2);
ChangeStatus_t removeNeighbour(Vertex_t *node1, Vertex_t *node2);
ChangeStatus_t addEdge(Vertex_t *first, uint64_t ID1, uint64_t ID2);
ChangeStatus_t removeEdge(Vertex_t *first, uint64_t ID1, uint64_t ID2);

#endif