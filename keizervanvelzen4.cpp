#include <iostream>
#include <cmath>
#include <cstdlib>
#include <iomanip>

using namespace std;

// Gives the first non-newline character on stdin
static char readCharacter()
{
    char kar;

    while((kar = cin.get()) == '\n') {};

    return kar;
}

struct Number {
    int value = 0;

    // Next goes from head to tail
    // That is, from Most Significant to Least Significant
    Number *next = nullptr;
    Number *prev = nullptr;

    static Number *getZero() {
        // Self referencing
        static Number zero = { 0, &zero, &zero };

        return &zero;
    }
};

class BigNumber {

    private:
        // Amount of digits in one number object
        const static int DIGITS_PER_NUM = 2;

        Number *head = nullptr;  // Most significant number(s)
        Number *tail = nullptr;  // Least significant number(s)

        // Adds number in front (Most Significant numbers)
        // Will take all previous numbers if num is in a list
        void prependNumber(Number *num) {
            if (num==nullptr) return;

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
        void appendNumber(Number *num) {
            Number *old_tail = tail;
            head = nullptr;
            tail = nullptr;
            prependNumber(num);
            prependNumber(old_tail);
        }

        // Deletes all numbers, set head and tail to nullptr
        void clearNumbers() {
            Number *num = head;
            Number *next;

            while (num != nullptr) {
                next = num->next;
                delete num;
                num = next;
            }

            head = nullptr;
            tail = nullptr;
        }

        bool isZero() {
            for(Number *n = head; n != nullptr; n = n->next)
            {
                if(n->value != 0) return false;
            }

            return true;
        }

        // Multiply this number with 10^n
        // Long long is at least 64 bits, thus to overflow
        // this parameter you need to create a number with
        // 2^64 - 1 digits which is impossible on today's hardware
        void shift(unsigned long long n) {
            
            if(n == 0) return;
            else if(n == 1) {
                
                if(isZero()) return;

                char overflow_digit = 0;
                int max_power_of_ten = powl(10, DIGITS_PER_NUM);

                for(Number *n = tail; n != nullptr; n = n->prev)
                {
                    n->value *= 10;
                    n->value += overflow_digit;
                    overflow_digit = n->value / max_power_of_ten;
                    n->value = n->value % max_power_of_ten;
                }

                if(overflow_digit != 0)
                {
                    Number *n = new Number();
                    n->value = overflow_digit;
                    prependNumber(n);
                }
            }
            else {

                for(unsigned i = 0; i < n/DIGITS_PER_NUM; i++)
                {
                    Number *n = new Number();
                    n->value = 0;
                    appendNumber(n);
                }
                
                for(unsigned i = 0; i < n % DIGITS_PER_NUM; i++) shift(1);
            }
        }

    public:

        void setOne()
        {
            clearNumbers();
            Number *n = new Number;
            n->value = 1;
            prependNumber(n);
        }
        
        // Prints the value of this BigNumber to cout
        // without leading zeros
        void print()
        {
            if(head != nullptr)
            {
                for(Number *num = head; num != nullptr; num = num->next)
                {
                    cout << setw(DIGITS_PER_NUM) << setfill('0') << num->value;
                }

                cout << endl;
            }
            else cout << "0" << endl;
        }

        // Reads a Big Number from cin, until a newline is encountered
        // Ignores leading newlines and non-digits
        void read()
        {
            Number *num = new Number;
            int chars_read = 0;

            for(char kar = readCharacter(); kar != '\n'; kar = cin.get())
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
        void add(BigNumber A, BigNumber B, bool clear_numbers = true)
        {
            Number *numA = A.tail;
            Number *numB = B.tail;

            if(clear_numbers) clearNumbers();

            // To put the result in
            Number *numRes = tail;

            int overflow = 0;

            const int max_power_of_ten = powl(10, DIGITS_PER_NUM);


            if(numA == nullptr) numA = Number::getZero();
            if(numB == nullptr) numB = Number::getZero();

            while (numA != Number::getZero()
                    || numB != Number::getZero()
                    || overflow > 0)
            {
                int sum = numA->value + numB->value + overflow;

                if(numRes != nullptr) sum += numRes->value;

                int value = sum % max_power_of_ten;
                overflow = sum / max_power_of_ten;

                if(numRes == nullptr)
                {
                    Number *n = new Number;
                    n->value = value;
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

        void multiply(BigNumber a_big, BigNumber b_big)
        {
            clearNumbers();

            BigNumber zero;
            BigNumber sub_product;

            const int max_power_of_ten = powl(10, DIGITS_PER_NUM);

            int overflow = 0;
            int a_count = 0;

            for(Number *a = a_big.tail; a != nullptr; a = a->prev)
            {
                sub_product.clearNumbers();
                sub_product.shift(a_count * DIGITS_PER_NUM);
                a_count += 1;

                for(Number *b = b_big.tail;
                        b != Number::getZero() || overflow > 0;
                        b = b->prev != nullptr ? b->prev : Number::getZero())
                {
                    // Maximum value is 999999999^2 which needs 64 bits to be represented
                    unsigned long long product = a->value * (unsigned long long)b->value;
                    product += overflow;

                    int value = product % max_power_of_ten;
                    overflow = product / max_power_of_ten;

                    Number *n = new Number;
                    n->value = value;

                    sub_product.prependNumber(n);
                    add(zero, sub_product, false);

                    n->value = 0;
                }
            }
        }
};

int main() {
    BigNumber *A = new BigNumber;
    BigNumber *B = new BigNumber;
    BigNumber *C = new BigNumber;

    A->read();
    B->read();

    cout << endl;
    A->print();
    B->print();

    C->add(A, B);
    C->print();

    return 0;
}
