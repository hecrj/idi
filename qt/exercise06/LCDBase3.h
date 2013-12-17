#include <QLCDNumber>

class LCDBase3: public QLCDNumber
{
    Q_OBJECT;

    public:
        LCDBase3(QWidget *parent);

    public slots:
        void display(int);
        void reset();
};
