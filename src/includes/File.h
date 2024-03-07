/*!
 * \file File.h
 * \brief Definicje funkcji zapisujących dane w postaci tekstowej i binarnej.
 * 
 * POPRO - 23Z - Zadanie projektowe.
 * 
 * Nazwa zadania:           Grafy.
 * Grupa projektowa:        Zespół nr 1.
 * Nazwa pliku:             File.h
 * 
 * \see Nodes.h
 */

#ifndef FILES_H
#define FILES_H

#include "Nodes.h"

void swap(uint64_t *a, uint64_t *b);
int partition(uint64_t *arr, int low, int high);
void sortArray(uint64_t *array, int low, int high);

ChangeStatus_t save(Vertex_t *first, char* filename);
ChangeStatus_t dump(Vertex_t *first, char* filename);

#endif