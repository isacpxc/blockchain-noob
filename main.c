#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "blockchain.h"
#include <string.h>

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
    Node *main_node = create_main_node();
    update_current_node(main_node);
    update_latest_added_node(main_node);
    setlocale(LC_ALL, "Portuguese");



    while(!out){
        printf("Nó conectado: %d\n", get_id_current_node());
        printf("Tamanho da rede: %d\n\n", get_network_length());
        printf("=======GUI Blockchain Noob=======\n\n");
        printf("0 - Instruções e Observações\n");
        printf("1 - Criar Bloco Gênesis\n");
        printf("2 - Minere um Novo Bloco e o adicione na blockchain\n");
        printf("3 - Exibir Blockchain\n");
        printf("4 - Validar Blockchain\n");
        printf("5 - Adultere os dados do penultimo bloco\n");
//        printf("6 - Limpar Blockchain\n");
        printf("7 - Visualização em Cadeia\n");
        printf("8 - Fazer transação\n");
        printf("9 - Imprimir pool de transações pendentes\n");
        printf("10 - histórico de transações\n");
        printf("11 - Sair\n\n");
        printf("====NOVAS FUNCIONALIDADES====\n");
        printf("12 - Criar e adicionar nó na rede\n");
        printf("13 - Trocar nó conectado\n");
        printf("14 - Imprimir mensagens pendentes\n");
        printf("15 - Processar mensagens\n");
        printf("16 - Exibir saldo de endereço\n");
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
            if(verify_null() == 0){
               print_visual_chain();
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
                blk_current = create_genesis_block();
                printf("\nBloco Gênesis Criado com Sucesso!\n");
            }

            pause_screen();
        }

        else if (choice == 2) {
            clear_screen();
            if (verify_null()) {
                printf("Antes de adicionar algum Bloco, Crie o Bloco Gênesis!\n");
            } else if (pool_length() == 0) {
                printf("A pool de transações está vazia. Adicione uma transação primeiro!\n");
            } else {
                char wallet_address[13];
                Address* wallet;
                print_addresses(list_address);
                printf("Digite qual carteira deseja usar para receber a taxa de mineração: \n");
                scanf("%s",wallet_address);
                if (validate_wallet_address(wallet_address)){
                    if (wallet_exists(list_address,wallet_address)) {
                        ////////
                        wallet = select_wallet(list_address,wallet_address);
                        if (verify_prev_null()){
                            Transaction trans = pull_first_transaction();
                            int conf = add_blk(trans, list_address,wallet);
                            if (conf == 1) printf("\n\nBloco adicionado com Sucesso!\n");
                        }
                        else if (validate_chain(blk_current)) {
                            Transaction trans = pull_first_transaction(); // Pega a primeira transação da pool
                            int conf = add_blk(trans, list_address,wallet);
                            if (conf == 1) printf("\n\nBloco adicionado com Sucesso!\n");
                        } else {
                            printf("\n\nO bloco não pode ser adicionado pois a blockchain não passou na validação!\n\n");
                        }
                        ////////
                    } else {
                        printf("\n\nEscolha um endereço que já existe para receber a taxa de mineração!\n");
                    }
                } else {
                    printf("\n\nEsse endereço não é válido\n");
                }
            }
            pause_screen();
        }


        else if (choice == 8){
            if (verify_null()){
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
                    int check_create_transaction = create_transaction(sender, receive, amount, list_address);
                    if ( check_create_transaction == 1){
                        printf("\nTransação adicionada com Sucesso!\n");
                    }
                } else {
                    printf("Algum dos endereços é inválido! Repita a operação com endereços válidos!\n");
                }

                print_transaction_pool();
//                flush_in();
            }
            pause_screen();
        }

        else if (choice == 9){
            int check_is_pool_empty = pool_length();
            if ( check_is_pool_empty> 0){
                print_transaction_pool();
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
            if (verify_null()){
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
            int check_three = verify_minimum_three();
            if (check_three == 0){
                printf("Insira pelo menos 3 blocos na blockchain para isso!\n");
            } else {
                int corruption = corrupt_chain();
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
//        else if(choice == 6){
//            clear_screen();
//            free_chain(blk_current);
//            blk_current = NULL;
//            printf("Blockchain excluída com Sucesso!\n");
//            pause_screen();
//        }
        else if (choice == 12){
            clear_screen();
            if(verify_null() == 0){
                create_node();
            } else {
                printf("Insira Pelo menos o bloco gênesis!\n");
            }
            pause_screen();
            clear_screen();
        } else if (choice == 13) {
            int node_choice;
            int success;
            clear_screen();
            print_network();
            printf("\n\nEscolha o nó a que deseja se conectar: \n");
            scanf("%d",&node_choice);
            success = change_node_to(node_choice);
            if (success == 1){
                printf("\nConectado com Sucesso!\n");
            } else {
                printf("erro");
            }
            pause_screen();
        } else if (choice == 14){
            clear_screen();
            print_message_queue();
            pause_screen();
        } else if (choice == 15){
            int check_process_message;
            clear_screen();
            check_process_message = process_message_queue();
            if (check_process_message) printf("\n\nAtualizado!\n");
            else printf("\n\nLista vazia!\n");
            pause_screen();
        } else if (choice == 16){
            char wallet[13];
            int balance;
            if (print_addresses(list_address) == 0) {
                printf("EMPTY\n");
            } else {
                clear_screen();
                print_addresses(list_address);
                printf("Digite qual carteira deseja visualizar o histórico: \n");
                scanf("%s",wallet);
                if (validate_wallet_address(wallet)){
                    if (wallet_exists(list_address,wallet)) {
                        balance = check_balance(list_address,wallet);
                        printf("\nSaldo de %s: %d\n\n", wallet, balance);
                    }
                } else {
                    printf("\n\nEsse endereço não é válido\n");
                }
            }
            pause_screen();
        }
        clear_screen();

    }
    blk_current = NULL;
    free_chain(blk_current);


    return 0;
}
