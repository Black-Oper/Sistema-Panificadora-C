# Sistema de Gerenciamento de Clientes e Pedidos

Este é um sistema simples para gerenciar clientes e pedidos de uma aplicação de padaria. Ele permite o cadastro, listagem, pesquisa e exclusão de clientes, além do registro e acompanhamento de pedidos.

## Funcionalidades

- **Cadastro de Clientes**: Os clientes são cadastrados com CPF, nome e e-mail.
- **Validação de CPF**: O sistema verifica se o CPF inserido é válido.
- **Listagem de Clientes**: Os clientes podem ser listados em ordem de inclusão ou ordem alfabética.
- **Pesquisa de Cliente**: Permite a pesquisa de clientes pelo CPF.
- **Cadastro de Pedidos**: Permite realizar pedidos associados a um cliente existente pelo CPF.
- **Listagem de Pedidos**: Exibe todos os pedidos cadastrados.
- **Exclusão e Desativação de Clientes**: Permite desativar ou excluir um cliente e atualizar os pedidos associados.
- **Reescrever Pedidos**: Remove pedidos de clientes que foram desativados ou excluídos.

## Arquitetura do Código

### Estruturas de Dados

- `Clientes`: Estrutura para armazenar informações do cliente (CPF, nome e e-mail).
- `Pedidos`: Estrutura para armazenar o ID do pedido e o CPF associado.

### Funções Principais

#### Validação de CPF
```c
bool validarCPF(long long int cpf)
```
Verifica a validade de um CPF utilizando o cálculo dos dígitos verificadores.

#### Cadastro de Cliente
```c
void cadastrarCliente(Clientes *clientes, int *qnt)
```
Realiza o cadastro de novos clientes e armazena as informações no arquivo `clientes.txt`.

#### Listagem de Clientes
```c
void listarClientes(Clientes *clientes, int qnt)
```
Lista os clientes cadastrados, podendo ser por ordem de inclusão ou alfabética.

#### Pesquisa de Cliente
```c
void pesquisarCliente(Clientes *cliente, int qnt)
```
Permite pesquisar um cliente através do CPF.

#### Cadastro de Pedidos
```c
void cadastrarPedido(Pedidos *pedido, int *qnt_pedido, Clientes *cliente, int qnt)
```
Registra novos pedidos para um cliente existente.

#### Listagem de Pedidos
```c
void listarPedidos(int qnt_pedidos)
```
Exibe os pedidos cadastrados no sistema.

#### Exclusão de Cliente
```c
void excluirClientes(Pedidos *pedido, int qnt_pedido, int qnt, Clientes *cliente)
```
Permite excluir um cliente e atualizar os pedidos correspondentes.

### Manipulação de Arquivos

O sistema utiliza arquivos de texto para armazenar os dados de clientes e pedidos:

- **clientes.txt**: Armazena informações dos clientes cadastrados (CPF, nome e e-mail).
- **pedidos.txt**: Armazena informações dos pedidos realizados (CPF e ID do pedido).

## Compilação e Execução

Para compilar o programa, utilize um compilador de C como o GCC:

```bash
gcc -o sistema main.c
```

Em seguida, execute o programa:

```bash
./sistema
```

## Requisitos

- **Linguagem**: C
- **Compilador**: GCC ou equivalente
- **Sistema Operacional**: Compatível com ambientes Unix/Linux e Windows.

## Observações

- O sistema cria automaticamente os arquivos `clientes.txt` e `pedidos.txt` caso não existam.
- A memória é realocada dinamicamente caso o número de clientes ou pedidos ultrapasse os limites predefinidos.

## Limites

- Máximo de **100 clientes** e **500 pedidos**. Estes valores podem ser modificados ajustando as constantes `MAX_CLIENTES` e `MAX_PEDIDOS`.