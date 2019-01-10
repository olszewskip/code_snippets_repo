#include <mpi.h>
#include <stdio.h>
#include <string.h>
int main (int argc, char *argv[]) {
	int rank;
	typedef struct {
		char nazwisko[50];
		int indeks;
	} student;
	
	student student1;

	MPI_Datatype mpi_student;
	
	MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	
	MPI_Aint addr_nazwisko, addr_indeks;
	MPI_Get_address (&student1.nazwisko, &addr_nazwisko);
	MPI_Get_address (&student1.indeks, &addr_indeks);
	int blocklength[] = { 50, 1 };
	MPI_Aint displacements[] = { 0, addr_indeks - addr_nazwisko};
	MPI_Datatype types[] = { MPI_CHAR, MPI_INT };
	MPI_Type_create_struct (2, blocklength, displacements, types, &mpi_student);
	MPI_Type_commit (&mpi_student);
			 
	if (rank == 0) {
		strcpy (student1.nazwisko, "Kowalski");
		student1.indeks = 1234;
		MPI_Send (&student1, 1, mpi_student, 1, 123, MPI_COMM_WORLD);
	} else {
		student student_recv;
		MPI_Recv (&student_recv, 1, mpi_student, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf ("%s %d\n", student_recv.nazwisko, student_recv.indeks);
	}
	MPI_Type_free(&mpi_student);	
	MPI_Finalize ();
	return 0;
}
