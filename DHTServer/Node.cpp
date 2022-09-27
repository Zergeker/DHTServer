#include "Node.h"
#include "sha1Encryptor.h"

Node::Node(int i, int nodesCount, int keySpace)
{
	rank = i;
	keySpace_size = keySpace;

	nodeKey = rank * keySpace_size/nodesCount;
	
	if (rank == nodesCount - 1)
	{
		successorNodeKey = 0;
	}
	else successorNodeKey = nodeKey + (keySpace_size / nodesCount);

	if (nodesCount > 1)
	{
		if (rank == (nodesCount - 1))
			successorRank = 0;
		else
			successorRank = i + 1;

		if (rank == 0)
			predecessorRank = nodesCount - 1;
		else
			predecessorRank = i - 1;
	}
	else
	{
		successorRank = NULL;
		predecessorRank = NULL;
	}
};

void Node::AddRecord(std::string inputKey, std::string inputValue)
{
	Record* rec = new Record(inputKey, inputValue, keySpace_size);
	unsigned long long hashKey = SHA1Encrypt(inputKey, keySpace_size);
	std::cout << hashKey << std::endl;
	
	if (hashKey >= nodeKey && successorRank != 0 && hashKey < successorNodeKey)
	{
		records.push_back(rec);
	}
	else if (hashKey >= nodeKey && hashKey < keySpace_size, successorRank == 0)
	{
		records.push_back(rec);
	}
	else
	{
		
		MPI_Request request1, request2, request3;
		char origKeyBuf[100];
		char valueBuf[100];

		strcpy(origKeyBuf, inputKey.c_str());
		strcpy(valueBuf, inputValue.c_str());

		MPI_Isend(origKeyBuf, 100, MPI_CHAR, successorRank, 2, MPI_COMM_WORLD, &request2);
		MPI_Isend(valueBuf, 100, MPI_CHAR, successorRank, 2, MPI_COMM_WORLD, &request3);

		MPI_Wait(&request2, MPI_STATUS_IGNORE);
		MPI_Wait(&request3, MPI_STATUS_IGNORE);
	}
};

std::string Node::FindRecord(unsigned long long inputKey, int starterRank)
{
	std::list<Record*>::iterator it;

	for (it = records.begin(); it!=records.end(); ++it)
	{
		if ((*it)->key == inputKey)
		{
			std::cout << rank << std::endl;
			return (*it)->value;
		}
	}

	if (rank == successorRank || successorRank == starterRank)
	{
		return "No such record found";
	}
	else
	{
		int intBuf[1] = { starterRank };
		unsigned long long keyBuf[1] = {inputKey};
		char valueBuf[100];

		MPI_Request request, request2;

		MPI_Isend(keyBuf, 1, MPI_UNSIGNED_LONG_LONG, successorRank, 1, MPI_COMM_WORLD, &request);
		MPI_Isend(intBuf, 1, MPI_INT, successorRank, 1, MPI_COMM_WORLD, &request2);

		MPI_Irecv(valueBuf, 100, MPI_CHAR, successorRank, 1, MPI_COMM_WORLD, &request);
		MPI_Wait(&request, MPI_STATUS_IGNORE);

		return std::string(valueBuf);
	}
};

int* Node::GetSuccessorRank()
{
	return &successorRank;
};

int* Node::GetPredecessorRank()
{
	return &predecessorRank;
};