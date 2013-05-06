set term png
set output 'grid.png'
set size square
set pm3d map
set xrange[0 : 19]
set yrange[0 : 19]
splot 'proj1.txt' u 1:2:3 title 'Siatka' 

set term postscript color
set output 'grid.eps'
set size square
set pm3d map
set xrange[0 : 19]
set yrange[0 : 19]
splot 'proj1.txt' u 1:2:3 title 'Siatka' 
