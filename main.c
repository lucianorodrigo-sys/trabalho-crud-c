#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definicao dos limites do nosso "Banco de Dados"
#define MAX_REGISTROS 50
#define MAX_LETRAS 30

// Protótipos das funções (avisando o C que elas existem)
void inicializar_banco(char banco[][MAX_LETRAS]);
void listar_todos(char banco[][MAX_LETRAS]);
int buscar_nome(char banco[][MAX_LETRAS], char nome[]);
void incluir_nome(char banco[][MAX_LETRAS]);
void modificar_nome(char banco[][MAX_LETRAS]);
void apagar_nome(char banco[][MAX_LETRAS]);

int main() {
    // Criacao da matriz que servira como banco de dados textual
    char banco_dados[MAX_REGISTROS][MAX_LETRAS];
    int opcao;

    // Garante que todas as gavetas comecem vazias (\0)
    inicializar_banco(banco_dados);

    do {
        // Menu interativo para o usuario
        printf("\n=============================\n");
        printf("       SISTEMA CRUD C        \n");
        printf("=============================\n");
        printf("1. Incluir Nome\n");
        printf("2. Buscar Nome\n");
        printf("3. Modificar Nome\n");
        printf("4. Apagar Nome\n");
        printf("5. Listar Todos\n");
        printf("0. Sair\n");
        printf("-----------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o 'Enter' do buffer do teclado

        switch(opcao) {
            case 1:
                incluir_nome(banco_dados);
                break;
            case 2: {
                char nome_busca[MAX_LETRAS];
                printf("Digite o nome para pesquisar: ");
                fgets(nome_busca, MAX_LETRAS, stdin);
                nome_busca[strcspn(nome_busca, "\n")] = '\0'; // Remove o \n do fgets
                
                int indice = buscar_nome(banco_dados, nome_busca);
                if(indice != -1) {
                    printf("Sucesso: Nome encontrado na linha (indice): %d\n", indice);
                } else {
                    printf("Nome nao encontrado.\n");
                }
                break;
            }
            case 3:
                modificar_nome(banco_dados);
                break;
            case 4:
                apagar_nome(banco_dados);
                break;
            case 5:
                listar_todos(banco_dados);
                break;
            case 0:
                printf("Encerrando o sistema...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while(opcao != 0);

    return 0;
}

// Coloca um caractere de fim de string na primeira posicao de cada linha
void inicializar_banco(char banco[][MAX_LETRAS]) {
    for(int i = 0; i < MAX_REGISTROS; i++) {
        banco[i][0] = '\0';
    }
}

// Realiza uma busca linear na matriz. Retorna a linha se achar, ou -1 se nao achar
int buscar_nome(char banco[][MAX_LETRAS], char nome[]) {
    for(int i = 0; i < MAX_REGISTROS; i++) {
        // Se a linha nao esta vazia e o nome for igual ao digitado
        if(banco[i][0] != '\0' && strcmp(banco[i], nome) == 0) {
            return i; 
        }
    }
    return -1; 
}

// Mostra na tela apenas os registros ativos e seus indices
void listar_todos(char banco[][MAX_LETRAS]) {
    printf("\n--- LISTAGEM DE REGISTROS ---\n");
    int tem_registro = 0;
    
    for(int i = 0; i < MAX_REGISTROS; i++) {
        if(banco[i][0] != '\0') {
            printf("Indice [%d]: %s\n", i, banco[i]);
            tem_registro = 1;
        }
    }
    
    if(!tem_registro) {
        printf("O banco de dados esta totalmente vazio.\n");
    }
}

// Adiciona um nome se ele for unico e houver espaco vago
void incluir_nome(char banco[][MAX_LETRAS]) {
    char novo_nome[MAX_LETRAS];
    
    printf("Digite o nome a ser cadastrado: ");
    fgets(novo_nome, MAX_LETRAS, stdin);
    novo_nome[strcspn(novo_nome, "\n")] = '\0';

    // Validacao: impede nomes duplicados
    if(buscar_nome(banco, novo_nome) != -1) {
        printf("Erro: Este nome ja existe no sistema. Escolha um nome unico.\n");
        return; 
    }

    // Busca pela primeira linha vaga para salvar
    for(int i = 0; i < MAX_REGISTROS; i++) {
        if(banco[i][0] == '\0') { 
            strcpy(banco[i], novo_nome);
            printf("Sucesso: Nome gravado no indice %d!\n", i);
            return;
        }
    }

    // Se percorrer tudo e nao achar \0, o banco esta cheio
    printf("Erro: Nao foi possivel salvar. O banco de dados esta cheio!\n");
}

// Altera um nome existente, checando se o novo nome tambem e valido
void modificar_nome(char banco[][MAX_LETRAS]) {
    char nome_antigo[MAX_LETRAS];
    char novo_nome[MAX_LETRAS];

    printf("Digite o nome que deseja alterar: ");
    fgets(nome_antigo, MAX_LETRAS, stdin);
    nome_antigo[strcspn(nome_antigo, "\n")] = '\0';

    // 1. Verifica se o nome antigo existe
    int indice = buscar_nome(banco, nome_antigo);

    if (indice == -1) {
        printf("Erro: Nome nao encontrado para alteracao.\n");
        return; 
    }

    // 2. Solicita o novo nome
    printf("Nome encontrado na linha %d. Digite o novo nome: ", indice);
    fgets(novo_nome, MAX_LETRAS, stdin);
    novo_nome[strcspn(novo_nome, "\n")] = '\0';

    // 3. Validacao: O novo nome nao pode ser igual a nenhum outro ja cadastrado
    if (buscar_nome(banco, novo_nome) != -1) {
        printf("Erro: Esse novo nome ja existe no sistema! Alteracao cancelada.\n");
    } else {
        // Substitui o texto na mesma linha (indice)
        strcpy(banco[indice], novo_nome);
        printf("Sucesso: Nome alterado com sucesso!\n");
    }
}

// "Apaga" o registro limpando a primeira posicao da linha correspondente
void apagar_nome(char banco[][MAX_LETRAS]) {
    char nome_apagar[MAX_LETRAS];
    
    printf("Digite o nome que deseja remover: ");
    fgets(nome_apagar, MAX_LETRAS, stdin);
    nome_apagar[strcspn(nome_apagar, "\n")] = '\0';

    // Encontra onde o nome esta guardado
    int indice = buscar_nome(banco, nome_apagar);

    if (indice != -1) {
        // Define a linha como vazia reiniciando o primeiro caractere para '\0'
        banco[indice][0] = '\0';
        printf("Sucesso: Nome removido e espaco liberado!\n");
    } else {
        printf("Erro: Nome nao encontrado no sistema.\n");
    }
}
