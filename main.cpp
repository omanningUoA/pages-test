#include <iostream>
#include <cstring>
#include <vector>

using std::string;
typedef unsigned int uint;
typedef std::vector<uint> uvec;

uvec string_to_uvec(string I);
string uvec_to_string(uvec D);

uvec school_sum(uvec D1, uvec D2, uint B);
uvec karatsuba_product(uvec D1, uvec D2, uint B);
uvec int_quotient(uvec D1, uvec D2, uint B);

int main(int argc, char* argv[]) {
    // Inputs
    string I1, I2; uint B;
    std::cin >> I1 >> I2 >> B;

    // Convert I1 and I2 to digit vectors
    uvec D1 = string_to_uvec(I1), 
         D2 = string_to_uvec(I2);
    
    // Check bounds of inputs
    if (D1.size() > 100 || D2.size() > 100 || B < 2 || B > 10) {
        std::cerr << "Error: Variable out of range" << std::endl;
        return EXIT_FAILURE;        
    }

    // Calculate results
    string sum      = uvec_to_string(school_sum(D1, D2, B)),
           product  = uvec_to_string(karatsuba_product(D1, D2, B)),
           quotient = uvec_to_string(int_quotient(D1, D2, B));

    // Output results
    std::cout << sum      << " "
              << product  << " "
              << quotient << std::endl;
}

/**
 * @brief converts string to unsigned integer vector
 * @param I A number parsed as a string
 * @return An unsigned integer list 
 */
uvec string_to_uvec(string I) {
    uvec D;
    // Push integers in reverse order
    for (auto i = I.rbegin(); i != I.rend(); i++)
        D.push_back(*i - '0');
    return D;
}

/**
 * @brief converts string to unsigned integer vector
 * @param D An unsigned integer list
 * @return A number parsed as a string
 */
string uvec_to_string(uvec D) {
    string I;
    // Remove trailing zeros
    while (D.size() > 1 && D.back() == 0)
        D.pop_back();
    // Push chars in reverse order
    for (auto d = D.rbegin(); d != D.rend(); d++)
        I.push_back(*d + '0');
    return I;
}

/**
 * @brief resizes two unsigned integer lists to the same length
 * @param D1 first digits list
 * @param D2 second digits list
 * @param extra any extra digits to add
 * @return the resized length of the vectors
 */
size_t resize_uvecs(uvec &D1, uvec &D2, uint extra = 0) {
    // Calculate new size
    size_t size = std::max(D1.size(), D2.size()) + extra;
    // Update size of D1
    for (size_t i = D1.size(); i < size; i++)
        D1.push_back(0);
    // Update size of D2
    for (size_t i = D2.size(); i < size; i++)
        D2.push_back(0);
    // Return new size
    return size;
}

/**
 * @brief performs a scalar multiplication on vector D by integer i
 * @param D an unsigned digit vector
 * @param i the scalar multiplier
 * @return a scaled vector
 */
uvec scalar_multiply(uvec D, int i) { 
    uvec D1 = D;
    // Scalar multiply each element
    for (auto& d : D1) 
        d *= i;
    return D;
}

/**
 * @brief performs a shift of an integer bector by k
 * @param D an unsigned digit vector
 * @param k the number of digits to add
 * @return a scaled vector
 */
uvec shift_uvec(uvec D, uint k) {
    // Init vector of k zeros and append D
    uvec D1(k,0);
    D1.insert(D1.end(), D.begin(), D.end());
    return D1;
}

/**
 * @brief performs school sum algorithm on two digit vectors of base B
 * @param D1 first digits list
 * @param D2 second digits list
 * @param B the base of the digits
 * @return the sum result
 */
uvec school_sum(uvec D1, uvec D2, uint B) {
    // Make D1 and D2 of equal length
    uint n = resize_uvecs(D1, D2, 1);
    // Initialise carry vector
    uvec C(n+1, 0), S(n, 0);
    // Calculate sum
    for (size_t i = 0; i < n; i++) {
        uint sum = D1[i] + D2[i] + C[i];
        S[i] = sum % B;
        C[i+1] = sum / B;
    }
    // Return sum
    return S;
}

/**
 * @brief performs school subtraction algorithm on two digit vectors of base B
 * @param D1 first digits list
 * @param D2 second digits list
 * @param B the base of the digits
 * @return the different result
 */
uvec school_sub(uvec D1, uvec D2, uint B) {
    // Make D1 and D2 of equal length
    uint n = resize_uvecs(D1, D2, 1);
    // Initialise carry vector
    uvec C(n+1, 0), D(n, 0);
    // Calculate sum
    for (size_t i = 0; i < n; i++) {
        // Calculate difference
        int diff = static_cast<int>(D1[i]) - static_cast<int>(D2[i]) - static_cast<int>(C[i]);
        if (diff < 0) {
            diff += B;
            C[i+1] = 1;
        }
        D[i] = diff;
    }
    // Return sum
    return D;
}

/**
 * @brief performs multiplication algorithm on two digit vectors of base B
 * @param D1 first digits list
 * @param D2 second digits list
 * @param B the base of the digits
 * @return the multiplication result
 */
uvec karatsuba_product(uvec D1, uvec D2, uint B) {
    // resize vectors to same length
    uint n = resize_uvecs(D1, D2);
    // Recursive case
    if (n > 4) { 
        uint k = n/2;
        uvec // Calculate number parts
             D1_0(D1.begin(), D1.begin()+k),
             D1_1(D1.begin()+k, D1.end()),
             D2_0(D2.begin(), D2.begin()+k),
             D2_1(D2.begin()+k, D2.end()),
             // Calculate products
             p0 = karatsuba_product(D1_0, D2_0, B),
             p1 = karatsuba_product(school_sum(D1_0, D1_1, B), school_sum(D2_0, D2_1, B), B),
             p2 = karatsuba_product(D1_1, D2_1, B),
             // Calculate terms
             t0 = shift_uvec(p2, 2*k),
             t1 = shift_uvec(school_sub(p1, school_sum(p0, p2, B), B), k);
        // Return sum
        return school_sum(school_sum(t0, t1, B), p0, B);
    
    // Edge case
    } else if (n == 1) {
        // Return product
        return {D1.front() * D2.front()};
    
    // Base case
    } else {
        uvec P(2*n+1);
        // Perform multiplication
        for (size_t i = 0; i < n; i++)
            for (size_t j = 0; j < n; j++)
                P[i+j] += D1[j] * D2[i];

        // Fix carry
        for (size_t i = 0; i < 2*n; i++)
            if (P[i] >= B) {
                P[i+1] += P[i] / B;
                P[i] %= B;
            }
        // Return product
        return P;
    }
}

uvec int_quotient(uvec D1, uvec D2, uint B) { return {0}; }