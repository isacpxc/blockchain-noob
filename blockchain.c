#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "blockchain.h"
#include <openssl/sha.h>
#include <ctype.h>

static Node* current_node = NULL;
static Node* latest_added_node = NULL;
static int network_length = 0;


void clear_screen() {
#ifdef _WIN32
    system("cls");
#elif defined(__linux__) || defined(__APPLE__)
    system("clear");
#endif
}

//const char* test(int i){
//    printf("print says: %d\n",i);
//    return "tudo ok";
//}

void print_instructions() {
    printf("===== Program Usage Instructions =====\n\n");
    printf("0 - Instructions\n");
    printf("    Displays these instructions.\n\n");
    printf("1 - Create Genesis Block\n");
    printf("    Creates the initial block of the blockchain. This must be the first step.\n\n");
    printf("2 - Add New Block to Chain\n");
    printf("    Adds a new block to the blockchain. This can only be done after creating the genesis block.\n\n");
    printf("3 - Display Blockchain\n");
    printf("    Displays all blocks in the blockchain.\n\n");
    printf("4 - Validate Blockchain\n");
    printf("    Checks the integrity of the blockchain, ensuring all blocks are valid.\n\n");
    printf("5 - Tamper with the Second-to-Last Block\n");
    printf("    Changes the data of the second-to-last block in the blockchain, generating a new hash for it.\n\n");
    printf("6 - Clear Blockchain\n");
    printf("    Removes all blocks from the blockchain, clearing it completely.\n\n");
    printf("7 - Chain View\n");
    printf("    Shows a chain view of the blocks in the blockchain.\n\n");
    printf("8 - Make Transaction\n");
    printf("    Initiates a transaction.\n\n");
    printf("9 - Print Pending Transactions Pool\n");
    printf("    If there are transactions in the transaction pool, prints them.\n\n");
    printf("10 - Transaction History\n");
    printf("     Displays the transaction history of a specified wallet.\n\n");
    printf("11 - Exit\n");
    printf("     Terminates the program.\n\n");
    printf("==============NEW FEATURES==============\n\n");
    printf("12 - Create and Add Node to Network\n");
    printf("     Creates a new node and adds it to the network with another user.\n\n");
    printf("13 - Switch Connected Node\n");
    printf("     Choose a node to connect to the network.\n\n");
    printf("14 - Print Pending Messages\n");
    printf("     Prints the pending messages for the current node.\n\n");
    printf("15 - Process Messages\n");
    printf("     Processes the pending messages for the current node and adds them either to the transaction pool or to the blockchain.\n\n");
    printf("16 - Select Address and View Balance\n");
    printf("     Select an existing address and check its current balance on the blockchain.\n\n");
    printf("============================How does fork resolution work in this project so far?============================\n");
    printf("*If during the \"process messages\" in the node a block with the same id as the current blockchain is found, we have a fork. If there is not yet a message with a block of subsequent id to the duplicate, the duplicate block message remains in the message pool. If the message in question exists, then the algorithm adopts the chain with the longest id. Furthermore, the transaction of the \"discarded\" block with duplicate id goes to the pending transaction pool, awaiting a new block to be mined.");
    printf("");
}


Block* create_genesis_block(){
    Block* blk = (Block *)malloc(sizeof(Block));
    if (blk == NULL) {
        fprintf(stderr, "Error allocating memory for the node\n");
        exit(1);
    }
    blk->root = (MerkleNode *)malloc(sizeof(MerkleNode));
    if (blk->root == NULL) {
        fprintf(stderr, "Error allocating memory for the node\n");
        exit(1);
    }
    blk->id = 0;
    blk->timestamp = time(NULL);
    blk->nonce = 0;
    blk->prev_hash = NULL;
    blk->prev_blk = NULL;
    blk->data = 0;
    blk->root->left = NULL;
    blk->root->right = NULL;
    blk->root->hash = data_hash256(blk->data);
    blk->hash = hash_block(*blk);
    blk->trans.amount = 0;
    strncpy(blk->trans.sender_wallet, "AAA00000000A", 12);
    blk->trans.sender_wallet[12] = '\0';
    strncpy(blk->trans.recipient_wallet, "AAA00000000A", 12);
    blk->trans.recipient_wallet[12] = '\0';

    current_node->blockchain = blk;

    return blk;
}

unsigned char *hash_block(Block block){
    unsigned char *block_record = malloc(sizeof(char)*512);
    sprintf((char*)block_record," %lli %lld %lu %s %s %i", block.id, block.timestamp, block.nonce, (char*)block.prev_hash,(char*)block.root->hash,block.data);
    unsigned char *buffer = malloc(sizeof(char)*SHA256_DIGEST_LENGTH);
    buffer = create_sha256((const unsigned char*)block_record,buffer);
    free(block_record);
    return buffer;
}

int compare_block_hashes(Block block1, Block block2) {
    return memcmp(block1.hash, block2.hash, SHA256_DIGEST_LENGTH) == 0;
}

int compare_block_and_prev_hashes(Block block1, Block block2) {
    return memcmp(block1.prev_hash, block2.hash, SHA256_DIGEST_LENGTH) == 0;
}

unsigned char* create_sha256(const unsigned char str[], unsigned char *buffer){
    SHA256(str, strlen((const char*)str), buffer);
    return buffer;
}

unsigned char* data_hash256(int data){
    unsigned char *block_record = malloc(sizeof(char)*512);
    sprintf((char*)block_record,"%i", data);
    unsigned char *buffer = malloc(sizeof(char)*SHA256_DIGEST_LENGTH);
    buffer = create_sha256((const unsigned char*)block_record,buffer);
    return buffer;
}

int is_valid_block(Block prev_blk, Block blk){
    if (prev_blk.id+1 != blk.id){
        printf("error: id invalid\n");
        return 0;
    }

    if (!compare_block_and_prev_hashes(blk,prev_blk)){
        printf("error: prev hash invalid\n");
        return 0;
    }

    return 1;
}


int print_blk(Block *blk){
    if (blk == NULL) {
        printf("NULL");
        return 0;
    }
    printf("BLK %lli ->\n",blk->id);
    printf("id: %lli\n",blk->id);
    printf("data: %i\n",blk->data);
    printf("timestamp: %lld\n",blk->timestamp);
    printf("merkle root: ");
    for(int i = 0; i<SHA256_DIGEST_LENGTH;i++){
        printf("%02hX", blk->root->hash[i]);
    }
    printf("\n");
    printf("nonce: %lu\n",blk->nonce);
    printf("hash: ");
    for(int i = 0; i<SHA256_DIGEST_LENGTH;i++){
        printf("%02hX", blk->hash[i]);
    }
    printf("\n");
    printf("prev_hash: ");
    if (blk->prev_hash == NULL){
        printf("NULL\n");
    } else {
        for(int i = 0; i<SHA256_DIGEST_LENGTH;i++){
            printf("%02hX", blk->prev_hash[i]);
        }
        printf("\n");
    }
    printf("Transaction:\n");
    printf("%s -> %s | amount: %d\n", blk->trans.sender_wallet,blk->trans.recipient_wallet,blk->trans.amount);
    printf("\n");
    return 1;
}

int print_chain(){
    if (current_node->blockchain == NULL){
        printf("\n\nYour Blockchain doesn't exist yet, my friend!\n\n");
        return 0;
    }

    Block *p = current_node->blockchain;

    while (p != NULL){
        print_blk(p);
        p = p->prev_blk;
    }
    return 1;
}

int add_blk(Transaction trans, Address **list_address, Address* wallet) {
    if (current_node == NULL || current_node->blockchain == NULL) {
        printf("Error: Node or blockchain not initialized.\n");
        return 0;
    }

    Block *current = current_node->blockchain;

    Block *blk = (Block *)malloc(sizeof(Block));
    if (!blk) {
        printf("Memory allocation error!\n");
        return 0;
    }

    MerkleNode *mk = (MerkleNode *)malloc(sizeof(MerkleNode));
    if (!mk) {
        printf("Memory allocation error!\n");
        free(blk);
        return 0;
    }

    mk->left = NULL;
    mk->right = NULL;
    mk->hash = data_hash256(trans.amount);

    blk->id = current->id + 1;
    blk->data = trans.amount;
    blk->nonce = 0;
    blk->prev_hash = current->hash;
    blk->root = mk;
    blk->timestamp = time(NULL);
    blk->prev_blk = current;
    blk->hash = hash_block(*blk);
    blk->trans = trans;

    Address *current_address = *list_address;
    Address *sender_address = NULL;
    Address *recipient_address = NULL;

    while (current_address != NULL) {
        if (strncmp(current_address->address, trans.sender_wallet, 13) == 0) {
            sender_address = current_address;
        }
        if (strncmp(current_address->address, trans.recipient_wallet, 13) == 0) {
            recipient_address = current_address;
        }
        current_address = current_address->next;
    }


    if (sender_address->balance >= trans.amount) {
        sender_address->balance -= trans.amount;

        recipient_address->balance += trans.amount;

        wallet->balance+=trans.tax;

        proof_of_work(blk);
        current_node->blockchain = blk;
        broadcast_block(blk);
        transaction_exists_in_messages(&trans); // retira transa��o das mensagens dos outros n�s

        return 1;
    } else {
        printf("Block cannot be mined because the sender does not have sufficient balance to complete the transaction!\n");
        free(blk);
        free(mk);
        return 0;
    }
}




int verify_null(){
    if (current_node->blockchain == NULL) return 1;
    return 0;
}

int verify_prev_null(){
    if (current_node->blockchain->prev_blk == NULL) return 1;
    return 0;
}

int validate_chain(){
    Block* head = (Block*)malloc(sizeof(Block));
    head = current_node->blockchain;
    if(head == NULL){
        return 0;
    }
    if(head->prev_blk == NULL){
        return 0;
    }

    if (head->prev_blk->id+1 != head->id){
        printf("\nerror: id invalid\n");
        return 0;
    }

    Block *p = head;
    Block *prev = p->prev_blk;

    while (prev != NULL){
        if (is_valid_block(*prev,*p)){
            printf(".\n");
        } else {
            return 0;
        }
        p = p->prev_blk;
        prev = prev->prev_blk;
    }
    printf("Blockchain validated!\n");
    return 1;
}

int free_chain(Block *head) {
    if (head == NULL){
        return 0;
    }
    Block *next = head;
    Block *current = next->prev_blk;

    while (current != NULL) {
        free(next);
        next = current;
        current = current->prev_blk;
    }
    free(current);
    return 1;
}

int corrupt_chain(){
    current_node->blockchain->prev_blk->data = 367256;
    Block *prev = malloc(sizeof(Block));
    prev = current_node->blockchain->prev_blk;
    current_node->blockchain->prev_blk->hash = hash_block(*prev);
    free(prev);
    return 1;
}

void print_visual_chain(){
    Block *p = (Block*)malloc(sizeof(Block));
    p = current_node->blockchain;

    while(p!=NULL){
        printf("||BLK %lli||",p->id);
        if (p->prev_blk != NULL) printf("==>");
        p = p->prev_blk;
    }
    printf("\n\n");
}

char random_uppercase_letter() {
    return 'A' + rand() % 26;
}

char random_digit() {
    return '0' + rand() % 10;
}

//void generate_wallet_address(char* wallet_address) {
//    for (int i = 0; i < 3; i++) {
//        wallet_address[i] = random_uppercase_letter();
//    } for (int i = 3; i < 11; i++) {
//        wallet_address[i] = random_digit();
//    }
//    wallet_address[11] = random_uppercase_letter();
//    wallet_address[12] = '\0';
//}

int validate_wallet_address(const char* wallet_address) {
    for (int i = 0; i < 3; i++) {
        if (!isupper(wallet_address[i])) return 0;
    }
    for (int i = 3; i < 11; i++) {
        if (!isdigit(wallet_address[i])) return 0;
    }
    if (!isupper(wallet_address[11])) return 0;
    if (wallet_address[12] != '\0') return 0;
    return 1;
}


int create_transaction(char sender[], char recipient[], int amount, Address **list_address) {
    add_unique_address(list_address, sender);
    add_unique_address(list_address, recipient);

    Address *current = *list_address;
    Address *sender_address = NULL;
    Address *recipient_address = NULL;

    while (current != NULL) {
        if (strncmp(current->address, sender, 13) == 0) {
            sender_address = current;
        }
        if (strncmp(current->address, recipient, 13) == 0) {
            recipient_address = current;
        }
        current = current->next;
    }

    if (sender_address == NULL) {
        printf("Sender address not found\n");
        return 0;
    }
    if (recipient_address == NULL) {
        printf("Recipient address not found\n");
        return 0;
    }

    Transaction t;
    strncpy(t.sender_wallet, sender, 13);
    strncpy(t.recipient_wallet, recipient, 13);
    t.amount = amount;
    t.tax = TAX_TRANSACTION;

    if (sender_address->balance >= t.tax + amount) {
        add_transaction(t,0);
        sender_address->balance -= (t.tax);
        return 1;
    } else {
        printf("Insufficient balance\n");
        return 0;
    }
}




int add_transaction(Transaction trans, int all) {
    if (current_node->transaction_count < MAX_TRANSACTIONS) {
        current_node->transaction_pool[current_node->transaction_count] = trans;
        current_node->transaction_count++;
        broadcast_transaction(trans,all);
        return 1;
    } else {
        return 0;
    }
}

void proof_of_work(Block *blk) {
    unsigned char *hash;
    char target[DIFFICULTY + 1];
    memset(target, '0', DIFFICULTY);
    target[DIFFICULTY] = '\0';

    unsigned long int counter = 0;
    hash = (unsigned char *)malloc(sizeof(unsigned char) * SHA256_DIGEST_LENGTH);
    printf("mining block");
    do {
        blk->nonce++;
        unsigned char *new_hash = hash_block(*blk);
        memcpy(hash, new_hash, SHA256_DIGEST_LENGTH);
        free(new_hash);
        counter++;

        if (counter % 4000000 == 0){
            clear_screen();
            printf("mining block");
        }

        if (counter % 1000000 == 0) {
            printf(".");
            fflush(stdout);
        }


    } while (strncmp((const char *)hash, target, DIFFICULTY) != 0);

    free(blk->hash);
    blk->hash = hash;

    printf("\nHash found after %lu attempts.\n", counter);

}

void print_transaction_pool() {
    int count = current_node->transaction_count;
    Transaction* pool = (Transaction*)malloc(100*sizeof(Transaction));
    memcpy(pool, current_node->transaction_pool, 100 * sizeof(int));
    if (count == 0) {
        printf("The transaction pool is empty.\n");
    } else {
        for (int i = 0; i < count; i++) {
            printf("Transaction %d:\n", i + 1);
            printf("  Sender: %s\n", pool[i].sender_wallet);
            printf("  Recipient: %s\n", pool[i].recipient_wallet);
            printf("  Amount: %d\n", pool[i].amount);
        }
    }
    free(pool);
}

int add_unique_address(Address **list_address, char address[]) {
    if (*list_address == NULL) {
        *list_address = (Address *)malloc(sizeof(Address));
        strncpy((*list_address)->address, address, 13);
        (*list_address)->balance = INITIAL_BALANCE;
        (*list_address)->next = NULL;
        return 1;
    }

    Address *current = *list_address;
    while (current->next != NULL) {
        if (strncmp(current->address, address, 13) == 0) {
            return 0;
        }
        current = current->next;
    }

    if (strncmp(current->address, address, 13) == 0) {
        return 0;
    }

    current->next = (Address *)malloc(sizeof(Address));
    current = current->next;
    strncpy(current->address, address, 13);
    current->balance = INITIAL_BALANCE;
    current->next = NULL;

    return 1;
}


int print_addresses(Address **list_address) {
    if (list_address == NULL || *list_address == NULL) {
        printf("The address list is empty.\n");
        return 0;
    }
    Address *current = *list_address;
    printf("Unique addresses in the Blockchain:\n");
    while (current != NULL) {
        printf("%s\n", current->address);
        current = current->next;
    }
    return 1;
}

int wallet_exists(Address **list_address, char wallet[13]) {
    if (list_address == NULL || *list_address == NULL) {
        return 0;
    }

    Address *current = *list_address;
    while (current != NULL) {
        if (strncmp(current->address, wallet, 13) == 0) {
            return 1;
        }
        current = current->next;
    }

    return 0;
}

void print_wallet_transactions(char wallet[13]) {
    Block *current = current_node->blockchain;
    int found = 0;

    printf("Transaction history for the wallet %s:\n", wallet);
    while (current != NULL) {
        if (strncmp(current->trans.sender_wallet, wallet, 13) == 0 ||
            strncmp(current->trans.recipient_wallet, wallet, 13) == 0) {
            printf("%s -> %s | Qtd: %d\n", current->trans.sender_wallet, current->trans.recipient_wallet, current->trans.amount);
            found = 1;
        }
        current = current->prev_blk;
    }

    if (!found) {
        printf("No transactions found for the wallet %s.\n", wallet);
    }
}

Node* create_main_node() {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Error allocating memory for the node\n");
        exit(1);
    }
    node->id = 0;
    node->blockchain = NULL;
    node->message_queue = NULL;
    node->transaction_count = 0;
    node->next = NULL;
    node->prev = NULL;
    network_length++;

    return node;
}

void update_latest_added_node(Node* node){
    latest_added_node = node;
}

void update_current_node(Node* node){
    current_node = node;
}


int get_id_current_node(){
    return current_node->id;
}

int get_network_length(){
    return network_length;
}

void create_node(){
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Error allocating memory for the node\n");
        exit(1);
    }

    new_node->blockchain = latest_added_node->blockchain;
    new_node->id = latest_added_node->id + 1;
    new_node->message_queue = latest_added_node->message_queue;
    new_node->next = NULL;
    new_node->prev = latest_added_node;
    memcpy(new_node->transaction_pool, latest_added_node->transaction_pool, 100 * sizeof(int));
    new_node->transaction_count = latest_added_node->transaction_count;
    new_node->transaction_count = latest_added_node->transaction_count;
    latest_added_node = new_node;
    current_node = new_node;
    network_length++;
    printf("You created and connected to the Node %d\n", current_node->id);
}

void print_network(){
    Node* hold = (Node*)malloc(sizeof(Node));
    if (hold == NULL) {
        fprintf(stderr, "Error allocating memory for the node\n");
        exit(1);
    }
    hold = latest_added_node;

    while(hold!= NULL){
        printf("Node %d\n", hold->id);
        hold = hold->prev;
    }

}

int change_node_to(int i){
    Node* hold = (Node*)malloc(sizeof(Node));
    int exists=0;
    if (hold == NULL) {
        fprintf(stderr, "Error allocating memory for the node\n");
        exit(1);
    }
    hold = latest_added_node;
    while (hold != NULL){
        if (hold->id == i){
            exists = 1;
            break;
        }
        hold = hold->prev;
    }
    if (exists == 0) return 0;
    current_node = hold;
    return 1;
}

int pool_length(){
    printf("\nsize: %d\n", current_node->transaction_count);
    return current_node->transaction_count;
}

Transaction pull_first_transaction(){
    Transaction trans = current_node->transaction_pool[0];
    for (int i = 0; i < current_node->transaction_count; i++) {
        current_node->transaction_pool[i] = current_node->transaction_pool[i + 1];
    }
    current_node->transaction_count--;
    return trans;
}


int verify_minimum_three() {
    int count = 0;
    Block *current_block = current_node->blockchain;

    while (current_block != NULL) {
        count++;
        if (count >= 3) {
            return 1;
        }
        current_block = current_block->prev_blk;
    }

    return 0;
}

void broadcast_transaction(Transaction trans, int all) {
    Message *new_message = (Message *)malloc(sizeof(Message));
    if (new_message == NULL) {
        fprintf(stderr, "Error allocating memory for the message\n");
        exit(1);
    }
    new_message->type = 'T';
    new_message->data.transaction = trans;
    new_message->last_message = NULL;

    Node *current = latest_added_node;
    while (current != NULL) {
        //all determina se o broadcast � para todos ou exclui o n� atual
        if (all == 1){
            new_message->last_message = current->message_queue;
            current->message_queue = new_message;
        } else {
            if (current != current_node){
                new_message->last_message = current->message_queue;
                current->message_queue = new_message;
            }
        }


        current = current->prev;
    }
}



void broadcast_block(Block *blk) {
    Message *new_message = (Message *)malloc(sizeof(Message));
    if (new_message == NULL) {
        fprintf(stderr, "Error allocating memory for the message\n");
        exit(1);
    }
    new_message->type = 'B';
    memcpy(&(new_message->data.block), blk, sizeof(Block));
    new_message->last_message = NULL;

    Node *current = latest_added_node;
    while (current != NULL) {
        if (current != current_node){
            new_message->last_message = current->message_queue;
            current->message_queue = new_message;
        }

        current = current->prev;
    }
}

void print_message_queue() {
    Message *msg = current_node->message_queue;
    printf("Node message queue %d:\n", current_node->id);
    if (msg == NULL) printf("\nA Message queue is empty!\n");
    while (msg != NULL) {
        if (msg->type == 'T') {
            printf("Type: Transaction\n");
            printf("Sender: %s\n", msg->data.transaction.sender_wallet);
            printf("Recipient: %s\n", msg->data.transaction.recipient_wallet);
            printf("Amount: %d\n", msg->data.transaction.amount);
        } else if (msg->type == 'B') {
            printf("Type: Block\n");
            printf("Block ID: %zu\n", msg->data.block.id);
            printf("Hash: ");
            for (size_t i = 0; i < SHA256_DIGEST_LENGTH; i++) {
                printf("%02X", msg->data.block.hash[i]);
            }
            printf("\n");
            printf("Nonce: %lu\n", msg->data.block.nonce);
            printf("Timestamp: %s", ctime(&msg->data.block.timestamp));
            printf("Transaction Amount: %d\n", msg->data.block.trans.amount);
        }
        printf("\n");
        msg = msg->last_message;
    }
}


int process_message_queue() {
    if (current_node->message_queue == NULL) return 0;

    Message *msg = current_node->message_queue;
    Message *prev_msg = NULL;

    while (msg != NULL) {
        printf("Checking message type %c\n", msg->type);

        if (msg->type != 'T' && msg->type != 'B') {
            printf("Invalid type message: %c\n", msg->type);
            return -1;
        }

        if (msg->type == 'T') {

            if (current_node->transaction_count < MAX_TRANSACTIONS) {
                current_node->transaction_pool[current_node->transaction_count] = msg->data.transaction;
                current_node->transaction_count++;
                printf("Transaction added\n");
            } else {
                printf("Transaction limit reached!\n");
            }
            if (prev_msg == NULL) {
                current_node->message_queue = msg->last_message;
            } else {
                prev_msg->last_message = msg->last_message;
            }
            Message *temp = msg;
            msg = msg->last_message;

        } else if (msg->type == 'B') {
            Block *current_block = current_node->blockchain;
            int exists = 0;
            while (current_block != NULL) {
                if (memcmp(current_block->hash, msg->data.block.hash, SHA256_DIGEST_LENGTH) == 0) {
                    exists = 1;
                    break;
                }
                current_block = current_block->prev_blk;
            }
            if (!exists) {
                int fork_resolution_result = handle_fork_resolution();
                if (fork_resolution_result == FORK_RESOLVED) {
                    printf("Fork resolved and longest chain adopted.\n");
                    if (prev_msg == NULL) {
                        current_node->message_queue = msg->last_message;
                    } else {
                        prev_msg->last_message = msg->last_message;
                    }
                    Message *temp = msg;
                    msg = msg->last_message;
                    // if (temp != NULL) {
                    //    free(temp);
                    //    temp = NULL;
                    // }
                } else if (fork_resolution_result == NO_NEXT_BLOCK_FOUND) {
                    printf("Fork detected, but no subsequent block. Message remains in the queue.\n");
                    prev_msg = msg;
                    msg = msg->last_message;
                } else if (fork_resolution_result == NO_BLOCK_WITH_SAME_ID_FOUND) {
                    printf("No message with the same ID as the last block was found.\n");
                    Block *new_block = (Block *)malloc(sizeof(Block));
                    if (new_block == NULL) {
                        fprintf(stderr, "Error allocating memory for the block\n");
                        exit(1);
                    }
                    memcpy(new_block, &(msg->data.block), sizeof(Block));
                    new_block->prev_blk = current_node->blockchain;
                    current_node->blockchain = new_block;
                    printf("Block added to the blockchain.\n");

                    // remove a mensagem da fila de mensagens
                    if (prev_msg == NULL) {
                        current_node->message_queue = msg->last_message;
                    } else {
                        prev_msg->last_message = msg->last_message;
                    }
                    Message *temp = msg;
                    msg = msg->last_message;
                    // if (temp != NULL) {
                    //    free(temp);
                    //    temp = NULL;
                    // }
                }
            } else {

                if (prev_msg == NULL) {
                    current_node->message_queue = msg->last_message;
                } else {
                    prev_msg->last_message = msg->last_message;
                }
                Message *temp = msg;
                msg = msg->last_message;

                // if (temp != NULL) {
                //    free(temp);
                //    temp = NULL;
                // }
            }
        }
    }

    return 1;
}





Message* find_message_with_block_id() {
    if (current_node->blockchain == NULL) {
        return NULL;
    }

    Block *latest_block = current_node->blockchain;
    Message *msg = current_node->message_queue;

    while (msg != NULL) {
        if (msg->type == 'B' && msg->data.block.id == latest_block->id) {
            return msg;
        }
        msg = msg->last_message;
    }
    return NULL;
}

Message* find_next_block_message(Block *current_block) {
    if (current_block == NULL) {
        return NULL;
    }

    int next_block_id = current_block->id + 1;
    Message *msg = current_node->message_queue;

    while (msg != NULL) {
        if (msg->type == 'B' && msg->data.block.id == next_block_id) {
            return msg;
        }
        msg = msg->last_message;
    }
    return NULL;
}

int handle_fork_resolution() {
    Message *current_msg = find_message_with_block_id();
    if (current_msg != NULL) {
        Block *current_block = &current_msg->data.block;
        printf("Current block found: ID %zu\n", current_block->id);

        Message *next_msg = find_next_block_message(current_block);
        if (next_msg != NULL) {
            printf("Next block in sequence found: ID %zu\n", next_msg->data.block.id);

            if (memcmp(next_msg->data.block.prev_hash, current_node->blockchain->hash, SHA256_DIGEST_LENGTH) == 0) {
                printf("Hashes match. Adding transaction from current_msg to the transaction pool.\n");
                add_transaction(current_msg->data.block.trans,1);
            } else {
                printf("Hashes do not match. Adding transaction from the last block of the blockchain to the transaction pool.\n");
                add_transaction(current_node->blockchain->trans,1);
            }

            current_node->blockchain = &next_msg->data.block;

            printf("Fork resolved by adopting the longest chain.\n");
            return FORK_RESOLVED;
        } else {
            printf("No message with the next block ID was found.\n");
            return NO_NEXT_BLOCK_FOUND;
        }
    } else {
        printf("No message with the same ID as the last block was found.\n");
        return NO_BLOCK_WITH_SAME_ID_FOUND;
    }
}


unsigned char* create_sha256_transaction(const unsigned char* data, unsigned char *buffer) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data, strlen((char*)data));
    SHA256_Final(buffer, &sha256);
    return buffer;
}

unsigned char* transaction_hash(Transaction transaction) {
    unsigned char *transaction_record = malloc(sizeof(char) * 128);
    if (transaction_record == NULL) {
        fprintf(stderr, "Error allocating memory for the transaction record\n");
        exit(1);
    }
    sprintf((char*)transaction_record, " %lld %s %s %i",
            transaction.timestamp,
            transaction.sender_wallet,
            transaction.recipient_wallet,
            transaction.amount);

    unsigned char *buffer = malloc(sizeof(char) * SHA256_DIGEST_LENGTH);
    if (buffer == NULL) {
        fprintf(stderr, "Error allocating memory for the hash\n");
        free(transaction_record);
        exit(1);
    }
    buffer = create_sha256_transaction((const unsigned char*)transaction_record, buffer);
    free(transaction_record);
    return buffer;
}

int transaction_exists_in_messages(Transaction *transaction) {
    unsigned char *target_hash = transaction_hash(*transaction);

    Node *current_node = latest_added_node;
    while (current_node != NULL) {
        Message *msg = current_node->message_queue;
        Message *prev_msg = NULL;

        while (msg != NULL) {
            if (msg->type == 'T') {
                unsigned char *msg_hash = transaction_hash(msg->data.transaction);
                if (memcmp(target_hash, msg_hash, SHA256_DIGEST_LENGTH) == 0) {
                    free(target_hash);
                    free(msg_hash);

                    if (prev_msg == NULL) {
                        current_node->message_queue = msg->last_message;
                    } else {
                        prev_msg->last_message = msg->last_message;
                    }
                    free(msg);

                    return 1;
                }
                free(msg_hash);
            }
            prev_msg = msg;
            msg = msg->last_message;
        }
        current_node = current_node->prev;
    }

    free(target_hash);
    return 0;
}

int check_balance(Address **list_address, char wallet_address[13]) {
    if (list_address == NULL || *list_address == NULL) {
        return 0;
    }
    Address *current = *list_address;

    while (current != NULL) {
        if (strncmp(current->address, wallet_address, 13) == 0) {
            return current->balance;
        }
        current = current->next;
    }

    printf("Wallet %s not found.\n", wallet_address);
    return 0;
}

Address* select_wallet(Address **list_address, char wallet_address[13]) {
    if (list_address == NULL || *list_address == NULL) {
        return NULL;
    }
    Address *current = *list_address;

    while (current != NULL) {
        if (strncmp(current->address, wallet_address, 13) == 0) {
            return current;
        }
        current = current->next;
    }

    printf("Wallet %s not found.\n", wallet_address);
    return NULL;
}





