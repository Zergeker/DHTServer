#include "Controller.h"
#include <stdlib.h>
#include "sha1Encryptor.h"

#include <cpprest/filestream.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace concurrency::streams;

Controller::Controller(std::string hostname)
{
	std::wstring widestr = std::wstring(hostname.begin(), hostname.end());
	storageKeyListener = http_listener((widestr + L"/storage"));
	neighborsListener = http_listener((widestr + L"/neighbors"));
};

void Controller::Start(Node* node)
{
	storageKeyListener.open().wait();
	
	storageKeyListener.support(methods::GET, [&](http_request request) {getValue(request, node); });

	storageKeyListener.support(methods::PUT, [&](http_request request) {putStorageKey(request, node);});

	neighborsListener.open().wait();

	neighborsListener.support(methods::GET, [&](http_request request) {getNeighbors(request, node); });

	int len;
	MPI_Get_processor_name(processorName, &len);

	MPI_Request request1, request2, request3, request4;

	MPI_Isend(processorName, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, *node->GetPredecessorRank(), 2, MPI_COMM_WORLD, &request1);
	MPI_Isend(processorName, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, *node->GetSuccessorRank(), 2, MPI_COMM_WORLD, &request2);

	MPI_Irecv(predecessorName, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, *node->GetPredecessorRank(), 2, MPI_COMM_WORLD, &request3);
	MPI_Wait(&request3, MPI_STATUS_IGNORE);
	MPI_Irecv(successorName, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, *node->GetSuccessorRank(), 2, MPI_COMM_WORLD, &request4);
	MPI_Wait(&request4, MPI_STATUS_IGNORE);

};

void Controller::Wait(Node* node)
{
	int starterRank[1];
	MPI_Request requestKey, requestEndRank;
	bool flag = 1;
	int predRank = *node->GetPredecessorRank();
	int succRank = *node->GetSuccessorRank();
	char valueBuf[100];
	char foundValue[100];

	while (true)
	{
		MPI_Irecv(valueBuf, 100, MPI_CHAR, predRank, MPI_ANY_TAG, MPI_COMM_WORLD, &requestKey);
		MPI_Wait(&requestKey, MPI_STATUS_IGNORE);

		MPI_Irecv(starterRank, 1, MPI_INT, predRank, MPI_ANY_TAG, MPI_COMM_WORLD, &requestEndRank);
		MPI_Wait(&requestEndRank, MPI_STATUS_IGNORE);

		strcpy(foundValue, (*node).FindRecord(valueBuf, *starterRank).c_str());
		MPI_Isend(foundValue, 100, MPI_CHAR, predRank, 0, MPI_COMM_WORLD, &requestKey);
	}
}

void Controller::Stop()
{
	storageKeyListener.close();
	neighborsListener.close();
}

void Controller::getValue(http_request request, Node* node)
{
	std::string input = utility::conversions::to_utf8string(request.relative_uri().to_string()).erase(0, 1);
	request.reply(200, node->FindRecord(SHA1Encrypt(input), node->rank));
};

void Controller::getNeighbors(http_request request, Node* node)
{
	auto res = web::json::value::array();
	res[0] = json::value::string(utility::conversions::to_utf16string(std::string(predecessorName)));
	res[1] = json::value::string(utility::conversions::to_utf16string(std::string(successorName)));
	request.reply(200, res);
};

void Controller::putStorageKey(http_request request, Node* node)
{
	std::string input = utility::conversions::to_utf8string(request.relative_uri().to_string()).erase(0, 1);
	stringstreambuf buffer;
	request.body().read_to_end(buffer).get();
	node->AddRecord(new Record(input, buffer.collection()));
	request.reply(200, buffer.collection());
}