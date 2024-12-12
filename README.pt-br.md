[![en](https://img.shields.io/badge/lang-en-red.svg)](https://github.com/isacpxc/blockchain-noob/blob/main/README.md)
[![pt-br](https://img.shields.io/badge/lang-pt--br-green.svg)](https://github.com/isacpxc/blockchain-noob/blob/main/README.pt-br.md)

# 🤷‍♂️O que é este repositório?
**💬Este repositório contém uma implemenação básica de uma blockchain na Linguagem C. \
Siga as próximas instruções para conseguir ultilizar esse código.\
(Obs:. O programa ja conta com uma GUI, então não se preocupe, será intuitivo ultilizá-lo).**

# ⚙️Como compilar e rodar o código?

## 🚨Pré-requisitos - Windows
Por via das dúvidas quando possível se utilize do modo administrador para fazer as instações e acessos a seguir.
Você precisará fazer a instalação do openSSL e do GCC. \
**⚠️<u>[Win64 OpenSSL v3.3.2⬇️](https://slproweb.com/download/Win64OpenSSL-3_3_2.exe)</u>** -> Windows  
**⚠️<u>[Win64 OpenSSL v3.3.2⬇️](https://slproweb.com/products.html)</u>** -> link alternativo
  * Atenção! Instale  no diretório do Windows.  E após isso coloque o diretório `C:\Program Files\OpenSSL-Win64\bin` do openSSL no PATH(_para saber como, clique [aqui](https://oobj.com.br/bc/article/como-configurar-variavel-de-ambiente-no-windows-para-emiss%C3%A3o-de-mf-e-1180.html)_).
  
  ✔️Agora você já é capaz de rodar `/.exe/blockchain.exe`.


**⚠️<u>[GCC - compilador de C⬇️](https://github.com/brechtsanders/winlibs_mingw/releases/download/13.2.0posix-18.1.5-11.0.1-ucrt-r8/winlibs-x86_64-posix-seh-gcc-13.2.0-llvm-18.1.5-mingw-w64ucrt-11.0.1-r8.zip)</u>** -> versão 64bits \
**⚠️<u>[GCC - compilador de C⬇️](https://winlibs.com/)</u>** -> Link alternativo com a versão em questão(13.2.0 (with POSIX threads) + LLVM/Clang/LLD/LLDB 18.1.5 + MinGW-w64 11.0.1 (UCRT) - release 8) e outras.

1. Após baixar o arquivo .zip, extraia-o em no diretório raiz C:(isso colocará a pasta mingw64 no diretório em questão). 
2. para ultilizar o gcc no prompt de comando, você deve colocar o diretório C:/mingw64/bin/ no PATH(_para saber como clique [aqui](https://oobj.com.br/bc/article/como-configurar-variavel-de-ambiente-no-windows-para-emiss%C3%A3o-de-mf-e-1180.html)_).
3. Agora vá em `C:\Program Files\OpenSSL-Win64\include\` copie o diretôrio `\openssl` e cole-o em `C:\mingw64\include\`.
4. Agora vá no diretório `C:\Program Files\OpenSSL-Win64\lib\VC\x64\MD\` e copie os arquivos `libssl.lib` e `libcrypto.lib` para o diretório `C:\mingw64\lib\`.
5. Prossiga para a pasta do projeto onde se encontra o `main.c` e execute o seguinte comando:
      ```console
   gcc main.c blockchain.c -o main -lssl -lcrypto
   ```
* `gcc`: Invoca o compilador GNU.

* `main.c`: O arquivo fonte que você quer compilar.
  
* `blockchain.c` = Arquivo de Funções

* `-o main`: Define o nome do arquivo executável de saída como main.

* `-lssl` com a flag `-l`: Linka a biblioteca OpenSSL (libssl), usada para funções de criptografia.

* `-lcrypto` com a flag `-l`: Linka a biblioteca OpenSSL (libcrypto), usada para funções criptográficas adicionais.\
  🎉Pronto, agora você tem um executável main.exe na raíz do projeto, e pode testar o código com o .exe que você mesmo gerou😄.


## 🚨Pré-requisitos - Linux
### 🚧Em construção... 
(_a necessidade do openssl e gcc se mantém, inclua a bibliotecas e e os headers necessários, e compile o código como mostrado na instrução para Windows_)

<br/>
<br/>

# 📄Breve Documentação do código

## 🧱Structs
```C
typedef struct Block {
    size_t id; //Identificador único do bloco.
    unsigned char *hash; //Hash do bloco atual.
    unsigned char *prev_hash; //Hash do bloco anterior.
    time_t timestamp; //Momento em que o bloco foi criado.
    unsigned long int nonce; //Número arbitrário usado uma única vez (PoW).
    struct Block *prev_blk; //Ponteiro para o bloco anterior.
    MerkleNode *root;
    int data; //Dados contidos no bloco.
} Block;
```

```C

typedef struct MerkleNode {
    unsigned char *hash; //Hash do nó raíz.
    struct merkle_node *left; //Ponteiro para o nó filho à esquerda.
    struct merkle_node *right; //Ponteiro para o nó filho à direita.
} MerkleNode;
```

```C

typedef struct {
    char sender_wallet[13]; //Endereço do remetente
    char recipient_wallet[13]; //Endereço do Destinatário
    int amount; //Quantidade enviada
    int tax; //Taxa de transação
} Transaction;

```

```C

typedef struct Address{
    char address[13]; //Endereço
    int balance; //Saldo
    struct Address *next; //Ponteiro para o próximo endereço
} Address;

```
```C

typedef struct Message {
    char type; // 'B' para Bloco, 'T' para Transação
    union { 
        Block block;
        Transaction transaction;
    } data; //Armazena ou Transação ou Bloco
    struct Message *last_message; //Ponteiro para próxima mensagem
} Message;
```
```C

typedef struct Node {
    int id;  // identificador do nó
    Block *blockchain; //Blockchain do nó
    Transaction transaction_pool[100]; // Pool de transações
    Message *message_queue; //Fila de mensagens recebidas
    int transaction_count; // Contagem de transações
    struct Node *next; //Ponteiro para próximo nó
    struct Node *prev; //Ponteiro para nó anterior(não usado)
} Node;
```


## ➡️Funções e Variáveis Importantes

* `current_node` = Guarda nó atual conectado à rede.
* `latest_added_node` = Guarda último nó adicionado na rede.
* `network_length` = guarda tamano da rede.
<br/><br/>

* `clear_screen` = Limpa a tela do console, dependendo do sistema operacional.
* `pause_screen` = Pausa a tela até que o usuário pressione uma tecla.
* `compare_block_hashes` = Compara os hashes de dois blocos.
* `compare_block_and_prev_hashes` = Compara o hash de um bloco com o hash anterior de outro bloco.
* `create_sha256` = Gera um hash SHA256 a partir de uma string.
* `hash_block` = Gera o hash de um bloco baseado em seus atributos.
* `is_valid_block` = Valida um bloco em relação ao bloco anterior.
* `print_blk` = Imprime as informações de um bloco.
* `print_chain` = Imprime todos os blocos da cadeia.
* `add_blk` = Adiciona um novo bloco à blockchain.
* `verify_null` = Verifica se a blockchain está vazia.
* `verify_prev_null` = Verifica se o bloco anterior é nulo.
* `validate_chain` = Valida a cadeia de blocos.
* `free_chain` = Libera a memória alocada pela blockchain.
* `corrupt_chain` = Adultera os dados do penúltimo bloco da blockchain.
* `print_visual_chain` = Imprime a blockchain de forma visual.
* `hash_block` = Gera o hash a partir de um inteiro.
* `random_uppercase_letter` = Gera uma letra maiúscula aleatória
* `random_digit` = Gera um dígito aleatório.
* `generate_wallet_address` = Gera um endereço de carteira aleatório
* `validate_wallet_address` = Valida um endereço de carteira
* `create_transaction` = Cria uma transação e adiciona endereços à lista de endereços únicos.
* `add_transaction` = Adiciona uma transação ao array de transações
* `proof_of_work` = Executa o algoritmo de Proof of Work para minerar um bloco.
* `print_transaction_pool` = Imprime todas as transações na pool de transações.
* `add_unique_address` = Adiciona um endereço à lista de endereços únicos, se ainda não estiver presente.
* `print_addresses` = Imprime todos os endereços únicos na lista de endereços.
* `wallet_exists` = Verifica se um endereço de carteira existe na lista de endereços.
* `print_wallet_transactions` = Procura na blockchain e imprime todas as transações envolvendo um endereço de carteira específico.
* `print_instructions` = Imprime as instruções de uso ou ajuda para o usuário.
* `data_hash256` = Gera um hash SHA256 a partir de um dado inteiro.
* `create_main_node` = Cria o nó principal da rede blockchain(o que gera o bloco gênesis).
* `create_genesis_block` = Cria o bloco gênesis(na versão anterior era criado no próprio main.c).
* `update_latest_added_node` = Atualiza o nó que foi adicionado mais recentemente na rede.
* `update_current_node` = Atualiza o nó atual na rede.
* `get_network_length` = Retorna o comprimento atual da rede blockchain.
* `get_id_current_node` = Retorna o ID do nó atual na rede.
* `change_node_to` = Muda para um nó específico na rede, identificando-o por seu ID.
* `pool_length` = Retorna o comprimento da pool de transações.
* `pull_first_transaction` = Retira a primeira transação da pool de transações e a retorna.
* `verify_minimum_three` = Verifica se a blockchain possui pelo menos três blocos.
* `broadcast_transaction` = Transmite uma transação para a rede via mensagem.
* `broadcast_block` = Transmite um bloco para a rede via mensagem.
* `print_message_queue` = Imprime a fila de mensagens.
* `process_message_queue` = Processa a fila de mensagens adicionando o bloco ou transação no nó atual.
* `check_balance` = Verifica o saldo de um endereço de carteira específico.
* `select_wallet` = Seleciona um endereço de carteira na lista de endereços.
* `create_node` = Cria um novo nó na rede.
* `print_network` = Imprime a estrutura da rede.
* `find_message_with_block_id` = Encontra uma mensagem na fila que contém um ID de bloco específico.
* `find_next_block_message` = Encontra a próxima mensagem de bloco na sequência a partir do bloco atual.
* `handle_fork_resolution` = Resolve um fork na blockchain.
* `create_sha256_transaction` = Gera um hash SHA256 para uma transação a partir de seus dados.
transaction_hash = Gera o hash de uma transação.
* `transaction_exists_in_messages` = Verifica se uma transação existe nas mensagens.
* `transaction_hash` = Gera o hash de uma transação baseada em seus atributos.


## 📁Arquivos
* _main.c_ = código principal
* _block.h_ = definições de structs
* _blockchain.c_ = Funções ultilizadas no _main.c_
* _blockchain.h_ = Definições de funções em _blockchain.c_ e e constantes

## 📚Bibliotecas usadas
* `stdio.h`: Funções de entrada e saída (ex.: printf, scanf).

* `stdlib.h`: Utilidades diversas (ex.: malloc, free).

* `time.h`: Manipulação de data e hora (ex.: time, clock).

* `string.h`: Manipulação de strings (ex.: strlen, strcpy).

* `locale.h`: Configurações de localidade (ex.: setlocale).

* `openssl/sha.h`: Funções de hash SHA (ex.: SHA256).

* `ctype.h`: Funções para testar e mapear caracteres

# ⌛Futuro
### As implementações futuros podem ser vistas nas issues do repositório.