/**
 * Test Case 1: Advanced C++ - OOP, Templates, STL, Inheritance
 * Tests: Classes, templates, STL containers, polymorphism, operator overloading
 */

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include <string>

// Template class with operator overloading
template<typename T>
class Vector2D {
private:
    T x, y;
public:
    Vector2D(T x = 0, T y = 0) : x(x), y(y) {}
    
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }
    
    Vector2D operator*(T scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }
    
    T dot(const Vector2D& other) const {
        return x * other.x + y * other.y;
    }
    
    void print() const {
        std::cout << "(" << x << ", " << y << ")";
    }
};

// Abstract base class
class Shape {
protected:
    std::string name;
public:
    Shape(const std::string& n) : name(n) {}
    virtual ~Shape() {}
    virtual double area() const = 0;
    virtual void display() const {
        std::cout << "Shape: " << name << ", Area: " << area() << std::endl;
    }
};

// Derived classes
class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) : Shape("Circle"), radius(r) {}
    double area() const override {
        return 3.14159 * radius * radius;
    }
};

class Rectangle : public Shape {
private:
    double width, height;
public:
    Rectangle(double w, double h) : Shape("Rectangle"), width(w), height(h) {}
    double area() const override {
        return width * height;
    }
};

// Template function with STL algorithms
template<typename T>
T findMax(const std::vector<T>& vec) {
    return *std::max_element(vec.begin(), vec.end());
}

// Functor class
class Multiplier {
private:
    int factor;
public:
    Multiplier(int f) : factor(f) {}
    int operator()(int x) const {
        return x * factor;
    }
};

int main() {
    std::cout << "=== Advanced C++ Test ===" << std::endl;
    
    // 1. Template classes and operator overloading
    std::cout << "\n1. Vector2D operations:" << std::endl;
    Vector2D<int> v1(3, 4);
    Vector2D<int> v2(1, 2);
    Vector2D<int> v3 = v1 + v2;
    std::cout << "v1 + v2 = ";
    v3.print();
    std::cout << std::endl;
    std::cout << "v1 dot v2 = " << v1.dot(v2) << std::endl;
    
    // 2. Polymorphism with smart pointers
    std::cout << "\n2. Polymorphic shapes:" << std::endl;
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(4.0, 6.0));
    
    for (const auto& shape : shapes) {
        shape->display();
    }
    
    // 3. STL containers and algorithms
    std::cout << "\n3. STL operations:" << std::endl;
    std::vector<int> numbers = {5, 2, 8, 1, 9, 3};
    std::cout << "Original: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << std::endl;
    
    std::sort(numbers.begin(), numbers.end());
    std::cout << "Sorted: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << std::endl;
    
    std::cout << "Max element: " << findMax(numbers) << std::endl;
    
    // 4. Map container
    std::cout << "\n4. Map operations:" << std::endl;
    std::map<std::string, int> scores;
    scores["Alice"] = 95;
    scores["Bob"] = 87;
    scores["Charlie"] = 92;
    
    for (const auto& pair : scores) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
    
    // 5. Functor usage
    std::cout << "\n5. Functor:" << std::endl;
    Multiplier times3(3);
    std::cout << "5 * 3 = " << times3(5) << std::endl;
    
    // 6. Lambda expressions
    std::cout << "\n6. Lambda expressions:" << std::endl;
    auto square = [](int x) { return x * x; };
    std::cout << "square(7) = " << square(7) << std::endl;
    
    std::vector<int> nums = {1, 2, 3, 4, 5};
    std::transform(nums.begin(), nums.end(), nums.begin(), 
                   [](int x) { return x * 2; });
    std::cout << "Doubled: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << std::endl;
    
    std::cout << "\n=== C++ Test Complete ===" << std::endl;
    return 0;
}
