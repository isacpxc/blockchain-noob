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
printf("Node connected: %d\n", get_id_current_node());
        printf("Network size: %d\n\n", get_network_length());
        printf("=======GUI Blockchain Noob=======\n\n");
        printf("0 - Instructions and Remarks\n");
        printf("1 - Create Genesis Block\n");
        printf("2 - Mine a New Block and add it to the blockchain\n");
        printf("3 - Display Blockchain\n");
        printf("4 - Validate Blockchain\n");
        printf("5 - Tamper with the data of the penultimate block\n");
//        printf("6 - Clear Blockchain\n");
        printf("7 - Chain Visualization\n");
        printf("8 - Make a transaction\n");
        printf("9 - Print pool of pending transactions\n");
        printf("10 - Transaction history\n");
        printf("11 - Exit\n\n");
        printf("====NEW FEATURES====\n");
        printf("12 - Create and add node to the network\n");
        printf("13 - Change connected node\n");
        printf("14 - Print pending messages\n");
        printf("15 - Process messages\n");
        printf("16 - Display address balance\n");
        printf("==============================================\n");
        printf("Choose one of the options above: ");

        scanf("%i",&choice);
        if (choice == 11){
            printf("Program finished!\n");
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
                printf("Insert at least the Genesis block for this!\n");
            }

            pause_screen();
        }

        else if (choice == 1){
            clear_screen();
            if (blk_current != NULL ){
                printf("The Genesis block has already been created.!\n");
            } else {
                blk_current = create_genesis_block();
                printf("\nGenesis Block Successfully Created!\n");
            }

            pause_screen();
        }

        else if (choice == 2) {
            clear_screen();
            if (verify_null()) {
                printf("Before adding any Block, Create the Genesis Block!\n");
            } else if (pool_length() == 0) {
                printf("The transaction pool is empty. Add a transaction first!\n");
            } else {
                char wallet_address[13];
                Address* wallet;
                print_addresses(list_address);
                printf("Enter which wallet you want to use to receive the mining fee: \n");
                scanf("%s",wallet_address);
                if (validate_wallet_address(wallet_address)){
                    if (wallet_exists(list_address,wallet_address)) {
                        ////////
                        wallet = select_wallet(list_address,wallet_address);
                        if (verify_prev_null()){
                            Transaction trans = pull_first_transaction();
                            int conf = add_blk(trans, list_address,wallet);
                            if (conf == 1) printf("\n\nBlock successfully added!\n");
                        }
                        else if (validate_chain()) {
                            Transaction trans = pull_first_transaction(); // Pega a primeira transação da pool
                            int conf = add_blk(trans, list_address,wallet);
                            if (conf == 1) printf("\n\nBlock successfully added!\n");
                        } else {
                            printf("\n\nO Block cannot be added as the blockchain failed validation!\n\n");
                        }
                        ////////
                    } else {
                        printf("\n\nChoose an existing address to receive the mining fee!\n");
                    }
                } else {
                    printf("\n\nThis address is not valid\n");
                }
            }
            pause_screen();
        }


        else if (choice == 8){
            if (verify_null()){
                printf("Before adding any Block, Create the Genesis Block!\n");
            } else {
                int check_wallet = 0;
                char receive[13], sender[13];
                int amount;
                Transaction new_t;
                printf("Sender's wallet: \n");
                scanf("%s",sender);
                check_wallet = validate_wallet_address(sender);
                printf("Recipient's wallet: \n");
                scanf("%s",receive);
                check_wallet += validate_wallet_address(sender);
                //printf("%s e %s", receive, sender);
                printf("amount: \n");
                scanf("%d",&amount);
                if (check_wallet == 2){
                    int check_create_transaction = create_transaction(sender, receive, amount, list_address);
                    if ( check_create_transaction == 1){
                        printf("\nTransaction added successfully!\n");
                    }
                } else {
                    printf("One of the addresses is invalid! Repeat the operation with valid addresses!\n");
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
                printf("The transaction pool is empty. Add a transaction first!\n");
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
                printf("Enter which wallet you want to view the history for: \n");
                scanf("%s",wallet);
                if (validate_wallet_address(wallet)){
                    if (wallet_exists(list_address,wallet)) {
                        print_wallet_transactions(wallet);
                    } else {
                        printf("\nThis wallet has not participated in any transactions yet!\n");
                    }
                } else {
                    printf("\n\nThis address is not valid\n");
                }
            }
            pause_screen();

        }

        else if (choice == 3){
            clear_screen();
            print_chain();
            pause_screen();
        }

        else if (choice == 4){
            clear_screen();
            if (verify_null()){
                printf("The blockchain does not exist yet to be validated!\n");
            } else {
                if (verify_prev_null(blk_current)){
                    printf("Only the Genesis block composes this blockchain!\n");
                } else {
                    if (!validate_chain()){
                        printf("Blockchain is not valid!\n");
                    } else {
                        printf("Blockchain is valid!\n");
                    }
                }
            }
            pause_screen();
        }
        else if (choice == 5){
            int check_three = verify_minimum_three();
            if (check_three == 0){
                printf("Insert at least 3 blocks in the blockchain for this!\n");
            } else {
                int corruption = corrupt_chain();
                if (corruption) {
                    printf("The penultimate block had its data altered to the value 367256, generating a new hash!\n");
                    printf("Next, try to validate the blockchain and see what happens!\n");
                    printf("If you want to continue with the interactions, clear the blockchain and start again with a valid blockchain!\n");
                }
                else {
                    printf("Something went wrong!\n");
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
                printf("Insert at least the Genesis block!\n");
            }
            pause_screen();
            clear_screen();
        } else if (choice == 13) {
            int node_choice;
            int success;
            clear_screen();
            print_network();
            printf("\n\nChoose the node you want to connect to: \n");
            scanf("%d",&node_choice);
            success = change_node_to(node_choice);
            if (success == 1){
                printf("\nSuccessfully connected!\n");
            } else {
                printf("error");
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
            if (check_process_message) printf("\n\nUpdated!\n");
            else printf("\n\nEmpty list!\n");
            pause_screen();
        } else if (choice == 16){
            char wallet[13];
            int balance;
            if (print_addresses(list_address) == 0) {
                printf("EMPTY\n");
            } else {
                clear_screen();
                print_addresses(list_address);
                printf("Enter which wallet you want to see the balance for: \n");
                scanf("%s",wallet);
                if (validate_wallet_address(wallet)){
                    if (wallet_exists(list_address,wallet)) {
                        balance = check_balance(list_address,wallet);
                        printf("\nBalance of %s: %d\n\n", wallet, balance);
                    }
                } else {
                    printf("\n\nThis address is not valid\n");
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
