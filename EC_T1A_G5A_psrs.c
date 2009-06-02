/* Para compilar: mpicc EC_T1A_G5A_psrs.c -o EC_T1A_G5A 
   Para executar:  mpiexec -n 10 ./EC_T1A_G5A 100
   */
#include <stdio.h>
#include <mpi.h>
#include <time.h>

	MPI_Status Stat;
	int *vet; /*Data Vector*/
	int tag = 1;
int master(int size, int sizeVector){
	printf("I am your master, I own %d salves, sizeVector = %d\n", size - 1, sizeVector);
	int i, out, dest;
	for( i = 0 ; i < sizeVector ; i++ ){
        	printf("%d ", vet[i]);
     	}
	printf("\n");
	for(i = 1; i < size; i++){
		out = i;
		dest = i;
		printf("You, slave number %d may start your work!\n", i);
		MPI_Send(&out, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
	}	
	
	out = 51;
	printf("All of you have to work now!\n", i);
	MPI_Bcast(&out, 1, MPI_INT, 0, MPI_COMM_WORLD);
	return 0;
}

int slave(int rank){
	int source = 0;
	int in;
	MPI_Recv(&in, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
	printf("I am a slave, my number is %d\n", rank);
	MPI_Bcast(&in, 1, MPI_INT, 0, MPI_COMM_WORLD);
	printf("I am a slave, my number is %d. I am working again = %d\n", rank, in);
}

int main (argc, argv)
     int argc;
     char *argv[];
{

	int rank, numThreads;
	int sizeVector = atoi(argv[1]); /*Size of data vector*/
	
	vet = malloc(sizeVector*sizeof(int)); /*Dinamic allocation*/
	srand (time(NULL));
    	
	int i;
	for( i = 0 ; i < sizeVector ; i++ ){
        	vet[i] = (rand()*(1e2/RAND_MAX))+1;
     	}

	MPI_Init (&argc, &argv);	/* starts MPI */
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);	/* get current process id */
	MPI_Comm_size (MPI_COMM_WORLD, &numThreads);	/* get number of processes */
	
	if (rank==0){
		master(numThreads, sizeVector);
	}
	else{
		slave(rank);
	}
	
	MPI_Finalize();
 
	return 0;
}
