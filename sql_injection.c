#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "vulnerable_app.h"

// SQL Injection vulnerabilities
void sql_injection_login(char* username, char* password) {
    sqlite3* db;
    char* err_msg = 0;
    int rc;
    
    rc = sqlite3_open("vulnerable.db", &db);
    if (rc != SQLITE_OK) {
        printf("Database error: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    // SQL Injection - direct string concatenation
    char sql[1024];
    sprintf(sql, "SELECT * FROM users WHERE username='%s' AND password='%s'", 
            username, password);
    
    printf("Executing: %s\n", sql);
    
    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    
    sqlite3_close(db);
}

void sql_injection_search(char* search_term) {
    sqlite3* db;
    char* err_msg = 0;
    int rc;
    
    rc = sqlite3_open("vulnerable.db", &db);
    if (rc != SQLITE_OK) {
        printf("Database error: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    // SQL Injection with LIKE clause
    char sql[1024];
    sprintf(sql, "SELECT * FROM products WHERE name LIKE '%%%s%%'", search_term);
    
    printf("Search query: %s\n", sql);
    
    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    
    sqlite3_close(db);
}

void sql_injection_order_by(char* column) {
    sqlite3* db;
    char* err_msg = 0;
    int rc;
    
    rc = sqlite3_open("vulnerable.db", &db);
    if (rc != SQLITE_OK) {
        printf("Database error: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    // SQL Injection in ORDER BY clause
    char sql[1024];
    sprintf(sql, "SELECT * FROM users ORDER BY %s", column);
    
    printf("Order by query: %s\n", sql);
    
    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    
    sqlite3_close(db);
}

void sql_injection_union(char* id) {
    sqlite3* db;
    char* err_msg = 0;
    int rc;
    
    rc = sqlite3_open("vulnerable.db", &db);
    if (rc != SQLITE_OK) {
        printf("Database error: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    // SQL Injection with UNION attack
    char sql[1024];
    sprintf(sql, "SELECT name, email FROM users WHERE id=%s", id);
    
    printf("Union query: %s\n", sql);
    
    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    
    sqlite3_close(db);
}

void sql_injection_time_based(char* id) {
    sqlite3* db;
    char* err_msg = 0;
    int rc;
    
    rc = sqlite3_open("vulnerable.db", &db);
    if (rc != SQLITE_OK) {
        printf("Database error: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    // Time-based SQL injection
    char sql[1024];
    sprintf(sql, "SELECT * FROM users WHERE id=%s", id);
    
    printf("Time-based query: %s\n", sql);
    
    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    
    sqlite3_close(db);
}

// Create vulnerable database
void create_vulnerable_database() {
    sqlite3* db;
    char* err_msg = 0;
    int rc;
    
    rc = sqlite3_open("vulnerable.db", &db);
    if (rc != SQLITE_OK) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    // Create tables
    const char* create_users = 
        "CREATE TABLE IF NOT EXISTS users("
        "id INTEGER PRIMARY KEY,"
        "username TEXT,"
        "password TEXT,"
        "email TEXT,"
        "role TEXT"
        ");";
    
    const char* create_products = 
        "CREATE TABLE IF NOT EXISTS products("
        "id INTEGER PRIMARY KEY,"
        "name TEXT,"
        "price REAL,"
        "description TEXT"
        ");";
    
    rc = sqlite3_exec(db, create_users, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    
    rc = sqlite3_exec(db, create_products, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    
    // Insert test data
    const char* insert_users = 
        "INSERT OR REPLACE INTO users VALUES"
        "(1, 'admin', 'password123', 'admin@example.com', 'admin'),"
        "(2, 'user1', 'userpass', 'user1@example.com', 'user'),"
        "(3, 'test', 'testpass', 'test@example.com', 'user');";
    
    const char* insert_products = 
        "INSERT OR REPLACE INTO products VALUES"
        "(1, 'Product A', 19.99, 'Description A'),"
        "(2, 'Product B', 29.99, 'Description B'),"
        "(3, 'Test Product', 9.99, 'Test Description');";
    
    rc = sqlite3_exec(db, insert_users, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    
    rc = sqlite3_exec(db, insert_products, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    
    sqlite3_close(db);
    printf("Vulnerable database created successfully\n");
}
