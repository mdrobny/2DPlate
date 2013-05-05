all:
	mpicc.mpich2 -o proj1 proj1.c -I/usr/lib/mpich/include -L/usr/lib/mpich/lib -L/usr/lib -lmpe -lm
fatcat:
	/opt/nfs/mpich-3.0.2/bin/mpicc -o proj1 proj1.c -lmpe -I/opt/nfs/mpe2-1.3.0/include -L/opt/nfs/mpe2-1.3.0/lib -I/opt/nfs/sprng4/include -L/opt/nfs/sprng4/lib -lsprng -lm
sprng:
	mpicc.mpich2 -o proj1 proj1.c -DSPRAND -I/usr/lib/mpich/include -I/usr/include/sprng/ -L/usr/lib/mpich/lib/ -L/usr/lib/libsprng* -lmpe -lsprng -lm
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
	@echo 'all: ogólna kompilacja (działa u mnie - linux mint)'
	@echo 'fatcat: kompilacja na komputerze pracowni'
	@echo 'sprng: kompilacja u mnie ze sprng (nie działa)'
	@echo 'run: uruchomienie na 3 procesach + rysowanie wykresu'
	@echo 'fatrun: uruchamianie na fatcacie'
	@echo 'tests: testy czasowe dla 2 4 i 6 procesów'
