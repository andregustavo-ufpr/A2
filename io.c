#include "io.h"

char* separa(char* linha){
    char *pos_virgula;
    
    if(!linha) return NULL;
    
    pos_virgula = strchr(linha, ',');
    
    if(!pos_virgula){
        if(strlen(linha)) return linha;
        
        return NULL;
    }
    *pos_virgula = NULL;
    
    return linha;
}

void add_espaco(char* word, int qtd_espacos){
    for(int i = 0; i < qtd_espacos; i++){
        strcat(word, " ");
    }
}

char* formata(char **formatacao, char* linha, unsigned short *maiores){
    int i = 0;
    unsigned short tam_palavra = 0, num_espaco;
    char *keeper, *word;

    keeper = separa(linha);
    
    while(keeper){
        tam_palavra = strlen(keeper);

        num_espaco = maiores[i] - tam_palavra;

        word = malloc(maiores[i] + 1);
        if(!word){
            printf("Erro na alocação\n");
            return NULL;
        }

        if((tam_palavra == 0) || (strcmp(keeper, "\n")) == 0){
            add_espaco(word, num_espaco - 3);
            strcat(word, "NaN");
        }
        else{
            add_espaco(word, num_espaco);
            strcat(word, keeper);
        }

        formatacao[i++] = word;
        keeper = separa(keeper + strlen(keeper) +1 );
    }
}
