all:
	mpicc -o proj1 proj1.c  -lm
fatcat:
	/opt/nfs/mpich-3.0.2/bin/mpicxx -DSIMPLE_SPRNG -DUSE_MPI proj1.c -I/opt/nfs/sprng4/include -I/opt/nfs/mpe2-1.3.0/include -L/opt/nfs/sprng4/lib -L/opt/nfs/mpe2-1.3.0/lib -lsprng -lmpe -lm
fatcat-ampe:
	/opt/nfs/mpich-3.0.2/bin/mpicxx -DSIMPLE_SPRNG -DAMPE -DSPRAND -DUSE_MPI proj1.c -I/opt/nfs/sprng4/include -I/opt/nfs/mpe2-1.3.0/include -L/opt/nfs/sprng4/lib -L/opt/nfs/mpe2-1.3.0/lib -lsprng -lmpe -lm
run:
	mpiexec -n 3 ./proj1
	gnuplot wykres.gp
fatrun:
	/opt/nfs/mpich-3.0.2/bin/mpiexec -n 3 ./a.out
tests:
	@time /opt/nfs/mpich-3.0.2/bin/mpiexec -n 2 ./proj1 
	@time /opt/nfs/mpich-3.0.2/bin/mpiexec -n 4 ./proj1
	@time /opt/nfs/mpich-3.0.2/bin/mpiexec -n 6 ./proj1
	@gnuplot wykres.gp
help:
	@echo 'Użycie:'
	@echo 'all: ogólna kompilacja  MPI(bez sprng i MPE)'
	@echo 'fatcat: kompilacja na komputerze pracowni'
	@echo 'fatcat-ampe: kompilacja na fatcacie z automatycznymi logami'
	@echo 'run: uruchomienie na 3 procesach + rysowanie wykresu'
	@echo 'fatrun: uruchamianie na fatcacie na 3 procesach + rysowanie wykresu'
	@echo 'tests: testy czasowe dla 2 4 i 6 procesów (na fatcacie)'
