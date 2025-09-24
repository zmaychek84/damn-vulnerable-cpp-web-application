# Vulnerable Web Application Implementation

A deliberately vulnerable C/C++ web application designed for testing SAST (Static Application Security Testing) tools, specifically targeting SAST SCA analysis.

## Completed Tasks

- [x] Create project directory structure and main files
- [x] Implement buffer overflows, use-after-free, and format string vulnerabilities
- [x] Create SQL injection vulnerabilities with SQLite database
- [x] Add hardcoded passwords and API keys
- [x] Add path traversal, command injection, and integer overflow vulnerabilities
- [x] Create Makefile with build instructions
- [x] Create README with build and deployment instructions

## In Progress Tasks

- [ ] Test the application build and functionality
- [ ] Verify all vulnerabilities are properly implemented

## Future Tasks

- [ ] Add more complex vulnerability patterns
- [ ] Create additional test cases
- [ ] Add performance testing capabilities

## Implementation Plan

The application was designed with multiple vulnerability categories to provide comprehensive testing for SAST tools:

### Memory-Based Vulnerabilities
- Buffer overflows (stack and heap)
- Use-after-free conditions
- Double free vulnerabilities
- Memory leaks
- Integer overflows
- Format string vulnerabilities
- Null pointer dereferences
- Uninitialized variable usage

### SQL Injection Vulnerabilities
- Basic SQL injection with string concatenation
- UNION-based injection attacks
- Time-based blind injection
- ORDER BY clause injection
- LIKE clause injection

### Hardcoded Credentials
- API keys and secrets
- Database passwords
- Encryption keys
- Admin credentials

### Other Security Issues
- Path traversal attacks
- Command injection
- Information disclosure
- Weak cryptography
- Race conditions
- Input validation issues
- Insecure random number generation
- Improper error handling
- Unvalidated redirects
- Cross-site scripting (XSS)
- Insecure file permissions

### Relevant Files

- `main.c` - Main CGI application with core vulnerabilities ✅
- `vulnerable_app.h` - Header file with function declarations ✅
- `memory_vulns.c` - Memory-based vulnerability implementations ✅
- `sql_injection.c` - SQL injection vulnerability implementations ✅
- `other_vulns.c` - Other security vulnerability implementations ✅
- `setup_database.c` - Database setup utility ✅
- `Makefile` - Build configuration and automation ✅
- `README.md` - Comprehensive documentation and instructions ✅

## Architecture Decisions

1. **CGI-based Design**: Chosen for simplicity and direct vulnerability exposure
2. **Multi-file Structure**: Separated vulnerabilities by category for better organization
3. **SQLite Database**: Lightweight database for SQL injection testing
4. **Makefile Build System**: Standard build system for easy compilation
5. **SAST Targeting**: Specific vulnerability patterns known to be detected by SAST SCA

## Data Flow

1. **HTTP Request** → CGI Environment Variables
2. **Query String Parsing** → Direct string operations (vulnerable)
3. **Database Operations** → SQLite with string concatenation (vulnerable)
4. **File Operations** → Direct path operations (vulnerable)
5. **System Commands** → Direct system() calls (vulnerable)
6. **Memory Operations** → Unsafe memory operations (vulnerable)

## Technical Components

- **Web Server**: Apache/Nginx with CGI support
- **Database**: SQLite3 for SQL injection testing
- **Compiler**: GCC with security-disabling flags
- **Build System**: Makefile with multiple targets
- **SAST Tool**: SAST SCA for static analysis

## Environment Configuration

- **Development**: Local build environment
- **Testing**: Web server with CGI support
- **Analysis**: SAST SCA installation required
- **Isolation**: Virtual machine recommended for testing
