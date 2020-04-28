set terminal png font " Timesi_New_Roman,12 "
set xlabel "fib(x)"
set ylabel "ns"
set output "analysis.png"
set key left

plot \
"fib.time" using 1:2 with linespoints linewidth 2 title "fib(100)", \
