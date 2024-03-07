/*!
 * \file Graph.c
 * \brief Główny sterownik aplikacji grafowej.'
 * 
 * Sterownik importujący poszczególne moduły i odpowiadający za samodzielną
 * komunikację z użytkownikiem. W przypadku, w którym programista chce
 * na własną rękę zaimplementować interfejs użytkownika, powinien
 * zainteresować się poszczególnymi modułami.
 * 
 * POPRO - 23Z - Zadanie projektowe.
 * 
 * Nazwa zadania:           Grafy.
 * Grupa projektowa:        Zespół nr 1.
 * Nazwa pliku:             Graph.c
 */

#include <stdio.h>
#include <stdlib.h>

#include "includes/Nodes.h"
#include "includes/File.h"
#include "includes/Input.h"

/*!
 * \fn int main(void)
 * \brief Podstawowa (a zarazem jedyna potrzebna) funkcja modułu głównego.
 * 
 * Funkcja inicjalizuje kilka niezbędnych zmiennych, jak np.:
 * - pierwszy wierzchołek grafu,
 * - miejsca do zapisu dwóch identyfikatorów wierzchołków,
 * - miejsce do zapisu aktualnego statusu funkcji składowych,
 * - miejsce na pliki wejścia/wyjścia oraz skryptu zewnętrznego,
 * - miejsce na zapis inputu oraz nazwy pliku skryptowego.
 * 
 * Następnie funkcja wywołuje w pętli funkcję z modułu Input.c - getCommand oraz testuje
 * poszczególne warunki.
 * 
 * \see getCommand
 *
 */
int main( void )
{
    Vertex_t *firstNode = NULL;

    uint64_t ID1 = 0;
    uint64_t ID2 = 0;
    ChangeStatus_t status = OK;

    FILE *source = stdin;
    FILE *script = NULL;

    char input[LINE_SIZE];
    char filename[FILENAME_MAX];

    while(1) switch( getCommand(source, &ID1, &ID2, filename, input) )
    {
        case ADD_NODE:
            status = addNode(&firstNode, ID1);

            switch( status ) {
                case FAIL_EXISTS:
                    printf("%sWierzcholek %llu juz jest w grafie%s\n", C_BLUE, ID1, C_DEFAULT);
                    break;

                case FAIL_OTHER:
                    printf("%sNie udalo sie dodac wierzcholka %llu%s\n", C_RED, ID1, C_DEFAULT);
                    break;

                default:
                    printf("%sPomyslnie dodano wierzcholek %llu%s\n", C_GREEN, ID1, C_DEFAULT);
                    break;
            }

            break;


        case ADD_EDGE:
            status = addEdge(firstNode, ID1, ID2);

            switch( status ) {
                case FAIL_NOT_EXIST:
                    printf("%sUpewnij sie, ze wierzcholki %llu i %llu istnieja%s\n", C_MAGENTA, ID1, ID2, C_DEFAULT);
                    break;

                case FAIL_EXISTS:
                    printf("%sWierzcholki %llu i %llu sa juz polaczone krawedzia%s\n", C_YELLOW, ID1, ID2, C_DEFAULT);
                    break;

                case FAIL_OTHER:
                    printf("%sNie udalo sie dodac krawedzi (%llu, %llu)%s\n", C_RED, ID1, ID2, C_DEFAULT);
                    break;

                default:
                    printf("%sPomyslnie dodano krawedz (%llu, %llu)%s\n", C_GREEN, ID1, ID2, C_DEFAULT);
                    break;
            }

            break;


        case DELETE_NODE:
            status = removeNode(&firstNode, ID1);

            switch( status ) {
                case FAIL_NOT_EXIST:
                    printf("%sUpewnij sie, że wierzcholek %llu istnieje%s\n", C_MAGENTA, ID1, C_DEFAULT);
                    break;

                case FAIL_OTHER:
                    printf("%sNie udalo sie usunac wierzcholka %llu%s\n", C_RED, ID1, C_DEFAULT);
                    break;

                default:
                    printf("%sPomyslnie usunieto wierzcholek %llu%s\n", C_GREEN, ID1, C_DEFAULT);
                    break;
            }
            
            break;


        case DELETE_EDGE:
            status = removeEdge(firstNode, ID1, ID2);

            switch( status ) {
                case FAIL_NOT_EXIST:
                    printf("%sUpewnij sie, ze wierzcholki %llu i %llu istnieja oraz sa polaczone krawedzia%s\n", C_MAGENTA, ID1, ID2, C_DEFAULT);
                    break;

                case FAIL_OTHER:
                    printf("%sNie udalo sie usunac krawedzi (%llu, %llu)%s\n", C_RED, ID1, ID2, C_DEFAULT);
                    break;

                default:
                    printf("%sPomyslnie usunieto krawedz (%llu, %llu)%s\n", C_GREEN, ID1, ID2, C_DEFAULT);
                    break;
            }
            break;


        case SAVE:
            if( OK == save(firstNode, filename) )
                printf("%sPomyslnie zapisano liste sasiedztwa grafu do pliku %s%s\n", C_GREEN, filename, C_DEFAULT);
            else
                printf("%sNie udalo sie zapisac listy sasiedztwa grafu do pliku %s%s\n", C_GREEN, filename, C_DEFAULT);
            break;


        case DUMP:
            if( OK == dump(firstNode, filename) )
                printf("%sPomyslnie zapisano liste wierzcholkow grafu do pliku %s%s\n", C_GREEN, filename, C_DEFAULT);
            else
                printf("%sNie udalo sie zapisac listy wierzcholkow grafu do pliku %s%s\n", C_GREEN, filename, C_DEFAULT);
            break;


        case PRINT:
            listNodes(firstNode);
            break;


        case RUN:
            if( NULL == (script = fopen(filename, "r")) )
                printf("%sNie udalo sie otworzyc pliku %s%s\n", C_RED, filename, C_DEFAULT);
            else
                source = script;
                break;

            break;


        case EXIT:
            printf("%sDziekuje! :>%s\n", C_BLUE, C_DEFAULT);
            return 0;
            break;


        case EMPTY_INPUT:
            break;


        default:
            printf("%sNiedozwolone polecenie:%s %s%s\n", C_RED, C_YELLOW, input, C_DEFAULT);
            break;
    };
}