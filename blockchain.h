#ifndef _FUNC_H
#define _FUNC_H
#include "block.h"

//char* test(int);
void print_instructions();
int compare_block_hashes(Block, Block);
int compare_block_and_prev_hashes(Block b1, Block b2);
unsigned char* create_sha256(const unsigned char[], unsigned char *);
unsigned char *hash_block(Block);
unsigned char* data_hash256(int);
int is_valid_block(Block prev_blk, Block blk);
int print_chain(Block*);
int print_blk(Block*);
int print_chain(Block *head);
int add_blk(Block *current, Transaction trans, Block **p);
int verify_null(Block *head);
int verify_prev_null(Block *head);
int validate_chain(Block *head);
int free_chain(Block *head);
int corrupt_chain(Block *head);
void print_visual_chain(Block *head);
char random_uppercase_letter();
char random_digit();
void generate_wallet_address(char* wallet_address);
Transaction create_transaction(char sender[], char recipient[], int amount,Address **list_address);
int add_transaction(Transaction transactions[], int num_trans, Transaction trans);
int validate_wallet_address(const char* wallet_address);
void proof_of_work(Block *blk);
void print_transaction_pool(Transaction pool[], int count);
int add_unique_address(Address **list_address, char address[]);
int print_addresses(Address **list_address);
int wallet_exists(Address **list_address, char wallet[13]);
void print_wallet_transactions(Block *head, char wallet[13]);

#define MAX_TRANSACTIONS 100
#define DIFFICULTY 2
#define ADDRESS_LENGTH 13

#endif // _FUNC_H
