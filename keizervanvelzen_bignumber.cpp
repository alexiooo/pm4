#include <cmath>
#include <iostream>
#include <cstdlib>
#include "keizervanvelzen_bignumber.h"

using namespace std;


Number *Number::getZero() {
    // Self referencing
    static Number zero = Number(0, &zero, &zero);
    return &zero;
}

Number::Number() : Number(0) {}

Number::Number(int v) : Number(v, nullptr, nullptr) {}

Number::Number(int v, Number *next, Number *prev)
        : value(v), next(next), prev(prev) {}


// Amount of digits in one number object
const int BigNumber::DIGITS_PER_NUM = 6;
const int BigNumber::MAX_POWER_TEN = powl(10, DIGITS_PER_NUM);


BigNumber::BigNumber() : head(nullptr), tail(nullptr) {}

BigNumber::~BigNumber() {
    clearNumbers();
}

// Adds number in front (Most Significant numbers)
// Will take all previous numbers if num is in a list
void BigNumber::prependNumber(Number *num) {
    if (num==nullptr || num==Number::getZero()) return;

    num->next = head;

    if (head != nullptr){
        head->prev = num;
    }
    Number *new_head = num;
    while (new_head->prev != nullptr){
        new_head = new_head->prev;
    }
    head = new_head;

    if (tail == nullptr){
        tail = num;
    }
}

// Adds number to the back (Least Significant)
// Will take all next numbers if num is in a list
void BigNumber::appendNumber(Number *num) {
    Number *old_tail = tail;
    head = nullptr;
    tail = nullptr;
    prependNumber(num);
    prependNumber(old_tail);
}

bool BigNumber::isZero() {
    for(Number *n = head; n != nullptr; n = n->next)
    {
        if(n->value != 0) return false;
    }

    return head==nullptr;
}

// Multiply this number with 10^n
// Long long is at least 64 bits, thus to overflow
// this parameter you need to create a number with
// 2^64 - 1 digits which is impossible on today's hardware
void BigNumber::shift(unsigned long long n) {

    if(n == 0) return;
    else if(n == 1) {

        if(isZero()) return;

        char overflow_digit = 0;

        for(Number *n = tail; n != nullptr; n = n->prev)
        {
            n->value *= 10;
            n->value += overflow_digit;
            overflow_digit = n->value / MAX_POWER_TEN;
            n->value = n->value % MAX_POWER_TEN;
        }

        if(overflow_digit != 0)
        {
            Number *n = new Number(overflow_digit);
            prependNumber(n);
        }
    }
    else {

        for(unsigned i = 0; i < n/DIGITS_PER_NUM; i++)
        {
            Number *n = new Number();
            appendNumber(n);
        }

        for(unsigned i = 0; i < n % DIGITS_PER_NUM; i++) shift(1);
    }
}

// Deletes all numbers, set head and tail to nullptr
void BigNumber::clearNumbers() {
    Number *num = head;
    Number *next;

    while (num != nullptr)
    {
        next = num->next;
        delete num;
        num = next;
    }

    head = nullptr;
    tail = nullptr;
}

// Set BigNumber to one Number with value 1
void BigNumber::setOne() {
    clearNumbers();
    Number *n = new Number(1);
    prependNumber(n);
}

// Fill BigNumber with m Number structs with value 0
void BigNumber::setZero(int m) {
    clearNumbers();
    for(int i=0; i<m; i++)
    {
        Number *n = new Number(0);
        prependNumber(n);
    }
}

// Prints the value of this BigNumber to cout
// without leading zeros
void BigNumber::print() {
    if(head != nullptr)
    {
        cout << head->value;
        for(Number *num = head->next; num != nullptr; num = num->next)
        {
            int temp_max = MAX_POWER_TEN/10 - 1;
            while (num->value < temp_max){
                temp_max /= 10;
                cout << 0;
            }
            cout << num->value;
        }

        cout << endl;
    }
    else cout << "Nog geen waarde toegekend (0)" << endl;
}

// Reads a Big Number from cin, until a newline is encountered
// Ignores leading newlines and non-digits
void BigNumber::read() {
    Number *num = new Number();
    int chars_read = 0;
    char kar;

    clearNumbers();

    while( (kar = cin.get()) == '\n') {}
    for(;kar != '\n'; kar = cin.get())
    {
        if ('0' <= kar && kar <= '9')
        {
            if (chars_read == DIGITS_PER_NUM)
            {
                appendNumber(num);
                num = new Number;
                chars_read = 0;
            }
            num->value *= 10;
            num->value += kar - '0';
            chars_read++;
        }
    }

    if(chars_read > 0)
    {
        if(tail == nullptr) appendNumber(num);
        else {
            shift(chars_read);
            tail->value += num->value;
            delete num;
        }
    }
    else delete num;
}

// Adds two BigNumbers, stores result in this BigNumber
// If clear_numbers is false, will also add the current value of this BigNumber
void BigNumber::add(BigNumber &A, BigNumber &B, bool clear_numbers) {
    Number *numA = A.tail;
    Number *numB = B.tail;

    // If either A or B share this BigNumbers list, call on auxiliary BigNumber
    if(tail != nullptr && (numA == tail || numB == tail)){
        BigNumber res;
        res.add(A, B);
        clearNumbers();
        tail = res.tail;
        res.tail = nullptr;
        head = res.head;
        res.head = nullptr;
        return;
    }

    if(clear_numbers) clearNumbers();

    // To put the result in
    Number *numRes = tail;

    int overflow = 0;

    if(numA == nullptr) numA = Number::getZero();
    if(numB == nullptr) numB = Number::getZero();

    while (numA != Number::getZero()
            || numB != Number::getZero()
            || overflow > 0)
    {
        int sum = numA->value + numB->value + overflow;

        if(numRes != nullptr) sum += numRes->value;

        int value = sum % MAX_POWER_TEN;
        overflow = sum / MAX_POWER_TEN;

        if(numRes == nullptr)
        {
            Number *n = new Number(value);
            prependNumber(n);
        }
        else
        {
            numRes->value = value;
        }

        numA = numA->prev != nullptr ? numA->prev : Number::getZero();
        numB = numB->prev != nullptr ? numB->prev : Number::getZero();
        numRes = numRes != nullptr ? numRes->prev : nullptr;
    }
}


void BigNumber::fibonacci(int n) {
    clearNumbers();

    if(n <= 2) setOne();
    else {
        BigNumber zero;
        BigNumber previous;
        BigNumber current;

        previous.setOne();
        current.setOne();

        // Temporary variable used to switch previous and current
        BigNumber next;

        for(int i = 2; i < n; i++)
        {
            // Prevent add function from deleting numbers
            // which are pointed to by 'current'
            next.head = next.tail = nullptr;
            next.add(previous, current);

            previous.clearNumbers(); // Prevent memory leak
            previous = current;
            current = next;
        }

        previous.clearNumbers(); // Prevent memory leak
        *this = current;
    }
}

// Copies the value of a BigNumber
void BigNumber::copyBigNumber(BigNumber &c){
    clearNumbers();

    for(Number *num = c.tail; num != nullptr; num=num->prev){
        Number *copyNum = new Number( num->value );
        prependNumber(copyNum);
    }
}

void BigNumber::multiply(BigNumber &a_big, BigNumber &b_big) {
    clearNumbers();

    BigNumber zero;
    BigNumber sub_product;

    int overflow = 0;
    int a_count = 0;

    if( a_big.isZero() || b_big.isZero() ){
        appendNumber( new Number(0) );
        return;
    }


    for(Number *a = a_big.tail; a != nullptr; a = a->prev)
    {
        sub_product.shift(a_count * DIGITS_PER_NUM);
        a_count += 1;

        for(Number *b = b_big.tail;
                b != Number::getZero() || overflow > 0;
                b = b->prev != nullptr ? b->prev : Number::getZero())
        {
            // Maximum value is 999999999^2 which needs 64 bits to be represented
            unsigned long long product = a->value * (unsigned long long)b->value;
            product += overflow;

            int value = product % MAX_POWER_TEN;
            overflow = product / MAX_POWER_TEN;

            Number *n = new Number(value);

            sub_product.prependNumber(n);
            add(zero, sub_product, false);

            n->value = 0;
        }

        sub_product.clearNumbers();
    }
}
