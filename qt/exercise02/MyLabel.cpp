#include <QLabel>
#include <sstream>
#include "MyLabel.h"

MyLabel::MyLabel(QWidget *parent): QLabel(parent) {}
void MyLabel::setRed(int r)
{
    red = r;
}

void MyLabel::setGreen(int g)
{
    green = g;
}

void MyLabel::setBlue(int b)
{
    blue = b;
}

void MyLabel::confirm()
{
    std::stringstream ss;
    ss << "background: rgb(" << red << ", " << green << ", " << blue << ")";
    setStyleSheet(QString::fromStdString(ss.str()));
}
