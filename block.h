#ifndef BLOCK_H
#define BLOCK_H

#include <time.h>

typedef struct {
    char sender_wallet[13];
    char recipient_wallet[13];
    int amount;
} Transaction;

typedef struct Address{
    char address[13];
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

#endif // BLOCK_H
