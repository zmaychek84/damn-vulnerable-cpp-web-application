# Makefile for Vulnerable Web Application

CC = gcc
CFLAGS = -Wall -Wextra -g -O0 -fno-stack-protector -z execstack
LDFLAGS = -lsqlite3
TARGET = vulnerable_app
SOURCES = main.c memory_vulns.c sql_injection.c other_vulns.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = vulnerable_app.h

# Default target
all: $(TARGET) setup_db

# Build the main executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

# Compile source files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Create vulnerable database
setup_db: $(TARGET)
	@echo "Setting up vulnerable database..."
	./$(TARGET) --setup-db || echo "Database setup completed"

# Install dependencies (Ubuntu/Debian)
install-deps:
	@echo "Installing dependencies..."
	sudo apt-get update
	sudo apt-get install -y gcc sqlite3 libsqlite3-dev apache2

# Install dependencies (CentOS/RHEL)
install-deps-rhel:
	@echo "Installing dependencies..."
	sudo yum install -y gcc sqlite3 sqlite3-devel httpd

# Install dependencies (macOS)
install-deps-macos:
	@echo "Installing dependencies..."
	brew install sqlite3

# Setup Apache for CGI
setup-apache:
	@echo "Setting up Apache for CGI..."
	sudo cp $(TARGET) /usr/lib/cgi-bin/
	sudo chmod +x /usr/lib/cgi-bin/$(TARGET)
	sudo chown www-data:www-data /usr/lib/cgi-bin/$(TARGET)
	@echo "Apache CGI setup complete"
	@echo "Access at: http://localhost/cgi-bin/$(TARGET)"

# Setup Nginx for CGI (alternative)
setup-nginx:
	@echo "Setting up Nginx for CGI..."
	sudo cp $(TARGET) /usr/lib/cgi-bin/
	sudo chmod +x /usr/lib/cgi-bin/$(TARGET)
	sudo chown www-data:www-data /usr/lib/cgi-bin/$(TARGET)
	@echo "Nginx CGI setup complete"

# Create test directory structure
setup-test-env:
	@echo "Creating test environment..."
	mkdir -p /tmp/uploads
	mkdir -p /var/www/uploads
	sudo chmod 777 /tmp/uploads
	sudo chmod 777 /var/www/uploads
	@echo "Test environment created"

# Run SAST SAST analysis
SAST-scan:
	@echo "Running SAST SAST analysis..."
	@if command -v sourceanalyzer >/dev/null 2>&1; then \
		sourceanalyzer -b vulnerable_app -clean; \
		sourceanalyzer -b vulnerable_app $(SOURCES) $(HEADERS); \
		sourceanalyzer -b vulnerable_app -scan -f vulnerable_app.fpr; \
		echo "SAST scan complete: vulnerable_app.fpr"; \
	else \
		echo "SAST SCA not found. Please install SAST SCA first."; \
	fi

# Run with debug flags for better SAST detection
debug-build: CFLAGS += -DDEBUG -fsanitize=address -fsanitize=undefined
debug-build: $(TARGET)

# Create a simple test script
create-test-script:
	@echo "Creating test script..."
	@echo '#!/bin/bash' > test_vulnerabilities.sh
	@echo 'echo "Testing Vulnerable Web Application"' >> test_vulnerabilities.sh
	@echo 'echo "=================================="' >> test_vulnerabilities.sh
	@echo '' >> test_vulnerabilities.sh
	@echo '# Test SQL Injection' >> test_vulnerabilities.sh
	@echo 'echo "Testing SQL Injection..."' >> test_vulnerabilities.sh
	@echo 'curl "http://localhost/cgi-bin/vulnerable_app?action=search&search='\'' OR 1=1--"' >> test_vulnerabilities.sh
	@echo '' >> test_vulnerabilities.sh
	@echo '# Test Path Traversal' >> test_vulnerabilities.sh
	@echo 'echo "Testing Path Traversal..."' >> test_vulnerabilities.sh
	@echo 'curl "http://localhost/cgi-bin/vulnerable_app?action=file&filename=../../../etc/passwd"' >> test_vulnerabilities.sh
	@echo '' >> test_vulnerabilities.sh
	@echo '# Test Command Injection' >> test_vulnerabilities.sh
	@echo 'echo "Testing Command Injection..."' >> test_vulnerabilities.sh
	@echo 'curl "http://localhost/cgi-bin/vulnerable_app?action=ping&host=127.0.0.1; cat /etc/passwd"' >> test_vulnerabilities.sh
	@echo '' >> test_vulnerabilities.sh
	@echo 'echo "Test completed"' >> test_vulnerabilities.sh
	@chmod +x test_vulnerabilities.sh

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET) vulnerable.db
	@echo "Clean complete"

# Deep clean
distclean: clean
	rm -f *.fpr *.log
	sudo rm -f /usr/lib/cgi-bin/$(TARGET)
	@echo "Deep clean complete"

# Show help
help:
	@echo "Available targets:"
	@echo "  all              - Build the application and setup database"
	@echo "  $(TARGET)        - Build the main executable"
	@echo "  setup_db         - Create vulnerable database"
	@echo "  install-deps     - Install dependencies (Ubuntu/Debian)"
	@echo "  install-deps-rhel- Install dependencies (CentOS/RHEL)"
	@echo "  install-deps-macos- Install dependencies (macOS)"
	@echo "  setup-apache     - Setup Apache for CGI"
	@echo "  setup-nginx      - Setup Nginx for CGI"
	@echo "  setup-test-env   - Create test directory structure"
	@echo "  SAST-scan     - Run SAST SAST analysis"
	@echo "  debug-build      - Build with debug and sanitizer flags"
	@echo "  create-test-script- Create test script for vulnerabilities"
	@echo "  clean            - Remove build artifacts"
	@echo "  distclean        - Remove all generated files"
	@echo "  help             - Show this help message"

# Phony targets
.PHONY: all setup_db install-deps install-deps-rhel install-deps-macos setup-apache setup-nginx setup-test-env SAST-scan debug-build create-test-script clean distclean help
