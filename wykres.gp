set term png
  
set output 'grid.png'
set size square
set pm3d map
splot 'proj1.txt' u 1:2:3 title 'Siatka' 