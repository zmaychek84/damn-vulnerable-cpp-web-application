#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include "vulnerable_app.h"

// Path traversal vulnerability
void path_traversal_vuln(char* filename) {
    char filepath[512];
    
    // Path traversal - no validation of filename
    sprintf(filepath, "/var/www/uploads/%s", filename);
    
    printf("Attempting to access: %s\n", filepath);
    
    FILE* file = fopen(filepath, "r");
    if (file) {
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), file)) {
            printf("%s", buffer);
        }
        fclose(file);
    } else {
        printf("File not found or access denied\n");
    }
}

// Command injection vulnerability
void command_injection_vuln(char* user_input) {
    char command[1024];
    
    // Command injection - direct user input in system command
    sprintf(command, "ping -c 1 %s", user_input);
    
    printf("Executing command: %s\n", command);
    system(command); // VULNERABILITY: command injection
}

void command_injection_file(char* filename) {
    char command[1024];
    
    // Command injection with file operations
    sprintf(command, "cat /tmp/%s", filename);
    
    printf("File command: %s\n", command);
    system(command); // VULNERABILITY
}

// Hardcoded credentials and secrets
void hardcoded_secrets() {
    // Hardcoded API keys
    const char* aws_key = "AKIAIOSFODNN7EXAMPLE";
    const char* aws_secret = "wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY";
    const char* jwt_secret = "mySecretKey123";
    const char* db_password = "superSecretDBPassword";
    
    // Hardcoded encryption keys
    const char* aes_key = "0123456789abcdef0123456789abcdef";
    const char* rsa_private = "-----BEGIN RSA PRIVATE KEY-----\nMIIEpAIBAAKCAQEA...";
    
    printf("AWS Key: %s\n", aws_key);
    printf("AWS Secret: %s\n", aws_secret);
    printf("JWT Secret: %s\n", jwt_secret);
    printf("DB Password: %s\n", db_password);
    printf("AES Key: %s\n", aes_key);
    printf("RSA Private: %s\n", rsa_private);
}

// Weak cryptography
void weak_crypto() {
    // Weak hash function
    char password[] = "userpassword";
    char hash[33];
    
    // Using weak MD5 hash
    sprintf(hash, "5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8");
    
    printf("Password hash (MD5): %s\n", hash);
    
    // Weak encryption key
    char weak_key[] = "123456";
    printf("Encryption key: %s\n", weak_key);
}

// Information disclosure
void information_disclosure() {
    // Exposing internal paths
    printf("Server root: /var/www/html\n");
    printf("Config file: /etc/vulnerable_app.conf\n");
    printf("Log file: /var/log/vulnerable_app.log\n");
    printf("Database file: /var/lib/vulnerable_app.db\n");
    
    // Exposing version information
    printf("Application version: 1.0.0\n");
    printf("Build date: 2024-01-01\n");
    printf("Compiler: gcc 9.4.0\n");
    
    // Exposing internal error details
    printf("Debug mode: enabled\n");
    printf("Stack trace: enabled\n");
}

// Race condition vulnerability
void race_condition_vuln(char* filename) {
    // Race condition in file operations
    if (access(filename, F_OK) == 0) {
        // Time-of-check to time-of-use vulnerability
        sleep(1); // Simulate delay
        FILE* file = fopen(filename, "w");
        if (file) {
            fprintf(file, "Modified by race condition\n");
            fclose(file);
        }
    }
}

// Improper input validation
void input_validation_vuln(char* input) {
    // No input validation
    char buffer[100];
    
    // Direct copy without validation
    strcpy(buffer, input); // VULNERABILITY: no bounds checking
    
    // No sanitization of special characters
    printf("Processed input: %s\n", buffer);
}

// Insecure random number generation
void insecure_random() {
    // Using predictable random seed
    srand(12345); // VULNERABILITY: predictable seed
    
    int random_num = rand();
    printf("Random number: %d\n", random_num);
    
    // Using time as seed (predictable)
    srand(time(NULL));
    random_num = rand();
    printf("Time-based random: %d\n", random_num);
}

// Improper error handling
void improper_error_handling(char* filename) {
    FILE* file = fopen(filename, "r");
    
    // Not checking return value properly
    if (file) {
        char buffer[1024];
        fgets(buffer, sizeof(buffer), file);
        printf("File content: %s\n", buffer);
    }
    
    // Not closing file on error path
    // fclose(file); // Missing in error case
}

// Unvalidated redirect
void unvalidated_redirect(char* url) {
    // No validation of redirect URL
    printf("Location: %s\r\n", url); // VULNERABILITY: open redirect
    printf("Content-Type: text/html\r\n\r\n");
    printf("<html><body>Redirecting...</body></html>");
}

// Cross-site scripting (XSS) in CGI context
void xss_vuln(char* user_input) {
    // No output encoding
    printf("User input: %s\n", user_input); // VULNERABILITY: XSS
}

// Insecure file permissions
void insecure_file_permissions() {
    // Creating file with world-writable permissions
    FILE* file = fopen("/tmp/insecure_file.txt", "w");
    if (file) {
        fprintf(file, "Sensitive data\n");
        fclose(file);
        
        // Set insecure permissions
        chmod("/tmp/insecure_file.txt", 0666); // VULNERABILITY: world-writable
    }
}
