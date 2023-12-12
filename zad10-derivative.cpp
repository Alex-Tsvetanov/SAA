#include <iostream>
#include <vector>

int GCD(int a, int b) {
    if (a == 0) 
        return b; 
    return GCD(b % a, a);
}

struct fraction {
    // a / b
    int a;
    int b;
    
    fraction(int x) : a (x), b (1) { }
    explicit fraction(int A, int B) {
        int gcd = GCD(A, B);
        a = A / gcd;
        b = B / gcd;
    }
    
    fraction invert() const {
        return fraction(b, a);
    }
    
    fraction& operator -= (const fraction& f1) {
        int new_a = a * f1.b - f1.a * b;
        int new_b = f1.b * b;
        int gcd = GCD(new_a, new_b);
        new_a /= gcd;
        new_b /= gcd;
        a = new_a;
        b = new_b;
        return *this;
    }
    
    friend std::ostream& operator << (std::ostream& out, const fraction& f) {
        return out << "(" << f.a << "/" << f.b << ")";
    }
};

// is the equation a constant
bool isConstant(int* coef, int size) {
    if (size == 1) return true;
    for (int i = 1 ; i < size ; i ++)
        if (coef[i] != coef[i - 1])
            return false;
    return true;
}

void integrate(fraction** f, const int power, fraction** out, const int expected) {
    out[power] = new fraction(expected);
    
    int current_power = power - 1;
    for (int i = 0 ; i < power ; i ++) {
        out[i] = new fraction(f[i]->a, f[i]->b * (current_power + 1));
        (*out[power]) -= (*out[i]);
        current_power --;
    }
}

// Function to find the recurrence relation using finite differences
fraction** findRecurrence(const int* seq, const int size) {
    fraction*** coef = new fraction**[size]; // up to polinomial of (size-1)-th power so last element of each row is nullptr
    
    int** arr = new int*[size + 1];
    arr[size] = new int[size];
    for (int i = 0 ; i < size ; i ++)
        arr[size][i] = seq[i];

    int rows;
    for (rows = size - 1 ; rows >= 1 ; rows --) {
        arr[rows] = new int[rows];
        for (int j = 0 ; j < rows ; j ++) {
            arr[rows][j] = arr[rows + 1][j + 1] - arr[rows + 1][j];
        }
        if (isConstant(arr[rows], rows)) {
            break;
        }
    }
    for (int i = size ; i >= rows ; i --) {
        std::cout << i << "| ";
        for (int j = 0 ; j < i ; j ++) {
            std::cout << arr[i][j] << " ";
        }
        std::cout << std::endl;
    }
    
    int number_of_coefs_in_current_row = 1;
    
    coef[0] = new fraction*[number_of_coefs_in_current_row + 1];
    coef[0][0] = new fraction(arr[rows][0]);

    //for (int i = 0 ; i < number_of_coefs_in_current_row ; i ++) {
    //    std::cout << *coef[0][i] << " ";
    //}
    //std::cout << std::endl;

    for (int i = 1 ; i <= size - rows ; i ++) {
        number_of_coefs_in_current_row ++;
        coef[i] = new fraction*[number_of_coefs_in_current_row + 1];
        std::cout << "integrating: f'(x) = ";
        for (int j = 0 ; j < number_of_coefs_in_current_row - 1 ; j ++) {
            std::cout << (*coef[i - 1][j]) << " * n^" << number_of_coefs_in_current_row-2-j << " ";
            if (j < number_of_coefs_in_current_row - 2) std::cout << "+ ";
        } 
        std::cout << " should get " << arr[rows + i][0] << std::endl;
        //std::cout << rows + i << std::endl;
        integrate(coef[i - 1], number_of_coefs_in_current_row - 1, coef[i], arr[rows + i][0]);
        std::cout << "so         : f(x) = ";
        for (int j = 0 ; j < number_of_coefs_in_current_row ; j ++) {
            std::cout << (*coef[i][j]) << " * n^" << number_of_coefs_in_current_row-1-j << " ";
            if (j < number_of_coefs_in_current_row - 1) std::cout << "+ ";
        } 
        std::cout << std::endl;
    }
    return coef[size - rows];
}

int main() {
    //const int sequence[] = { 1, 4, 9, 16, 25 };
    //const int sequence[] = { 1, 3, 5, 7, 9 };
    const int sequence[] = { 1, 1, 2, 3, 5, 8, 13, 21 };
    const int size = sizeof(sequence) / sizeof(int);

    fraction** recurrence = findRecurrence(sequence, size);
    int coef_len = 1;
    for (int i = 0 ; recurrence[i] != nullptr ; i ++) { coef_len ++; }
    std::cout << "f(n) = ";
    for (int i = 1 ; recurrence[i - 1] != nullptr ; i ++) {
        std::cout << (*recurrence[i - 1]) << " * n^" << coef_len-i-1 << " ";
        if (recurrence[i] != nullptr) std::cout << "+ ";
    }
    std::cout << std::endl;

    return 0;
}
/*
2
- 2x+c = 3 => c = 1
2x+1


*/