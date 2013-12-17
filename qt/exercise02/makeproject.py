vars_dict = {
    'app_name': 'rgb', # Nom de l'arxiu ui sense extensio
    'form_name': 'Form', # Nom de la classe principal de la ui
    'promoted_name': 'MyLabel', # Nom de la classe a la que hagis fet promote
    'inherited_from': 'QLabel', # Nom de la classe de la qual hereda
    'methods' : [  # Capcaleres dels slots sense el void
        'setRed(int r);',
        'setGreen(int g);',
        'setBlue(int b);',
        'confirm();',
    ]
}

PRO_FILE_CONTENTS = """TEMPLATE = app
TARGET = main
DEPENDPATH += .
INCLUDEPATH += .
QT += core widgets gui
HEADERS += %(form_name)s.h %(promoted_name)s.h
FORMS += %(app_name)s.ui
SOURCES += main.cpp %(form_name)s.cpp %(promoted_name)s.cpp
"""

MAIN_CPP_CONTENTS ="""#include <QApplication>
#include "%(form_name)s.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    %(form_name)s form;
    form.show();
    return app.exec();
}
"""

FORM_H_CONTENTS = """#include "ui_%(app_name)s.h"

class %(form_name)s: public QWidget
{
    Q_OBJECT

    public:
        %(form_name)s(QWidget *parent = 0);

    private:
        Ui::%(form_name)s ui;
};
"""

FORM_CPP_CONTENTS = """#include "%(form_name)s.h"

%(form_name)s::%(form_name)s(QWidget *parent): QWidget(parent)
{
    ui.setupUi(this);
}
"""

PROMOTED_H_CONTENTS = """#include <%(inherited_from)s>

class %(promoted_name)s: public %(inherited_from)s
{
    Q_OBJECT;

    public:
        %(promoted_name)s(QWidget *parent);

    public slots:
        void """ + "\n        void ".join(vars_dict['methods']) + """
};
"""

PROMOTED_CPP_CONTENTS = """#include <%(inherited_from)s>
#include "%(promoted_name)s.h"

%(promoted_name)s::%(promoted_name)s(QWidget *parent): %(inherited_from)s(parent) {}
""" + "\n".join("void %(promoted_name)s::" + name for name in vars_dict['methods']) + """
"""

with open(vars_dict['app_name'] + '.pro', 'w') as pro_file:
    pro_file.write(PRO_FILE_CONTENTS % vars_dict)

with open('main.cpp', 'w') as pro_file:
    pro_file.write(MAIN_CPP_CONTENTS % vars_dict)

with open(vars_dict['form_name'] + '.h', 'w') as pro_file:
    pro_file.write(FORM_H_CONTENTS % vars_dict)

with open(vars_dict['form_name'] + '.cpp', 'w') as pro_file:
    pro_file.write(FORM_CPP_CONTENTS % vars_dict)

with open(vars_dict['promoted_name'] + '.h', 'w') as pro_file:
    pro_file.write(PROMOTED_H_CONTENTS % vars_dict)

with open(vars_dict['promoted_name'] + '.cpp', 'w') as pro_file:
    pro_file.write(PROMOTED_CPP_CONTENTS % vars_dict)
