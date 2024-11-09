#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "blockchain.h"
#include <string.h>


void clear_screen() {
#ifdef _WIN32
    system("cls");
#elif defined(__linux__) || defined(__APPLE__)
    system("clear");
#endif
}

void pause_screen() {
#ifdef _WIN32
    system("pause");
#elif defined(__linux__) || defined(__APPLE__)
    printf("Pressione Enter para continuar...");
    getchar();
#endif
}

void flush_in(){
    int ch;
    while( (ch = fgetc(stdin)) != EOF && ch != '\n' ){}
}



int main()
{

//    pause_screen();
    Block *blk_current = NULL;
    int out = 0;
    int choice;
    Transaction pool_wait[MAX_TRANSACTIONS];
    int count_t=0;
    Address **list_address = (Address**)malloc(sizeof(Address*));
    *list_address = NULL;

    setlocale(LC_ALL, "Portuguese");



    while(!out){
        printf("=======GUI Blockchain Noob=======\n\n");
        printf("0 - Instruções e Observações\n");
        printf("1 - Criar Bloco Gênesis\n");
        printf("2 - Minere um Novo Bloco e o adicione na blockchain\n");
        printf("3 - Exibir Blockchain\n");
        printf("4 - Validar Blockchain\n");
        printf("5 - Adultere os dados do penultimo bloco\n");
        printf("6 - Limpar Blockchain\n");
        printf("7 - Visualização em Cadeia\n");
        printf("8 - Fazer transação\n");
        printf("9 - Imprimir pool de transações pendentes\n");
        printf("10 - histórico de transações\n");
        printf("11 - Sair\n\n");
        printf("==============================================\n");
        printf("Escolha uma das opções acima: ");
        scanf("%i",&choice);
        if (choice == 11){
            printf("Programa finalizado!\n");
            out = 1;
            pause_screen();
        }

        else if (choice == 0){
            clear_screen();
            print_instructions();
            pause_screen();
        }

        else if(choice == 7){
            clear_screen();
            if(verify_null(blk_current) == 0){
               print_visual_chain(blk_current);
            } else {
                printf("Insira pelo menos o bloco Gênesis para isso!\n");
            }

            pause_screen();
        }

        else if (choice == 1){
            clear_screen();
            if (blk_current != NULL ){
                printf("O bloco Gênesis ja foi criado anteriormente!\n");
            } else {


                //Block blk_gen;
                blk_current = (Block *)malloc(sizeof(Block));
                blk_current->root = (MerkleNode *)malloc(sizeof(MerkleNode));
                blk_current->id = 0; blk_current->timestamp = time(NULL);
                blk_current->nonce = 0;
                blk_current->prev_hash = NULL;
                blk_current->prev_blk = NULL;
                blk_current->data = 0;
                blk_current->root->left = NULL; blk_current->root->right = NULL;
                blk_current->root->hash = data_hash256(blk_current->data);
                blk_current->hash = hash_block(*blk_current);
                blk_current->trans.amount = 0;
                strncpy(blk_current->trans.sender_wallet, "AAA00000000A", 12);
                blk_current->trans.sender_wallet[12] = '\0';
                strncpy(blk_current->trans.recipient_wallet, "AAA00000000A", 12);
                blk_current->trans.recipient_wallet[12] = '\0';
                printf("\nBloco Gênesis Criado com Sucesso!\n");
            }

            pause_screen();
        }

        else if (choice == 2) {
            clear_screen();
            if (verify_null(blk_current)) {
                printf("Antes de adicionar algum Bloco, Crie o Bloco Gênesis!\n");
            } else if (count_t == 0) {
                printf("A pool de transações está vazia. Adicione uma transação primeiro!\n");
            } else {
                if (verify_prev_null(blk_current)){
                    Transaction trans = pool_wait[0];
                    for (int i = 0; i < count_t - 1; i++) {
                        pool_wait[i] = pool_wait[i + 1];
                    }
                    count_t--;
                    //int data = trans.amount;
                    int conf = add_blk(blk_current, trans, &blk_current);
                    if (conf == 1) printf("\n\nBloco adicionado com Sucesso!\n");
                }
                else if (validate_chain(blk_current)) {
                    Transaction trans = pool_wait[0]; // Pega a primeira transação da pool
                    for (int i = 0; i < count_t - 1; i++) {
                        pool_wait[i] = pool_wait[i + 1];
                    }
                    count_t--;
                    //int data = trans.amount;
                    int conf = add_blk(blk_current, trans, &blk_current);
                    if (conf == 1) printf("\n\nBloco adicionado com Sucesso!\n");
                } else {
                    printf("\n\nO bloco não pode ser adicionado pois a blockchain não passou na validação!\n\n");
                }
            }
            pause_screen();
        }


        else if (choice == 8){
            if (verify_null(blk_current)){
                printf("Antes de adicionar algum Bloco, Crie o Bloco Gênesis!\n");
            } else {
                int check_wallet = 0;
                char receive[13], sender[13];
                int amount;
                Transaction new_t;
                printf("Carteira de envio: \n");
                scanf("%s",sender);
                check_wallet = validate_wallet_address(sender);
                printf("Carteira de recebimento: \n");
                scanf("%s",receive);
                check_wallet += validate_wallet_address(sender);
                //printf("%s e %s", receive, sender);
                printf("amount: \n");
                scanf("%d",&amount);
                if (check_wallet == 2){
                    new_t = create_transaction(sender, receive, amount, list_address);
                    count_t+=add_transaction(pool_wait,count_t,new_t);
                } else {
                    printf("Algum dos endereços é inválido! Repita a operação com endereços válidos!");
                }

                flush_in();
                print_transaction_pool(pool_wait,count_t);
            }
            pause_screen();
        }

        else if (choice == 9){
            if (count_t > 0){
                print_transaction_pool(pool_wait, count_t);
            } else {
                printf("A pool de transações está vazia. Adicione uma transação primeiro!\n");
            }
            pause_screen();
        }

        else if (choice == 10){
            char wallet[13];
            if (print_addresses(list_address) == 0) {
                printf("EMPTY\n");
            } else {
                clear_screen();
                print_addresses(list_address);
                printf("Digite qual carteira deseja visualizar o histórico: \n");
                scanf("%s",wallet);
                if (validate_wallet_address(wallet)){
                    if (wallet_exists(list_address,wallet)) {
                        print_wallet_transactions(blk_current,wallet);
                    } else {
                        printf("\nEssa carteira não participa de nenhuma transação ainda!\n");
                    }
                } else {
                    printf("\n\nEsse endereço não é válido\n");
                }
            }
            pause_screen();

        }

        else if (choice == 3){
            clear_screen();
            print_chain(blk_current);
            pause_screen();
        }

        else if (choice == 4){
            clear_screen();
            if (verify_null(blk_current)){
                printf("A blockchain ainda não existe para ser validada!\n");
            } else {
                if (verify_prev_null(blk_current)){
                    printf("Apenas o bloco gênesis compõe essa blockchain!\n");
                } else {
                    if (!validate_chain(blk_current)){
                        printf("Blockchain não é válida!\n");
                    } else {
                        printf("Blockchain é válida!\n");
                    }
                }
            }
            pause_screen();
        }
        else if (choice == 5){
            //int count=3;
            if (blk_current == NULL || blk_current->prev_blk == NULL || blk_current->prev_blk->prev_blk == NULL || blk_current->prev_blk->prev_blk->prev_blk == NULL){
                printf("Insira pelo menos 3 blocos na blockchain para isso!\n");
            } else {
                int corruption = corrupt_chain(blk_current);
                if (corruption) {
                    printf("O penúltimo bloco sofreu alteração nos dados para o valor 367256 assim gerando um novo hash!\n");
                    printf("A seguir, tente validar a blockchain e veja o que acontece!\n");
                    printf("Se quiser continuar com as interações, limpe a blockchain e recomece com uma blockchain válida!\n");
                }
                else {
                    printf("algo ocorreu errado!\n");
                }
            }
            pause_screen();
            clear_screen();
        }
        else if(choice == 6){
            clear_screen();
            free_chain(blk_current);
            blk_current = NULL;
            printf("Blockchain excluída com Sucesso!\n");
            pause_screen();
        }
        clear_screen();

    }
    blk_current = NULL;
    free_chain(blk_current);


    return 0;
}
