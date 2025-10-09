/**
 * Test Case 8: Floating Point and Mathematical Operations
 * Tests: Trigonometry, complex numbers, numerical methods, precision
 */

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <complex.h>

#define PI 3.14159265358979323846
#define EPSILON 1e-10

// Complex number structure (custom)
typedef struct {
    double real;
    double imag;
} Complex;

// Complex arithmetic
Complex complex_add(Complex a, Complex b) {
    Complex result;
    result.real = a.real + b.real;
    result.imag = a.imag + b.imag;
    return result;
}

Complex complex_multiply(Complex a, Complex b) {
    Complex result;
    result.real = a.real * b.real - a.imag * b.imag;
    result.imag = a.real * b.imag + a.imag * b.real;
    return result;
}

double complex_magnitude(Complex c) {
    return sqrt(c.real * c.real + c.imag * c.imag);
}

// Newton-Raphson method for square root
double newton_sqrt(double n) {
    if (n < 0) return NAN;
    if (n == 0) return 0;
    
    double x = n;
    double prev;
    int iterations = 0;
    
    do {
        prev = x;
        x = (x + n / x) / 2.0;
        iterations++;
    } while (fabs(x - prev) > EPSILON && iterations < 100);
    
    return x;
}

// Numerical integration using Simpson's rule
double simpsons_rule(double (*f)(double), double a, double b, int n) {
    double h = (b - a) / n;
    double sum = f(a) + f(b);
    
    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        sum += (i % 2 == 0) ? 2 * f(x) : 4 * f(x);
    }
    
    return sum * h / 3.0;
}

// Test function for integration
double test_function(double x) {
    return x * x;
}

// Matrix operations
void matrix_multiply(double a[][3], double b[][3], double result[][3], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = 0;
            for (int k = 0; k < n; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void matrix_print(double m[][3], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%8.3f ", m[i][j]);
        }
        printf("\n");
    }
}

// Taylor series for sine (approximation)
double taylor_sin(double x) {
    double term = x;
    double sum = term;
    int n = 1;
    
    while (fabs(term) > EPSILON && n < 20) {
        term *= -x * x / ((2 * n) * (2 * n + 1));
        sum += term;
        n++;
    }
    
    return sum;
}

// Euler's method for differential equations
void euler_method(double (*f)(double, double), double y0, double x0, 
                  double xn, double h) {
    double x = x0;
    double y = y0;
    
    printf("Euler's Method (dy/dx = x + y):\n");
    printf("x = %.2f, y = %.6f\n", x, y);
    
    while (x < xn) {
        y = y + h * f(x, y);
        x = x + h;
        printf("x = %.2f, y = %.6f\n", x, y);
    }
}

double diff_eq(double x, double y) {
    return x + y;
}

// Factorial using recursion (for precision testing)
double factorial_recursive(int n) {
    if (n <= 1) return 1.0;
    return n * factorial_recursive(n - 1);
}

// Fast exponentiation
double fast_power(double base, int exp) {
    if (exp == 0) return 1.0;
    if (exp < 0) return 1.0 / fast_power(base, -exp);
    
    double half = fast_power(base, exp / 2);
    if (exp % 2 == 0) {
        return half * half;
    } else {
        return base * half * half;
    }
}

// Check floating point precision
void test_precision() {
    printf("Floating Point Precision Test:\n");
    printf("FLT_EPSILON: %e\n", FLT_EPSILON);
    printf("DBL_EPSILON: %e\n", DBL_EPSILON);
    
    float f1 = 0.1f;
    float f2 = 0.2f;
    float f3 = 0.3f;
    
    printf("0.1 + 0.2 = %.20f\n", f1 + f2);
    printf("0.3 = %.20f\n", f3);
    printf("Equal? %s\n", (fabs((f1 + f2) - f3) < FLT_EPSILON) ? "Yes (within epsilon)" : "No");
}

// Dot product
double dot_product(double* v1, double* v2, int n) {
    double result = 0.0;
    for (int i = 0; i < n; i++) {
        result += v1[i] * v2[i];
    }
    return result;
}

// Cross product (3D)
void cross_product(double* a, double* b, double* result) {
    result[0] = a[1] * b[2] - a[2] * b[1];
    result[1] = a[2] * b[0] - a[0] * b[2];
    result[2] = a[0] * b[1] - a[1] * b[0];
}

int main() {
    printf("=== Floating Point and Math Operations Test ===\n");
    
    // 1. Trigonometric functions
    printf("\n1. Trigonometric Functions:\n");
    double angle = PI / 4;  // 45 degrees
    printf("sin(π/4) = %.10f\n", sin(angle));
    printf("cos(π/4) = %.10f\n", cos(angle));
    printf("tan(π/4) = %.10f\n", tan(angle));
    printf("Taylor sin(π/4) = %.10f\n", taylor_sin(angle));
    
    // 2. Complex numbers
    printf("\n2. Complex Number Operations:\n");
    Complex c1 = {3.0, 4.0};
    Complex c2 = {1.0, 2.0};
    Complex sum = complex_add(c1, c2);
    Complex product = complex_multiply(c1, c2);
    printf("(3+4i) + (1+2i) = %.2f + %.2fi\n", sum.real, sum.imag);
    printf("(3+4i) * (1+2i) = %.2f + %.2fi\n", product.real, product.imag);
    printf("|3+4i| = %.2f\n", complex_magnitude(c1));
    
    // 3. Newton-Raphson square root
    printf("\n3. Newton-Raphson Square Root:\n");
    double num = 25.0;
    printf("sqrt(%.0f) = %.10f (Newton)\n", num, newton_sqrt(num));
    printf("sqrt(%.0f) = %.10f (library)\n", num, sqrt(num));
    
    // 4. Numerical integration
    printf("\n4. Simpson's Rule Integration:\n");
    double integral = simpsons_rule(test_function, 0, 10, 100);
    printf("∫(x²)dx from 0 to 10 = %.6f\n", integral);
    printf("Analytical result = %.6f\n", 1000.0 / 3.0);
    
    // 5. Matrix operations
    printf("\n5. Matrix Multiplication:\n");
    double m1[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    double m2[3][3] = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    double result[3][3];
    matrix_multiply(m1, m2, result, 3);
    printf("Result:\n");
    matrix_print(result, 3);
    
    // 6. Euler's method
    printf("\n6. Euler's Method for Differential Equations:\n");
    euler_method(diff_eq, 1.0, 0.0, 0.5, 0.1);
    
    // 7. Floating point precision
    printf("\n7. Floating Point Precision:\n");
    test_precision();
    
    // 8. Fast exponentiation
    printf("\n8. Fast Exponentiation:\n");
    printf("2^10 = %.0f\n", fast_power(2.0, 10));
    printf("3^5 = %.0f\n", fast_power(3.0, 5));
    printf("2^-3 = %.6f\n", fast_power(2.0, -3));
    
    // 9. Vector operations
    printf("\n9. Vector Operations:\n");
    double v1[] = {1.0, 2.0, 3.0};
    double v2[] = {4.0, 5.0, 6.0};
    double cross[3];
    printf("v1 · v2 = %.2f\n", dot_product(v1, v2, 3));
    cross_product(v1, v2, cross);
    printf("v1 × v2 = (%.2f, %.2f, %.2f)\n", cross[0], cross[1], cross[2]);
    
    // 10. Logarithms and exponentials
    printf("\n10. Logarithms and Exponentials:\n");
    printf("e = %.10f\n", exp(1.0));
    printf("ln(e) = %.10f\n", log(exp(1.0)));
    printf("log10(100) = %.2f\n", log10(100.0));
    printf("e^2 = %.6f\n", exp(2.0));
    
    printf("\n=== Floating Point and Math Test Complete ===\n");
    return 0;
}
