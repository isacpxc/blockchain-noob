[![en](https://img.shields.io/badge/lang-en-red.svg)](https://github.com/isacpxc/blockchain-noob/blob/main/README.md)
[![pt-br](https://img.shields.io/badge/lang-pt--br-green.svg)](https://github.com/isacpxc/blockchain-noob/blob/main/README.pt-br.md)

# 🤷‍♂️What is this repository?
**💬This repository contains a basic implementation of a blockchain in the C Language. Follow the instructions below to use this code. (Note: The program already includes a GUI, so don't worry, it will be intuitive to use).**\

# ⚙️How to compile and run the code?

## 🚨Prerequisites - Windows
To be on the safe side, when possible, use the administrator mode to perform the following installations and accesses. You will need to install OpenSSL and GCC. \
**⚠️<u>[Win64 OpenSSL v3.3.2⬇️](https://slproweb.com/download/Win64OpenSSL-3_3_2.exe)</u>** -> Windows  
**⚠️<u>[Win64 OpenSSL v3.3.2⬇️](https://slproweb.com/products.html)</u>** -> alternative link
  * Attention! Install it in the Windows directory. After that, add the OpenSSL directory `C:\Program Files\OpenSSL-Win64\bin` to the PATH (to learn how, click [_here_](https://www.architectryan.com/2018/03/17/add-to-the-path-on-windows-10/).
  
  ✔️Now you are ready to run `/.exe/blockchain.exe`.


**⚠️<u>[GCC - C compiler⬇️](https://github.com/brechtsanders/winlibs_mingw/releases/download/13.2.0posix-18.1.5-11.0.1-ucrt-r8/winlibs-x86_64-posix-seh-gcc-13.2.0-llvm-18.1.5-mingw-w64ucrt-11.0.1-r8.zip)</u>** -> 64bits version \
**⚠️<u>[GCC - C compiler⬇️](https://winlibs.com/)</u>** -> Alternate link with the specific version(13.2.0 (with POSIX threads) + LLVM/Clang/LLD/LLDB 18.1.5 + MinGW-w64 11.0.1 (UCRT) - release 8) and others.

1. After downloading the .zip file, extract it to the root directory C: (this will place the mingw64 folder in the specified directory). 
2. To use GCC in the command prompt, you should add the directory C:/mingw64/bin/ to the PATH(to learn how, click [_here_](https://www.architectryan.com/2018/03/17/add-to-the-path-on-windows-10/).
3. Go to `C:\Program Files\OpenSSL-Win64\include\`, copy the directory `\openssl`, and paste it into `C:\mingw64\include\`.
4. Now go to the directory `C:\Program Files\OpenSSL-Win64\lib\VC\x64\MD\` and copy the files `libssl.lib` and `libcrypto.lib` to the directory `C:\mingw64\lib\`.
5. Navigate to the project folder where main.c is located and execute the following command:
      ```console
   gcc main.c blockchain.c -o main -lssl -lcrypto
   ```
* `gcc`: Invokes the GNU compiler.

* `main.c`: The source file you want to compile.

* `blockchain.c` = Functions File.

* `-o main`: Sets the output executable file name to main.

* `-lssl` with the `-l` option: Links the OpenSSL library (libssl), used for cryptographic functions.

* `-lcrypto` with the `-l` option: Links the OpenSSL library (libcrypto), used for additional cryptographic functions.\
  🎉Done! Now you have an executable `main.exe` in the root of the project, and you can test the code with the `.exe` you generated yourself.😄.


## 🚨Prerequisites - Linux
### 🚧Under construction... 
(the need for OpenSSL and GCC remains, include the necessary libraries and headers, and compile the code as shown in the Windows instructions).
<br/>
<br/>

# 📄Brief Documentation of the Code

## 🧱Structs
```C
typedef struct Block {
    size_t id; // Unique identifier for the block.
    unsigned char *hash; // Hash of the current block.
    unsigned char *prev_hash; // Hash of the previous block.
    time_t timestamp; // Time when the block was created.
    unsigned long int nonce; // Arbitrary number used once (PoW).
    struct Block *prev_blk; // Pointer to the previous block.
    MerkleNode *root; // Pointer to the Merkle Root.
    int data; // Data contained in the block.
} Block;
```

```C
typedef struct MerkleNode {
    unsigned char *hash; // Hash of the root node.
    struct merkle_node *left; // Pointer to the left child node.
    struct merkle_node *right; // Pointer to the right child node.
} MerkleNode;
```

```C
typedef struct {
    char sender_wallet[13]; ///Sender's address
    char recipient_wallet[13]; //Recipient's address
    int amount; //Amount sent
    int tax; //Transaction fee
} Transaction;
```

```C

typedef struct Address{
    char address[13]; //Address
    int balance; //Balance
    struct Address *next; //Pointer to the next address
} Address;
```

```C

typedef struct Message {
    char type; // 'B' for Block, 'T' for Transaction
    union { 
        Block block;
        Transaction transaction;
    } data; //Stores either a Transaction or Block
    struct Message *last_message; //Pointer to the next message
} Message;

```

```C

typedef struct Node {
    int id;  // Identifier of the node
    Block *blockchain; //Blockchain of the node
    Transaction transaction_pool[100]; // Pool of transactions
    Message *message_queue; //Queue of received messages
    int transaction_count; // Count of transactions
    struct Node *next; //Pointer to the next node
    struct Node *prev; //Pointer to the previous node (not used)
} Node;

```

## ➡️Functions and Important Variables

* `current_node` = Stores the current node connected to the network.
* `latest_added_node` = Stores the last node added to the network.
* `network_length`
<br/><br/>

* `clear_screen` = Clears the console screen, depending on the operating system.
* `pause_screen` = Pauses the screen until the user presses a key.
* `compare_block_hashes` = Compares the hashes of two blocks.
* `compare_block_and_prev_hashes` = Compares the hash of a block with the previous block's hash.
* `create_sha256` = Generates a SHA256 hash from a string.
* `hash_block` = Generates the hash of a block based on its attributes.
* `is_valid_block` = Validates a block against the previous block.
* `print_blk` = Prints the information of a block.
* `print_chain` = Prints all the blocks in the chain.
* `add_blk` = Adds a new block to the blockchain.
* `verify_null` = Checks if the blockchain is empty.
* `verify_prev_null` = Checks if the previous block is null.
* `validate_chain` = Validates the entire blockchain.
* `free_chain` = Frees the memory allocated for the blockchain.
* `corrupt_chain` = Alters the data of the penultimate block in the blockchain.
* `print_visual_chain` = Visually prints the blockchain.
* `hash_block` = Generates a hash from an integer.
* random_uppercase_letter = Generates a random uppercase letter
* `random_digit` = Generates a random digit.
* `generate_wallet_address` = Generates a random wallet address
* `validate_wallet_address` = Validates a wallet address.
* `create_transaction` = Creates a transaction and adds addresses to the list of unique addresses.
* `add_transaction` = Adds a transaction to the transaction array
* `proof_of_work` = Performs the Proof of Work algorithm to mine a block
* `print_transaction_pool` = Prints all transactions in the transaction pool.
* `add_unique_address` = Adds an address to the list of unique addresses if it is not already present.
* `print_addresses` = Prints all unique addresses in the address list
* `wallet_exists` = Checks if a wallet address exists in the address list
* `print_wallet_transactions` = Searches the blockchain and prints all transactions involving a specific wallet address.
* * `print_instructions` = Prints usage instructions or help for the user.
* `data_hash256` = Generates a SHA256 hash from an integer.
* `create_main_node` = Creates the main node of the blockchain network (which generates the genesis block).
* `create_genesis_block` = Creates the genesis block (previously created in main.c).
* `update_latest_added_node` = Updates the most recently added node in the network.
* `update_current_node` = Updates the current node in the network.
* `get_network_length` = Returns the current length of the blockchain network.
* `get_id_current_node` = Returns the ID of the current node in the network.
* `change_node_to` = Switches to a specific node in the network, identified by its ID.
* `pool_length` = Returns the length of the transaction pool.
* `pull_first_transaction` = Pulls the first transaction from the transaction pool and returns it.
* `verify_minimum_three` = Checks if the blockchain has at least three blocks.
* `broadcast_transaction` = Broadcasts a transaction to the network via message.
* `broadcast_block` = Broadcasts a block to the network via message.
* `print_message_queue` = Prints the message queue.
* ` process_message_queue` = Processes the message queue by adding the block or transaction to the current node.
* `check_balance` = Checks the balance of a specific wallet address.
* `select_wallet` = Selects a wallet address from the address list.
* `create_node` = Creates a new node in the network.
* `print_network` = Prints the structure of the network.
* `find_message_with_block_id` = Finds a message in the queue that contains a specific block ID.
* `find_next_block_message` = Finds the next block message in sequence from the current block.
* `handle_fork_resolution` = Resolves a fork in the blockchain.
* `create_sha256_transaction` = Generates a SHA256 hash for a transaction from its data. transaction_hash = Generates the hash of a transaction.
* `transaction_exists_in_messages` = Checks if a transaction exists in the messages.
* `transaction_hash` = Generates the hash of a transaction based on its attributes.

## Files
* _main.c_ = Main Code.
* _block.h_ = Struct Definitions.
* _blockchain.c_ = Functions used in _main.c_
* _blockchain.h_ = Function definitions in _blockchain.c_ and constants

## 📚Libraries Used
* `stdio.h`: Input and output functions (e.g., printf, scanf).

* `stdlib.h`: Various utilities (e.g., malloc, free).

* `time.h`: Date and time manipulation (e.g., time, clock).

* `string.h`: String manipulation (e.g., strlen, strcpy).

* `locale.h`: Locale settings (e.g., setlocale).

* `openssl/sha.h`: SHA hash functions (e.g., SHA256).

* `ctype.h`: Functions for testing and mapping characters.

# Future
### Future implementations can be viewed in the issues section of the repository.