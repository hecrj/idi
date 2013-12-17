#include <QLCDNumber>
#include "LCDBase3.h"

LCDBase3::LCDBase3(QWidget *parent): QLCDNumber(parent) {}

void base3(int n, int &result)
{
    int q = n / 3;

    if(q != 0)
    {
        base3(q, result);
        result *= 10;
    }

    result += n % 3;
}

int base3(int n)
{
    int result = 0;
    base3(n, result);

    return result;
}

void LCDBase3::display(int number)
{
    QLCDNumber::display(base3(number));
}

void LCDBase3::reset()
{
    display(0);
}

