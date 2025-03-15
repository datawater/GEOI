#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <pthread.h>

static void *(*real_malloc)(size_t) = NULL;
static void *(*real_free)(void *) = NULL;
static size_t total_allocated = 0;
static size_t memory_limit = 1024 * 1024; // Default 1MB
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

__attribute__((constructor))
void init() {
    char *env_limit = getenv("MEM_LIMIT");
    if (env_limit) {
        memory_limit = strtoull(env_limit, NULL, 10);
    }
}

void *malloc(size_t size) {
    if (!real_malloc) real_malloc = dlsym(RTLD_NEXT, "malloc");

    pthread_mutex_lock(&lock);
    if (total_allocated + size > memory_limit) {
        fprintf(stderr, "Memory limit exceeded! (%zu bytes)\n", memory_limit);
        pthread_mutex_unlock(&lock);
        exit(1);
    }

    void *ptr = real_malloc(size);
    if (ptr) total_allocated += size;
    pthread_mutex_unlock(&lock);

    return ptr;
}

void free(void *ptr) {
    if (!real_free) real_free = dlsym(RTLD_NEXT, "free");
    pthread_mutex_lock(&lock);
    real_free(ptr);
    pthread_mutex_unlock(&lock);
}
