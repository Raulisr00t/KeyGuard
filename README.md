# KeyGuard
KeyGuard is a simple encryption utility that generates and protects encryption keys using a combination of XOR operations and key modifications. This README provides an overview of the code, usage instructions, and detailed explanations of the encryption algorithm.

## Table of Contents
Overview
Installation
Usage
Encryption Algorithm
Function Details
Example
License

## Overview
KeyGuard is designed to generate and protect encryption keys. It uses a random hint byte and a random key modifier to create a protected key that can be used for encryption. The key generation process ensures that the original key is obfuscated, providing a layer of security.

## Installation
Clone the repository or download the source code.
Ensure you have a C compiler installed (e.g., GCC, MSVC).
Compile the code using your preferred compiler.

### Usage
To generate a protected key, run the compiled executable with the desired key size as an argument:

```cmd
keyguard.exe <KeySize>
```
### Encryption Algorithm
The encryption algorithm used in KeyGuard involves the following steps:
Random Byte Generation: A random byte b is generated.
Original Key Generation: The original key (pKey) is created, starting with the provided hint byte, followed by random bytes.
Protected Key Generation: Each byte of the protected key (pProtectedKey) is calculated using the formula:

```shell
pProtectedKey[i]=(pKey[i]+i)⊕b
```
### Function Details
VOID GenerateProtectedKey(IN BYTE HintByte, IN SIZE_T sKey, OUT PBYTE* ppProtectedKey)
Generates a protected key using the provided hint byte and key size.

HintByte: The starting byte for the original key.
sKey: The size of the key.
ppProtectedKey: A pointer to the location where the protected key will be stored.
VOID PrintHex(LPCSTR Name, PBYTE Data, SIZE_T Size)
Prints the provided data in hexadecimal format.

Name: The name of the data.
Data: The data to be printed.
Size: The size of the data.

VOID PrintFunction()
Prints the decryption function used to recover the original key from the protected key.

### Example
Here is an example of running KeyGuard with a key size of 16 bytes:

```sh
keyguard.exe 16
```

The output will include the original key, the protected key, and the decryption function. Example output:
```c
[i] Input Key Size : 16 
[+] Using "0xA2" As A Hint Byte 

[+] Use The Following Key For [Encryption] 
unsigned char OrginalKey[] = {
    0xA2, 0x4B, 0x23, 0x7F, 0x9C, 0x5E, 0x72, 0x15, 
    0xDA, 0xEF, 0x6B, 0x34, 0x9F, 0x2C, 0x91, 0x68 
};

[+] Use The Following For [Implementations] 
unsigned char ProtectedKey[] = {
    0xFF, 0x3E, 0x67, 0x82, 0x3A, 0x55, 0x91, 0x6C, 
    0xC9, 0xD5, 0x2E, 0x6A, 0x1C, 0x2F, 0x63, 0x75 
};

-------------------------------------------------

#include <Windows.h>

#define HINT_BYTE 0xA2

unsigned char ProtectedKey[] = {
    0xFF, 0x3E, 0x67, 0x82, 0x3A, 0x55, 0x91, 0x6C, 
    0xC9, 0xD5, 0x2E, 0x6A, 0x1C, 0x2F, 0x63, 0x75 
};

BYTE BruteForceDecryption(IN BYTE HintByte, IN PBYTE pProtectedKey, IN SIZE_T sKey, OUT PBYTE* ppRealKey) {

    BYTE b = 0;
    INT i = 0;
    PBYTE pRealKey = (PBYTE)malloc(sKey);

    if (!pRealKey)
        return NULL;

    while (1) {
        if (((pProtectedKey[0] ^ b)) == HintByte)
            break;
        else
            b++;
    }

    for (int i = 0; i < sKey; i++) {
        pRealKey[i] = (BYTE)((pProtectedKey[i] ^ b) - i);
    }

    *ppRealKey = pRealKey;
    return b;
}

// Example calling:

// PBYTE pRealKey = NULL;
// BruteForceDecryption(HINT_BYTE, ProtectedKey, sizeof(ProtectedKey), &pRealKey); 
```

## License
This project is licensed under the MIT License - see the LICENSE file for details.
