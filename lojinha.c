#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>


typedef struct{
    int id;
    char nome[50];
    float preco;
}Produto;

typedef struct {
    Produto produto;
    int quantidade;
} ItemCarrinho;


void menu();
void verCarrinho();
void cadastrarProduto();
void comprarProduto();
void fecharPedido();
void listarProdutos();
void listarProdutosParaCompra();

void infoProduto(Produto prod);


int main(){
    setlocale(LC_ALL, "Portuguese");
    menu();
    return 0;
}

void infoProduto(Produto prod){
    printf(" ID: %d\n Nome: %s\n Preço: %.2f", prod.id, strtok(prod.nome, "\n"), prod.preco);
}


void menu(){
    printf("\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("=-=-=-=-=-=-=-=- Bem  vindo(a) -=-=-=-=-=-=-=\n");
    printf("=-=-=-=-=-=-=-=-  Ney  Shop  -=-=-=-=-=-=-=-=\n");
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");

    printf("Selecione uma das opções abaixo: \n");
    printf("1 - Cadastrar um novo produto \n");
    printf("2 - Comprar produto \n");
    printf("3 - Ver carrinho \n");
    printf("4 - Listar produtos da loja \n");
    printf("5 - Fechar pedido \n");
    printf("6 - sair do sistema\n ");

    int opcao;
    scanf("%d", &opcao);
    getchar();

    switch (opcao)
    {
    case 1:
        cadastrarProduto();
        break;

    case 2:
        comprarProduto();
        break;

    case 3:
        verCarrinho();
        break;

    case 4:
        listarProdutos();
        break;

    case 5:
        fecharPedido();
        break;

    case 6:
        printf("Volte sempre! \n");
        Sleep(2);
        exit(0);

    default:
        printf("Opção inválida!");
        Sleep(2);
        menu();
        break;
    }
}

void cadastrarProduto(){
    Produto prod;
    printf("Cadastro de produto\n");
    printf("=-=-=-=-=-=-=-=-=-=\n");

    FILE *arq;
    arq = fopen("dados.txt", "a");

    if(arq){
        printf("Informe o nome do produto: ");
        scanf("%s", prod.nome);
        getchar();
        

        printf("Informe o preço do produto: ");
        scanf("%f", &prod.preco);
        

        prod.id = gerarID();
        

        fprintf(arq, "Nome: %s Preço: %f ID: %d\n", prod.nome, prod.preco, prod.id);



        printf("Produto cadastrado com sucesso!\n");
        fclose(arq);

        Sleep(2);
        menu();

    }else{
        printf("Não foi possivel abrir o arquivo! \n");
    }
}

//gerarID ve no arquivo qual o ultimo codigo usado e adiciona mais um

int gerarID(){
    FILE *arq;
    arq = fopen("dados.txt", "r");
    if (arq) {
        int id = 0;
        char linha[100];
        while (fgets(linha, sizeof(linha), arq) != NULL) {
            int idLido;
            char nome[50];
            float preco;

            if (sscanf(linha, "Nome: %s Preço: %f ID: %d", nome, &preco, &idLido) == 3) {
                if (idLido > id) {
                    id = idLido;
                }
            }
        }
        fclose(arq);
        return id + 1;
    } else {
        printf("Não foi possivel abrir o arquivo!\n");
        return -1; 
    }
}




void comprarProduto() {
    printf("Compra de produto\n");
    printf("=-=-=-=-=-=-=-=-\n");

    listarProdutosParaCompra();
    

    int codigo;
    printf("Informe o ID do produto: ");
    scanf("%d", &codigo);
    getchar();

    // Abrir o arquivo para leitura
    FILE* arq;
    arq = fopen("dados.txt", "r");
    if (arq) {
        // VariÃ¡veis temporÃ¡rias para armazenar os dados lidos do arquivo
        int idLido;
        char nome[50];
        float preco;

        // Verificar se o produto com o cÃ³digo informado existe no arquivo
        
        int encontrado = 0;
        while (fscanf(arq, "Nome: %s Preço: %f ID: %d\n", nome, &preco, &idLido) == 3) {
            if (idLido == codigo) {
                encontrado = 1;
                break;
            }
        }


        fclose(arq);

        if (encontrado) {
            // Adicionar o produto ao carrinho
            ItemCarrinho item;
            item.produto.id = idLido;
            strcpy(item.produto.nome, nome);
            item.produto.preco = preco;

            printf("Informe a quantidade do produto: ");
            scanf("%d", &item.quantidade);
            getchar();

            // Abrir o arquivo do carrinho para adicionar o item
            FILE* carrinho;
            carrinho = fopen("carrinho.txt", "a");
            if (carrinho) {
                fprintf(carrinho, "Código: %d, Nome: %s, Preço: %.2f, Quantidade: %d\n", item.produto.id, item.produto.nome, item.produto.preco, item.quantidade);
                fclose(carrinho);

                printf("Produto adicionado ao carrinho!\n");
            }}}
            else {
                printf("Não foi possivel abrir o arquivo do carrinho!\n");
            }
                

                Sleep(2);
              menu();
}
    

void fecharPedido() {
    printf("Fechando o pedido...\n");

    FILE* carrinho;
    carrinho = fopen("carrinho.txt", "r");
    if (carrinho) {
        printf("Itens no carrinho:\n");
        char linha[100];
        float valorTotal = 0;

        while (fgets(linha, sizeof(linha), carrinho) != NULL) {
            int codigo;
            char nome[50];
            float preco;
            int quantidade;
            sscanf(linha, "Código: %d, Nome: %[^,], Preço: %f, Quantidade: %d", &codigo, nome, &preco, &quantidade);
            printf("Código: %d, Nome: %s, Preço: %.2f, Quantidade: %d\n", codigo, nome, preco, quantidade);
            valorTotal += preco * quantidade;
        }

        fclose(carrinho);

        printf("Valor total do pedido: %.2f\n", valorTotal);

        printf("Deseja confirmar o pagamento? (S/N) ");
        char resposta;
        scanf(" %c", &resposta);
        getchar();

        if (resposta == 'S' || resposta == 's') {
            carrinho = fopen("carrinho.txt", "w");
            if (carrinho) {
                fclose(carrinho);
                remove("carrinho.txt");
                printf("Pedido finalizado.\n");
            } else {
                printf("Não foi possível limpar o carrinho.\n");
            }
        } else {
            printf("Pagamento cancelado.\n");
        }
    } else {
        printf("O carrinho está vazio.\n");
    }

    Sleep(2000);
    menu();
}

void verCarrinho() {
    printf("Carrinho de compras\n");
    printf("=-=-=-=-=-=-=-=-=-=\n");

    FILE* carrinho;
    carrinho = fopen("carrinho.txt", "r");
    if (carrinho) {
        char linha[100];
        while (fgets(linha, sizeof(linha), carrinho) != NULL) {
            printf("%s", linha);
        }
        fclose(carrinho);
    } else {
        printf("O carrinho está vazio.\n");
    }

    Sleep(2);
    menu();
}

void listarProdutos() {
    printf("Produtos cadastrados\n");
    printf("====================\n");

    FILE* arq;
    arq = fopen("dados.txt", "r");
    if (arq) {
        char linha[100];
        while (fgets(linha, sizeof(linha), arq) != NULL) {
            int id;
            char nome[50];
            float preco;

            if (sscanf(linha, "Nome: %s Preço: %f ID: %d", nome, &preco, &id) == 3) {
                printf("Nome: %s Preço: %.2f ID: %d\n", nome, preco, id);
            }
        }
        fclose(arq);
    } else {
        printf("Não foi possivel abrir o arquivo de dados.\n");
    }
    
    Sleep(2000);
    
    menu();
}

void listarProdutosParaCompra() {
    printf("Produtos disponiveis\n");
    printf("====================\n");

    FILE* arq;
    arq = fopen("dados.txt", "r");
    if (arq) {
        char linha[100];
        while (fgets(linha, sizeof(linha), arq) != NULL) {
            int id;
            char nome[50];
            float preco;

            if (sscanf(linha, "Nome: %s Preço: %f ID: %d", nome, &preco, &id) == 3) {
                printf("ID: %d, Nome: %s, Preço: %.2f\n", id, nome, preco);
            }
        }
        fclose(arq);
    } else {
        printf("Não foi possivel abrir o arquivo de dados.\n");
    }
}


