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

struct number {
    int value = 0;

    // Next goes from head to tail
    // That is, from Most Significant to Least Significant
    number *next = nullptr;
    number *prev = nullptr;
};

class BigNumber {

    private:
        // Amount of digits in one number object
        const static int digits_per_number = 2;

        number *head = nullptr;  // Most significant number(s)
        number *tail = nullptr;  // Least significant number(s)
        int n = 0;  // Amount of numbers in our list

        // Adds number in front (Most Significant numbers)
        // Will take all previous numbers if num is in a list
        void prependNumber(number *num) {
            if (num==nullptr) return;

            num->next = head;

            if (head != nullptr){
                head->prev = num;
            }
            number *new_head = num;
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
        void appendNumber(number *num) {
            if (num==nullptr) return;

            num->prev = tail;

            if( tail != nullptr ){
                tail->next = num;
            }
            number *new_tail = num;
            while (new_tail->next != nullptr){
                new_tail = new_tail->next;
            }
            tail = new_tail;

            if(head == nullptr){
                head = num;
            }
        }

        // Deletes all numbers, dereferences head and tail
        void clearNumbers(){
            number *num = head;
            number *next;

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
            number *num = head;
            while (num != nullptr)
            {
                cout << setw(digits_per_number) << setfill('0') << num->value;
                num = num->next;
            }
            cout << endl;
        }

        // Reads a Big Number from cin, until a newline is encountered
        // Ignores leading newlines and non-digits
        void read()
        {
            number *num = new number;
            int chars_read = 0;
            char kar = readCharacter();

            do
            {
                if ('0' <= kar && kar <= '9')
                {
                    if (chars_read >= digits_per_number)
                    {
                        appendNumber(num);
                        num = new number;
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
            number *numA = A->tail;
            number *numB = B->tail;
            int overflow = 0;
            int max_value = pow(10, digits_per_number);

            clearNumbers();

            while (numA != nullptr && numB != nullptr)
            {
                number *num = new number;
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
                    number *num = new number;
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
                    number *num = new number;
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
                number *num = new number;
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
