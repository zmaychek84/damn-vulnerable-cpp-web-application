# Vulnerable Web Application for SAST Testing

This is a deliberately vulnerable C/C++ web application designed to test Static Application Security Testing (SAST) tools, specifically targeting SAST SCA analysis.

## ⚠️ WARNING

**This application contains intentional security vulnerabilities and should NEVER be deployed in a production environment. It is designed solely for educational and testing purposes.**

## Vulnerabilities Included

### Memory-Based Vulnerabilities
- **Buffer Overflows**: Stack and heap buffer overflows
- **Use-After-Free**: Accessing freed memory
- **Double Free**: Freeing the same memory twice
- **Memory Leaks**: Allocated memory not freed
- **Integer Overflows**: Arithmetic operations causing overflow
- **Format String Vulnerabilities**: User input as format string
- **Null Pointer Dereference**: Accessing null pointers
- **Uninitialized Variables**: Using uninitialized memory

### SQL Injection Vulnerabilities
- **Basic SQL Injection**: Direct string concatenation in queries
- **UNION-based Injection**: Extracting data from other tables
- **Time-based Blind Injection**: Delayed responses for data extraction
- **ORDER BY Injection**: SQL injection in ORDER BY clauses
- **LIKE Clause Injection**: SQL injection in search functionality

### Hardcoded Credentials
- **API Keys**: Hardcoded AWS keys, JWT secrets
- **Database Passwords**: Hardcoded database credentials
- **Encryption Keys**: Weak and hardcoded encryption keys
- **Admin Credentials**: Hardcoded admin username/password

### Other Security Issues
- **Path Traversal**: Directory traversal attacks
- **Command Injection**: System command execution
- **Information Disclosure**: Exposing internal paths and versions
- **Weak Cryptography**: MD5 hashing, weak encryption
- **Race Conditions**: Time-of-check to time-of-use vulnerabilities
- **Input Validation**: Missing input validation and sanitization
- **Insecure Random**: Predictable random number generation
- **Improper Error Handling**: Information leakage through errors
- **Unvalidated Redirects**: Open redirect vulnerabilities
- **Cross-Site Scripting (XSS)**: Output encoding issues
- **Insecure File Permissions**: World-writable files

## Project Structure

```
SAST_demo/
├── main.c                 # Main CGI application
├── vulnerable_app.h       # Header file with declarations
├── memory_vulns.c         # Memory-based vulnerabilities
├── sql_injection.c        # SQL injection vulnerabilities
├── other_vulns.c          # Other security vulnerabilities
├── setup_database.c       # Database setup utility
├── Makefile              # Build configuration
└── README.md             # This file
```

## Prerequisites

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install -y gcc sqlite3 libsqlite3-dev apache2
```

### CentOS/RHEL
```bash
sudo yum install -y gcc sqlite3 sqlite3-devel httpd
```

### macOS
```bash
brew install sqlite3
```

## Build Instructions

### 1. Build the Application
```bash
# Build the main application
make all

# Or build step by step
make vulnerable_app
make setup_db
```

### 2. Install Dependencies (if needed)
```bash
# For Ubuntu/Debian
make install-deps

# For CentOS/RHEL
make install-deps-rhel

# For macOS
make install-deps-macos
```

### 3. Setup Web Server

#### Apache Setup
```bash
# Setup Apache for CGI
make setup-apache

# Access the application at:
# http://localhost/cgi-bin/vulnerable_app
```

#### Nginx Setup (Alternative)
```bash
# Setup Nginx for CGI
make setup-nginx
```

### 4. Create Test Environment
```bash
# Create test directories
make setup-test-env
```

## Running SAST SAST Analysis

### Prerequisites
- SAST SCA (Source Code Analyzer) must be installed
- SAST SCA must be in your PATH

### Run Analysis
```bash
# Run SAST SAST scan
make SAST-scan

# This will generate: vulnerable_app.fpr
```

### Manual SAST Commands
```bash
# Clean previous build
sourceanalyzer -b vulnerable_app -clean

# Build with SAST
sourceanalyzer -b vulnerable_app *.c *.h

# Run scan
sourceanalyzer -b vulnerable_app -scan -f vulnerable_app.fpr
```

## Testing Vulnerabilities

### 1. Create Test Script
```bash
make create-test-script
```

### 2. Manual Testing

#### SQL Injection Tests
```bash
# Basic SQL injection
curl "http://localhost/cgi-bin/vulnerable_app?action=search&search=' OR 1=1--"

# UNION-based injection
curl "http://localhost/cgi-bin/vulnerable_app?action=search&search=' UNION SELECT 1,2,3--"

# Time-based blind injection
curl "http://localhost/cgi-bin/vulnerable_app?action=search&search='; SELECT SLEEP(5)--"
```

#### Path Traversal Tests
```bash
# Directory traversal
curl "http://localhost/cgi-bin/vulnerable_app?action=file&filename=../../../etc/passwd"
```

#### Command Injection Tests
```bash
# Command injection
curl "http://localhost/cgi-bin/vulnerable_app?action=ping&host=127.0.0.1; cat /etc/passwd"
```

## Expected SAST Findings

When running SAST SCA on this codebase, you should expect to find:

1. **Buffer Overflow** - Multiple instances in `memory_vulns.c`
2. **SQL Injection** - Multiple instances in `sql_injection.c` and `main.c`
3. **Hardcoded Password** - Multiple instances across all files
4. **Use After Free** - In `memory_vulns.c` and `main.c`
5. **Format String** - In `memory_vulns.c` and `main.c`
6. **Path Manipulation** - In `other_vulns.c`
7. **Command Injection** - In `other_vulns.c`
8. **Information Exposure** - In `other_vulns.c`
9. **Weak Cryptography** - In `other_vulns.c`
10. **Race Condition** - In `other_vulns.c`

## Debug Build

For enhanced vulnerability detection:
```bash
# Build with debug flags and sanitizers
make debug-build
```

## Cleanup

```bash
# Remove build artifacts
make clean

# Remove all generated files
make distclean
```

## Security Notes

- This application is intentionally vulnerable
- Never deploy in production environments
- Use only for educational and testing purposes
- Ensure proper isolation when testing
- Consider using virtual machines for testing

## Troubleshooting

### Common Issues

1. **Permission Denied**: Ensure CGI binary has execute permissions
   ```bash
   sudo chmod +x /usr/lib/cgi-bin/vulnerable_app
   ```

2. **Database Not Found**: Run database setup
   ```bash
   make setup_db
   ```

3. **SAST Not Found**: Ensure SAST SCA is installed and in PATH
   ```bash
   which sourceanalyzer
   ```

4. **Apache CGI Not Working**: Check Apache configuration
   ```bash
   sudo a2enmod cgi
   sudo systemctl restart apache2
   ```

## Contributing

This is a test application. If you find additional vulnerability patterns that should be included, please consider the educational value and ensure they are clearly documented as intentional vulnerabilities.

## License

This project is for educational purposes only. Use at your own risk.
