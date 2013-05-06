#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#ifdef AMPE
#include <mpe.h>
#endif
#ifdef MMPE
#include <mpe.h>
#endif
#ifdef SPRAND
//#define SIMPLE_SPRNG	// simple interface
#define USE_MPI		//use MPI to find number of processes
#include <sprng.h>
#endif


#define N 10 			//plate size
#define MonteN 10000	//MonteCarlo trys amount
#define EPS 1e-4		//small for faster tests


// MPI Logs
#define START_BCAST 0
#define END_BCAST 1
#define START_ALLRED 2
#define END_ALLRED 3
#define START_RECV 4
#define END_RECV 5
#define START_SEND 6
#define END_SEND 7

//SPRNG
#define SEED 985456376

#ifdef SPRAND
int *stream;
#endif

//PMPI - for MPI logging

#ifdef AMPE
int MPI_Init(int *argc, char **argv[]){
	int result;
	int rank = 0;
	
	result = PMPI_Init(argc, argv);
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPE_Init_log();
	if(rank == 0) {
		//MPE_Describe_state(START_BCAST,END_BCAST, "broadcast", "red");
		MPE_Describe_state(START_ALLRED,END_ALLRED, "reduction", "green");
		//MPE_Describe_state(START_RECV,END_RECV, "receive", "blue");
		//MPE_Describe_state(START_SEND,END_SEND, "send", "yellow");
	}
	MPE_Start_log();	
	return result;
}

int MPI_Finalize(){
	int result;
	MPE_Finish_log("logs");
	result = PMPI_Finalize();	
	return result;
}

/*int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root,MPI_Comm comm){
	int result;
	MPE_Log_event(START_BCAST,0,"pierwszy bcast");
		result = PMPI_Bcast(buffer,count,datatype,root,comm);
	MPE_Log_event(END_BCAST,0,"pierwszy bcast");
	return result;
}

int MPI_Send(void *buf, int count, MPI_Datatype datatype, int dest, int tag,MPI_Comm comm) {
	int result;
	MPE_Log_event(START_SEND,0,"send");
		result = PMPI_Send(buf, count, datatype, dest, tag, comm);
	MPE_Log_event(END_SEND,0,"send");
	return result;
}

int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status){
	int result;
	MPE_Log_event(START_RECV,0,"receive");
		result = PMPI_Recv(buf, count, datatype, source, tag, comm, status);
	MPE_Log_event(END_RECV,0,"receive");
	return result;
}*/

int MPI_Allreduce( void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm ){
	int result;
	MPE_Log_event(START_ALLRED,0,"reduciton");
		result = PMPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm );
	MPE_Log_event(END_ALLRED,0,"reduciton");
	return result;
}
#endif

/**
*	1-north, 2-east, 3-south, 4-west
*/
int randDirection(){
	#ifdef SPRAND
	//return isprng()%5+1; //integers
	return isprng(stream)%5+1; //integers
	#else
	return rand()%5+1;
	#endif
}

/**
*	start conditions - temperature at edges of plate
*/
void setEdgeTemp(double p[N][N]){
	int i,j;
	
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			p[i][j] = -1.0;
		}
	}
	for(i = 0 ; i < N ; ++i){
		p[0][i] = 0.0;
		p[i][0] = 0.0;
		p[i][N-1] = 0.0;
	}
	
	for(j=0;j<N;j++)
		p[N-1][j] = 100.0;
	
}

/**
*	recursive walk
*	ends after reaching counted temperature
*/
double walk2(double p[N][N],int x, int y){
	switch(randDirection()){
		case 1:	if(p[--x][y] < 0) break;
			else return p[x][y]; break;
		case 2:  if(p[x][++y] < 0) break;
			else return p[x][y]; break;
		case 3:  if(p[++x][y] < 0) break;
			else return p[x][y]; break;
		case 4:  if(p[x][--y] < 0) break;
			else return p[x][y]; break;
	}
	return walk2(p,x,y);
}


/**
*	find temperature at given point using MonteCarlo method
*	x,y - cooridinates of point in array
*/
inline long temperature(double p[N][N],int x, int y, int worldSize){
	int i;
	long n = 0, alln;
	double accu = 0.0;
	double temp = 0.0;
	double ownTemp;
	double oldTemp = -1.0;
	
	while(fabs(temp - oldTemp) > EPS){
		oldTemp = temp;
		for( i = 0 ; i < 500 ; ++i, ++n){
			accu += walk2(p,x,y);
		}
		ownTemp = accu/n;
		MPI_Allreduce( &ownTemp, &temp, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );
		temp /= worldSize;
	}
	p[x][y] = temp;
	MPI_Allreduce( &n, &alln, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );
	
	return alln;
}

/**
*	if out==1 shows plate at std output
*	save plate in file in format to generate pm3d map
*/
void showAndSavePlate(double p[N][N], int out){
	FILE* file = fopen("proj1.txt","w");
	int i,j;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++) if(out) printf("%3.2f ", p[i][j]);
		if(out) printf("\n");
	}
	
	//file output, used to create grid image
	for(i=0;i<N;i++){
		for(j=0;j<N;j++)
			fprintf(file,"%d %d %3.6f\n",i,j,p[i][j]);
		fprintf(file,"\n");
	}
	fclose(file);
}


/**
*	program implements problem of:
*		Finding temperature at a point inside a 2D plate using MonteCarlo method. Temperature is in kelvin
*/

int main(int argc, char** argv)
{
	double p[N][N];
	
	setEdgeTemp(p);
	int worldSize, rank;
	
	
	/** SPRNG
	 * Available generators; use corresponding numeral:
		   lfg     --- 0 
		   lcg     --- 1 
		   lcg64     --- 2 
		   cmrg     --- 3 
		   mlfg     --- 4 
		   pmlcg     --- 5 
	 */
	int gtype;
	gtype = 0;
	
	MPI_Init( &argc, &argv );
	MPI_Comm_size( MPI_COMM_WORLD, &worldSize );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	
	
	#ifdef MMPE
	MPE_Init_log();
	if(rank == 0) {
		MPE_Describe_state(START_ALLRED,END_ALLRED, "reduction", "green");
	}
	MPE_Start_log();
	#endif
	
	#ifdef SPRAND
	//int *stream;
	//SPRNG4
	//init_sprng(SEED,SPRNG_DEFAULT,gtype);	/* initialize stream*/
	//SPRNG2
	stream = init_sprng(gtype,rank,worldSize,SEED,SPRNG_DEFAULT);
	#endif
	
	/*    main loop in plate array	*/
	int i,j;
	long n;
	for(i=1;i<N-1;i++){
		for(j=1;j<N-1;j++){
			n = temperature(p,i,j,worldSize);
			if(rank == 0)
				printf("[%d %d] %ld ",i,j,n); //<--- BLAD, WYNIKI SIE ZERUJA <--- tere-fere fcale nie
		}
		if(rank == 0)
			printf("\n");
	}
	
	printf("Counted\n");
	if(rank == 0)		
		showAndSavePlate(p,0);
		
	#ifdef SPRAND
	free_sprng(stream);
	#endif
	
	#ifdef MMPE
	MPE_Finish_log("logs");
	#endif
	
	MPI_Finalize();

	return 0;
}



