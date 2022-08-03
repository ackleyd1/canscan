#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>
#include<string.h>

#define N 2048

// this is the frequency of the arbitration ID
static long cnt[N] = {0};

// measurement accuracy for doubles
const double epsilon = 0.0000001f;

// could be better implemented with a circular buffer
static double prev[N] = {0.0};

// period of the arbitration ID
static double prd[N] = {0.0};

static long ham[N] = {0};
static int clz[N] = {0};
static int ctz[N] = {0};


// static long diff[N] = {0};
// static long max[N] = {0};
// static long min[N] = {0};

long max_cnt = 0;

typedef struct can_msg {
    uint16_t hdr;
    uint8_t data[8];
} can_msg;

int main(int argc, char *argv[])
{
	if ( argc < 2)
		exit(-1);
	
	// open socket can log file
	FILE * fd = fopen(argv[1], "r");
	if ( fd == NULL )
		exit(-1);

	char buf[64];
	
	double t;
	uint32_t id;
	uint64_t d;

	while(fgets(buf, sizeof(buf)-2, fd))
	{
		// process each line
		sscanf(strtok(buf, "() "), "%lf", &t);
		// throw away bus interface for now
		strtok(NULL, " ");
		sscanf(strtok(NULL, "#"), "%X", &id);
		sscanf(strtok(NULL, "\n"), "%lX", &d);

		// ham stats
		ham[id] += __builtin_popcountl(d);
		clz[id] += __builtin_clz(d);
		ctz[id] += __builtin_ctz(d);

		// update interval stats
		if ( (prev[id]-epsilon)  > 0.0f )
		{
			prd[id] += t - prev[id];
			prev[id] = t;
		}
		else
			prev[id] = t;

		// update cnt stats
		cnt[id]++;
		if ( cnt[id] > max_cnt)
			max_cnt = cnt[id];
	}
		
	// normalize stats
	for(size_t i = 0; i < N; i++)
	{
		ctz[i] /= (cnt[i]-1);
		clz[i] /= (cnt[i]-1);
		ham[i] /= (cnt[i]-1);
		prd[i] /= (cnt[i]-1);
	}
		
	// for width of count field
	int num_digits = 0;
	for(long n = max_cnt; n > 0; n/=10)
		num_digits++;
	
	printf("Trace %s statistics\n", argv[1]);
	puts("=======================================");
	for(size_t i = 0; i < N; i++)
		if (cnt[i])
			printf("id: %3lX cnt: %*li prd: %f ham: %li clz: %i ctz: %i\n", i, num_digits, cnt[i], prd[i], ham[i], clz[i], ctz[i]);

	fclose(fd);
}
