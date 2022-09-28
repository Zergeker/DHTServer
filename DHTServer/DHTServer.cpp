#include <iostream>
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
    if (argc != 2) {
        printf("The port wasn't specified. Please run the program with specifying the port\n", argv[0]);
        exit(1);
    }

    int nproc;
    int myrank;

    int port = atoi(argv[1]);

    int keyspace = 128;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    Controller controller(("http://localhost:" + std::string(argv[1])), port, 128);

    Node node(myrank, nproc, keyspace);

    controller.Start(&node);

    controller.Wait(&node);

    controller.Stop();

    MPI_Finalize();

    return 0;
}
