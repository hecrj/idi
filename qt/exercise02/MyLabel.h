#include <QLabel>

class MyLabel: public QLabel
{
    Q_OBJECT;

    private:
        int red, green, blue;
        
    public:
        MyLabel(QWidget *parent);

    public slots:
        void setRed(int r);
        void setGreen(int g);
        void setBlue(int b);
        void confirm();
};
