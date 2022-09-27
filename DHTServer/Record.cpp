#include <iostream>
#include <string>
#include "Record.h"
#include "sha1Encryptor.h"

Record::Record(std::string inputKey, std::string inputValue, int keySpace_size) 
{
	value = inputValue;
	key = SHA1Encrypt(inputKey, keySpace_size);
	originalKey = inputKey;
};