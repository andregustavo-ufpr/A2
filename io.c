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

char* check_type(char* word){
    char *type = "S";
    
    if(atof(word)){
        type = "N";
    }

    return type;
}

int count_columns_and_define_types(FILE *file, char **types){
    char *keeper;
    char buffer[LINE_SIZE];
    int column_qtd = 1;

    keeper = separa(fgets(buffer, 1024, file));
    while(keeper != NULL){
        keeper = separa(keeper + strlen(keeper) + 1);
        column_qtd++;
    }


    types = (char **) malloc(sizeof(char)*column_qtd);

    int i = 0;
    keeper = separa(fgets(buffer, 1024, file));
    types[i] = check_type(keeper);

    while(keeper != NULL){
        i++;
        keeper = separa(keeper + strlen(keeper) + 1);
        if(keeper){
            types[i] = check_type(keeper);
        }
    }

    rewind(file);
    return column_qtd;
}

int column_position(arq_csv *file, char *title){
    
    int pos=0;
    char *keeper;
    char buffer[LINE_SIZE];
    
    keeper = separa(fgets(buffer, 1024, file));
    while(keeper != NULL && (strcmp(keeper, title) != 0)){
        pos++;
        keeper = separa(keeper + strlen(keeper) + 1);
    }

    rewind(file->arquivo);
    return pos;
}

void find_biggest_word_by_column(arq_csv *file){
    char buffer[LINE_SIZE], *line, *keeper;
    unsigned short* biggest;
    int i = 0;
    

    while (!feof(file->arquivo)){
        line = fgets(buffer, LINE_SIZE, file);
        i = 0;

        while((keeper = separa(line)) != NULL){
            
            if(strlen(keeper) >= biggest[i]){
                biggest[i] = strlen(keeper);
            }
            
            i++;
            keeper = separa(keeper + strlen(keeper) + 1);
        }
    }

    rewind(file->arquivo);
    file->tam_colunas = biggest;
}

int count_lines(FILE *file){
    char buffer[LINE_SIZE], *result;
    int qtt = 0;

    while (!feof(file)){
        result = fgets(buffer, LINE_SIZE, file);
        qtt++;
    }
    
    rewind(file);
    return qtt;
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
        keeper = separa(keeper + strlen(keeper) + 1);
    }
}

void save_all_data(arq_csv *file){
    char buffer[LINE_SIZE], *line, *keeper, **data_matrix;
    unsigned short* biggest;
    int row = 0, col = 0;
    
    data_matrix = (char **) malloc(sizeof(char)*file->colunas*file->linhas);

    while (!feof(file->arquivo)){
        line = fgets(buffer, LINE_SIZE, file);
        row = 0;

        while((keeper = separa(line)) != NULL){
            data_matrix[row][col] = keeper;
            
            col++;
            keeper = separa(keeper + strlen(keeper) + 1);
        }
    }

    rewind(file->arquivo);
    file->dados = data_matrix;
}

arq_csv* abrir(char* file_path){
    FILE *file_opened;
    arq_csv file_object;
    char *keeper;
    char buffer[LINE_SIZE];

    if(!file_path){
        printf("Caminho de arquivo vazio\n");
        return NULL;
    }

    file_opened = fopen(file_path, "r");

    file_object.arquivo = file_opened;
    file_object.colunas = count_columns_and_define_types(file_object.arquivo, file_object.tipos);
    file_object.linhas = count_lines(file_opened);
    find_biggest_word_by_column(&file_object);
    save_all_data(&file_object);
}

void sumario(arq_csv *file){
    int i=0;
    char *header, buffer[LINE_SIZE];
    
    header = separa(fgets(buffer, 1024, file));
    while(header != NULL){
        
        printf("%s [%s] \n", header, file->tipos[i]);

        i++;
        header = separa(header + strlen(header) + 1);
    }

    printf("%d variaveis encontradas\n\n", i);
    rewind(file->arquivo);
}