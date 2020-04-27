#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIB_DATA "fib.data"
#define GUARD 8
#define MAX 196

struct BigN {
    char num[MAX];
    char shadow[GUARD];
    int nonzero;
};

static inline void str_reverse(char *s)
{
	int i,j;
	char tmp = '\x00';
	
	for (i = 0, j = strlen(s) - 1; i < j; ++i, --j) {
		tmp = s[i]; 
        s[i] = s[j];
        s[j] = tmp;
    }
}

static inline void BigN_init(struct BigN *n, char *s)
{   
    memset(n->num, '\x00', MAX);
    memset(n->shadow, '\x00', GUARD);
    n->nonzero = 0;
    strncpy(n->num, s, strlen(s));
}

static inline void BigN_init_zero(struct BigN *n)
{
    memset(n->num, '\x00', MAX);
}

static inline void BigN_dump(struct BigN *n)
{
    str_reverse(n->num);
    printf("%s\n", n->num);
}

void BigN_add(struct BigN *output, struct BigN x, struct BigN y)
{
    int carry = 0;
    int sum = 0;
    int i = 0;
    while (i < MAX) {
        sum = (x.num[i] + y.num[i] + carry) % '0';
        output->num[i] = (sum % 10) + '0';
        carry = 0;
        if (sum > 9) {
            carry = 1;
        }

        i++;
    }
    output->num[i] = '\x00';
}

/* k < 935 can normally work */
static struct BigN fib(long long k)
{
    struct BigN f[k + 2];
    for (int i = 0; i < k + 2; i++)
        BigN_init_zero(&f[i]);

    BigN_init(&f[0], "0");
    BigN_init(&f[1], "1");

    for (int i = 2; i <= k; i++) {
        BigN_add(&f[i], f[i - 1], f[i - 2]);
    }

    return f[k];
}

int main(int argc, char **argv)
{

#ifdef FIB_DATA_W
    FILE *fp;
    fp = fopen(FIB_DATA, "w+");
#endif

    if (argc < 2) {
        printf("Usage: ./fib <number> \n");
        goto f_close;
    }
    int n = atoll(argv[1]);

    if (n < 2) {
        printf("number need > 1");
        goto f_close;
    }

    struct BigN result;
    BigN_init(&result, "\x00");

    for (int i = 2; i <= n; i++) {
        result = fib(i);
        BigN_dump(&result);
#ifdef FIB_DATA_W     
        fprintf(fp, "%s\n", result.num);
#endif
    }

f_close:
#ifdef FIB_DATA_W
    fclose(fp);
#endif
    return 0;
}
