#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vulnerable_app.h"

// Buffer overflow vulnerabilities
void buffer_overflow_vuln(char* input) {
    char buffer[100];
    
    // Classic buffer overflow - no bounds checking
    strcpy(buffer, input); // VULNERABILITY: strcpy without bounds check
    
    printf("Buffer content: %s\n", buffer);
}

void stack_overflow_vuln(int depth) {
    char local_buffer[1000];
    
    // Fill local buffer to demonstrate stack usage
    memset(local_buffer, 'A', sizeof(local_buffer));
    
    if (depth > 0) {
        // Recursive call that can cause stack overflow
        stack_overflow_vuln(depth - 1);
    }
}

// Use-after-free vulnerabilities
void use_after_free_vuln() {
    char* ptr = malloc(100);
    if (!ptr) return;
    
    strcpy(ptr, "This is a test string");
    printf("Before free: %s\n", ptr);
    
    free(ptr);
    
    // Use after free - accessing freed memory
    printf("After free: %s\n", ptr); // VULNERABILITY
    strcpy(ptr, "Modified after free"); // VULNERABILITY
}

// Double free vulnerability
void double_free_vuln() {
    char* ptr = malloc(50);
    if (!ptr) return;
    
    strcpy(ptr, "Double free test");
    
    free(ptr);
    free(ptr); // VULNERABILITY: double free
}

// Memory leak vulnerability
void memory_leak_vuln(int iterations) {
    for (int i = 0; i < iterations; i++) {
        char* leaky_ptr = malloc(1024);
        if (leaky_ptr) {
            strcpy(leaky_ptr, "This memory will be leaked");
            // VULNERABILITY: memory leak - no free()
        }
    }
}

// Integer overflow vulnerabilities
void integer_overflow_vuln(int size) {
    // Integer overflow in allocation size
    int total_size = size * 1000;
    if (total_size < 0) { // Overflow occurred
        printf("Integer overflow detected!\n");
        return;
    }
    
    char* buffer = malloc(total_size);
    if (buffer) {
        memset(buffer, 0, total_size);
        free(buffer);
    }
}

// Format string vulnerability
void format_string_vuln(char* user_input) {
    // Format string vulnerability - user input as format string
    printf(user_input); // VULNERABILITY: format string
    printf("\n");
}

// Heap overflow vulnerability
void heap_overflow_vuln(char* input) {
    char* heap_buffer = malloc(50);
    if (!heap_buffer) return;
    
    // Heap overflow - writing beyond allocated size
    strcpy(heap_buffer, input); // VULNERABILITY: no bounds check
    
    printf("Heap buffer: %s\n", heap_buffer);
    free(heap_buffer);
}

// Null pointer dereference
void null_pointer_deref() {
    char* null_ptr = NULL;
    
    // Null pointer dereference
    printf("Value: %s\n", null_ptr); // VULNERABILITY
    strcpy(null_ptr, "This will crash"); // VULNERABILITY
}

// Uninitialized variable usage
void uninitialized_var_vuln() {
    char uninit_buffer[100];
    
    // Using uninitialized memory
    printf("Uninitialized content: %s\n", uninit_buffer); // VULNERABILITY
}
