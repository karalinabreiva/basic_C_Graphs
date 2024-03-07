/*!
 * \file Input.h
 * \brief Definicje niezbędne do obsługi I/O.
 * 
 * POPRO - 23Z - Zadanie projektowe.
 * 
 * Nazwa zadania:           Grafy.
 * Grupa projektowa:        Zespół nr 1.
 * Nazwa pliku:             Input.h
 * 
 * Definicje funkcji obsługujących wejście/wyjście oraz typów wspomagających
 * działanie standardowego wyjścia na konsoli.
 * 
 * \see Nodes.h
 * \see File.h
 */


#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

/*!
 * \def LINE_SIZE
 * \brief Maksymalna dozwolona długość jednej linii wejścia.
 */
#define LINE_SIZE   100

/*!
 * \def WORD_SIZE
 * \brief Maksymalna dozwolona długość jednego słowa w linii wejścia.
 */
#define WORD_SIZE   50

/*!
 * \def COMMANDS
 * \brief Liczba obsługiwanych komend.
 *
 * Makro wspomagające Commands_def.
 * 
 * \see Commands_def
 * \see Input.c
 */
#define COMMANDS    7

/**
 * \defgroup C_COLORS Makra definiujące kolory w terminalu.
 * @{
 */

/** \brief Wywołanie domyślnego stanu koloru w terminalu. **/
#define C_DEFAULT   "\x1B[0m"

/** \brief Kolor czerwony. **/
#define C_RED       "\x1B[31m" 

/** \brief Kolor zielony. **/
#define C_GREEN     "\x1B[32m" 

/** \brief Kolor żółty. **/
#define C_YELLOW    "\x1B[33m" 

/** \brief Kolor niebieski. **/
#define C_BLUE      "\x1B[34m" 

/** \brief Kolor magenta. **/
#define C_MAGENTA   "\x1B[35m" 

/** \brief Kolor cyjan. **/
#define C_CYAN      "\x1B[36m" 

/** \brief Kolor biały. **/
#define C_WHITE     "\x1B[37m" 

/**@}*/


/*!
 * \enum Commands_t
 * \brief Definicje możliwych trybów pracy aplikacji. Liczenie od 0.
 * 
 * \see Input.c
 */
typedef enum {
    EXIT,
    SAVE,
    DUMP,
    PRINT,
    RUN,
    ADD,
    DELETE,
    ADD_NODE,
    ADD_EDGE,               
    DELETE_NODE,
    DELETE_EDGE,
    INCORRECT_INPUT = -1,
    EMPTY_INPUT = -2
} Commands_t;

extern const char* Commands_def[COMMANDS];

void clearSpaces(char* line);
void replaceSeparators(char* line);

Commands_t getCommand(FILE* source, uint64_t* ID1, uint64_t* ID2, char* filename, char* input);
Commands_t seekOption(char* word);

#endif
