#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIB_DATA "fib.data"
#define GUARD 196 + 16
#define MAX 196

struct BigN {
    char num[GUARD];
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
    memset(n->num, '\x00', GUARD);
    strncpy(n->num, s, strlen(s));
    str_reverse(n->num);
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

    for (int i = 0; i < MAX; i++) {
        sum = (x.num[i] + y.num[i] + carry) % '0';
        output->num[i] = (sum % 10) + '0';
        carry = 0;
        if (sum > 9) {
            carry = 1;
        }
    }
    output->num[MAX] = '\x00';
}

/* k < 935 can normally work */
static struct BigN fib(long long k)
{
    struct BigN f[k + 2];
    BigN_init(&f[0], "0");
    BigN_init(&f[1], "1");

    for (int i = 2; i <= k; i++) {
        BigN_add(&f[i], f[i - 1], f[i - 2]);
    }

    return f[k];
}

int main(int argc, char **argv)
{
    FILE *fp;
    fp = fopen(FIB_DATA, "w+");

    if (argc < 2) {
        printf("Usage: ./fib <number> \n");
        fclose(fp);
        return 0;
    }
    int n = atoll(argv[1]);

    if (n < 2) {
        printf("number need > 1");
        fclose(fp);
        return 0;
    }

    struct BigN result;
    BigN_init(&result, "\x00");

    for (int i = 2; i <= n; i++) {
        result = fib(i);
        BigN_dump(&result);
        fprintf(fp, "%s\n", result.num);
    }

    fclose(fp);
    return 0;
}
