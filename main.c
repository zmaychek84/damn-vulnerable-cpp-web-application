#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sqlite3.h>
#include "vulnerable_app.h"

// Hardcoded credentials - VULNERABILITY
const char* ADMIN_USERNAME = "admin";
const char* ADMIN_PASSWORD = "password123";
const char* API_KEY = "sk-1234567890abcdef";
const char* DB_PASSWORD = "dbpass123";

// Global variables for use-after-free vulnerability
char* global_buffer = NULL;
int buffer_size = 0;

int main() {
    printf("Content-Type: text/html\r\n\r\n");
    printf("<html><head><title>Vulnerable Web App</title></head><body>");
    printf("<h1>Vulnerable Web Application for SAST Testing</h1>");
    
    char* query_string = getenv("QUERY_STRING");
    char* request_method = getenv("REQUEST_METHOD");
    
    if (query_string && strstr(query_string, "action=")) {
        handle_request(query_string, request_method);
    } else {
        show_main_page();
    }
    
    printf("</body></html>");
    return 0;
}

void handle_request(char* query_string, char* method) {
    char action[256] = {0};
    char username[256] = {0};
    char password[256] = {0};
    char search_term[512] = {0};
    
    // Buffer overflow vulnerability - no bounds checking
    // Parse action parameter
    char* action_start = strstr(query_string, "action=");
    if (action_start) {
        action_start += 7; // Skip "action="
        char* action_end = strchr(action_start, '&');
        if (action_end) {
            strncpy(action, action_start, action_end - action_start);
        } else {
            strcpy(action, action_start);
        }
    }
    
    // Parse username parameter
    char* user_start = strstr(query_string, "username=");
    if (user_start) {
        user_start += 9; // Skip "username="
        char* user_end = strchr(user_start, '&');
        if (user_end) {
            strncpy(username, user_start, user_end - user_start);
        } else {
            strcpy(username, user_start);
        }
    }
    
    // Parse password parameter
    char* pass_start = strstr(query_string, "password=");
    if (pass_start) {
        pass_start += 9; // Skip "password="
        char* pass_end = strchr(pass_start, '&');
        if (pass_end) {
            strncpy(password, pass_start, pass_end - pass_start);
        } else {
            strcpy(password, pass_start);
        }
    }
    
    // Parse search parameter
    char* search_start = strstr(query_string, "search=");
    if (search_start) {
        search_start += 7; // Skip "search="
        char* search_end = strchr(search_start, '&');
        if (search_end) {
            strncpy(search_term, search_start, search_end - search_start);
        } else {
            strcpy(search_term, search_start);
        }
    }
    
    if (strcmp(action, "login") == 0) {
        handle_login(username, password);
    } else if (strcmp(action, "search") == 0) {
        handle_search(search_term);
    } else if (strcmp(action, "upload") == 0) {
        handle_file_upload();
    } else if (strcmp(action, "admin") == 0) {
        handle_admin_panel(username, password);
    } else {
        printf("<p>Invalid action</p>");
    }
}

void handle_login(char* username, char* password) {
    // Hardcoded credential check - VULNERABILITY
    if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
        printf("<p>Login successful! Welcome admin.</p>");
        printf("<p>API Key: %s</p>", API_KEY); // Exposing API key
    } else {
        printf("<p>Login failed. Invalid credentials.</p>");
    }
}

void handle_search(char* search_term) {
    sqlite3* db;
    char* err_msg = 0;
    int rc;
    
    rc = sqlite3_open("vulnerable.db", &db);
    if (rc != SQLITE_OK) {
        printf("<p>Database error: %s</p>", sqlite3_errmsg(db));
        return;
    }
    
    // SQL Injection vulnerability - direct string concatenation
    char sql[1024];
    sprintf(sql, "SELECT * FROM users WHERE username LIKE '%%%s%%'", search_term);
    
    printf("<p>Executing query: %s</p>", sql);
    
    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    if (rc != SQLITE_OK) {
        printf("<p>SQL error: %s</p>", err_msg);
        sqlite3_free(err_msg);
    }
    
    sqlite3_close(db);
}

void handle_file_upload() {
    char* content_length = getenv("CONTENT_LENGTH");
    if (!content_length) {
        printf("<p>No content length specified</p>");
        return;
    }
    
    int len = atoi(content_length);
    
    // Integer overflow vulnerability
    if (len < 0 || len > 1000000) {
        printf("<p>Invalid content length</p>");
        return;
    }
    
    char* buffer = malloc(len + 1);
    if (!buffer) {
        printf("<p>Memory allocation failed</p>");
        return;
    }
    
    // Buffer overflow - reading more than allocated
    fread(buffer, 1, len + 100, stdin);
    buffer[len] = '\0';
    
    // Use-after-free vulnerability
    global_buffer = buffer;
    buffer_size = len;
    
    // Free the buffer but keep global reference
    free(buffer);
    
    // Using freed memory
    printf("<p>Uploaded content: %s</p>", global_buffer);
}

void handle_admin_panel(char* username, char* password) {
    // Format string vulnerability
    printf("<p>Admin panel access attempt for user: ");
    printf(username); // VULNERABILITY: format string
    printf("</p>");
    
    if (strcmp(username, ADMIN_USERNAME) == 0) {
        printf("<p>Admin access granted</p>");
        printf("<p>Database password: %s</p>", DB_PASSWORD);
        
        // Command injection vulnerability
        char command[512];
        sprintf(command, "ls -la /tmp/%s", username);
        printf("<p>Executing: %s</p>", command);
        
        // Execute command without proper sanitization
        system(command);
    }
}

void show_main_page() {
    printf("<h2>Available Actions:</h2>");
    printf("<ul>");
    printf("<li><a href='?action=login&username=admin&password=password123'>Login</a></li>");
    printf("<li><a href='?action=search&search=test'>Search Users</a></li>");
    printf("<li><a href='?action=upload'>File Upload</a></li>");
    printf("<li><a href='?action=admin&username=admin'>Admin Panel</a></li>");
    printf("</ul>");
    
    printf("<h2>Test SQL Injection:</h2>");
    printf("<p>Try: <a href='?action=search&search=' OR 1=1--'>' OR 1=1--</a></p>");
    printf("<p>Try: <a href='?action=search&search='; DROP TABLE users;--'>'; DROP TABLE users;--</a></p>");
}

int callback(void* data, int argc, char** argv, char** azColName) {
    printf("<tr>");
    for (int i = 0; i < argc; i++) {
        printf("<td>%s</td>", argv[i] ? argv[i] : "NULL");
    }
    printf("</tr>");
    return 0;
}
