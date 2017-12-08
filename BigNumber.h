struct Number {
    int value;

    // Next goes from head to tail
    // That is, from Most Significant to Least Significant
    Number *next;
    Number *prev;

    static Number *getZero();

    Number();
    Number(int v);
    Number(int v, Number *next, Number *prev);
};

class BigNumber {

    // Amount of digits in one number object
    const static int DIGITS_PER_NUM = 6;
    const static int MAX_POWER_TEN = powl(10, DIGITS_PER_NUM);

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
    // Constructor
    BigNumber();

    // Destructor, simply calls clearNumbers()
    ~BigNumber();

    // Deletes all numbers, set head and tail to nullptr
    void clearNumbers();

    // Sets the value to one
    void setOne();

    // Prints the value of this BigNumber to cout
    // without leading zeros
    void print();

    // Reads a Big Number from cin, until a newline is encountered
    // Ignores leading newlines and non-digits
    void read();

    // Adds two BigNumbers, stores result in this BigNumber
    void add(BigNumber A, BigNumber B, bool clear_numbers = true);

    // Stores the nth Fibonacci number in this BigNumber
    void fibonacci(int n);

    // Copies the value of a BigNumber
    void copyBigNumber(BigNumber c);

    // Multiplies two BigNumbers, stores result in this BigNumber
    void multiply(BigNumber a_big, BigNumber b_big);

};
