#include "test.h"
#include <pthread.h>
#include <pthread_internal.h>

int count = 0; 

pthread_mutex_t mutex;

void *thread_entry(void *arg)
{
    while (1) {
        pthread_mutex_lock(&mutex);
        printf("child count: %d\n", count);
        count++;

        pthread_mutex_unlock(&mutex);
        if (count > 100)
            return (void *) count;
        usleep(100);
    }
}

int pthread_test(int argc, char *argv[])
{
    pthread_mutex_init(&mutex, NULL);
    pthread_t thread;
    if (pthread_create(&thread, NULL, thread_entry, NULL) < 0) {
        printf("create thread failed!\n");
        return -1;
    }

    while (1) {
        pthread_mutex_lock(&mutex);
        printf("parent count: %d\n", count);
        count++;
        
        pthread_mutex_unlock(&mutex);
        if (count > 100)
            break;
        usleep(100);
    }

    void *status;
    pthread_join(thread, &status);
    printf("thread exit with %x!\n", status);
    return 0;
}

pthread_key_t   key;
void echomsg(int t)
{
    printf("destructor excuted in thread %d,param=%d\n",pthread_self(),t);
}
void * child1(void *arg)
{
    pthread_t tid=pthread_self();
    printf("thread %d enter\n",tid);
    pthread_setspecific(key,(void *)tid);
    sleep(2);
    printf("thread %d returns %d and key %d\n",tid,pthread_getspecific(key),key);
    sleep(5);
    return NULL;
}
void * child2(void *arg)
{
    pthread_t tid=pthread_self();
    printf("thread %d enter\n",tid);
    pthread_setspecific(key,(void *)tid);
    sleep(1);
    printf("thread %d returns %d and key %d\n",tid,pthread_getspecific(key),key);
    sleep(5);
    return NULL;
}

int pthread_test_key(int argc, char *argv[])
{
    pthread_t tid1,tid2;
    printf("this is main thread\n");
    pthread_key_create(&key,(void (*)(void *))echomsg);
    pthread_create(&tid1,NULL,child1,NULL);
    pthread_create(&tid2,NULL,child2,NULL);
    sleep(10);
    pthread_key_delete(key);
    printf("main thread exit\n");
    return 0;
}

void * thread_id(void *arg)
{
    pthread_t tid=pthread_self();
    printf("thread %d enter\n",tid);
    struct pthread *thread = THREAD_SELF;
    printf("thread %d,%d enter\n",thread->tid, thread->pid);
    return NULL;
}

int pthread_test_struct(int argc, char *argv[])
{
    pthread_t tid1,tid2,tid3;
    printf("this is main thread\n");
    pthread_create(&tid1,NULL,thread_id,NULL);
    pthread_create(&tid2,NULL,thread_id,NULL);
    pthread_create(&tid3,NULL,thread_id,NULL);
    printf("thread %d,%d,%d\n",tid1,tid2,tid3);
    sleep(3);
    printf("main thread exit\n");
    return 0;
}
