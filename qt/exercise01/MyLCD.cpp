#include <QLCDNumber>
#include "MyLCD.h"

MyLCD::MyLCD(QWidget *parent): QLCDNumber(parent) {}
void MyLCD::display(int n)
{
    if(n == 0) setStyleSheet(QString("color: green"));
    else if(n & 1) setStyleSheet(QString("color: red"));
    else setStyleSheet(QString("color: blue"));
    
    QLCDNumber::display(n);
}

void MyLCD::reset()
{
    display(0);
}
