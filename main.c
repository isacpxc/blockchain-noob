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
        printf("N� conectado: %d\n", get_id_current_node());
        printf("Tamanho da rede: %d\n\n", get_network_length());
        printf("=======GUI Blockchain Noob=======\n\n");
        printf("0 - Instru��es e Observa��es\n");
        printf("1 - Criar Bloco G�nesis\n");
        printf("2 - Minere um Novo Bloco e o adicione na blockchain\n");
        printf("3 - Exibir Blockchain\n");
        printf("4 - Validar Blockchain\n");
        printf("5 - Adultere os dados do penultimo bloco\n");
//        printf("6 - Limpar Blockchain\n");
        printf("7 - Visualiza��o em Cadeia\n");
        printf("8 - Fazer transa��o\n");
        printf("9 - Imprimir pool de transa��es pendentes\n");
        printf("10 - hist�rico de transa��es\n");
        printf("11 - Sair\n\n");
        printf("====NOVAS FUNCIONALIDADES====\n");
        printf("12 - Criar e adicionar n� na rede\n");
        printf("13 - Trocar n� conectado\n");
        printf("14 - Imprimir mensagens pendentes\n");
        printf("15 - Processar mensagens\n");
        printf("16 - Exibir saldo de endere�o\n");
        printf("==============================================\n");
        printf("Escolha uma das op��es acima: ");
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
                printf("Insira pelo menos o bloco G�nesis para isso!\n");
            }

            pause_screen();
        }

        else if (choice == 1){
            clear_screen();
            if (blk_current != NULL ){
                printf("O bloco G�nesis ja foi criado anteriormente!\n");
            } else {
                blk_current = create_genesis_block();
                printf("\nBloco G�nesis Criado com Sucesso!\n");
            }

            pause_screen();
        }

        else if (choice == 2) {
            clear_screen();
            if (verify_null()) {
                printf("Antes de adicionar algum Bloco, Crie o Bloco G�nesis!\n");
            } else if (pool_length() == 0) {
                printf("A pool de transa��es est� vazia. Adicione uma transa��o primeiro!\n");
            } else {
                char wallet_address[13];
                Address* wallet;
                print_addresses(list_address);
                printf("Digite qual carteira deseja usar para receber a taxa de minera��o: \n");
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
                            Transaction trans = pull_first_transaction(); // Pega a primeira transa��o da pool
                            int conf = add_blk(trans, list_address,wallet);
                            if (conf == 1) printf("\n\nBloco adicionado com Sucesso!\n");
                        } else {
                            printf("\n\nO bloco n�o pode ser adicionado pois a blockchain n�o passou na valida��o!\n\n");
                        }
                        ////////
                    } else {
                        printf("\n\nEscolha um endere�o que j� existe para receber a taxa de minera��o!\n");
                    }
                } else {
                    printf("\n\nEsse endere�o n�o � v�lido\n");
                }
            }
            pause_screen();
        }


        else if (choice == 8){
            if (verify_null()){
                printf("Antes de adicionar algum Bloco, Crie o Bloco G�nesis!\n");
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
                        printf("\nTransa��o adicionada com Sucesso!\n");
                    }
                } else {
                    printf("Algum dos endere�os � inv�lido! Repita a opera��o com endere�os v�lidos!\n");
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
                printf("A pool de transa��es est� vazia. Adicione uma transa��o primeiro!\n");
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
                printf("Digite qual carteira deseja visualizar o hist�rico: \n");
                scanf("%s",wallet);
                if (validate_wallet_address(wallet)){
                    if (wallet_exists(list_address,wallet)) {
                        print_wallet_transactions(blk_current,wallet);
                    } else {
                        printf("\nEssa carteira n�o participa de nenhuma transa��o ainda!\n");
                    }
                } else {
                    printf("\n\nEsse endere�o n�o � v�lido\n");
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
                printf("A blockchain ainda n�o existe para ser validada!\n");
            } else {
                if (verify_prev_null(blk_current)){
                    printf("Apenas o bloco g�nesis comp�e essa blockchain!\n");
                } else {
                    if (!validate_chain(blk_current)){
                        printf("Blockchain n�o � v�lida!\n");
                    } else {
                        printf("Blockchain � v�lida!\n");
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
                    printf("O pen�ltimo bloco sofreu altera��o nos dados para o valor 367256 assim gerando um novo hash!\n");
                    printf("A seguir, tente validar a blockchain e veja o que acontece!\n");
                    printf("Se quiser continuar com as intera��es, limpe a blockchain e recomece com uma blockchain v�lida!\n");
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
//            printf("Blockchain exclu�da com Sucesso!\n");
//            pause_screen();
//        }
        else if (choice == 12){
            clear_screen();
            if(verify_null() == 0){
                create_node();
            } else {
                printf("Insira Pelo menos o bloco g�nesis!\n");
            }
            pause_screen();
            clear_screen();
        } else if (choice == 13) {
            int node_choice;
            int success;
            clear_screen();
            print_network();
            printf("\n\nEscolha o n� a que deseja se conectar: \n");
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
                printf("Digite qual carteira deseja visualizar o hist�rico: \n");
                scanf("%s",wallet);
                if (validate_wallet_address(wallet)){
                    if (wallet_exists(list_address,wallet)) {
                        balance = check_balance(list_address,wallet);
                        printf("\nSaldo de %s: %d\n\n", wallet, balance);
                    }
                } else {
                    printf("\n\nEsse endere�o n�o � v�lido\n");
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
