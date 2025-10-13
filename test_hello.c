#include <stdio.h>
#include <string.h>

int main() {
    char message[] = "Hello, Phantron Obfuscator!";
    int length = strlen(message);
    
    printf("Message: %s\n", message);
    printf("Length: %d\n", length);
    
    for (int i = 0; i < length; i++) {
        printf("%c", message[i]);
    }
    printf("\n");
    
    return 0;
}
