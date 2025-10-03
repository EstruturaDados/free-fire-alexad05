#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Biblioteca necessária para medir o tempo (clock_t, clock())

// --- ESTRUTURAS ---

/**
 * @brief Define a estrutura de um componente para a torre de fuga.
 *
 * Armazena o nome, tipo e a prioridade de montagem do componente. [cite: 1221]
 */
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Componente;


// --- PROTÓTIPOS DAS FUNÇÕES (ASSINATURAS) ---

// Função auxiliar para troca de componentes
void trocar(Componente* a, Componente* b);

// Funções de gerenciamento de componentes
void cadastrarComponente(Componente componentes[], int* total);
void mostrarComponentes(const Componente componentes[], int total);

// Funções de ordenação
void bubbleSortNome(Componente componentes[], int total);
void insertionSortTipo(Componente componentes[], int total);
void selectionSortPrioridade(Componente componentes[], int total);

// Função de busca
int buscaBinariaPorNome(Componente componentes[], int total, const char* nomeBusca);


// --- FUNÇÃO PRINCIPAL ---

int main() {
    Componente torreComponentes[20]; // Capacidade para até 20 componentes [cite: 1223]
    int totalComponentes = 0;
    int opcao;
    // Flag para verificar se o vetor está ordenado por nome para a busca binária
    int ordenadoPorNome = 0;

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    do {
        printf("\n--- MENU DE MONTAGEM DA TORRE DE FUGA ---\n");
        printf("1. Cadastrar Componente\n");
        printf("2. Listar Componentes\n");
        printf("3. Ordenar por Nome (Bubble Sort)\n");
        printf("4. Ordenar por Tipo (Insertion Sort)\n");
        printf("5. Ordenar por Prioridade (Selection Sort)\n");
        printf("6. Buscar Componente-Chave (Busca Binaria por Nome)\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer do teclado (consome o '\n' deixado pelo scanf)

        switch (opcao) {
            case 1:
                cadastrarComponente(torreComponentes, &totalComponentes);
                ordenadoPorNome = 0; // Cadastro novo pode desordenar a lista
                break;
            case 2:
                mostrarComponentes(torreComponentes, totalComponentes);
                break;
            case 3:
                bubbleSortNome(torreComponentes, totalComponentes);
                ordenadoPorNome = 1; // Agora está ordenado por nome
                break;
            case 4:
                insertionSortTipo(torreComponentes, totalComponentes);
                ordenadoPorNome = 0; // A ordem por nome foi alterada
                break;
            case 5:
                selectionSortPrioridade(torreComponentes, totalComponentes);
                ordenadoPorNome = 0; // A ordem por nome foi alterada
                break;
            case 6:
                if (!ordenadoPorNome) {
                    printf("\nERRO: A busca binaria so funciona se os componentes estiverem ordenados por nome.\n");
                    printf("Por favor, use a opcao 3 primeiro.\n");
                } else {
                    char nomeChave[30];
                    printf("Digite o nome do componente-chave a ser buscado: ");
                    fgets(nomeChave, 30, stdin);
                    nomeChave[strcspn(nomeChave, "\n")] = 0; // Remove o '\n' do final

                    int pos = buscaBinariaPorNome(torreComponentes, totalComponentes, nomeChave);
                    if (pos != -1) {
                        printf("\nSUCESSO! Componente-chave '%s' encontrado na posicao %d.\n", nomeChave, pos);
                        printf("Montagem da torre autorizada!\n");
                    } else {
                        printf("\nFALHA! Componente-chave '%s' nao encontrado. Impossivel montar a torre.\n", nomeChave);
                    }
                }
                break;
            case 0:
                printf("Saindo do sistema de montagem...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}


// --- IMPLEMENTAÇÃO DAS FUNÇÕES ---

/**
 * @brief Troca dois componentes de posição.
 *
 * Função auxiliar utilizada pelos algoritmos de ordenação.
 * @param a Ponteiro para o primeiro componente.
 * @param b Ponteiro para o segundo componente.
 */
void trocar(Componente* a, Componente* b) {
    Componente temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Cadastra um novo componente na lista.
 *
 * @param componentes O vetor de componentes.
 * @param total Ponteiro para o contador total de componentes.
 */
void cadastrarComponente(Componente componentes[], int* total) {
    if (*total >= 20) {
        printf("Inventario de componentes cheio!\n");
        return;
    }
    printf("\n--- Cadastro de Novo Componente ---\n");
    printf("Nome do componente: ");
    fgets(componentes[*total].nome, 30, stdin);
    componentes[*total].nome[strcspn(componentes[*total].nome, "\n")] = 0;

    printf("Tipo (controle, suporte, propulsao): ");
    fgets(componentes[*total].tipo, 20, stdin);
    componentes[*total].tipo[strcspn(componentes[*total].tipo, "\n")] = 0;

    printf("Prioridade (1 a 10): ");
    scanf("%d", &componentes[*total].prioridade);
    getchar(); // Limpa o buffer

    (*total)++;
    printf("Componente cadastrado com sucesso!\n");
}

/**
 * @brief Exibe todos os componentes cadastrados.
 *
 * @param componentes O vetor de componentes.
 * @param total O número de componentes no vetor.
 */
void mostrarComponentes(const Componente componentes[], int total) {
    if (total == 0) {
        printf("\nNenhum componente cadastrado.\n");
        return;
    }
    printf("\n--- Lista de Componentes da Torre ---\n");
    for (int i = 0; i < total; i++) {
        printf("Indice %d: Nome: %-15s | Tipo: %-10s | Prioridade: %d\n",
               i, componentes[i].nome, componentes[i].tipo, componentes[i].prioridade);
    }
    printf("-------------------------------------\n");
}

/**
 * @brief Ordena os componentes por nome usando o algoritmo Bubble Sort. [cite: 1226]
 *
 * @param componentes O vetor de componentes.
 * @param total O número de componentes no vetor.
 */
void bubbleSortNome(Componente componentes[], int total) {
    long long comparacoes = 0;
    clock_t inicio = clock(); // Mede o tempo de início

    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - 1 - i; j++) {
            comparacoes++; // Incrementa o contador de comparações [cite: 1233]
            // Usa strcmp para comparar strings (nomes)
            if (strcmp(componentes[j].nome, componentes[j + 1].nome) > 0) {
                trocar(&componentes[j], &componentes[j + 1]);
            }
        }
    }

    clock_t fim = clock(); // Mede o tempo de fim
    double tempoGasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC; // Calcula o tempo [cite: 1234]

    printf("\nComponentes ordenados por NOME (Bubble Sort).\n");
    printf("Numero de comparacoes: %lld\n", comparacoes);
    printf("Tempo de execucao: %f segundos\n", tempoGasto);
    mostrarComponentes(componentes, total);
}


/**
 * @brief Ordena os componentes por tipo usando o algoritmo Insertion Sort. [cite: 1227]
 *
 * @param componentes O vetor de componentes.
 * @param total O número de componentes no vetor.
 */
void insertionSortTipo(Componente componentes[], int total) {
    long long comparacoes = 0;
    clock_t inicio = clock();

    for (int i = 1; i < total; i++) {
        Componente chave = componentes[i];
        int j = i - 1;

        // Move os elementos que são maiores que a chave para uma posição à frente
        while (j >= 0) {
            comparacoes++;
            if (strcmp(componentes[j].tipo, chave.tipo) > 0) {
                componentes[j + 1] = componentes[j];
                j--;
            } else {
                break;
            }
        }
        componentes[j + 1] = chave;
    }

    clock_t fim = clock();
    double tempoGasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("\nComponentes ordenados por TIPO (Insertion Sort).\n");
    printf("Numero de comparacoes: %lld\n", comparacoes);
    printf("Tempo de execucao: %f segundos\n", tempoGasto);
    mostrarComponentes(componentes, total);
}


/**
 * @brief Ordena os componentes por prioridade usando o algoritmo Selection Sort. [cite: 1228]
 *
 * @param componentes O vetor de componentes.
 * @param total O número de componentes no vetor.
 */
void selectionSortPrioridade(Componente componentes[], int total) {
    long long comparacoes = 0;
    clock_t inicio = clock();

    for (int i = 0; i < total - 1; i++) {
        int indiceMenor = i;
        for (int j = i + 1; j < total; j++) {
            comparacoes++;
            // Compara as prioridades (inteiros)
            if (componentes[j].prioridade < componentes[indiceMenor].prioridade) {
                indiceMenor = j;
            }
        }
        if (indiceMenor != i) {
            trocar(&componentes[i], &componentes[indiceMenor]);
        }
    }

    clock_t fim = clock();
    double tempoGasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("\nComponentes ordenados por PRIORIDADE (Selection Sort).\n");
    printf("Numero de comparacoes: %lld\n", comparacoes);
    printf("Tempo de execucao: %f segundos\n", tempoGasto);
    mostrarComponentes(componentes, total);
}


/**
 * @brief Busca um componente pelo nome usando o algoritmo de Busca Binária. [cite: 1230]
 *
 * @param componentes O vetor de componentes (deve estar ordenado por nome).
 * @param total O número de componentes no vetor.
 * @param nomeBusca O nome do componente a ser procurado.
 * @return O índice do componente se encontrado, ou -1 caso contrário.
 */
int buscaBinariaPorNome(Componente componentes[], int total, const char* nomeBusca) {
    int inicio = 0;
    int fim = total - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        // Compara o nome buscado com o nome no meio do vetor
        int res = strcmp(nomeBusca, componentes[meio].nome);

        if (res == 0) { // Encontrou
            return meio;
        }
        if (res > 0) { // O nome buscado está na metade direita (alfabeticamente depois)
            inicio = meio + 1;
        } else { // O nome buscado está na metade esquerda (alfabeticamente antes)
            fim = meio - 1;
        }
    }
    return -1; // Não encontrado
}