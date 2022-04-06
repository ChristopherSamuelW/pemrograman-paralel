#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char **argv)
{
	int n;
	n = 10;
	int comm_sz,world_rank,world_size,sub_rank,sub_size,signal;
	int message;

	// setup for world
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	MPI_Group world_group;
	MPI_Comm_group(MPI_COMM_WORLD, &world_group);

	// Create a sub group for odd process and even process
	int odd_num;
	int even_num;
	if ((world_size - 1) % 2 == 0) {
		odd_num = (world_size-1)/2;
	} else {
		odd_num = world_size/2;
	}
	even_num = world_size - 1 - odd_num;
	int odd_ranks[odd_num];
	int even_ranks[even_num];
	int i;
	for(i = 0; i < odd_num; i++){
		odd_ranks[i] = (i * 2) + 1;
	}
	for(i = 0; i < even_num; i++) {
		even_ranks[i] = (i * 2) + 2;
	}
	MPI_Group odd_group;
	MPI_Group even_group;
	MPI_Group_incl(world_group, odd_num, odd_ranks, &odd_group);
	MPI_Group_incl(world_group, even_num, even_ranks, &even_group);

	// Create a new communicator for sub group
	MPI_Comm odd_comm;
	MPI_Comm even_comm;
	MPI_Comm sub_comm;
	MPI_Comm_create_group(MPI_COMM_WORLD, odd_group, 0, &odd_comm);
	MPI_Comm_create_group(MPI_COMM_WORLD, even_group, 0, &even_comm);

	if (world_rank != 0) {
		if (world_rank % 2 == 0) {
			MPI_Comm_rank(even_comm, &sub_rank);
			MPI_Comm_size(even_comm, &sub_size);
			MPI_Comm_dup(even_comm, &sub_comm);
		} else {
			MPI_Comm_rank(odd_comm, &sub_rank);
			MPI_Comm_size(odd_comm, &sub_size);
			MPI_Comm_dup(odd_comm, &sub_comm);
		}
	}

	/* 
	 * P0: Not in either sub-group
	 * */
	if (world_rank == 0) {
		signal = 1;
		printf("P%d: What is the color of the pen?\n", world_rank);
		MPI_Send(&signal, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		MPI_Send(&signal, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
	}
	/*
	 * P1: Send message to other odd ranks
	 * */
	else if (world_rank == 1) {
		message = 255;
		MPI_Recv(&signal, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("P%d: The pen is blue\n", world_rank);
		if (world_size > 3) {
			MPI_Send(&message, 1, MPI_INT, sub_rank + 1, 0, sub_comm);
		}		
	}
	/*
	 * P2: Send message to other even ranks
	 * */
	else if (world_rank == 2) {
		message = 256;
		MPI_Recv(&signal, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("P%d: The pen is red\n", world_rank);
		if (world_size > 3) {
			MPI_Send(&message, 1, MPI_INT, sub_rank + 1, 0, sub_comm);
		}
	} else {
		MPI_Recv(&message, 1, MPI_INT, sub_rank - 1, 0, sub_comm, MPI_STATUS_IGNORE);
		if(message == 255) {
			printf("P%d: The pen is blue\n", world_rank);
		} else if(message == 256){
			printf("P%d: The pen is red\n", world_rank);
		} else {
			printf("P%d: I don't know\n", world_rank);
		}
		if((sub_rank + 1) != sub_size) {
			MPI_Send(&message, 1, MPI_INT, sub_rank + 1, 0, sub_comm);
		}
	}
	MPI_Finalize();
	return 0;
}
