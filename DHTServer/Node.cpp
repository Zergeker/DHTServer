#include "Node.h"
#include "sha1Encryptor.h"

Node::Node(int i, int nodesCount)
{
	rank = i;

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

void Node::AddRecord(Record* rec)
{
	records.push_back(rec);
};

std::string Node::FindRecord(std::string inputKey, int starterRank)
{
	std::list<Record*>::iterator it;
	for (it = records.begin(); it!=records.end(); ++it)
	{
		if ((*it)->key == inputKey)
			return (*it)->value;
	}

	if (rank == successorRank || successorRank == starterRank)
	{
		return "No such record found";
	}
	else
	{
		int intBuf[1] = { starterRank };
		const char* keyBuf = inputKey.c_str();
		char valueBuf[100];

		MPI_Request request, request2;

		MPI_Isend(keyBuf, strlen(keyBuf) + 1, MPI_CHAR, successorRank, 0, MPI_COMM_WORLD, &request);
		MPI_Isend(intBuf, 1, MPI_INT, successorRank, 0, MPI_COMM_WORLD, &request2);

		MPI_Irecv(valueBuf, 100, MPI_CHAR, successorRank, 0, MPI_COMM_WORLD, &request);
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