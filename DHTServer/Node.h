#include <iostream>
#include <string>
#include <list>
#include "mpi.h"
#include "Record.h"

class Node {
public:
	void AddRecord(std::string inputKey, std::string inputValue);
	void ShowContents();
	std::string FindRecord(unsigned long long inputKey, std::string inputOrigKey, int starterRank);
	int* GetSuccessorRank();
	int* GetPredecessorRank();
	int rank;
	int nodeKey;
	int keySpace_size;
	int successorNodeKey;
	Node(int rank, int nodesCount, int keySpace);
private:
	int successorRank;
	int predecessorRank;
	std::list<Record*> records;
};