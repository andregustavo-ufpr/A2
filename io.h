#ifndef __io__
#define __io__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LINE_SIZE 1024

typedef struct arq_csv {
    FILE *arquivo;
    unsigned long linhas;
    unsigned long colunas;
    char **tipos;
    unsigned short *tam_colunas;
    char **dados;
} arq_csv;

char* separa(char* linha);
char* formata(char** formatacao, char* linha, unsigned short *maiores);

arq_csv* verifica(arq_csv* csv);

// Funcoes de Setup
arq_csv* abrir(char* file_path);
void save_all_data(arq_csv *file);
void find_biggest_word_by_column(arq_csv *file);
int count_lines(FILE *file);
char* check_type(char* word);
int count_columns_and_define_types(FILE *file, char **types);
arq_csv* fechar(arq_csv* file);

//Funcoes auxiliares
int column_position(arq_csv *file, char *title);
void add_espaco(char *word, int space_qtt);

//Funcoes para o leitor csv
void sumario(arq_csv* file);
void mostrar(arq_csv* file);

#endif