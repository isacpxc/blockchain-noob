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
   gcc main.c -o main -lssl -lcrypto
   ```
* `gcc`: Invokes the GNU compiler.

* `main.c`: The source file you want to compile.

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

## Functions

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

## Files
* _main.c_ = Main Code.
* _block.h_ = Struct Definitions.

## 📚Libraries Used
* `stdio.h`: Input and output functions (e.g., printf, scanf).

* `stdlib.h`: Various utilities (e.g., malloc, free).

* `time.h`: Date and time manipulation (e.g., time, clock).

* `string.h`: String manipulation (e.g., strlen, strcpy).

* `locale.h`: Locale settings (e.g., setlocale).

* `openssl/sha.h`: SHA hash functions (e.g., SHA256).

# Future
### Future implementations can be viewed in the issues section of the repository.