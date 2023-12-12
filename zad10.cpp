#include <iostream>
#include <vector>

// Function to calculate the factorial
constexpr int factorial(int n) {
    if (n <= 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

struct fraction {
private:
    // a / b
    int a;
    int b;
    
    static int GCD(int a, int b) {
        if (a == 0) 
            return b; 
        return GCD(b % a, a);
    }

    void set(int A, int B) {
        int gcd = GCD(A, B);
        a = A / gcd;
        b = B / gcd;
    }
public:
    fraction(int x = 0) : a (x), b (1) { }
    
    explicit fraction(int A, int B) {
        set(A, B);
    }
    
    fraction invert() const {
        return fraction(b, a);
    }
    
    fraction& operator -= (const fraction& f1) {
        const int new_a = a * f1.b - f1.a * b;
        const int new_b = f1.b * b;
        set(new_a, new_b);
        return *this;
    }
    
    fraction operator + (const fraction& f1) {
        const int new_a = a * f1.b + f1.a * b;
        const int new_b = f1.b * b;
        return fraction(new_a, new_b);
    }
    
    fraction operator - (const fraction& f1) {
        const int new_a = a * f1.b - f1.a * b;
        const int new_b = f1.b * b;
        return fraction(new_a, new_b);
    }
    
    fraction operator / (const fraction& f1) {
        const int new_a = a * f1.b;
        const int new_b = f1.a * b;
        return fraction(new_a, new_b);
    }
    
    fraction operator * (const fraction& f1) {
        const int new_a = a * f1.a;
        const int new_b = f1.b * b;
        return fraction(new_a, new_b);
    }
    
    friend std::ostream& operator << (std::ostream& out, const fraction& f) {
        return out << "(" << f.a << "/" << f.b << ")";
    }
};

// Function to find the recurrence relation using finite differences
std::vector<fraction> findRecurrence(const std::vector<fraction>& sequence) {
    int n = sequence.size() - 1;
    std::vector<std::vector<fraction>> differences(n, std::vector<fraction>(n));

    // Initializing differences matrix with the given sequence
    for (int i = 0; i < n; ++i) {
        differences[i][0] = sequence[i];
    }

    // Calculating finite differences
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < n - i; ++j) {
            differences[j][i] = differences[j + 1][i - 1] - differences[j][i - 1];
        }
    }

    // Find the factorial coefficients of the binomial theorem
    std::vector<fraction> coefficients(n);
    for (int i = 0; i < n; ++i) {
        coefficients[i] = differences[0][i] / factorial(i);
    }

    return coefficients;
}

int main() {
    std::vector<fraction> sequence =     
        { 1, 4, 9, 16, 25 };
        //{ 1, 3, 5, 7, 9 };
        //{ 1, 1, 2, 3, 5, 8, 13, 21 }; // not fib
        //{ 1, 2 }; // fib
        //{1, 2, 4};
    std::vector<fraction> recurrence = findRecurrence(sequence);

    // Display the obtained recurrence relation
    std::cout << "Recurrence relation coefficients: ";
    for (int i = 0; i < recurrence.size(); ++i) {
        std::cout << recurrence[i];
        if (i != recurrence.size() - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;

    int k = 10;
    for (int i = sequence.size() ; i <= k ; i++) {
        fraction next = 0;
        for (int j = 0; j < recurrence.size(); ++j) {
            next = next + sequence[i - j - 1] * recurrence[j];
        }
        sequence.push_back(next);
    }

    for (auto& x : sequence)
        std::cout << x << " ";

    return 0;
}
