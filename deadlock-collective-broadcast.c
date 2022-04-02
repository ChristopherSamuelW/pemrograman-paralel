#include <stdio.h>
#include <string.h>
#include <mpi.h>
#ifdef _WIN32
#include <Windows.h> // sleep library call if run on windows
#else
#include <unistd.h> // sleep library call if run on other
#endif

const int MAX_STRING = 1000;

int main(int argc, char **argv)
{
	int comm_sz;
	int my_rank;
	int message1;
	int message2;
	int greeting;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if (my_rank == 0) { // deadlocking process
		// Initiate greeting, send processor_1 to greet
		printf("Greetings from processor %d of %d!\n", my_rank, comm_sz);
		greeting = 0;
		MPI_Send(&greeting, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		
		// sleep 5 seconds so process wouldn't be terminated
		// due to non-deadlocking process (processor_2) finishing early
		sleep(5);

		// deadlock cause
		message1 = 1;
		MPI_Bcast(&message1, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&message2, 1, MPI_INT, 1, MPI_COMM_WORLD);

		printf("Processor %d done!\n", my_rank);
	} else if (my_rank == 1) { // deadlocking process
		// Receive greet from processor_0, send processor_2 to greet
		MPI_Recv(&greeting, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Greetings from processor %d of %d!\n", my_rank, comm_sz);
		MPI_Send(&greeting, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);

		// sleep 5 seconds so process wouldn't be terminated
		// due to non-deadlocking process (processor_2) finishing early
		sleep(5);

		// deadlocking cause
		message2 = 2;
		MPI_Bcast(&message2, 1, MPI_INT, 1, MPI_COMM_WORLD);
		MPI_Bcast(&message1, 1, MPI_INT, 0, MPI_COMM_WORLD);

		printf("Processor %d done!\n", my_rank);
	} else if (my_rank == 2) {
		// Last to greet
		MPI_Recv(&greeting, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		// Since processor_0 and processor_1 is deadlock, only processor_2
		// will print out done.
		printf("Greetings from processor %d of %d!\n", my_rank, comm_sz);
		printf("Processor %d done!\n", my_rank);
	}

	// Because MPI_Finalize() is commented out, deadlocking process would
	// not be handled, hence it wont print out done.
	//MPI_Finalize();
	return 0;
}
