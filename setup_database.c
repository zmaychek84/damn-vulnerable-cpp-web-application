#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "vulnerable_app.h"

// Forward declaration
void create_vulnerable_database();

int main(int argc, char* argv[]) {
    if (argc > 1 && strcmp(argv[1], "--setup-db") == 0) {
        create_vulnerable_database();
        return 0;
    }
    
    printf("Usage: %s --setup-db\n", argv[0]);
    return 1;
}
