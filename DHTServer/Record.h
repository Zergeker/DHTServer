#include <iostream>
#include <string>

class Record {
public:
	Record(std::string inputKey, std::string inputValue);
	std::string value;
	std::string key;
};