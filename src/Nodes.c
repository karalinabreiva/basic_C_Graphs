/*!
 * \file Nodes.c
 * \brief Moduł zarządzający grafem jako obiektem abstrakcyjnym - tworzący, edytujący i kasujący go.
 * 
 * POPRO - 23Z - Zadanie projektowe.
 * 
 * Nazwa zadania:           Grafy.
 * Grupa projektowa:        Zespół nr 1.
 * Nazwa pliku:             Nodes.c
 */

#include <stdio.h>
#include <stdlib.h>

#include "includes/Nodes.h"


/*!
 * \fn listNodes(Vertex_t *first)
 * \brief Przedstawianie grafu w formie listy zależności.
 * 
 * \param first Odnośnik do pierwszego wierzchołka grafu.
 */
void listNodes(Vertex_t *first)
{
    Vertex_t *current = first;
    while( current )
    {
        printf("%llu\n", current->id);
        current = current->next;
    }
}


/*!
 * \fn unsigned int getGraphLength(Vertex_t *first)
 * \brief Sprawdzenie długości grafu, rozumianej jako liczba odnalezionych wierzchołków w grafie.
 * 
 * \param first Odnośnik do pierwszego wierzchołka grafu.
 * 
 * \returns Liczba odnalezionych wierzchołków.
 */
unsigned int getGraphLength(Vertex_t *first)
{
    unsigned int length = 0;
    Vertex_t *current = first;

    while( current )
    {
        length += 1;
        current = current->next;
    }

    return length;
}


/*!
 * \fn unsigned int getNeighborsLength(Vertex_t *first)
 * \brief Sprawdzenie liczby sąsiadów danego wierzchołka.
 * 
 * \param first Odnośnik do badanego wierzchołka grafu.
 * 
 * \returns Liczba odnalezionych sąsiadów.
 */
unsigned int getNeighborsLength(Vertex_t *first)
{
    unsigned int length = 0;
    NeighbourVertex_t *current = first->firstNeighbour;

    while( current )
    {
        length += 1;
        current = current->next;
    }

    return length;
}


/*!
 * \fn bool seekNeighbour(Vertex_t *node, uint64_t ID)
 * \brief Poszukiwanie wierzchołka sąsiadującego wg podanego ID.
 * 
 * \param node Odnośnik do wierzchołka, wśród którego sąsiadów przeszukujemy po ID.
 * \param ID Identyfikator, po którym szukamy sąsiada.
 * 
 * \returns Informacja o tym, czy dany sąsiad istnieje - true albo false.
 */
bool seekNeighbour(Vertex_t *node, uint64_t ID)
{
    NeighbourVertex_t *current = node->firstNeighbour;

    while( current )
    {
        if( current->data->id == ID )
            return true;

        current = current->next;
    }

    return false;
}


/*!
 * \fn Vertex_t* seekNode(Vertex_t *first, uint64_t ID)
 * \brief Poszukiwanie wierzchołka w grafie wg podanego ID.
 * 
 * \param first Odnośnik do pierwszego wierzchołka grafu.
 * \param ID Identyfikator, po którym szukamy wierzchołka.
 * 
 * \returns Odnośnik do wierzchołka typu Vertex_t *, jeżeli takowy istnieje. 
 *          NULL, jeżeli wierzchołek o danym ID nie istnieje.
 */
Vertex_t* seekNode(Vertex_t *first, uint64_t ID)
{
    Vertex_t *current = first;

    while( NULL != current )
    {
        if(current->id == ID) return current;
        current = current->next;
    }

    return NULL;
}


/*!
 * \fn ChangeStatus_t addNode(Vertex_t **first, uint64_t ID)
 * \brief Dodanie wierzchołka o danym ID do grafu.
 * 
 * \param first Adres odnośnika do pierwszego wierzchołka grafu.
 * \param ID Identyfikator, po którym szukamy wierzchołka do dodania.
 * 
 * \returns Informacja o efekcie operacji - ChangeStatus_t.
 */
ChangeStatus_t addNode(Vertex_t **first, uint64_t ID)
{
    if( seekNode(*first, ID) )
        return FAIL_EXISTS;

    Vertex_t *new = NULL;
    if( NULL == (new = (Vertex_t *) malloc(sizeof(Vertex_t))) )
        return FAIL_OTHER;

    new->next = *first;
    new->id = ID;
    new->firstNeighbour = NULL;

    *first = new;
    return OK;
}


/*!
 * \fn ChangeStatus_t removeNode(Vertex_t **first, uint64_t ID)
 * \brief Dodanie wierzchołka o danym ID do grafu.
 * 
 * \param first Adres odnośnika do pierwszego wierzchołka grafu.
 * \param ID Identyfikator, po którym szukamy wierzchołka do usunięcia.
 * 
 * \returns Informacja o efekcie operacji - ChangeStatus_t.
 */
ChangeStatus_t removeNode(Vertex_t **first, uint64_t ID)
{
    Vertex_t *toRemove = seekNode(*first, ID);

    if( !toRemove )
        return FAIL_NOT_EXIST;

    Vertex_t *current = *first;

    while( current )
    {
        if( seekNeighbour(current, ID) )
        {
            ChangeStatus_t status = removeNeighbour( current, toRemove );
            
            if( FAIL_NOT_EXIST == status || FAIL_OTHER == status )
                return status;
        }
        current = current->next;
    }

    NeighbourVertex_t *neighCurrent, *neighTmp;

    neighCurrent = toRemove->firstNeighbour;
    neighTmp = NULL;

    while( neighCurrent )
    {
        neighTmp = neighCurrent->next;
        free( neighCurrent );
        neighCurrent = neighTmp;
    }

    current = *first;
    Vertex_t *prevNode = NULL;

    while( current )
    {
        if( current == toRemove )
        {
            if( prevNode != NULL )
                prevNode->next = toRemove->next;
            else
                *first = toRemove->next;

            free(toRemove);
            return OK;
        }

        prevNode = current;
        current = current->next;
    }

    return FAIL_OTHER;
}


/*!
 * \fn ChangeStatus_t addNeighbour(Vertex_t *node1, Vertex_t *node2)
 * \brief Dodanie wierzchołka 2 do listy sąsiadów wierzchołka 1. 
 * 
 * \param node1 Identyfikator wierzchołka 1.
 * \param node2 Identyfikator wierzchołka 2.
 */
ChangeStatus_t addNeighbour(Vertex_t *node1, Vertex_t *node2)
{
    NeighbourVertex_t *new;
    
    if( NULL == (new = (NeighbourVertex_t *) malloc(sizeof(NeighbourVertex_t))) )
        return FAIL_OTHER;

    new->next = node1->firstNeighbour;
    new->data = node2;
    node1->firstNeighbour = new;

    return OK;
}


/*!
 * \fn ChangeStatus_t removeNeighbour(Vertex_t *node1, Vertex_t *node2)
 * \brief Usuwanie wierzchołka 2 z listy sąsiadów wierzchołka 1.
 * 
 * \param node1 Identyfikator wierzchołka 1.
 * \param node2 Identyfikator wierzchołka 2.
 */
ChangeStatus_t removeNeighbour(Vertex_t *node1, Vertex_t *node2)
{
    if( !seekNeighbour(node1, node2->id) )
        return FAIL_NOT_EXIST;

    NeighbourVertex_t *current = node1->firstNeighbour;
    NeighbourVertex_t *previous = NULL;

    while( current )
    {
        if( current->data->id == node2->id )
        {
            if( previous )
                previous->next = current->next;
            else
                node1->firstNeighbour = current->next;

            free( current );
            return OK;
        }

        previous = current;
        current = current->next;
    }

    return FAIL_OTHER;
}


/*!
 * \fn ChangeStatus_t addEdge(Vertex_t *first, uint64_t ID1, uint64_t ID2)
 * \brief Łączenie krawędzią wierzchołków o podanych identyfikatorach.
 * 
 * Funkcja sprawdza, czy wierzchołki istnieją oraz sąsiadują ze sobą. Następnie
 * wzajemnie dopisuje przeciwne wierzchołki do swoich list sąsiedztwa.
 * 
 * \param first Odnośnik do pierwszego wierzchołka grafu.
 * \param ID1 Identyfikator wierzchołka 1.
 * \param ID2 Identyfikator wierzchołka 2.
 */
ChangeStatus_t addEdge(Vertex_t *first, uint64_t ID1, uint64_t ID2)
{
    Vertex_t *node1, *node2;

    node1 = seekNode(first, ID1);
    node2 = seekNode(first, ID2);

    if( !(node1 && node2) )
        return FAIL_NOT_EXIST;

    if( seekNeighbour(node1, node2->id) || seekNeighbour(node2, node1->id) )
        return FAIL_EXISTS;

    if( addNeighbour(node1, node2) != OK || addNeighbour(node2, node1) != OK )
        return FAIL_OTHER;

    return OK;
}


/*!
 * \fn ChangeStatus_t removeEdge(Vertex_t *first, uint64_t ID1, uint64_t ID2)
 * \brief Usuwanie krawędzi łączącej wierzchołki o podanych identyfikatorach.
 * 
 * Funkcja sprawdza, czy wierzchołki istnieją, sąsiadują ze sobą oraz są połączone
 * krawędzią. Następnie wzajemnie usuwa przeciwne wierzchołki ze swoich list sąsiedztwa.
 * 
 * \param first Odnośnik do pierwszego wierzchołka grafu.
 * \param ID1 Identyfikator wierzchołka 1.
 * \param ID2 Identyfikator wierzchołka 2.
 */
ChangeStatus_t removeEdge(Vertex_t *first, uint64_t ID1, uint64_t ID2)
{
    Vertex_t *node1, *node2;

    node1 = seekNode(first, ID1);
    node2 = seekNode(first, ID2);

    if( !(node1 && node2) )
        return FAIL_NOT_EXIST;

    if( !seekNeighbour(node1, node2->id) || !seekNeighbour(node2, node1->id) )
        return FAIL_NOT_EXIST;

    if( removeNeighbour(node1, node2) != OK || removeNeighbour(node2, node1) != OK )
        return FAIL_OTHER;

    return OK;
}