#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NOME 100
#define MAX_EMAIL 50
#define MAX_CLIENTES 100
#define MAX_PEDIDOS 500

typedef struct Clientes {
    long long int cpf;
    char nome[MAX_NOME];
    char email[MAX_EMAIL];
} Clientes;

typedef struct Pedidos
{
    int id_pedido;
    long long int cpf;
}Pedidos;

bool validarCPF(long long int cpf) {
    int digitos[11];
    int i, soma, resto;

    for (i = 10; i >= 0; i--) {
        digitos[i] = cpf % 10;
        cpf /= 10;
    }

    soma = 0;
    for (i = 0; i < 9; i++) {
        soma += digitos[i] * (10 - i);
    }
    resto = soma % 11;
    int digito1 = (resto < 2) ? 0 : 11 - resto;

    if (digito1 != digitos[9]) {
        return false;
    }

    soma = 0;
    for (i = 0; i < 10; i++) {
        soma += digitos[i] * (11 - i);
    }
    resto = soma % 11;
    int digito2 = (resto < 2) ? 0 : 11 - resto;

    if (digito2 != digitos[10]) {
        return false;
    }

    return true;
}
int quantidadeClientes() {
    int qnt = 0;
    char car;

    FILE *arq = fopen("clientes.txt", "r");
    if (arq == NULL) {
        fclose(arq);
        printf("Erro ao abrir o arquivo clientes...\n");
        printf("\nCriando arquivo...\n");
        arq = fopen("clientes.txt", "w");
        fclose(arq);
        return 0;
    }

    while (fscanf(arq, "%c", &car) != EOF) {
        if (car == '\n') {
            qnt++;
        }
    }

    qnt = qnt / 3;
    fclose(arq);
    return qnt;
}

void cadastrarCliente(Clientes *clientes, int *qnt) {
    *qnt += 1;
    if (*qnt > MAX_CLIENTES) {
        printf("Limite de clientes atingido!\n");
        printf("Realocando memória...\n");
        clientes = realloc(clientes, (MAX_CLIENTES * 2) * sizeof(clientes));
    }

    printf("========================================================================================================================\n\n\t\t\t\t\t\tCadastrar novo cliente:\n\n========================================================================================================================");
    printf("\nDigite o nome do cliente: ");
    scanf(" %[^\n]", clientes[*qnt - 1].nome);

    bool cpfValido = false;

    while (!cpfValido) {
        printf("Digite o CPF do cliente: ");
        scanf("%lld", &(clientes[*qnt - 1].cpf));

        if (validarCPF(clientes[*qnt - 1].cpf)) {
            printf("CPF valido!\n");
            cpfValido = true;
        } else {
            printf("CPF invalido! Tente novamente.\n");
        }
    }
    

    printf("Digite o email do cliente: ");
    scanf(" %[^\n]", clientes[*qnt - 1].email);

    registrarClienteArquivo(clientes[*qnt - 1]);
}

void registrarClienteArquivo(Clientes cliente) {
    FILE *arq = fopen("clientes.txt", "a+");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fprintf(arq, "%lld\n", cliente.cpf);
    fprintf(arq, "%s\n", cliente.nome);
    fprintf(arq, "%s\n", cliente.email);

    fclose(arq);
}

void listarClientes(Clientes *clientes, int qnt) {
    int op;

    printf("========================================================================================================================\n\n\t\t\t\t\t\tListar clientes:\n\n========================================================================================================================");
    printf("\nDeseja listar os clientes em qual formato?\n1 - Ordem numerica: Ordem de inclusao\n2 - Ordem alfabetica\n");
    scanf("%d", &op);

    switch (op) {
        case 1:
            listarOrdemInclusao(clientes, qnt);
            break;
        case 2:
            listarOrdemAlfabetica(clientes, qnt);
            break;
        default:
            printf("Opcao invalida!\n");
            break;
    }
}

void listarOrdemInclusao(Clientes *clientes, int qnt) {
    FILE *arq = fopen("clientes.txt", "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    system("cls");

    for (int i = 0; i < qnt; i++) {
        fscanf(arq, "%lld", &clientes[i].cpf);
        fscanf(arq, " %[^\n]", clientes[i].nome);
        fscanf(arq, "%s", clientes[i].email);

        printf("\nCliente %d:\n", i + 1);
        printf("CPF: %lld\n", clientes[i].cpf);
        printf("Nome: %s\n", clientes[i].nome);
        printf("Email: %s\n", clientes[i].email);
        printf("\n");
    }

    fclose(arq);
}

int comparar(const void *a, const void *b) {
    const Clientes *c1 = (const Clientes *)a;
    const Clientes *c2 = (const Clientes *)b;
    return strcmp(c1->nome, c2->nome);
}

void listarOrdemAlfabetica(Clientes *clientes, int qnt) {
    FILE *arq = fopen("clientes.txt", "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Clientes *listagem = (Clientes *)malloc(qnt * sizeof(Clientes));
    if (listagem == NULL) {
        printf("Erro ao alocar memória.\n");
        return;
    }

    for (int i = 0; i < qnt; i++) {
        fscanf(arq, "%lld", &listagem[i].cpf);
        fscanf(arq, " %[^\n]", listagem[i].nome);
        fscanf(arq, "%s", listagem[i].email);
    }

    qsort(listagem, qnt, sizeof(Clientes), comparar);

    system("cls");
    for (int i = 0; i < qnt; i++) {
        printf("\nCliente %d:\n", i + 1);
        printf("CPF: %lld\n", listagem[i].cpf);
        printf("Nome: %s\n", listagem[i].nome);
        printf("Email: %s\n", listagem[i].email);
        printf("\n");
    }

    free(listagem);
    fclose(arq);
}

void pesquisarCliente(Clientes *cliente, int qnt){

    long long int cpf, cpfc = 0;
    int pos = -1;

    FILE *arq = fopen("clientes.txt", "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("Informe CPF: ");
    scanf("%lld", &cpf);

    Clientes *listagem = (Clientes *)malloc(qnt * sizeof(Clientes));

    for (int i = 0; i < qnt; i++) {
        fscanf(arq, "%lld", &(listagem + i)->cpf);
        if (cpf == (listagem+i)->cpf)
        {
            cpfc = (listagem+i)->cpf;
            pos = i;
        }
        fscanf(arq, " %[^\n]", (listagem + i)->nome);
        fscanf(arq, "%s", (listagem + i)->email);
    }
    system("cls");
    if(pos != -1){
        printf("\nCPF: %lld \nNome: %s \nEmail: %s ", cpfc, (listagem+pos)->nome, (listagem+pos)->email);
        printf("\n");
    } else {
        printf("CPF não encontrado.\n");
    }

    fclose(arq);
    free(listagem);
}

int quantidadePedidos() {
    FILE *arq = fopen("pedidos.txt", "r");
    if (arq == NULL) {
        fclose(arq);
        printf("Erro ao abrir o arquivo pedidos...\n");
        printf("\nCriando arquivo...\n");
        arq = fopen("pedidos.txt", "w");
        fclose(arq);
        return 0;

    }

    int qnt_pedido = 0;
    char car;

    while (fscanf(arq, "%c", &car) != EOF) {
        if (car == '\n') {
            qnt_pedido++;
        }
    }

    qnt_pedido = qnt_pedido / 2;
    fclose(arq);
    return qnt_pedido;
}

void cadastrarPedido(Pedidos *pedido, int *qnt_pedido, Clientes *cliente, int qnt) {
    *qnt_pedido += 1;
    if (*qnt_pedido > MAX_PEDIDOS) {
        printf("Limite de pedidos atingido!\n");
        printf("Realocando memória...\n");
        pedido = realloc(pedido, (MAX_PEDIDOS * 2) * sizeof(Pedidos));
    }

    long long int cpf;

    printf("========================================================================================================================\n\n\t\t\t\t\t\tRealizar pedido:\n\n========================================================================================================================");
    printf("\n\nInforme o CPF para realizar o pedido: ");
    scanf("%lld", &cpf);

    FILE *arq = fopen("clientes.txt", "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(-1);
    }

    Clientes *listagem = (Clientes *)malloc(qnt * sizeof(Clientes));

    int ver = 0;

    for (int i = 0; i < qnt; i++) {
        fscanf(arq, "%lld", &(listagem + i)->cpf);
        if (cpf == (listagem + i)->cpf) {
            printf("CPF encontrado!");
            ver++;
            pedido[*qnt_pedido].cpf = cpf;
            menuPedidos(pedido, *qnt_pedido);
        }
        fscanf(arq, " %[^\n]", (listagem + i)->nome);
        fscanf(arq, "%s", (listagem + i)->email);
    }

    if (ver == 0)
    {
        printf("CPF nao encontrado!\n");
        return;        
    }
    
    fclose(arq);
    free(listagem);
}

void menuPedidos(Pedidos *pedido, int qnt_pedido) {
    int op;
    system("cls");
    printf("\tMENU PEDIDOS:");
    printf("\n1 - Pao cacetinho");
    printf("\n2 - Bolo de abacaxi");
    printf("\n3 - Bolo de chocolate");
    printf("\n4 - Rocambole (De dar agua na boca)");
    printf("\n5 - Bomba");
    printf("\n6 - Mousse de maracuja");
    printf("\n7 - Milkshake");
    printf("\nInforme o ID do produto: ");
    scanf("%d", &op);

    switch (op) {
        case 1:
            imprimirPedidoArq(pedido, qnt_pedido, 1);
            break;
        case 2:
            imprimirPedidoArq(pedido, qnt_pedido, 2);
            break;
        case 3:
            imprimirPedidoArq(pedido, qnt_pedido, 3);
            break;
        case 4:
            imprimirPedidoArq(pedido, qnt_pedido, 4);
            break;
        case 5:

            imprimirPedidoArq(pedido, qnt_pedido, 5);
            break;
        case 6:
            imprimirPedidoArq(pedido, qnt_pedido, 6);
            break;
        case 7:
            imprimirPedidoArq(pedido, qnt_pedido, 7);
            break;
        default:
            printf("\nOpcao inválida!");
            break;
    }
}

void imprimirPedidoArq(Pedidos *pedido, int qnt_pedido, int id) {
    FILE *arq = fopen("pedidos.txt", "a+");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(-1);
    }

    pedido[qnt_pedido].id_pedido = id;

    fprintf(arq, "%lld\n", pedido[qnt_pedido].cpf);
    fprintf(arq, "%d\n", pedido[qnt_pedido].id_pedido);

    fclose(arq);
}

void desativarCliente(Pedidos *pedido, int qnt_pedido, int qnt) {
    long long int cpf;
    char op;
    int cont = 0;

    printf("========================================================================================================================\n\n\t\t\t\t\t\tDesativar cliente(CPF):\n\n========================================================================================================================");
    printf("\n\nInforme o CPF a ser desativado: ");
    scanf("%lld", &cpf);
    printf("Tem certeza que deseja desativar este CPF?: (S/N)\n");
    scanf(" %c", &op);

    if (op == 'S' || op == 's') {
        FILE *arq = fopen("clientes.txt", "r");
        if (arq == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            exit(-1);
        }

        Clientes *listagem = (Clientes *)malloc(qnt * sizeof(Clientes));

        for (int i = 0; i < qnt; i++) {
            fscanf(arq, "%lld\n", &(listagem + i)->cpf);
            fscanf(arq, " %[^\n]\n", (listagem + i)->nome);
            fscanf(arq, "%s\n", (listagem + i)->email);
            if (cpf == (listagem + i)->cpf)
            {
                cont = 1;
            }
        }

        if (cont == 0)
        {
            printf("\nCPF nao encontrado!\n");
            return;
        }

        fclose(arq);
        free(listagem);
        reescreverPedidos(pedido, qnt_pedido, cpf);
    } else if (op == 'N' || op == 'n') {
        return;
    }
}

void reescreverPedidos(Pedidos *pedido, int qnt_pedido, long long int cpf) {
    int cont = 0, pos = 0;

    FILE *arq = fopen("pedidos.txt", "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de pedidos.\n");
        exit(-1);
    }

    Pedidos *listagem = (Pedidos *)malloc(qnt_pedido * sizeof(Pedidos));

    for (int i = 0; i < qnt_pedido; i++)
    {
        fscanf(arq, "%lld\n", &(listagem + i)->cpf);
        fscanf(arq, "%d\n", &(listagem + i)->id_pedido);
        if (cpf != (listagem + i)->cpf)
        {
            cont++;
        }     
    }
    
    Pedidos *listagem1 = (Pedidos *)malloc(qnt_pedido * sizeof(Pedidos));

    for (int i = 0; i < qnt_pedido; i++)
    {
        if (cpf != (listagem+i)->cpf)
        {
            (listagem1+pos)->cpf = (listagem+i)->cpf;
            (listagem1+pos)->id_pedido = (listagem+i)->id_pedido;
            pos++;
        }
        
    }
    fclose(arq);
    free(listagem);

    arq = fopen("pedidos.txt", "w");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de pedidos.\n");
        exit(-1);
    }

    for (int i = 0; i < cont; i++)
    {
        fprintf(arq, "%lld\n", (listagem1+i)->cpf);
        fprintf(arq, "%d\n", (listagem1+i)->id_pedido);
    }
    fclose(arq);
    free(listagem1);
}

void excluirClientes(Pedidos *pedido, int qnt_pedido, int qnt, Clientes *cliente){
    long long int cpf;
    char op;
    int cont = 0;

    printf("========================================================================================================================\n\n\t\t\t\t\t\tExcluir clientes:\n\n========================================================================================================================");
    printf("\n\nInforme o CPF do cliente a ser excluido: ");
    scanf("%lld", &cpf);
    printf("Tem certeza que deseja desativar este CPF?: (S/N)\n");
    scanf(" %c", &op);

    if (op == 'S' || op == 's') {
        FILE *arq = fopen("clientes.txt", "r");
        if (arq == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            exit(-1);
        }

        Clientes *listagem = (Clientes *)malloc(qnt * sizeof(Clientes));

        for (int i = 0; i < qnt; i++) {
            fscanf(arq, "%lld\n", &(listagem + i)->cpf);
            fscanf(arq, " %[^\n]\n", (listagem + i)->nome);
            fscanf(arq, "%s\n", (listagem + i)->email);
            if (cpf == (listagem + i)->cpf)
            {
                cont = 1;
            }
        }

        if (cont == 0)
        {
            printf("\nCPF nao encontrado!\n");
            return;
        }

        fclose(arq);
        free(listagem);
        reescreverPedidos(pedido, qnt_pedido, cpf);
        excluirClientesArquivo(cliente, qnt, cpf);
    } else if (op == 'N' || op == 'n') {
        return;
    }
}

void excluirClientesArquivo(Clientes *cliente, int qnt, long long int cpf){
    
    int cont = 0, pos = 0;

    FILE *arq = fopen("clientes.txt", "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de pedidos.\n");
        exit(-1);
    }

    Clientes *listagem = (Clientes *)malloc(qnt * sizeof(Clientes));

    for (int i = 0; i < qnt; i++)
    {
        fscanf(arq, "%lld\n", &(listagem + i)->cpf);
        fscanf(arq, " %[^\n]\n", (listagem + i)->nome);
        fscanf(arq, "%s\n", (listagem + i)->email);
        if (cpf != (listagem + i)->cpf)
        {
            cont++;
        }     
    }
    
    Clientes *listagem1 = (Clientes *)malloc(qnt * sizeof(Clientes));

    for (int i = 0; i < qnt; i++)
    {
        if (cpf != (listagem+i)->cpf)
        {
            (listagem1 + pos)->cpf = (listagem + i)->cpf;
            strcpy((listagem1 + pos)->nome, (listagem + i)->nome);
            strcpy((listagem1 + pos)->email, (listagem + i)->email);
            pos++;
        }
    }
    fclose(arq);
    free(listagem);

    arq = fopen("clientes.txt", "w");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de pedidos.\n");
        exit(-1);
    }

    for (int i = 0; i < cont; i++)
    {
        fprintf(arq, "%lld\n", (listagem1+i)->cpf);
        fprintf(arq, "%s\n", (listagem1+i)->nome);
        fprintf(arq, "%s\n", (listagem1+i)->email);
    }
    fclose(arq);
    free(listagem1);
}

void listarPedidos(int qnt_pedidos){
    FILE *arq = fopen("pedidos.txt", "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Pedidos *lista = (Pedidos *)malloc(qnt_pedidos * sizeof(Pedidos));

    system("cls");

    for (int i = 0; i < qnt_pedidos; i++) {
        fscanf(arq, "%lld", &lista[i].cpf);
        fscanf(arq, "%d", &lista[i].id_pedido);

        printf("\nPedido %d:\n", i + 1);
        printf("CPF: %lld\n", lista[i].cpf);
        printf("ID do pedido: %d\n", lista[i].id_pedido);
        printf("\n");
    }

    free(lista);
    fclose(arq);
}

void menuPrincipal(){
    int opcao = 0, encerrar;
    int qnt = quantidadeClientes();
    Clientes *clientes = (Clientes *)malloc(MAX_CLIENTES * sizeof(Clientes));
    int qnt_pedidos = quantidadePedidos();
    Pedidos *pedido = (Pedidos *)malloc(MAX_PEDIDOS * sizeof(Pedidos));

    system("cls");

    do {

        printf("========================================================================================================================\n\n\t\t\t\t\t\t   PANIFICADORA ALPHA\n\n\t\t\t\t\t       Pao quentinho a toda hora!\n\n========================================================================================================================");
        printf("\n\n\t\t\t\t\t\t1 - Cadastrar cliente\n\t\t\t\t\t\t2 - Listar clientes\n\t\t\t\t\t\t3 - Pesquisar cliente\n\t\t\t\t\t\t4 - Realizar pedido\n\t\t\t\t\t\t5 - Listar pedidos\n\t\t\t\t\t\t6 - Desativar cliente\n\t\t\t\t\t\t7 - Excluir cliente\n\n\t\t\t\t\t\t0 - Sair\n\t\t\t\t\t\t\n\t\t\t\t\t\tDigite a opcao desejada: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                do {
                    system("cls");
                    qnt = quantidadeClientes();
                    cadastrarCliente(clientes, &qnt);
                    printf("Deseja realizar um novo cadastro?\n1 - Sim\n0 - Nao\n");
                    scanf("%d", &encerrar);
                } while (encerrar != 0);
                system("cls");
                break;
            case 2:
                system("cls");
                qnt = quantidadeClientes();
                listarClientes(clientes, qnt);
                break;
            case 3:
                system("cls");
                qnt = quantidadeClientes();
                pesquisarCliente(clientes, qnt);
                break;
            case 4:
                do {
                    system("cls");
                    qnt = quantidadeClientes();
                    qnt_pedidos = quantidadePedidos();
                    cadastrarPedido(pedido, &qnt_pedidos, clientes, qnt);
                    printf("Deseja realizar um novo pedido?\n1 - Sim\n0 - Nao\n");
                    scanf("%d", &encerrar);
                } while (encerrar != 0);
                system("cls");
                break;
            case 5:
                system("cls");
                qnt_pedidos = quantidadePedidos();
                listarPedidos(qnt_pedidos);
                break;
            case 6:
                system("cls");
                do {
                    system("cls");
                    qnt = quantidadeClientes();
                    qnt_pedidos = quantidadePedidos();
                    desativarCliente(pedido, qnt_pedidos, qnt);
                    printf("Deseja desativar outro cliente?\n1 - Sim\n0 - Nao\n");
                    scanf("%d", &encerrar);
                } while (encerrar != 0);
                system("cls");
                break;
            case 7:
                system("cls");
                do {
                    system("cls");
                    qnt = quantidadeClientes();
                    qnt_pedidos = quantidadePedidos();
                    excluirClientes(pedido, qnt_pedidos, qnt, clientes);
                    printf("Deseja excluir outro cliente?\n1 - Sim\n0 - Nao\n");
                    scanf("%d", &encerrar);
                } while (encerrar != 0);
                system("cls");
                break;
            case 0:
                system("cls");
                printf("\nSaindo...\n");
                break;
            default:
                system("cls");
                printf("\nOpcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    free(clientes);
    free(pedido);
}

int main() {
    menuPrincipal();
    return 0;
}
