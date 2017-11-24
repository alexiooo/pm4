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
};

class BigNumber {

    private:
        // Amount of digits in one number object
        const static int DIGITS_PER_NUM = 2;

        Number *head = nullptr;  // Most significant number(s)
        Number *tail = nullptr;  // Least significant number(s)
        int n = 0;  // Amount of numbers in our list

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
        void clearNumbers(){
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


    public:
        // Prints the value of this BigNumber to cout
        void print()
        {
            Number *num = head;
            while (num != nullptr)
            {
                cout << setw(DIGITS_PER_NUM) << setfill('0') << num->value;
                num = num->next;
            }
            cout << endl;
        }

        // Reads a Big Number from cin, until a newline is encountered
        // Ignores leading newlines and non-digits
        void read()
        {
            Number *num = new Number;
            int chars_read = 0;
            char kar = readCharacter();

            do
            {
                if ('0' <= kar && kar <= '9')
                {
                    if (chars_read >= DIGITS_PER_NUM)
                    {
                        appendNumber(num);
                        num = new Number;
                        chars_read = 0;
                    }
                    num->value *= 10;
                    num->value += kar - '0';
                    chars_read++;
                }
                kar = cin.get();
            }
            while(kar != '\n');

            appendNumber(num);
        }

        // Adds two BigNumbers, stores result in this BigNumber
        void add(BigNumber *A, BigNumber *B)
        {
            Number *numA = A->tail;
            Number *numB = B->tail;
            int overflow = 0;
            int max_value = pow(10, DIGITS_PER_NUM);

            clearNumbers();

            while (numA != nullptr && numB != nullptr)
            {
                Number *num = new Number;
                int sum = numA->value + numB->value + overflow;

                num->value = sum % max_value;
                overflow = sum - num->value;
                prependNumber(num);

                cerr << numA->value << " + " << numB->value << " = " << num->value << " +" << overflow << endl;

                numA = numA->prev;
                numB = numB->prev;
            }
            if (numA != nullptr)
            {
                while (overflow>0 && numA!=nullptr)
                {
                    Number *num = new Number;
                    int sum = numA->value + overflow;

                    num->value = sum % max_value;
                    overflow = sum - num->value;
                    prependNumber(num);

                    numA = numA->prev;
                }
                if(numA != nullptr){
                    prependNumber(numA);
                }
            } else if (numB != nullptr)
            {
                while (overflow>0 && numB!=nullptr)
                {
                    Number *num = new Number;
                    int sum = numB->value + overflow;

                    num->value = sum % max_value;
                    overflow = sum - num->value;
                    prependNumber(num);

                    numB = numB->prev;
                }
                if(numB != nullptr){
                    prependNumber(numB);
                }
            } else if (overflow>0)
            {
                Number *num = new Number;
                num->value = overflow;
                prependNumber(num);
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
