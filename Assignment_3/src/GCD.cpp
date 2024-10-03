#include <iostream>
#include "GCD.h"

// Continuously subtract the larger number from the smaller one until both are equal
mpz_class By_Subtraction(mpz_class a, mpz_class b)
{
    while (a != b)
    {
        if (a > b)
            a -= b;
        else
            b -= a;
    }

    // The result is the GCD when a and b are equal
    return a;
}

// Continuously divide and replace a by b and b by the remainder until the remainder is 0
mpz_class By_Remainder_Division(mpz_class a, mpz_class b)
{
    while (b != 0)
    {
        mpz_class remainder = a % b;
        a = b;
        b = remainder;
    }

    // When b becomes 0, a is the GCD
    return a;
}

// Uses bitwise operations and subtraction to find GCD efficiently
mpz_class By_Euclid_Algorithm(mpz_class a, mpz_class b)
{
    if (a == 0)
        return b;
    if (b == 0)
        return a;

    int shift = 0;
    // While both a and b are even, divide both by 2 (bitwise right shift)
    while (((a | b) & 1) == 0)
    {
        a >>= 1;
        b >>= 1;
        shift++; // Keep track of how many times we divided by 2
    }

    // Now a is odd, divide a by 2 while it's even
    while ((a & 1) == 0)
        a >>= 1;

    do
    {
        // While b is even, divide by 2
        while ((b & 1) == 0)
            b >>= 1;

        // Ensure a is smaller than or equal to b
        if (a > b)
            std::swap(a, b); // Swap a and b if a > b
        b -= a;              // Subtract a from b
    } while (b != 0); // Continue until b becomes 0

    return a << shift; // Return a multiplied by 2^shift (restore factors of 2)
}

// Finds the GCD and also coefficients x and y such that a*x + b*y = GCD(a, b)
mpz_class By_Extended_Euclid(mpz_class &x, mpz_class &y, const mpz_class &a, const mpz_class &b)
{
    // a.x + b.y = GCD(a, b)
    if (b == 0)
    {
        // Base case: If b is 0, GCD is a, and x = 1, y = 0
        x = 1;
        y = 0;
        return a;
    }
    else
    {
        mpz_class x1, y1;
        mpz_class q = a / b; // Quotient of a divided by b
        mpz_class r = a % b; // Remainder of a divided by b

        // Recursively calculate GCD and the coefficients
        mpz_class result = By_Extended_Euclid(x1, y1, b, r);

        // Update x and y using the recursive result
        x = y1;
        y = x1 - q * y1;

        return result;
    }
}

mpz_class GCD(const mpz_class &a, const mpz_class &b)
{
    mpz_class x, y; // Coefficients x and y (not used in this case)
    mpz_class result = By_Extended_Euclid(x, y, a, b);

    return result;
}