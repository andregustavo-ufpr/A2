#include "io.h"

void print_menu(){
printf("1) Sumário do Arquivo\n");
printf("2) Mostrar\n");
printf("3) Filtros\n");
printf("4) Descrição dos Dados\n");
printf("5) Ordenação\n");
printf("6) Seleção\n");
printf("7) Dados Faltantes\n");
printf("8) Salvar Dados\n");
printf("9) Fim\n");
}

int main(int argc, char **argv){
    int menu = 0;
    char *entry = 0, buffer[LINE_SIZE];
    arq_csv *file;

    entry = strdup(argv[1]);
    file = abrir(entry);
    
    if(file == NULL || !file->arquivo){
        return 0;
    }

    while (menu != 9){
        print_menu();
        scanf("%d", &menu);

        switch (menu)
        {
        case 1:
            sumario(file);
            break;
        case 2:
            mostrar(file);
            break;
        default:
            break;
        }

        if(menu != 9){
            printf("\nPressione ENTER para continuar.\n");
            getchar();
        }
    }

    
}
