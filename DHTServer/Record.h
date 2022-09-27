#include <iostream>
#include <string>

class Record {
public:
	Record(std::string inputKey, std::string inputValue, int keySpaceSize);
	std::string value;
	std::string originalKey;
	unsigned long long key;
};