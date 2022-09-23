﻿#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "omp.h"
#include <mpi.h>

#include "sha1Encryptor.h"

#include <cpprest/http_listener.h>
#include <cpprest/json.h>

#include "Controller.h"

using namespace std;

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;



int main(int argc, char** argv)
{
    int nproc;
    int myrank; 
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    Controller controller(("http://localhost:666" + to_string(myrank)));

    Node node(myrank, nproc);

    controller.Start(&node);

    controller.Wait(&node);

    controller.Stop();

    MPI_Finalize();

    return 0;
}