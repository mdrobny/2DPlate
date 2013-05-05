all:
	mpicc.mpich2 -o proj1 proj1.c -I/usr/lib/mpich/include -I/usr/include/sprng/ -L/usr/lib/mpich/lib -lmpe -lm
fatcat:
	/opt/nfs/mpich-3.0.2/bin/mpicc -DSIMPLE_SPRNG -DUSE_MPI -o proj1 proj1.c -lmpe -I/opt/nfs/mpe2-1.3.0/include -L/opt/nfs/mpe2-1.3.0/lib  -lm
sprng:
	/opt/nfs/mpich-3.0.2/bin/mpicc -DSIMPLE_SPRNG -DUSE_MPI -o proj1 proj1.c -lmpe -I/opt/nfs/mpe2-1.3.0/include -L/opt/nfs/mpe2-1.3.0/lib -I/opt/nfs/sprng4/include -L/opt/nfs/sprng4/lib -lsprng -lm
run:
	mpiexec -n 3 ./proj1
fatrun:
	/opt/nfs/mpich-3.0.2/bin/mpiexec -n 3 ./proj1
tests:
	@time /opt/nfs/mpich-3.0.2/bin/mpiexec -n 3 ./proj1
	@time /opt/nfs/mpich-3.0.2/bin/mpiexec -n 5 ./proj1
	@time /opt/nfs/mpich-3.0.2/bin/mpiexec -n 6 ./proj1
	@gnuplot wykres.gp
