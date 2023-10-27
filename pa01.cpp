#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <string.h>
#include <iomanip>

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        cout << "Incorrect number of arguments. Exiting safetly";
        exit(1);
    }

    ifstream kf(argv[1]);

    int KEY_SIZE;
    if (!(kf >> KEY_SIZE))
    {
        cerr << "Error reading matrix size. Exiting safetly" << endl;
        kf.close();
        exit(1);
    }

    vector<vector<int>> key(KEY_SIZE, vector<int>(KEY_SIZE));

    for (int i = 0; i < KEY_SIZE; i++)
    {
        for (int j = 0; j < KEY_SIZE; j++)
        {
            if (!(kf >> key[i][j]))
            {
                cerr << "Not possible to read the matrix entry. Exiting." << endl;
                kf.close();
                exit(1);
            }
        }
    }

    kf.close();

    cout << "\nKey matrix:\n";
    for (int i = 0; i < KEY_SIZE; i++)
    {
        for (int j = 0; j < KEY_SIZE; j++)
        {
            cout << setw(4) << right << key[i][j] << "";
        }
        cout << endl;
    }

    ifstream pt(argv[2]);
    if (!pt.is_open())
    {
        cerr << "Not possible to read text file. Exiting safetly" << endl;
        exit(1);
    }
    int MAX_SIZE = 10000;
    char fileText[MAX_SIZE];
    char c;
    int l = 0;
    while (pt >> c)
    {
        if (isalpha(c))
        {
            fileText[l] = c;
            l++;
        }
    }
    fileText[l] = '\0'; //end of char array
    int fileLength = strlen(fileText);

    int rmdr = fileLength % KEY_SIZE; 
    int padding = KEY_SIZE - rmdr;
    if ((rmdr) != 0)
    {
        for (int i = 0; i < padding; i++)
        {
            fileText[fileLength] = 'x';
            fileLength++; // updating fileLength.
        }
    }

    char plainText[MAX_SIZE];

    // Second filter to convert all uppercase letter to lowercase letters
    int i;
    for ( i = 0; i < fileLength; i++)
    {
        if (isupper(fileText[i]))
        {
            plainText[i] = (char)tolower(fileText[i]);
        }
        else 
        {
            plainText[i] = fileText[i];
        }
    }

    plainText[i] = '\0';
    int plainTextLen = strlen(plainText);

    cout << "\nPlaintext:" << endl;
    for (int i = 0; i < plainTextLen; i++)
    {
        if ( (i != 0) && (i % 80 == 0))
        {
            cout << endl;
        }

        cout << plainText[i];
    }
    cout << endl;
    char alphabet[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
                         'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    int encryptArr[KEY_SIZE] = {};
    int matrixProduct[KEY_SIZE] = {};
    int cipherText[MAX_SIZE] = {};
    int control;
    int control2;

    // Finding the letter indices in the alphabet
    int numLetters = 0;
    while (numLetters < plainTextLen) 
    {
        for (int i = 0; i < KEY_SIZE; i++)
        {
            for (int j = 0; j < 26; j++)
            {
                if (plainText[numLetters] == alphabet[j])
                {
                    encryptArr[i] = j;
                    break;
                }
            }
            numLetters++;
        }

        control = numLetters;

        // getting chunks of key_size for matrix multiplication
        if ( (control) % KEY_SIZE == 0)
        {
            // MATRIX MULTIPLICATION WITH MOD 26
            for (int row = 0; row < KEY_SIZE; row++)
            {
                for (int column = 0; column < KEY_SIZE; column++)
                {
                    matrixProduct[row] += key[row][column] * encryptArr[column]; 
                }
                matrixProduct[row] %= 26;
            }
        }

        control2 = numLetters - KEY_SIZE;
        for (int i = 0; i < KEY_SIZE; i++)
        {
            cipherText[control2] = matrixProduct[i];
            control2++;
        }

        // Restarting values of matrixProduct arr for a new iteration
        for (int i = 0; i < KEY_SIZE; i++)
            matrixProduct[i] = 0;
    }

    // Putting a flag at the end of the ciphertext
    cipherText[numLetters] = -1;
    cout << endl;

    // Printing cipher text
    cout << "Ciphertext:\n";
    int idx = 0;
    while (cipherText[idx] != -1)
    {
        if ((idx != 0) && (idx % 80 == 0))
            cout << endl;
        cout << alphabet[cipherText[idx]];
        idx++;
    }
    cout << endl;

    return 0;
}