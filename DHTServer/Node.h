#include <iostream>
#include <string>
#include <list>
#include "mpi.h"
#include "Record.h"

class Node {
public:
	void AddRecord(Record* record);
	void ShowContents();
	std::string FindRecord(std::string inputKey, int starterRank);
	int* GetSuccessorRank();
	int* GetPredecessorRank();
	int rank;
	Node(int rank, int nodesCount);
private:
	int successorRank;
	int predecessorRank;
	std::list<Record*> records;
};