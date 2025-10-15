/**
 * Test Case 3: Bit Manipulation
 * Tests: Bitwise operations, bit fields, bit counting, bit reversal, masks
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Bit field structure
typedef struct {
    unsigned int flag1 : 1;
    unsigned int flag2 : 1;
    unsigned int value : 6;
    unsigned int mode : 3;
    unsigned int reserved : 21;
} BitField;

// Count set bits (Brian Kernighan's algorithm)
int countSetBits(unsigned int n) {
    int count = 0;
    while (n) {
        n &= (n - 1);  // Clear least significant set bit
        count++;
    }
    return count;
}

// Check if a number is power of 2
int isPowerOf2(unsigned int n) {
    return n && !(n & (n - 1));
}

// Reverse bits of a 32-bit integer
uint32_t reverseBits(uint32_t n) {
    uint32_t result = 0;
    for (int i = 0; i < 32; i++) {
        result <<= 1;
        result |= (n & 1);
        n >>= 1;
    }
    return result;
}

// Get bit at position
int getBit(unsigned int num, int pos) {
    return (num >> pos) & 1;
}

// Set bit at position
unsigned int setBit(unsigned int num, int pos) {
    return num | (1 << pos);
}

// Clear bit at position
unsigned int clearBit(unsigned int num, int pos) {
    return num & ~(1 << pos);
}

// Toggle bit at position
unsigned int toggleBit(unsigned int num, int pos) {
    return num ^ (1 << pos);
}

// Find position of rightmost set bit
int positionOfRightmostSetBit(unsigned int n) {
    if (n == 0) return -1;
    int pos = 0;
    while ((n & 1) == 0) {
        n >>= 1;
        pos++;
    }
    return pos;
}

// Swap two numbers without using temp variable
void swapWithoutTemp(int* a, int* b) {
    if (a != b) {  // Check if not same address
        *a ^= *b;
        *b ^= *a;
        *a ^= *b;
    }
}

// Check if two integers have opposite signs
int oppositeSigns(int x, int y) {
    return (x ^ y) < 0;
}

// Add two numbers using bitwise operations
int addWithoutArithmetic(int a, int b) {
    while (b != 0) {
        int carry = a & b;
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}

// Print binary representation
void printBinary(unsigned int n) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i % 8 == 0) printf(" ");
    }
    printf("\n");
}

// Extract and combine using masks
void demonstrateMasks() {
    unsigned int data = 0xABCD1234;
    
    // Extract bytes
    unsigned char byte0 = data & 0xFF;
    unsigned char byte1 = (data >> 8) & 0xFF;
    unsigned char byte2 = (data >> 16) & 0xFF;
    unsigned char byte3 = (data >> 24) & 0xFF;
    
    printf("Original: 0x%08X\n", data);
    printf("Byte 0: 0x%02X, Byte 1: 0x%02X, Byte 2: 0x%02X, Byte 3: 0x%02X\n",
           byte0, byte1, byte2, byte3);
    
    // Combine bytes
    unsigned int combined = (byte3 << 24) | (byte2 << 16) | (byte1 << 8) | byte0;
    printf("Combined: 0x%08X\n", combined);
}

int main() {
    printf("=== Bit Manipulation Test ===\n");
    
    // 1. Bit counting
    printf("\n1. Bit counting:\n");
    unsigned int num = 0b10110101;
    printf("Number: %u (binary: ", num);
    printBinary(num);
    printf("Set bits: %d\n", countSetBits(num));
    
    // 2. Power of 2 check
    printf("\n2. Power of 2 check:\n");
    printf("16 is power of 2: %s\n", isPowerOf2(16) ? "Yes" : "No");
    printf("15 is power of 2: %s\n", isPowerOf2(15) ? "Yes" : "No");
    
    // 3. Bit operations
    printf("\n3. Bit operations:\n");
    unsigned int n = 0b10101100;
    printf("Original: ");
    printBinary(n);
    printf("Set bit 1: ");
    printBinary(setBit(n, 1));
    printf("Clear bit 3: ");
    printBinary(clearBit(n, 3));
    printf("Toggle bit 2: ");
    printBinary(toggleBit(n, 2));
    
    // 4. Bit reversal
    printf("\n4. Bit reversal:\n");
    uint32_t val = 0x12345678;
    printf("Original:  0x%08X\n", val);
    printf("Reversed:  0x%08X\n", reverseBits(val));
    
    // 5. Bit fields
    printf("\n5. Bit fields:\n");
    BitField bf;
    memset(&bf, 0, sizeof(bf));
    bf.flag1 = 1;
    bf.flag2 = 0;
    bf.value = 42;
    bf.mode = 5;
    printf("flag1: %u, flag2: %u, value: %u, mode: %u\n",
           bf.flag1, bf.flag2, bf.value, bf.mode);
    
    // 6. Swap without temp
    printf("\n6. Swap without temp variable:\n");
    int a = 25, b = 50;
    printf("Before: a=%d, b=%d\n", a, b);
    swapWithoutTemp(&a, &b);
    printf("After:  a=%d, b=%d\n", a, b);
    
    // 7. Opposite signs
    printf("\n7. Opposite signs:\n");
    printf("-10 and 20 have opposite signs: %s\n", oppositeSigns(-10, 20) ? "Yes" : "No");
    printf("10 and 20 have opposite signs: %s\n", oppositeSigns(10, 20) ? "Yes" : "No");
    
    // 8. Addition using bitwise
    printf("\n8. Addition using bitwise operations:\n");
    int x = 15, y = 27;
    printf("%d + %d = %d\n", x, y, addWithoutArithmetic(x, y));
    
    // 9. Masks
    printf("\n9. Byte extraction and combination:\n");
    demonstrateMasks();
    
    // 10. Rightmost set bit
    printf("\n10. Rightmost set bit position:\n");
    printf("Position in %u: %d\n", 40, positionOfRightmostSetBit(40));
    
    printf("\n=== Bit Manipulation Test Complete ===\n");
    return 0;
}
