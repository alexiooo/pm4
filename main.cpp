#include <iostream>
#include <cmath>
#include <cstdlib>
#include <limits>
#include "BigNumber.h"

using namespace std;

// Gives the first non-newline character on stdin
char readCharacter()
{
    char kar;

    while((kar = cin.get()) == '\n') {};

    return kar;
}

// Reads a number from stdin, stops when number would exceed maximum
int readNumber(int maximum)
{
    int num = 0;

    char kar = readCharacter();
    do {
        if ('0' <= kar && kar <= '9')
        {
            int new_num = num * 10;
            new_num += kar - '0';
            if(new_num <= maximum)
            {
                num = new_num;
            } else {
                // Prevent extra numbers from being seen as menu-input
                cin.ignore( numeric_limits<streamsize>::max(), '\n' );
                return num;
            }
        }
        kar = cin.get();
    } while(kar != '\n');
    return num;
}



void printMenu(BigNumber bg_array[])
{
    cout << string(40, '\n');
    cout << "Welkom! Dit is een programma om met extreem grote getallen te "
        << "kunnen rekenen." << endl;
    cout << "Deze getallen kunnen bijna oneindig groot zijn "
        << "(als je maar genoeg geheugen hebt)" << endl << endl << endl;
    cout << "De grote getallen zijn momenteel:" << endl;
    for (int i=0;i<3;i++)
    {
        cout << static_cast<char> (i+'A') << ": ";
        bg_array[i].print();
        cout << endl << endl;
    }
    cout << endl << "Je kunt kiezen uit de volgende operaties: " << endl;
    cout << "\t1. Zet een waarde" << endl;
    cout << "\t2. Tel twee getallen op" << endl;
    cout << "\t3. Vermenigvuldig twee getallen" << endl;
    cout << "\t4. Bereken Fibonacci getal" << endl;
    cout << "\t5. Kopieer getal" << endl << endl << endl << endl;
}

// Lets the user choose a BigNumber A,B or C
// returns the corresponding zero-based index
int readBigNumberIndex()
{
    int bg_index = -1;
    while (bg_index < 0)
    {
        char kar = readCharacter();
        if (kar>'Z')
            kar -= 'a'-'A';
        if ('A' <= kar && kar <= 'C'){
            bg_index = kar -= 'A';
        } else if (kar == '\n') {
            cout << "Je moet kiezen tussen A, B of C!" << endl;
        }
    }
    cin.ignore( numeric_limits<streamsize>::max(), '\n' );
    return bg_index;
}

// Handles menu input
// Returns true when the menu needs to be redrawn
bool handleInput(BigNumber bg_array[])
{
    int result_bg; // Index of the number where the result is stored
    int op1, op2; // Index of the operand numbers (if used)
    int kar = cin.get();

    if (kar < '1' || kar > '5') {
        if (kar == '\n'){
            cout << "Optie niet herkend, probeer opnieuw..." << endl;
        }
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Kies een getal om het resultaat in op te slaan (A, B of C):"
        << endl;
    result_bg = readBigNumberIndex();

    switch (kar)
    {
    case '1':
        cout << "Geef nu de waarde die je " << static_cast<char> (result_bg+'A')
            << " wilt geven, gevolgd door Enter:" << endl;
        bg_array[result_bg].read();
        break;

    case '2':
        cout << "Geef het eerste getal dat je wilt optellen, "
            << "gevolgd door een Enter:" << endl;
        op1 = readBigNumberIndex();
        cout << "En het tweede getal:" << endl;
        op2 = readBigNumberIndex();
        bg_array[result_bg].add( bg_array[op1], bg_array[op2] );
        break;

    case '3':
        cout << "Geef het eerste getal dat je wilt vermenigvuldigen, "
            << "gevolgd door een Enter:" << endl;
        op1 = readBigNumberIndex();
        cout << "En het tweede getal:" << endl;
        op2 = readBigNumberIndex();
        bg_array[result_bg].multiply( bg_array[op1], bg_array[op2] );
        break;

    case '4': {
        cout << "Geef nu welk Fibonacci getal je wilt berekenen (max 100000), "
            << "gevolgd door Enter: " << endl;
        int n = readNumber(100000);
        cout << "Bezig met het berekenen van Fibonacci getal " << n << " ..."
            << endl;
        bg_array[result_bg].fibonacci(n);
        break;
    }

    case '5':
        cout << "Kies een getal om vandaan te kopieren, gevolgd door Enter: "
            << endl;
        op1 = readBigNumberIndex();
        bg_array[result_bg].copyBigNumber( bg_array[op1] );
        break;
    }
    return true;
}

int main() {
    BigNumber bg_array[3];

    while (true) {
        printMenu(bg_array);
        while (!handleInput(bg_array)){};
    }

    return 0;
}
