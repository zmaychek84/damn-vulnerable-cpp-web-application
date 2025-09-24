#ifndef VULNERABLE_APP_H
#define VULNERABLE_APP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sqlite3.h>

// Function declarations
void handle_request(char* query_string, char* method);
void handle_login(char* username, char* password);
void handle_search(char* search_term);
void handle_file_upload();
void handle_admin_panel(char* username, char* password);
void show_main_page();
int callback(void* data, int argc, char** argv, char** azColName);

// Global variables for vulnerabilities
extern char* global_buffer;
extern int buffer_size;

#endif
