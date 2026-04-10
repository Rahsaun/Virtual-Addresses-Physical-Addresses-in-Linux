#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

// Same function reused
uint64_t virtual_to_physical(void *virtual_addr) {
    uint64_t value;
    uint64_t page_frame_number;
    int page_size = getpagesize();

    int fd = open("/proc/self/pagemap", O_RDONLY);
    if (fd < 0) {
        perror("Error opening pagemap");
        return 0;
    }

    uint64_t offset = ((uint64_t)virtual_addr / page_size) * sizeof(uint64_t);

    if (lseek(fd, offset, SEEK_SET) == -1) {
        perror("Error seeking pagemap");
        close(fd);
        return 0;
    }

    if (read(fd, &value, sizeof(value)) != sizeof(value)) {
        perror("Error reading pagemap");
        close(fd);
        return 0;
    }

    close(fd);

    if (!(value & (1ULL << 63))) {
        printf("Page not present in memory\n");
        return 0;
    }

    page_frame_number = value & ((1ULL << 55) - 1);

    uint64_t physical_addr = (page_frame_number * page_size) +
                             ((uint64_t)virtual_addr % page_size);

    return physical_addr;
}

int main() {
    // Create shared memory
    int *shared_mem = mmap(NULL, sizeof(int),
                           PROT_READ | PROT_WRITE,
                           MAP_SHARED | MAP_ANONYMOUS,
                           -1, 0);

    if (shared_mem == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    *shared_mem = 123;

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child process
        sleep(1); // ensure parent prints first
        printf("\n========== CHILD PROCESS ==========\n");
        printf("Virtual Address : %p\n", (void*)shared_mem);
        printf("Physical Address: 0x%lx\n",
               virtual_to_physical(shared_mem));
    } else {
        // Parent process
        printf("\n========== PARENT PROCESS ==========\n");
        printf("Virtual Address : %p\n", (void*)shared_mem);
        printf("Physical Address: 0x%lx\n",
               virtual_to_physical(shared_mem));
    }

    munmap(shared_mem, sizeof(int));
    return 0;
}