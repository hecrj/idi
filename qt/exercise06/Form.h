#include "ui_base3.h"

class Form: public QWidget
{
    Q_OBJECT

    public:
        Form(QWidget *parent = 0);

    private:
        Ui::Form ui;
};
