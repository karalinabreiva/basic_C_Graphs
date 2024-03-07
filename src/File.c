/*!
 * \file File.c
 * \brief Moduł obsługujący zapis do pliku w postaci tekstowej oraz binarnej.
 * 
 * POPRO - 23Z - Zadanie projektowe.
 * 
 * Nazwa zadania:           Grafy.
 * Grupa projektowa:        Zespół nr 1.
 * Nazwa pliku:             File.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "includes/File.h"


/*!
 * \fn swap(uint64_t *a, uint64_t *b) 
 * \brief Zamiana miejscami dwóch zmiennych typu integer.
 * 
 * Funkcja pomocnicza dla funkcji sortArray.
 * 
 * \param a Liczba A do zamiany z liczbą B.
 * \param b Liczba B do zamiany z liczbą A.
 */
void swap(uint64_t *a, uint64_t *b) 
{  
    uint64_t t = *a;  
    *a = *b;  
    *b = t;  
}


/*!
 * \fn int partition(uint64_t *arr, int low, int high)
 * \brief Poszukiwanie liczby-pivota i zamiana miejscami w razie potrzeby.
 * 
 * Funkcja pomocnicza dla funkcji sortArray.
 * 
 * \param arr Tablica liczb całkowitych do posortowania.
 * \param low Najmniejsza liczba w tablicy.
 * \param high Największa liczba w tablicy.
 * 
 * \returns Aktualny pivot.
 */
int partition(uint64_t *arr, int low, int high)
{
    uint64_t pivot = arr[high];
    unsigned int i = (low - 1);  
  
    for (unsigned int j = low; j <= high - 1; ++j) 
    {  
        if (arr[j] < pivot) 
        {  
            i++;  
            swap(&arr[i], &arr[j]);  
        }  
    }  

    swap(&arr[i + 1], &arr[high]);  
    return (i + 1);  
}  

/*!
 * \fn sortArray(uint64_t *array, int low, int high);
 * \brief Wdrożenie algorytmu quicksort dla liczb całkowitych
 * 
 * \param array Tablica liczb całkowitych do posortowania.
 * \param low Najmniejsza liczba w tablicy.
 * \param high Największa liczba w tablicy.
 */
void sortArray(uint64_t *array, int low, int high)
{
    if (low < high) 
    {  
        int pi = partition(array, low, high);  
        sortArray(array, low, pi - 1);  
        sortArray(array, pi + 1, high);  
    }  
}


/*!
 * \fn ChangeStatus_t save(Vertex_t *first, char *filename)
 * \brief Zapis listy sąsiedztwa do postaci pliku tekstowego.
 * 
 * Lista sąsiedztwa jest posortowana wg wierzchołków oraz wg elementów
 * sąsiadujących, rosnąco.
 * 
 * \param first Odnośnik do pierwszego wierzchołka grafu.
 * \param filename Nazwa pliku do zapisu.
 * 
 * \returns ChangeStatus_t
 */
ChangeStatus_t save(Vertex_t *first, char *filename)
{
    FILE *dump = fopen(filename, "w");
    if( !(dump) || ferror(dump) )
        return FAIL_NOT_EXIST;

    Vertex_t *current = first;
    NeighbourVertex_t *neighCurrent = NULL;
    unsigned int graphLength = getGraphLength(current);

    if( 0 == graphLength )
        return FAIL_OTHER;

    uint64_t *vertecies;
    if( NULL == (vertecies = (uint64_t *)malloc((uint64_t)graphLength * sizeof(uint64_t))) )
        return FAIL_OTHER;

    for( unsigned int i = 0; i < graphLength; ++i )
    {
        vertecies[i] = current->id;
        current = current->next;
    }

    sortArray(vertecies, 0, (graphLength - 1));

    for( unsigned int i = 0; i < graphLength; ++i )
    {
        fprintf(dump, "%llu:", vertecies[i]);
        
        Vertex_t *neighParent = seekNode(first, vertecies[i]);
        unsigned int neighLength = getNeighborsLength(neighParent);

        uint64_t *neighbors;
        if( NULL == (neighbors = (uint64_t *)malloc(neighLength * sizeof(uint64_t))) )
            return FAIL_OTHER;

        neighCurrent = neighParent->firstNeighbour;

        for( unsigned int j = 0; j < neighLength; ++j )
        {
            neighbors[j] = neighCurrent->data->id;
            neighCurrent = neighCurrent->next;
        }

        sortArray(neighbors, 0, (neighLength - 1));

        for( unsigned int j = 0; j < neighLength; ++j ) 
        {
            fprintf(dump, " %llu", neighbors[j]);
            
            if(j != (neighLength - 1))
                fputc(',', dump);
        }

        if(i != (graphLength - 1))
            fputc('\n', dump);
    }

    fclose(dump);
    return OK;
}


/*!
 * \fn ChangeStatus_t dump(Vertex_t *first, char *filename)
 * \brief Zapis listy wierzchołków do postaci pliku binarnego.
 * 
 * Lista wierzchołków jest ułożona zgodnie z kolejnością występowania w grafie.
 * 
 * \param first Odnośnik do pierwszego wierzchołka grafu.
 * \param filename Nazwa pliku do zapisu.
 */
ChangeStatus_t dump(Vertex_t *first, char* filename)
{
    FILE *dump = fopen(filename, "wb");
    if( !(dump) || ferror(dump) )
        return FAIL_NOT_EXIST;

    Vertex_t *current = first;
    NeighbourVertex_t *neighCurrent = NULL;
    unsigned int graphLength = getGraphLength(current);

    if( 0 == graphLength )
        return FAIL_OTHER;

    uint64_t *vertecies;
    if( NULL == (vertecies = (uint64_t *)malloc(graphLength * sizeof(uint64_t))) )
        return FAIL_OTHER;

    for(unsigned int i = 0; i < graphLength; ++i )
    {
        vertecies[i] = current->id;
        current = current->next;
        fwrite(&vertecies[i], sizeof(uint64_t), 1, dump);
    }

    fclose(dump);
    return OK;
}