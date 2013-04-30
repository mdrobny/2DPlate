all:
	@mpicc -o proj1 proj1.c -lm
run:
	@mpiexec ./proj1
	@gnuplot wykres.gp
