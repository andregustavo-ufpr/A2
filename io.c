#include "io.h"

char* separa(char* linha){
    char *pos_virgula, *str = strdup(linha);
    
    if(!linha) return NULL;
    
    pos_virgula = strchr(str, ',');

    if(pos_virgula == NULL){
        if(strlen(linha) >0){
            
            linha[strlen(linha) - 1] = '\0';
            free(str);
            
            return linha;
        }
        return NULL;
    }

    *pos_virgula = '\0';
    return str;
}

char* check_type(char* word){
    char *type = "S";
    
    if(atof(word)){
        type = "N";
    }

    return type;
}

int count_columns(FILE *file){
    char *keeper;
    char buffer[LINE_SIZE];
    int column_qtd = 1;

    keeper = separa(fgets(buffer, 1024, file));
    while(keeper != NULL){
        keeper = separa(keeper + strlen(keeper) + 1);
        if(keeper != NULL) column_qtd++;
    }
    free(keeper);
    rewind(file);
    return column_qtd;
}

void define_types(char **types, FILE *file){
    int i = 0;
    char *keeper, buffer[LINE_SIZE];

    fgets(buffer, 1024, file);
    keeper = separa(fgets(buffer, 1024, file));
    
    types[i] = check_type(keeper);

    while(keeper != NULL){
        i++;
        keeper = separa(keeper + strlen(keeper) + 1);
    
        if(keeper != NULL){
            types[i] = check_type(keeper);
        }
    }
    free(keeper); 
    rewind(file);
}

int column_position(arq_csv *file, char *title){
    
    int pos=0;
    char *keeper;
    char buffer[LINE_SIZE];
    
    keeper = separa(fgets(buffer, 1024, file->arquivo));
    while(keeper != NULL && (strcmp(keeper, title) != 0)){
        pos++;
        keeper = separa(keeper + strlen(keeper) + 1);
    }

    free(keeper);
    rewind(file->arquivo);
    return pos;
}

int number_of_digits(int n)  
{  
    int counter=0; // variable declaration  
    while(n!=0)  
    {  
        n=n/10;  
        counter++;  
    }  
    return counter;  
} 

void find_biggest_word_by_column(arq_csv *file){
    char buffer[LINE_SIZE], *line = NULL, *keeper;
    unsigned short* biggest;
    int i = 0, line_counter = 0;
    
    biggest = (short *) malloc(sizeof(short)*file->linhas);
    rewind(file->arquivo);
    while(!feof(file->arquivo)){

        line = fgets(buffer, LINE_SIZE, file->arquivo);
        if(line != NULL){

            keeper = separa(line);
            i= 0;
            while(keeper != NULL && i < file->colunas){
                
                if(line_counter == 0){
                    biggest[i] = 0;
                }
                
                if(strlen(keeper) >= biggest[i]){
                    biggest[i] = strlen(keeper);
                }
            
                i++;
                keeper = separa(keeper + strlen(keeper) + 1);
            }
        }
        free(keeper);
        line_counter++;
    }

    rewind(file->arquivo);
    file->tam_colunas = biggest;
}

int count_lines(FILE *file){
    char buffer[LINE_SIZE], *result;
    int qtt = 0;

    while (fgets(buffer, LINE_SIZE, file) != NULL){
        if(!feof(file)){
            if(!(strlen(buffer) == 0 || (strlen(buffer) == 1 && buffer[0] == '\n'))){
                qtt++;
            }
        }
    }
    
    rewind(file);
    return qtt;
}

void add_espaco(char** word, int space_qtt){
    int len = strlen(*word);
    int new_len = len + space_qtt;

    char *new_word = (char *) malloc((new_len +1) *sizeof(char));
    if(!new_word){
        printf("Falha na alocacao\n");
        return;
    }

    for(int i = 0; i < space_qtt; i++){
        new_word[i] = ' ';
    }
    
    strcpy(new_word+space_qtt, *word);

    *word = new_word;
}

void formata(char **formatacao, char* linha, unsigned short *maiores){
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
            break;
        }

        if((tam_palavra == 0) || (strcmp(keeper, "\n")) == 0){
            add_espaco(&word, num_espaco - 3);
            strcat(word, "NaN");
        }
        else{
            add_espaco(&word, num_espaco);
            strcat(word, keeper);
        }

        formatacao[i++] = word;
        keeper = separa(keeper + strlen(keeper) + 1);
    }
    free(keeper);
}

void save_all_data(arq_csv *file){
    char buffer[LINE_SIZE], *line, *keeper, ***data_matrix, *x;
    unsigned short* biggest;
    int row = 0, col = 0, i=0;
    
    data_matrix = (char ***) malloc(sizeof(char **)*file->linhas);
    for(i = 0; i< file->linhas; i++) data_matrix[i] = (char **)malloc(sizeof(char *)*file->colunas);
    
    line = fgets(buffer, LINE_SIZE, file->arquivo);

    while(line != NULL){
        line = fgets(buffer, LINE_SIZE, file->arquivo);
        
        
        for(row = 0; row < file->linhas && line != NULL; row++){
            keeper = separa(line);
            
            for(col = 0; col < file->colunas && keeper != NULL; col++){
                data_matrix[row][col] = keeper;
                keeper = separa(keeper + strlen(keeper) + 1);
            }
            
            line = fgets(buffer, LINE_SIZE, file->arquivo);
        }
        free(keeper);


    }
    
    rewind(file->arquivo);
    file->dados = data_matrix;
}

arq_csv* abrir(char* file_path){
    FILE *file_opened;
    arq_csv* file_object;
    char *keeper;
    char buffer[LINE_SIZE];

    if(!file_path){
        printf("Caminho de arquivo vazio\n");
        return NULL;
    }

    file_opened = fopen(file_path, "r");
    if(!file_opened){
        printf("Falha ao abrir o arquivo");
        return NULL;
    }

    file_object = (arq_csv *) malloc(sizeof(arq_csv));

    file_object->arquivo = file_opened;
    file_object->colunas = count_columns(file_object->arquivo);

    file_object->tipos = (char **) malloc(sizeof(char *)*file_object->colunas);
    define_types(file_object->tipos, file_object->arquivo);
    
    file_object->linhas = count_lines(file_opened);
    file_object->dados = malloc(sizeof(char)*file_object->linhas*file_object->colunas); 
    
    find_biggest_word_by_column(file_object);
    save_all_data(file_object);

    return file_object;
}

void sumario(arq_csv *file){
    int i=0;
    char *header, *line;
    char buffer[LINE_SIZE];
    
    if(file->arquivo){
    
        header = separa(fgets(buffer, LINE_SIZE, file->arquivo));
        
        while(header != NULL){
            printf("%s [%s] \n", header, file->tipos[i]);

            i++;
            header = separa(header + strlen(header) + 1);
        }

        printf("%d variaveis encontradas\n\n", i);
        rewind(file->arquivo);
    }
}

void mostrar(arq_csv *file){
    int i=0;
    char *keeper, *header, **format;
    char buffer[LINE_SIZE];
    
    header = separa(fgets(buffer, LINE_SIZE, file->arquivo));
    int j = 0;
    while(header != NULL){
        //add_espaco(&header, file->tam_colunas[j] - strlen(header));
        printf("%s ", header); 
        header = separa(header +strlen(header) +1);
        j++;
    }
    printf("\n");

    //Printando da linha 0 -> 5
    for (i = 0; i < 5; i++){
        for(j = 0; j < file->colunas; j++){
            char *data= file->dados[i][j];
            
            add_espaco(&data, file->tam_colunas[j] - strlen(data));
            printf("%s ", data);

            free(data);
        }
        printf("\n");
    }

    //Printando ...
    for(int i = 0; i < file->colunas; i++){
        char *ellipsis= "...";
        
        add_espaco(&ellipsis, file->tam_colunas[i] - strlen(ellipsis));
        printf("%s ", ellipsis);
        free(ellipsis);
    }
    printf("\n");

    //Printando da linha (linhas -4) -> (linhas)
    for (i = file->linhas - 5; i < file->linhas -1; i++){
        for(j = 0; j < file->colunas; j++){
            char *data= file->dados[i][j];
            
            add_espaco(&data, file->tam_colunas[j] - strlen(data));
            printf("%s ", data);
            free(data);
        }
        printf("\n");
    }

    printf("\n[%d rows x %d columns]\n", file->linhas, file->colunas);
}

arq_csv *fechar(arq_csv *file){
    fclose(file->arquivo);
    
    for(int i=0; i<file->linhas;i++) free(file->dados[i]);
    for(int j=0; j<file->colunas; j++) free(file->tipos[j]);
    file->linhas = 0;
    file->colunas = 0;
    
    free(file->tipos);
    free(file->dados);
    free(file->tam_colunas);

    return file;
}
