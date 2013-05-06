all:
	mpicc -o proj1 proj1.c  -lm
mmpe:
	mpicc -o proj1 proj1.c -DMMPE -I/usr/lib/mpich/include -L/usr/lib/mpich/lib/ -lmpe -lm
sprng:
	mpicc -o proj1 proj1.c -DSPRAND -I/usr/lib/mpich/include -I/usr/include/sprng/ -L/usr/lib/mpich/lib/ -lmpe -lsprng -lm
fatcat:
	/opt/nfs/mpich-3.0.2/bin/mpicc -o proj1 proj1.c  -lm
fatcat-mmpe:
	/opt/nfs/mpich-3.0.2/bin/mpicc -o proj1 proj1.c -DMMPE -lmpe -I/opt/nfs/mpe2-1.3.0/include -L/opt/nfs/mpe2-1.3.0/lib -I/opt/nfs/sprng4/include -L/opt/nfs/sprng4/lib -lsprng -lm
fatcat-sprng:
	/opt/nfs/mpich-3.0.2/bin/mpicc -o proj1 proj1.c -DSPRAND -I/opt/nfs/sprng4/include -L/opt/nfs/sprng4/lib -lsprng -lm
sprng-mpe:
	mpicc -o proj1 proj1.c -DSPRAND -DAMPE -I/usr/lib/mpich/include -I/usr/include/sprng/ -L/usr/lib/mpich/lib/ -lmpe -lsprng -lm
mpe:
	mpicc -o proj1 proj1.c -DAMPE -I/usr/lib/mpich/include -L/usr/lib/mpich/lib -L/usr/lib -lmpe -lm
run:
	mpiexec -n 3 ./proj1
	gnuplot wykres.gp
fatrun:
	/opt/nfs/mpich-3.0.2/bin/mpiexec -n 3 ./proj1
tests:
	@time /opt/nfs/mpich-3.0.2/bin/mpiexec -n 2 ./proj1 
	@time /opt/nfs/mpich-3.0.2/bin/mpiexec -n 4 ./proj1
	@time /opt/nfs/mpich-3.0.2/bin/mpiexec -n 6 ./proj1
	@gnuplot wykres.gp
help:
	@echo 'Użycie:'
	@echo 'all: ogólna kompilacja (działa u mnie - linux mint) MPI + manual MPE'
	@echo 'fatcat: kompilacja na komputerze pracowni'
	@echo 'sprng: kompilacja u mnie ze sprng2 + manual MPE'
	@echo 'sprng-mpe: kompilacja u mnie ze sprng2 + auto MPE'
	@echo 'mpe: kompilacja u mnie  auto MPE'
	@echo 'run: uruchomienie na 3 procesach + rysowanie wykresu'
	@echo 'fatrun: uruchamianie na fatcacie'
	@echo 'tests: testy czasowe dla 2 4 i 6 procesów (na fatcacie)'
