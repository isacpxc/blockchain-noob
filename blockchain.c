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

Block* create_genesis_block(){
    Block* blk = (Block *)malloc(sizeof(Block));
    if (blk == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o nó\n");
        exit(1);
    }
    blk->root = (MerkleNode *)malloc(sizeof(MerkleNode));
    if (blk->root == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o nó\n");
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
    printf("Transação:\n");
    printf("%s -> %s | quantidade: %d\n", blk->trans.sender_wallet,blk->trans.recipient_wallet,blk->trans.amount);
    printf("\n");
    return 1;
}

int print_chain(){
    if (current_node->blockchain == NULL){
        printf("\n\nSua Blockchain ainda não existe amigo!\n\n");
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
        printf("Erro: Nó ou blockchain não inicializado.\n");
        return 0;
    }

    Block *current = current_node->blockchain;

    Block *blk = (Block *)malloc(sizeof(Block));
    if (!blk) {
        printf("Erro de alocação de memória!\n");
        return 0;
    }

    MerkleNode *mk = (MerkleNode *)malloc(sizeof(MerkleNode));
    if (!mk) {
        printf("Erro de alocação de memória!\n");
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

    // Verificar novamente se o remetente tem saldo suficiente
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
        transaction_exists_in_messages(&trans); // retira transação das mensagens dos outros nós

        return 1;
    } else {
        printf("Bloco não pode ser minerado, pois sender não tem saldo para realizar transação!\n");
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

//int create_transaction(char sender[], char recipient[], int amount,Address **list_address) {
//    Transaction t;
//    strncpy(t.sender_wallet, sender, 13);
//    strncpy(t.recipient_wallet, recipient, 13);
//    t.amount = amount;
//    add_unique_address(list_address, sender);
//    add_unique_address(list_address, recipient);
//    add_transaction(t);
//    return 1;
//}

int create_transaction(char sender[], char recipient[], int amount, Address **list_address) {
    // Adicionar endereços únicos à lista
    add_unique_address(list_address, sender);
    add_unique_address(list_address, recipient);

    // Encontrar o endereço do remetente e do destinatário na lista para verificar o saldo
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
        printf("Endereço do remetente não encontrado\n");
        return 0;
    }
    if (recipient_address == NULL) {
        printf("Endereço do destinatário não encontrado\n");
        return 0;
    }

    Transaction t;
    strncpy(t.sender_wallet, sender, 13);
    strncpy(t.recipient_wallet, recipient, 13);
    t.amount = amount;
    t.tax = TAX_TRANSACTION;

    if (sender_address->balance >= t.tax + amount) {
        // Criar a transação se o saldo for suficiente
        add_transaction(t);
        // Atualizar saldo do remetente e destinatário
        sender_address->balance -= (t.tax);
        return 1;
    } else {
        printf("Saldo insuficiente\n");
        return 0;
    }
}




int add_transaction(Transaction trans) {
    if (current_node->transaction_count < MAX_TRANSACTIONS) {
        current_node->transaction_pool[current_node->transaction_count] = trans;
        current_node->transaction_count++;
        broadcast_transaction(trans);
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

printf("\nHash encontrado após %lu tentativas.\n", counter);

}

void print_transaction_pool() {
    int count = current_node->transaction_count;
    Transaction* pool = (Transaction*)malloc(100*sizeof(Transaction));
    memcpy(pool, current_node->transaction_pool, 100 * sizeof(int));
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
//    printf("printado");
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

void print_wallet_transactions(char wallet[13]) {
    Block *current = current_node->blockchain;
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

Node* create_main_node() {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o nó\n");
        exit(1);
    }
    node->id = 0;
    node->blockchain = NULL;
    node->message_queue = NULL;
    node->transaction_count = 0; // Inicializa contagem de transações
    node->next = NULL;
    node->prev = NULL;
    network_length++;
//    printf("teste: %d", node->id);

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
        fprintf(stderr, "Erro ao alocar memória para o nó\n");
        exit(1);
    }

    new_node->blockchain = latest_added_node->blockchain;
//    printf("id = %d\n", latest_added_node->id);
    new_node->id = latest_added_node->id + 1;
//    printf("id = %d\n", new_node->id);
    new_node->message_queue = latest_added_node->message_queue;
    new_node->next = NULL;
    new_node->prev = latest_added_node;
    memcpy(new_node->transaction_pool, latest_added_node->transaction_pool, 100 * sizeof(int));
    new_node->transaction_count = latest_added_node->transaction_count;
    new_node->transaction_count = latest_added_node->transaction_count;
    latest_added_node = new_node;
    current_node = new_node;
    network_length++;
    printf("Você criou e se conectou ao Nó %d\n", current_node->id);
}

void print_network(){
    Node* hold = (Node*)malloc(sizeof(Node));
    if (hold == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o nó\n");
        exit(1);
    }
    hold = latest_added_node;

    while(hold!= NULL){
        printf("Nó %d\n", hold->id);
        hold = hold->prev;
    }

}

int change_node_to(int i){
    Node* hold = (Node*)malloc(sizeof(Node));
    int exists=0;
    if (hold == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o nó\n");
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
    printf("\ntamanho: %d\n", current_node->transaction_count);
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
            return 1; // Retorna 1 se existirem pelo menos 3 blocos
        }
        current_block = current_block->prev_blk;
    }

    return 0; // Retorna 0 se existirem menos de 3 blocos
}

void broadcast_transaction(Transaction trans) {
    Message *new_message = (Message *)malloc(sizeof(Message));
    if (new_message == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a mensagem\n");
        exit(1);
    }
    new_message->type = 'T';
    new_message->data.transaction = trans;
    new_message->last_message = NULL;

    Node *current = latest_added_node;
    while (current != NULL) {
        // Adiciona a mensagem na fila de mensagens do nó
        new_message->last_message = current->message_queue;
        current->message_queue = new_message;

        current = current->prev;
    }
}



void broadcast_block(Block *blk) {
    Message *new_message = (Message *)malloc(sizeof(Message));
    if (new_message == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a mensagem\n");
        exit(1);
    }
    new_message->type = 'B';
    memcpy(&(new_message->data.block), blk, sizeof(Block));
    new_message->last_message = NULL;

    Node *current = latest_added_node;
    while (current != NULL) {
        // Adiciona a mensagem na fila de mensagens do nó
        new_message->last_message = current->message_queue;
        current->message_queue = new_message;

        current = current->prev;
    }
}

void print_message_queue() {
    Message *msg = current_node->message_queue;
    printf("Fila de mensagens do nó %d:\n", current_node->id);
    if (msg == NULL) printf("\nA fila de mensagens está vazia!\n");
    while (msg != NULL) {
        if (msg->type == 'T') {
            printf("Tipo: Transação\n");
            printf("Remetente: %s\n", msg->data.transaction.sender_wallet);
            printf("Destinatário: %s\n", msg->data.transaction.recipient_wallet);
            printf("Montante: %d\n", msg->data.transaction.amount);
        } else if (msg->type == 'B') {
            printf("Tipo: Bloco\n");
            printf("ID do Bloco: %zu\n", msg->data.block.id);
            printf("Hash: ");
            for (size_t i = 0; i < SHA256_DIGEST_LENGTH; i++) {
                printf("%02X", msg->data.block.hash[i]);
            }
            printf("\n");
            printf("Nonce: %lu\n", msg->data.block.nonce);
            printf("Timestamp: %s", ctime(&msg->data.block.timestamp));
            printf("Montante da Transação: %d\n", msg->data.block.trans.amount);
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
        printf("Verificando mensagem do tipo %c\n", msg->type);  // Depuração

        // Adicione uma verificação de tipo de mensagem válido
        if (msg->type != 'T' && msg->type != 'B') {
            printf("Tipo de mensagem inválido: %c\n", msg->type);  // Depuração
            return -1;  // Sai da função em caso de tipo inválido
        }

        if (msg->type == 'T') {
            // Adiciona a transação à pool sem verificar duplicidade
            if (current_node->transaction_count < MAX_TRANSACTIONS) {
                current_node->transaction_pool[current_node->transaction_count] = msg->data.transaction;
                current_node->transaction_count++;
                printf("Transação adicionada\n");  // Depuração
            } else {
                printf("Limite de transações atingido!\n");
            }
            // Remove a mensagem da fila de mensagens
            if (prev_msg == NULL) {
                current_node->message_queue = msg->last_message;
            } else {
                prev_msg->last_message = msg->last_message;
            }
            Message *temp = msg;
            msg = msg->last_message;
            // Adiciona verificação antes de liberar a memória
            // if (temp != NULL) {
            //    free(temp);
            //    temp = NULL;  // Garanta que o ponteiro não seja usado após a liberação
            // }
        } else if (msg->type == 'B') {
            // Verifica se o bloco já existe na blockchain
            Block *current_block = current_node->blockchain;
            int exists = 0;
            while (current_block != NULL) {
                if (memcmp(current_block->hash, msg->data.block.hash, SHA256_DIGEST_LENGTH) == 0) {
                    exists = 1;
                    break;
                }
                current_block = current_block->prev_blk;
            }
            // Adiciona o bloco à blockchain se não existir
            if (!exists) {
                int fork_resolution_result = handle_fork_resolution();
                if (fork_resolution_result == FORK_RESOLVED) {
                    printf("Fork resolvido e cadeia mais longa adotada.\n");
                    // Remove a mensagem da fila de mensagens
                    if (prev_msg == NULL) {
                        current_node->message_queue = msg->last_message;
                    } else {
                        prev_msg->last_message = msg->last_message;
                    }
                    Message *temp = msg;
                    msg = msg->last_message;
                    // Adiciona verificação antes de liberar a memória
                    // if (temp != NULL) {
                    //    free(temp);
                    //    temp = NULL;  // Garanta que o ponteiro não seja usado após a liberação
                    // }
                } else if (fork_resolution_result == NO_NEXT_BLOCK_FOUND) {
                    printf("Fork detectado, mas sem bloco subsequente. Mensagem permanece na fila.\n");
                    prev_msg = msg;
                    msg = msg->last_message;
                } else if (fork_resolution_result == NO_BLOCK_WITH_SAME_ID_FOUND) {
                    printf("Nenhuma mensagem com o mesmo ID do último bloco foi encontrada.\n");
                    // Adiciona o bloco da mensagem à blockchain
                    Block *new_block = (Block *)malloc(sizeof(Block));
                    if (new_block == NULL) {
                        fprintf(stderr, "Erro ao alocar memória para o bloco\n");
                        exit(1);
                    }
                    memcpy(new_block, &(msg->data.block), sizeof(Block));
                    new_block->prev_blk = current_node->blockchain;
                    current_node->blockchain = new_block;
                    printf("Bloco adicionado à blockchain.\n");

                    // remove a mensagem da fila de mensagens
                    if (prev_msg == NULL) {
                        current_node->message_queue = msg->last_message;
                    } else {
                        prev_msg->last_message = msg->last_message;
                    }
                    Message *temp = msg;
                    msg = msg->last_message;
                    // Adiciona verificação antes de liberar a memória
                    // if (temp != NULL) {
                    //    free(temp);
                    //    temp = NULL;  // Garanta que o ponteiro não seja usado após a liberação
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
                //    temp = NULL;  // Garanta que o ponteiro não seja usado após a liberação
                // }
            }
        }
    }

    return 1;
}





Message* find_message_with_block_id() {
    if (current_node->blockchain == NULL) {
        return NULL; // Blockchain não inicializada
    }

    Block *latest_block = current_node->blockchain;
    Message *msg = current_node->message_queue;

    while (msg != NULL) {
        if (msg->type == 'B' && msg->data.block.id == latest_block->id) {
            return msg; // Encontrou uma mensagem com o mesmo ID do último bloco
        }
        msg = msg->last_message;
    }
    return NULL; // Nenhuma mensagem com o mesmo ID do último bloco foi encontrada
}

Message* find_next_block_message(Block *current_block) {
    if (current_block == NULL) {
        return NULL; // Bloco atual não fornecido
    }

    int next_block_id = current_block->id + 1;
    Message *msg = current_node->message_queue;

    while (msg != NULL) {
        if (msg->type == 'B' && msg->data.block.id == next_block_id) {
            return msg; // Encontrou uma mensagem com o próximo ID de bloco
        }
        msg = msg->last_message;
    }
    return NULL; // Nenhuma mensagem com o próximo ID de bloco foi encontrada
}

int handle_fork_resolution() {
    // Primeiro, encontre a mensagem com o mesmo ID do último bloco da blockchain
    Message *current_msg = find_message_with_block_id();
    if (current_msg != NULL) {
        Block *current_block = &current_msg->data.block;
        printf("Bloco atual encontrado: ID %zu\n", current_block->id);

        // Agora, procure por uma mensagem que contenha o próximo bloco na sequência
        Message *next_msg = find_next_block_message(current_block);
        if (next_msg != NULL) {
            printf("Próximo bloco na sequência encontrado: ID %zu\n", next_msg->data.block.id);

            // Verifica se prev_hash de next_msg é igual ao hash do último bloco da blockchain atual
            if (memcmp(next_msg->data.block.prev_hash, current_node->blockchain->hash, SHA256_DIGEST_LENGTH) == 0) {
                printf("Hashes correspondem. Adicionando transação de current_msg à pool de transações.\n");
                add_transaction(current_msg->data.block.trans);
            } else {
                printf("Hashes não correspondem. Adicionando transação do último bloco da blockchain à pool de transações.\n");
                add_transaction(current_node->blockchain->trans);
            }

            // Integra a blockchain do bloco next_msg na blockchain do nó atual
            current_node->blockchain = &next_msg->data.block;

            printf("Fork resolvido adotando a cadeia mais longa.\n");
            return FORK_RESOLVED;
        } else {
            printf("Nenhuma mensagem com o próximo ID de bloco foi encontrada.\n");
            return NO_NEXT_BLOCK_FOUND;
        }
    } else {
        printf("Nenhuma mensagem com o mesmo ID do último bloco foi encontrada.\n");
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
        fprintf(stderr, "Erro ao alocar memória para o registro da transação\n");
        exit(1);
    }
    sprintf((char*)transaction_record, " %lld %s %s %i",
            transaction.timestamp,
            transaction.sender_wallet,
            transaction.recipient_wallet,
            transaction.amount);

    unsigned char *buffer = malloc(sizeof(char) * SHA256_DIGEST_LENGTH);
    if (buffer == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o hash\n");
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

                    // Remover a mensagem da lista
                    if (prev_msg == NULL) {
                        current_node->message_queue = msg->last_message;
                    } else {
                        prev_msg->last_message = msg->last_message;
                    }
                    free(msg);

                    return 1; // Transação encontrada e mensagem removida
                }
                free(msg_hash);
            }
            prev_msg = msg;
            msg = msg->last_message;
        }
        current_node = current_node->prev;
    }

    free(target_hash);
    return 0; // Transação não encontrada
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

    printf("Carteira %s não encontrada.\n", wallet_address);
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

    printf("Carteira %s não encontrada.\n", wallet_address);
    return NULL;
}





