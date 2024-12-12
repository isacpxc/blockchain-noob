#ifndef _FUNC_H
#define _FUNC_H
#include "block.h"

#define MAX_TRANSACTIONS 100
#define DIFFICULTY 2
#define ADDRESS_LENGTH 13

#define FORK_RESOLVED 1
#define NO_NEXT_BLOCK_FOUND 0
#define NO_BLOCK_WITH_SAME_ID_FOUND -1
#define TAX_TRANSACTION 2
#define INITIAL_BALANCE 100

//char* test(int);
void print_instructions();
int compare_block_hashes(Block, Block);
int compare_block_and_prev_hashes(Block b1, Block b2);
unsigned char* create_sha256(const unsigned char[], unsigned char *);
unsigned char *hash_block(Block);
unsigned char* data_hash256(int);
int is_valid_block(Block prev_blk, Block blk);
int print_chain();
int print_blk(Block*);

//int add_blk(Block *current, Transaction trans, Block **p);
int add_blk(Transaction,Address **,Address* );

int verify_null();
int verify_prev_null();
int validate_chain();
int free_chain(Block *head);
int corrupt_chain();
void print_visual_chain();
char random_uppercase_letter();
char random_digit();
void generate_wallet_address(char* wallet_address);
int create_transaction(char sender[], char recipient[], int amount,Address **list_address);
int add_transaction(Transaction trans);
int validate_wallet_address(const char* wallet_address);
void proof_of_work(Block *blk);
void print_transaction_pool();
int add_unique_address(Address **list_address, char address[]);
int print_addresses(Address **list_address);
int wallet_exists(Address **list_address, char wallet[13]);
void print_wallet_transactions(char wallet[13]);
void clear_screen();
Node* create_main_node();
Block* create_genesis_block();
void update_latest_added_node(Node*);
void update_current_node(Node*);
int get_network_length();
int get_id_current_node();
int change_node_to(int);
int pool_length();
Transaction pull_first_transaction();
int verify_minimum_three();
void broadcast_transaction(Transaction trans);
void broadcast_block(Block *blk);
void print_message_queue();
int process_message_queue();
int check_balance(Address **list_address, char wallet_address[13]);
Address* select_wallet(Address **list_address, char wallet_address[13]);
void create_node();
void print_network();
Message* find_message_with_block_id();
Message* find_next_block_message(Block *current_block);
int handle_fork_resolution();
unsigned char* create_sha256_transaction(const unsigned char* data, unsigned char *buffer);
unsigned char* transaction_hash(Transaction transaction);
int transaction_exists_in_messages(Transaction *transaction);
#endif // _FUNC_H
