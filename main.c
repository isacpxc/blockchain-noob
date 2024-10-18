#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#include <openssl/sha.h>
#include "block.h"



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
    printf("8 - Sair\n");
    printf("    Encerra o programa.\n\n");
    printf("==============================================\n");
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#elif defined(__linux__) || defined(__APPLE__)
    system("clear");
#endif
}

void pause_screen() {
#ifdef _WIN32
    system("pause");
#elif defined(__linux__) || defined(__APPLE__)
    printf("Pressione Enter para continuar...");
    getchar();
#endif
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

unsigned char *hash_block(Block block){
    unsigned char *block_record = malloc(sizeof(char)*512);
    sprintf((char*)block_record," %i %ld %lu %s %s %i", block.id, block.timestamp, block.nonce, (char*)block.prev_hash,(char*)block.root->hash,block.data);
    unsigned char *buffer = malloc(sizeof(char)*SHA256_DIGEST_LENGTH);
    buffer = create_sha256((const unsigned char*)block_record,buffer);
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
        printf("\nerror: id invalid");
        return 0;
    }

    if (!compare_block_and_prev_hashes(blk,prev_blk)){
        printf("\nerror: prev hash invalid");
        return 0;
    }

    return 1;
}

void print_blk(Block *blk){
    if (blk == NULL) {
        printf("NULL!!!!!!!\n");
        return;
    }
    printf("BLK %i ->\n",blk->id);
    printf("id: %i\n",blk->id);
    printf("data: %i\n",blk->data);
    printf("timestamp: %ld\n",blk->timestamp);
    printf("merkle root: ");
    for(int i = 0; i<SHA256_DIGEST_LENGTH;i++){
        printf("%02hhX", blk->root->hash[i]);
    }
    printf("\n");
    printf("nonce: %lu\n",blk->nonce);
    printf("hash: ");
    for(int i = 0; i<SHA256_DIGEST_LENGTH;i++){
        printf("%02hhX", blk->hash[i]);
    }
    printf("\n");
    printf("prev_hash: ");
    if (blk->prev_hash == NULL){
        printf("NULL\n");
    } else {
        for(int i = 0; i<SHA256_DIGEST_LENGTH;i++){
            printf("%02hhX", blk->prev_hash[i]);
        }
        printf("\n");
    }
    printf("\n");
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
}


int add_blk(Block *current, int data, Block **p){

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
    mk->hash = data_hash256(data);

    blk->id = current->id+1;
    blk->data = data;
    blk->nonce = current->nonce + 1;
    blk->prev_hash = current->hash;
    blk->root = mk;
    blk->timestamp = time(NULL);
    blk->prev_blk = current;
    blk->hash = hash_block(*blk);

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
        printf("||BLK %i||",p->id);
        if (p->prev_blk != NULL) printf("==>");
        p = p->prev_blk;
    }
    printf("\n\n");

}


int main()
{

//    pause_screen();
    Block *blk_current = NULL;
    int out = 0;
    int choice;
    setlocale(LC_ALL, "Portuguese");



    while(!out){
        printf("=======GUI Blockchain Example Structure=======\n\n");
        printf("0 - Instruções\n");
        printf("1 - Criar Bloco Gênesis\n");
        printf("2 - Adicionar Novo Bloco à cadeia\n");
        printf("3 - Exibir Blockchain\n");
        printf("4 - Validar Blockchain\n");
        printf("5 - Adultere os dados do penultimo bloco\n");
        printf("6 - Limpar Blockchain\n");
        printf("7 - Visualização em Cadeia\n");
        printf("8 - Sair\n\n");
        printf("==============================================\n");
        printf("Escolha uma das opções acima: ");
        scanf("%i",&choice);
        if (choice == 8){
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
            if(verify_null(blk_current) == 0){
               print_visual_chain(blk_current);
            } else {
                printf("Insira pelo menos o bloco Gênesis para isso!\n");
            }

            pause_screen();
        }

        else if (choice == 1){
            clear_screen();
            if (blk_current != NULL ){
                printf("O bloco Gênesis ja foi criado anteriormente!\n");
            } else {


                Block blk_gen;
                blk_gen.root = malloc(sizeof(MerkleNode));

                blk_gen.id = 0;
                blk_gen.timestamp = time(NULL);
                blk_gen.nonce = 0;
                blk_gen.prev_hash = NULL;
                blk_gen.prev_blk = NULL;
                blk_gen.data = 0;
                blk_gen.root->left = NULL;
                blk_gen.root->right = NULL;
                blk_gen.root->hash = data_hash256(blk_gen.data);
                blk_gen.hash = hash_block(blk_gen);
                blk_current = &blk_gen;

                printf("\nBloco Gênesis Criado com Sucesso!\n");
            }

            pause_screen();
        }

        else if (choice == 2){
            clear_screen();
            if (verify_null(blk_current)){
                printf("Antes de adicionar algum Bloco, Crie o Bloco Gênesis!\n");
            } else {
                if (verify_prev_null(blk_current)){
                    int data;
                    printf("(valor esperado a seguir: inteiro)\nDigite o dado desse bloco(abstração de transação): ");
                    scanf("%i",&data);
                    int conf = add_blk(blk_current,data,&blk_current);
                    if (conf == 1) printf("\n\nBloco adicionado com Sucesso!\n");
                } else {
                    if (validate_chain(blk_current)){
                        int data;
                        printf("Digite o dado desse bloco(int): ");
                        scanf("%i",&data);
                        int conf = add_blk(blk_current,data,&blk_current);
                        if (conf == 1) printf("\n\nBloco adicionado com Sucesso!\n");
                    } else {
                        printf("\n\nO bloco não pode ser adicionado pois a blockchain não passou na validação!\n\n");
                    }

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
            if (verify_null(blk_current)){
                printf("A blockchain ainda não existe para ser validada!\n");
            } else {
                if (verify_prev_null(blk_current)){
                    printf("Apenas o bloco gênesis compõe essa blockchain!\n");
                } else {
                    if (!validate_chain(blk_current)){
                        printf("Blockchain não é válida!\n");
                    } else {
                        printf("Blockchain é válida!\n");
                    }
                }
            }
            pause_screen();
        }
        else if (choice == 5){
            int count=3;
            if (blk_current == NULL || blk_current->prev_blk == NULL || blk_current->prev_blk->prev_blk == NULL || blk_current->prev_blk->prev_blk->prev_blk == NULL){
                printf("Insira pelo menos 3 blocos na blockchain para isso!\n");
            } else {
                int corruption = corrupt_chain(blk_current);
                if (corruption) {
                    printf("O penúltimo bloco sofreu alteração nos dados para o valor 367256 assim gerando um novo hash!\n");
                    printf("A seguir, tente validar a blockchain e veja o que acontece!\n");
                    printf("Se quiser continuar com as interações, limpe a blockchain e recomece com uma blockchain válida!\n");
                }
                else {
                    printf("algo ocorreu errado!\n");
                }
            }
            pause_screen();
            clear_screen();
        }
        else if(choice == 6){
            clear_screen();
            free_chain(blk_current);
            blk_current = NULL;
            printf("Blockchain excluída com Sucesso!\n");
            pause_screen();
        }
        clear_screen();

    }
    blk_current = NULL;
    free_chain(blk_current);


    return 0;
}
