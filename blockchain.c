#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "blockchain.h"
#include <openssl/sha.h>
#include <ctype.h>

const char* test(int i){
    printf("print says: %d\n",i);
    return "tudo ok";
}

void print_instructions() {
    printf("===== Instruções de Utilização do Programa Blockchain =====\n\n");
    printf("0 - Instruções\n");
    printf("    Exibe estas instruções.\n\n");
    printf("1 - Criar Bloco Gênesis\n");
    printf("    Cria o bloco inicial da blockchain. Este deve ser o primeiro passo.\n\n");
    printf("2 - Adicionar Novo Bloco à cadeia\n");
    printf("    Adiciona um novo bloco à blockchain. Somente pode ser feito após a criação do bloco gênesis.\n\n");
    printf("3 - Exibir Blockchain\n");
    printf("    Exibe todos os blocos presentes na blockchain.\n\n");
    printf("4 - Validar Blockchain\n");
    printf("    Verifica a integridade da blockchain, garantindo que todos os blocos são válidos.\n\n");
    printf("5 - Adultere os dados da penultima blockchain\n");
    printf("    Altera os dados do penúltimo bloco na blockchain, gerando um novo hash para ele.\n\n");
    printf("6 - Limpar Blockchain\n");
    printf("    Remove todos os blocos da blockchain, limpando-a completamente.\n\n");
    printf("7 - Visualização em Cadeia\n");
    printf("    Mostra uma visualização dos blocos da blockchain em formato de cadeia.\n\n");
    printf("8 - Fazer transação\n");
    printf("    Inicializa uma transação.\n\n");
    printf("9 - Imprimir pool de transações pendentes\n");
    printf("    Se existirem transações na pool de transações, faz a impressão.\n\n");
    printf("10 - histórico de transações\n");
    printf("     Exibe histórico de transação de uma determinada carteira.\n\n");
    printf("11 - Sair\n");
    printf("     Encerra o programa.\n\n");
    printf("==============================================\n\n");
    printf("Observações: \n\n");
    printf("*Quando se adiciona uma transação ela fica em espera numa pool de transações\n");
    printf("*Após isso minere um bloco\n");
    printf("*Quando um novo bloco for encontrado automaticamente a transação será colocada nele\n");
    printf("*No momento cada bloco suporta apenas uma transação\n");
    printf("*Formato esperado de endereço válido: 3 letras maiúsculas seguidos de 8 inteiros decimais seguido de uma letra maiúscula\n");
    printf("**EXEMPLO: ABC12345678Z\n");
    printf("*Tipo de 'amount' na transação: int\n");
    printf("*A dificuldade(padrão = 2) de mineração do bloco pode ser ajustada em blockchain.h(!!!cuidado com a ousadia!!!)\n");
    printf("*Se a dificuldade é X, o hash resultante deve conter uma quantidade X de '30' na frente, lembre-se que a impressão é em hexadecimal.\n");
    printf("*'30'(hex) = '48'(dec) = '0'(string).\n\n");
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
    printf("Transação:\n");
    printf("%s -> %s | quantidade: %d\n", blk->trans.sender_wallet,blk->trans.recipient_wallet,blk->trans.amount);
    printf("\n");
    return 1;
}

int print_chain(Block *head){
    if (head == NULL){
        printf("\n\nSua Blockchain ainda não existe amigo!\n\n");
        return 0;
    }

    Block *p = head;

    while (p != NULL){
        print_blk(p);
        p = p->prev_blk;
    }
    return 1;
}

int add_blk(Block *current, Transaction trans, Block **p){

    Block *blk = malloc(sizeof(Block));
    if (!blk) {
        printf("Erro de alocação de memória!\n");
        return 0;
    }

    MerkleNode *mk = (MerkleNode*)malloc(sizeof(MerkleNode));
    if (!mk) {
        printf("Erro de alocação de memória!\n");
        return 0;
    }

    mk->left = NULL;
    mk->right = NULL;
    mk->hash = data_hash256(trans.amount);

    blk->id = current->id+1;
    blk->data = trans.amount;
    blk->nonce = current->nonce + 1;
    blk->prev_hash = current->hash;
    blk->root = mk;
    blk->timestamp = time(NULL);
    blk->prev_blk = current;
    blk->hash = hash_block(*blk);
    blk->trans = trans;

    proof_of_work(blk);

    *p = blk;

    return 1;

}

int verify_null(Block *head){
    if (head == NULL) return 1;
    return 0;
}

int verify_prev_null(Block *head){
    if (head->prev_blk == NULL) return 1;
    return 0;
}

int validate_chain(Block *head){
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
    printf("Blockchain validada!\n");
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

int corrupt_chain(Block *head){
    head->prev_blk->data = 367256;
    Block *prev = malloc(sizeof(Block));
    prev = head->prev_blk;
    head->prev_blk->hash = hash_block(*prev);
    return 1;
}

void print_visual_chain(Block *head){
    Block *p = (Block*)malloc(sizeof(Block));
    p = head;

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

void generate_wallet_address(char* wallet_address) {
    for (int i = 0; i < 3; i++) {
        wallet_address[i] = random_uppercase_letter();
    } for (int i = 3; i < 11; i++) {
        wallet_address[i] = random_digit();
    }
    wallet_address[11] = random_uppercase_letter();
    wallet_address[12] = '\0';
}

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

Transaction create_transaction(char sender[], char recipient[], int amount,Address **list_address) {
    Transaction t;
    strncpy(t.sender_wallet, sender, 13);
    strncpy(t.recipient_wallet, recipient, 13);
    t.amount = amount;
    add_unique_address(list_address, sender);
    add_unique_address(list_address, recipient);
    return t;
}

int add_transaction(Transaction transactions[], int num_trans, Transaction trans) {
    if (num_trans < MAX_TRANSACTIONS) {
        transactions[num_trans] = trans;
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

    int counter = 0;

    do {
        blk->nonce++;
        hash = hash_block(*blk);
        counter++;

        if (counter % 10000 == 0) {
            printf(".");
            fflush(stdout);
        }
    } while (strncmp((const char*)hash, target, DIFFICULTY) != 0);

    free(blk->hash);
    blk->hash = hash;

    printf("\nHash encontrado após %d tentativas.\n", counter);
}

void print_transaction_pool(Transaction pool[], int count) {
    if (count == 0) {
        printf("A pool de transações está vazia.\n");
    } else {
        for (int i = 0; i < count; i++) {
            printf("Transação %d:\n", i + 1);
            printf("  Remetente: %s\n", pool[i].sender_wallet);
            printf("  Destinatário: %s\n", pool[i].recipient_wallet);
            printf("  Quantidade: %d\n", pool[i].amount);
        }
    }
}

int add_unique_address(Address **list_address, char address[]) {

    if (*list_address == NULL) {
        *list_address = (Address *)malloc(sizeof(Address));
        strncpy((*list_address)->address, address, 13);
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
    current->next = NULL;

    return 1;
}

int print_addresses(Address **list_address) {
    if (list_address == NULL || *list_address == NULL) {
        printf("A lista de endereços está vazia.\n");
        return 0;
    }
    Address *current = *list_address;
    printf("Endereços únicos na Blockchain:\n");
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

void print_wallet_transactions(Block *head, char wallet[13]) {
    Block *current = head;
    int found = 0;

    printf("Histórico de transações para a carteira %s:\n", wallet);
    while (current != NULL) {
        if (strncmp(current->trans.sender_wallet, wallet, 13) == 0 ||
            strncmp(current->trans.recipient_wallet, wallet, 13) == 0) {
            printf("%s -> %s | Qtd: %d\n", current->trans.sender_wallet, current->trans.recipient_wallet, current->trans.amount);
            found = 1;
        }
        current = current->prev_blk;
    }

    if (!found) {
        printf("Nenhuma transação encontrada para a carteira %s.\n", wallet);
    }
}


