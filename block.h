#ifndef BLOCK_H
#define BLOCK_H

#include <time.h>

typedef struct Block {
    size_t id;
    unsigned char *hash;
    unsigned char *prev_hash;
    time_t timestamp;
    int nonce;
    struct Block *prev_blk;
    int data;
} Block;

typedef struct merkle_node {
    unsigned char *hash;
    struct MerkleNode *left;
    struct MerkleNode *right;
} m_n;

#endif // BLOCK_H
