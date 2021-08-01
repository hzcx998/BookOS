#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

void* runner(void* param) {
	int n = *(int*)param;
	int i = 0;
	for (; i < 10; ++i) {
		printf("Tid[%d]: %d\n", n, i);
	}
}

int main(int argc, char**argv) {
	int n1 = 1, n2 = 2;

	pthread_t tid1, tid2;

	pthread_create(&tid1, 0, runner, (void*)&n1);
	pthread_create(&tid2, 0, runner, (void*)&n2);

	pthread_join(tid1, 0);
	pthread_join(tid2, 0);

	return 0;
}
