all:
	mpicc -o proj1 proj1.c -lm
mpe:
	mpicc -o proj1 proj1.c -lmpe -lm
run:
	@time mpiexec -n 3 ./proj1
	@time mpiexec -n 5 ./proj1
	@time mpiexec -n 6 ./proj1
	@gnuplot wykres.gp
