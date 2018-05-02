#include "stdafx.h"

#include <iostream>
#include <windows.h>
#include <Wincrypt.h>
#include <fstream>

using namespace std;

const string path = "D:\\lab4\\lab4.txt";

int main() {

	HCRYPTPROV hProv;
	HCRYPTHASH hHash;

	FILE *myFile;
	DWORD errorNo;

	CryptAcquireContext(&hProv, NULL, MS_STRONG_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);

	CryptCreateHash(hProv, CALG_MD5, NULL, 0, &hHash);

	errorNo = fopen_s(&myFile, path.c_str(), "rb");
	if (myFile == NULL) {
		cout << "File error" << endl;
		return 0;
	}
	else {
		BYTE *buffer = (BYTE *)malloc(100);
		size_t howManyRead;
		DWORD dwDataLen;
		while (feof(myFile) == 0) {
			howManyRead = fread(buffer, 1, 96, myFile);
			if (ferror(myFile) != 0) {
				cout << "File error" << endl;
				return 0;
			}		
			dwDataLen = howManyRead;
			CryptHashData(hHash, buffer, dwDataLen, 0);
		}
	}

	DWORD dwHashSize;
	CryptGetHashParam(hHash, HP_HASHVAL, NULL, &dwHashSize, 0);

	BYTE* pbHashValue = (BYTE *) malloc(dwHashSize);
	CryptGetHashParam(hHash, HP_HASHVAL, pbHashValue, &dwHashSize, 0);

	cout << pbHashValue << endl;

	free(pbHashValue);
	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);

	return 0;
}