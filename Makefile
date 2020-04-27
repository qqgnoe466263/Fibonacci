
all: fib.c
	gcc fib.c -o fib

check:
	gcc fib.c -o fib -DFIB_DATA_W
	./fib 934
	./verify_fib.py

clean:
	rm fib fib.data fib.time
