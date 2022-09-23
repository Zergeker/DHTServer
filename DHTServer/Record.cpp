#include <iostream>
#include <string>
#include "Record.h"
#include "sha1Encryptor.h"

Record::Record(std::string inputKey, std::string inputValue) 
{
	value = inputValue;
	key = SHA1Encrypt(inputKey);
};