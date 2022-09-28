#pragma once

#include <iostream>
#include <string>

#include "Node.h"

#include <cpprest/http_listener.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class Controller
{
public:
	Controller(std::string hostname, int inputPort, int inputKeySpace);
	void Start(Node* node);
	void Wait(Node* node);
	void Stop();
private:
	int port;
	int keySpace;
	char processorName[MPI_MAX_PROCESSOR_NAME];
	char successorName[MPI_MAX_PROCESSOR_NAME];
	char predecessorName[MPI_MAX_PROCESSOR_NAME];
	http_listener storageKeyListener;
	http_listener neighborsListener;
	void getValue(http_request request, Node* node);
	void getNeighbors(http_request request, Node* node);
	void putStorageKey(http_request request, Node* node);
};