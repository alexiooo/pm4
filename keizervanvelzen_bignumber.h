
#ifndef BIGNUMBER_H__INCLUDED
#define BIGNUMBER_H__INCLUDED

struct Number {
    int value;

    // Next goes from head to tail
    // That is, from Most Significant to Least Significant
    Number *next;
    // Previous number
    Number *prev;

    // Gets a number with value zero and prev=next=itself
    static Number *getZero();

    // Constructor with value 0, next=prev=nullptr
    Number();
    // Constructor with value v, next=prev=nullptr
    Number(int v);
    // Constructor with value v, use given next and prev
    Number(int v, Number *next, Number *prev);
};

class BigNumber {

    // Amount of digits in one number object
    const static int DIGITS_PER_NUM;
    // 10^DIGITS_PER_NUM, maximum value in one Number
    const static int MAX_POWER_TEN;

    Number *head;  // Most significant number(s)
    Number *tail;  // Least significant number(s)

    // Adds number in front (Most Significant numbers)
    // Will take all previous numbers if num is in a list
    void prependNumber(Number *num);

    // Adds number to the back (Least Significant)
    // Will take all next numbers if num is in a list
    void appendNumber(Number *num);

    // Is this BigNumber's value zero
    bool isZero();

    // Multiply this number with 10^n
    // Long long is at least 64 bits, thus to overflow
    // this parameter you need to create a number with
    // 2^64 - 1 digits which is impossible on today's hardware
    void shift(unsigned long long n);

public:
    // Constructor, set head and tail to nullptr
    BigNumber();

    // Destructor, calls clearNumbers()
    ~BigNumber();

    // Delete all numbers and set head and tail to nullptr
    void clearNumbers();

    // Set BigNumber to one Number with value 1
    void setOne();

    // Fill BigNumber with m Number structs with value 0
    void setZero(int m);

    // Prints the value of this BigNumber to cout
    // without leading zeros
    void print();

    // Reads a Big Number from cin, until a newline is encountered
    // Ignores leading newlines and non-digits
    void read();

    // Adds BigNumbers, stores result in this BigNumber
    // If clear_numbers is false, also add the current value of this BigNumber
    // If either A or B are this BigNumber, clear_numbers is ignored
    //  and A+B is calculated
    void add(BigNumber &A, BigNumber &B, bool clear_numbers = true);

    // Stores the nth Fibonacci number in this BigNumber
    void fibonacci(int n);

    // Copies the value of a BigNumber
    void copyBigNumber(BigNumber &c);

    // Multiplies two BigNumbers, stores result in this BigNumber
    void multiply(BigNumber &a_big, BigNumber &b_big);
};

#endif // BIGNUMBER_H__INCLUDED
