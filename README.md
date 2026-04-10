# Virtual-Addresses-Physical-Addresses-in-Linux
This project explores low-level memory management concepts in operating systems using C programming. It demonstrates how virtual memory addresses are translated into physical memory addresses and how shared memory behaves across multiple processes.

# How to run programs 
1. Open a terminal on a Linux machine and run
2. Use gcc to compile both source files:
   gcc part1_virtual_physical.c -o part1
   gcc part2_shared_memory.c -o part2
3. These programs must be run with root privileges to correctly access physical memory information.
   sudo ./part1
   sudo ./part2

# Important Notes 
1. Must be run on a Linux system
  Requires access to:
  /proc/self/pagemap
2. Root privileges (sudo) may be required depending on the system configuration
3. Virtual addresses may change between runs
4. In Part 2, both processes should show the same physical address, demonstrating shared memory

# Troubleshooting
  1. If physical addresses show as 0:
  Run with sudo
  2. If compilation fails:
  3. Ensure gcc is installed:
  4. sudo apt install build-essential

# Analysis
 Part 1: The virtual to physical address conversion algorithm works by translating a process’s virtual memory address into an actual physical memory location using the operating system’s paging system.First, the program obtains the system page size, and uses it to divide the virtual address into two parts: the virtual page number and the offset within that page.The virtual page number is used to look up the corresponding page table entry in Linux’s /proc/self/pagemap, the program extracts the physical frame number (PFN), which identifies the actual location of the page in physical memory. inally, the physical address is computed using the formula: physical address = (PFN × page size) + offset. This combines the physical page location with the original offset inside the page, producing the final physical memory address that corresponds to the given virtual address.
