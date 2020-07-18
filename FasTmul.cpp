//multiply two large numbers 
//source geeksforgeeks

#include<iostream>
#include<string>
#include<vector>
#include<time.h> 
#include<ctime>

using namespace std;
#define BASE 100000000
// Multiplies str1 and str2, and prints result. 
const char* multiply(unsigned long long int num1[], unsigned long long int num2[], int len1, int len2)
{


    int i_n1 = 0;
    int i_n2 = 0;
    unsigned long long int result[64100] = { 0 };
    for (int i = len1 - 1; i >= 0; i--)
    {
        unsigned long long int carry = 0;
        unsigned long long int n1 = num1[i];
        i_n2 = 0;
        for (int j = len2 - 1; j >= 0; j--)
        {
            unsigned long long int n2 = num2[j];
            unsigned long long int sum = (n1 * n2) + result[i_n1 + i_n2] + carry;
            carry = sum / BASE;
            result[i_n1 + i_n2] = sum % BASE;

            i_n2++;
        }

        if (carry > 0)
            result[i_n1 + i_n2] += carry;
        i_n1++;
    }

    // ignore '0's from the right 
    int i = len1 + len2 + 1;
    while (i >= 0 && result[i] == 0)
        i--;

    // If all were '0's - means either both or 
    // one of num1 or num2 were '0' 
    if (i == -1)
        return nullptr;

    // generate the result string 
    string s = "";
    cout << i << endl;
    while (i >= 0)
        s += std::to_string(result[i--]);

    return s.c_str();
}
using ull = unsigned long long int;
// Driver code 
int main()
{
#define SIZE 32000
    char arr[] = { '0','1', '2','3','4','5','6','7','8','9' };
    char str1[SIZE + 5];
    char str2[SIZE + 5];
    for (int i = 0; i < SIZE; i++)
    {
        str1[i] = arr[9];
        str2[i] = arr[9];

    }
    str1[SIZE] = '\0';
    str2[SIZE] = '\0';
    std::clock_t start;
    double n_duration;


    start = std::clock();
    int ilen1 = strlen(str1);
    int ilen2 = strlen(str2);
    int len1 = (ilen1 / 8) + ((ilen1 % 8) != 0);
    int len2 = (ilen2 / 8) + ((ilen2 % 8) != 0);
    cout << ilen1 << endl;
    unsigned long long int num1[4001] = { 0 };
    unsigned long long int num2[4001] = { 0 };
    int rem = ilen1 % 8;
    int blocks = ilen1 / 8;

    int star = 0;
    if (rem)
    {
        ull power = 1;
        ull base = 10;
        ull value = 0;
        for (int i = rem - 1; i >= 0; i--)
        {
            value += (str1[i] - '0') * power;
            power = power * base;
        }
        num1[star++] = value;
    }
    int idx = rem;
    while (blocks--)
    {

        ull power = 1;
        ull base = 10;
        ull value = 0;
        for (int i = idx + 7; i >= idx; i--)
        {
            value += (str1[i] - '0') * power;
            power = power * base;
        }
        num1[star++] = value;
        idx += 8;
    }
    rem = ilen2 % 8;
    blocks = ilen2 / 8;
    star = 0;
    if (rem)
    {
        ull power = 1;
        ull base = 10;
        ull value = 0;
        for (int i = rem - 1; i >= 0; i--)
        {
            value += (str2[i] - '0') * power;
            power = power * base;
        }
        num2[star++] = value;
    }
    idx = rem;
    while (blocks--)
    {

        ull power = 1;
        ull base = 10;
        ull value = 0;
        for (int i = idx + 7; i >= idx; i--)
        {
            value += (str2[i] - '0') * power;
            power = power * base;
        }
        num2[star++] = value;
        idx += 8;
    }
#if 0
    for (int i = 0; i < len1; i++)
    {
        cout << num1[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < len2; i++)
    {
        cout << num2[i] << " ";
    }
    cout << endl;
#endif // 0




    multiply(num1, num2, len1, len2);
    n_duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    std::cout << "seq printf: " << n_duration << '\n';

    return 0;
    }
