#ifndef __io__
#define __io__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct arq_csv {
    FILE *arquivo;
    unsigned long linhas;
    unsigned long colunas;
    char *tipos;
    long quintopenultima;
    unsigned short *tam_colunas;
} arq_csv;

char* separa(char* linha);
char* formata(char** formatacao, char* linha);

arq_csv* verifica(arq_csv* csv);
arq_csv* abrir(char* file_path);
arq_csv* fechar(arq_csv* file);

void sumario(arq_csv* file);
void exibe(arq_csv* file);

#endif