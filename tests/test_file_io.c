/**
 * Test Case 2: File I/O Operations
 * Tests: fopen, fwrite, fread, fseek, ftell, file handling, error checking
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256
#define TEST_FILE "/tmp/test_file_io.txt"
#define BINARY_FILE "/tmp/test_binary.dat"

typedef struct {
    int id;
    char name[50];
    double score;
} Student;

// Write text to file
int writeTextFile(const char* filename, const char* content) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        perror("Error opening file for writing");
        return -1;
    }
    
    size_t written = fwrite(content, 1, strlen(content), fp);
    fclose(fp);
    
    return (written == strlen(content)) ? 0 : -1;
}

// Read text from file
char* readTextFile(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file for reading");
        return NULL;
    }
    
    // Get file size
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    char* buffer = (char*)malloc(size + 1);
    if (!buffer) {
        fclose(fp);
        return NULL;
    }
    
    size_t read = fread(buffer, 1, size, fp);
    buffer[read] = '\0';
    fclose(fp);
    
    return buffer;
}

// Write binary data
int writeBinaryFile(const char* filename, Student* students, int count) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        perror("Error opening binary file for writing");
        return -1;
    }
    
    size_t written = fwrite(students, sizeof(Student), count, fp);
    fclose(fp);
    
    return (written == (size_t)count) ? 0 : -1;
}

// Read binary data
int readBinaryFile(const char* filename, Student* students, int max_count) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        perror("Error opening binary file for reading");
        return -1;
    }
    
    int count = fread(students, sizeof(Student), max_count, fp);
    fclose(fp);
    
    return count;
}

// Append to file
int appendToFile(const char* filename, const char* content) {
    FILE* fp = fopen(filename, "a");
    if (!fp) {
        perror("Error opening file for appending");
        return -1;
    }
    
    fprintf(fp, "%s", content);
    fclose(fp);
    
    return 0;
}

// Line-by-line reading
void readLineByLine(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file");
        return;
    }
    
    char line[BUFFER_SIZE];
    int line_num = 1;
    
    printf("File contents (line by line):\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("  Line %d: %s", line_num++, line);
    }
    
    fclose(fp);
}

int main() {
    printf("=== File I/O Test ===\n");
    
    // 1. Text file writing and reading
    printf("\n1. Text file operations:\n");
    const char* text = "Hello, File I/O!\nThis is a test file.\nLine 3 here.\n";
    
    if (writeTextFile(TEST_FILE, text) == 0) {
        printf("Text file written successfully\n");
    }
    
    char* content = readTextFile(TEST_FILE);
    if (content) {
        printf("Read from file:\n%s", content);
        free(content);
    }
    
    // 2. Append to file
    printf("\n2. Appending to file:\n");
    appendToFile(TEST_FILE, "Appended line 1\n");
    appendToFile(TEST_FILE, "Appended line 2\n");
    
    readLineByLine(TEST_FILE);
    
    // 3. Binary file operations
    printf("\n3. Binary file operations:\n");
    Student students[3] = {
        {1, "Alice Johnson", 95.5},
        {2, "Bob Smith", 87.3},
        {3, "Charlie Brown", 92.1}
    };
    
    if (writeBinaryFile(BINARY_FILE, students, 3) == 0) {
        printf("Binary data written successfully\n");
    }
    
    Student read_students[3];
    int count = readBinaryFile(BINARY_FILE, read_students, 3);
    
    printf("Read %d students from binary file:\n", count);
    for (int i = 0; i < count; i++) {
        printf("  ID: %d, Name: %s, Score: %.1f\n",
               read_students[i].id, read_students[i].name, read_students[i].score);
    }
    
    // 4. File positioning (fseek, ftell)
    printf("\n4. File positioning:\n");
    FILE* fp = fopen(TEST_FILE, "r");
    if (fp) {
        fseek(fp, 10, SEEK_SET);
        printf("Position after seek: %ld\n", ftell(fp));
        
        char buffer[20];
        fgets(buffer, sizeof(buffer), fp);
        printf("Read from position 10: %s\n", buffer);
        
        fclose(fp);
    }
    
    // 5. Error handling test
    printf("\n5. Error handling:\n");
    FILE* invalid = fopen("/invalid/path/file.txt", "r");
    if (!invalid) {
        printf("Correctly handled invalid file path\n");
    }
    
    // Cleanup
    remove(TEST_FILE);
    remove(BINARY_FILE);
    
    printf("\n=== File I/O Test Complete ===\n");
    return 0;
}
