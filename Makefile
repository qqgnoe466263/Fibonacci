
all: fib.c
	gcc fib.c -o fib

check:
	./fib 900
	./verify_fib.py

clean:
	rm fib fib.data fib.time
