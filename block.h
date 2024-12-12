#ifndef BLOCK_H
#define BLOCK_H

#include <time.h>

typedef struct {
    time_t timestamp;
    char sender_wallet[13];
    char recipient_wallet[13];
    int amount;
    int tax;
} Transaction;

typedef struct Address{
    char address[13];
    int balance;
    struct Address *next;
} Address;


typedef struct MerkleNode {
    unsigned char *hash;
    struct MerkleNode *left;
    struct MerkleNode *right;
} MerkleNode;

typedef struct Block {
    size_t id;
    unsigned char *hash;
    unsigned char *prev_hash;
    time_t timestamp;
    unsigned long int nonce;
    struct Block *prev_blk;
    MerkleNode *root;
    Transaction trans;
    int data;
} Block;

typedef struct Message {
    char type; // 'B' para Bloco, 'T' para Transação
    union {
        Block block;
        Transaction transaction;
    } data;
    struct Message *last_message;
} Message;

typedef struct Node {
    int id;  // identificador do nó
    Block *blockchain;
    Transaction transaction_pool[100]; // Pool de transações
    Message *message_queue;
    int transaction_count; // Contagem de transações
    struct Node *next;
    struct Node *prev;
} Node;

#endif // BLOCK_H
