/*!
 * \file Input.c
 * \brief Moduł I/O (obsługujący standardowe wejście, wyjście i skrypty).
 * 
 * POPRO - 23Z - Zadanie projektowe.
 * 
 * Nazwa zadania:           Grafy.
 * Grupa projektowa:        Zespół nr 1.
 * Nazwa pliku:             Input.c
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "includes/Input.h"


/*!
 * \var Commands_def
 * \brief Pomocnicza definicja poleceń, które użytkownik może wprowadzić do wiersza lub skryptu.
 */
const char* Commands_def[COMMANDS] = {
    "Exit",
    "Save",
    "Dump",
    "Print",
    "Run",
    "A",
    "R"
};

/*!
 * \fn getCommand(FILE* source, uint64_t* ID1, uint64_t* ID2, char* filename, char* input)
 * \brief Odczytywanie wierszy po kolei z danego źródła (pliku lub stdin).
 * 
 * Funkcja wczytuje linie tekstu z podanego źródła oraz sprawdza, czy
 * odpowiadają one wzorcowi ustalonemu wcześniej. Zapisuje dane do
 * przygotowanych wcześniej komórek pamięci.
 * 
 * \param source Wskaźnik do pliku bądź standardowego wejścia.
 * \param ID1 Adres zmiennej na identyfikator pierwszy (lub podstawowy, w przypadku operacji jednoargumentowych).
 * \param ID2 Adres zmiennej na identyfikator drugi.
 * \param filename Wskaźnik do miejsca, w którym można przechować nazwę pliku dla funkcji Save i Dump.
 * \param input Wskaźnik do miejsca, w którym można zapisać treść aktualnie analizowanego polecenia na wypadek błędnego inputu.
 * 
 */
Commands_t getCommand(FILE* source, uint64_t* ID1, uint64_t* ID2, char* filename, char* input)
{
    char word[WORD_SIZE];

    if( feof(source) || NULL == source )
        source = stdin;

    // Wiersz poleceń
    if( source == stdin )
        printf("%s$ %s", C_CYAN, C_DEFAULT);

    // Sprawdzanie linii po kolei
    if( NULL != fgets(input, sizeof(char) * LINE_SIZE, source) ) 
    {
        size_t len = strlen(input);

        if( input[len - 1] == '\n' )
            input[len - 1] = '\0';

        if( sscanf(input, "%s", word) )
            if( strcmp(word, Commands_def[1]) && strcmp(word, Commands_def[2]) && strcmp(word, Commands_def[3]) && strcmp(word, Commands_def[4]) )
            {
                clearSpaces(input);
                replaceSeparators(input);

                if( '\0' == input[0] )
                    return EMPTY_INPUT;
            }
            
        if( sscanf(input, "%s", word) )
        {
            Commands_t command = seekOption(word);

            switch(command)
            {
                case EXIT:
                case PRINT:
                    return command;

                case SAVE:
                case DUMP:
                case RUN:
                    if ( 2 == sscanf(input, "%s %s", word, filename) )
                        return command;

                    return INCORRECT_INPUT;

                case ADD:
                case DELETE:
                    {
                        char id_char_1[21], id_char_2[21];

                        int options = sscanf(input, "%s %s %s", word, id_char_1, id_char_2 );

                        if( 2 == options )
                        {
                            if( '-' == id_char_1[0] || 2 != sscanf(input, "%s %llu", word, ID1) )
                                return INCORRECT_INPUT;

                            return ( ADD == command ) ? ADD_NODE: DELETE_NODE;
                        }

                        else if( 3 == options )
                        {
                            if( '-' == id_char_1[0] || '-' == id_char_2[0] || 3 != sscanf(input, "%s %llu %llu", word, ID1, ID2) )
                                return INCORRECT_INPUT;

                            return ( ADD == command ) ? ADD_EDGE : DELETE_EDGE;
                        }

                        return INCORRECT_INPUT;
                    }

                default:
                    return INCORRECT_INPUT;

            }
        }
    }
    else
    {
        source = stdin;
        return EMPTY_INPUT;
    }
}


/*!
 * \fn seekOption(char* word)
 * \brief Sprawdzenie słowa kluczowego w komendzie.
 * 
 * Funkcja odwołuje się do Commands_def i sprawdza, czy podane słowo odpowiada któremukolwiek ze wzorców.
 * W razie problemu zwraca INCORRECT_INPUT.
 * 
 * \param word Fraza do analizy.
 */
Commands_t seekOption(char* word)
{
    for(int i = 0; i < COMMANDS; ++i)
        if( !strcmp(word, Commands_def[i]) )
            return i;

    return INCORRECT_INPUT;
}


/*!
 * \fn clearSpaces(char* line)
 * \brief Oczyszczanie linii wejścia kolejno ze spacji i ze znaków specjalnych.
 * 
 * \param line Linia wejścia do oczyszczenia.
 */
void clearSpaces(char* line)
{
    char* d = line;
    do {
        while (*d == ' ' || *d == '\t') {
            ++d;
        }
    } while (*line++ = *d++);
}


/*!
 * \fn replaceSeparators(char* line)
 * \brief Zamiana znaków charakterystycznych dla wybranych poleceń na spacje.
 * 
 * \param line Linia wejścia do analizy.
 */
void replaceSeparators(char* line)
{
    do
    {
        if( *line == ',' || *line == ':' ) 
            *line = ' ';

        line++;
    } while(*line != '\0');
}
