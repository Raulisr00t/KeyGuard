#include <Windows.h>
#include <stdio.h>
#include <time.h>

VOID PrintHex(LPCSTR Name, PBYTE Data, SIZE_T Size) {
	printf("unsigned char %s[] = {", Name);
	for (int i = 0; i < Size; i++) {
		if (i % 16 == 0) {
			printf("\n\t");
		}
		if (i < Size - 1) {
			printf("0x%0.2X, ", Data[i]);
		} else {
			printf("0x%0.2X ", Data[i]);
		}
	}
	printf("};\n\n");
}

VOID GenerateProtectedKey(IN BYTE HintByte, IN SIZE_T sKey, OUT PBYTE* ppProtectedKey) {
	srand(time(NULL) / 3);
	BYTE b;
	PBYTE pKey;
	PBYTE pProtectedKey;
	b = rand() % 0xFF;
	pKey = (PBYTE)malloc(sKey);
	pProtectedKey = (PBYTE)malloc(sKey);

	if (!pKey || !pProtectedKey) {
		if (pKey) free(pKey);
		if (pProtectedKey) free(pProtectedKey);
		return;
	}
	srand(time(NULL) * 2);

	pKey[0] = HintByte;

	for (int i = 1; i < sKey; i++) { // start from 1 since pKey[0] is already set
		pKey[i] = (BYTE)rand() % 0xFF;
	}

	PrintHex("Original Key", pKey, sKey);

	for (int i = 0; i < sKey; i++) {
		pProtectedKey[i] = (BYTE)((pKey[i] + i) ^ b);
	}

	*ppProtectedKey = pProtectedKey;

	free(pKey);
}

VOID PrintFunction() {
	CHAR* buf =
		"BYTE BruteForceDecryption(IN BYTE HintByte, IN PBYTE pProtectedKey, IN SIZE_T sKey, OUT PBYTE* ppRealKey) {\n\n"
		"\tBYTE		b			= 0;\n"
		"\tINT		i			= 0;\n"
		"\tPBYTE		pRealKey		= (PBYTE)malloc(sKey);\n\n"
		"\tif (!pRealKey)\n"
		"\t\treturn NULL;\n\n"
		"\twhile (1){\n\n"
		"\t\tif (((pProtectedKey[0] ^ b)) == HintByte)\n"
		"\t\t\tbreak;\n"
		"\t\telse\n"
		"\t\t\tb++;\n\n"
		"\t}\n\n"
		"\tfor (int i = 0; i < sKey; i++){\n"
		"\t\tpRealKey[i] = (BYTE)((pProtectedKey[i] ^ b) - i);\n"
		"\t}\n\n"
		"\t*ppRealKey = pRealKey;\n"
		"\treturn b;\n"
		"}\n\n";

	printf("%s", buf);
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("[!] Usage: keyguard.exe <KeySize>\n");
		return -1;
	}

	srand(time(NULL));

	PBYTE pProtectedKey = NULL;
	DWORD sKeySize = (DWORD)atoi(argv[1]);
	BYTE HintByte = (BYTE)((rand() % 0xFF) * 2);

	printf("/*\n\n");
	printf("[i] Input Key Size : %d \n", sKeySize);
	printf("[+] Using \"0x%0.2X\" As A Hint Byte \n\n", HintByte);

	printf("[+] Use The Following Key For [Encryption] \n");

	GenerateProtectedKey(HintByte, sKeySize, &pProtectedKey);

	printf("[+] Use The Following For [Implementations] \n");
	PrintHex("ProtectedKey", pProtectedKey, sKeySize);

	printf("\n\n\t\t\t-------------------------------------------------\n\n");
	printf("*/\n\n");

	printf("#include <Windows.h>\n\n");
	printf("#define HINT_BYTE 0x%0.2X\n\n", HintByte);

	PrintHex("ProtectedKey", pProtectedKey, sKeySize);
	PrintFunction();

	printf("// Example calling:\n\n// PBYTE\tpRealKey\t=\tNULL;\n// BruteForceDecryption(HINT_BYTE, ProtectedKey, sizeof(ProtectedKey), &pRealKey); \n\n");

	free(pProtectedKey);

	return 0;
}
