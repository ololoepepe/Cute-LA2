#include "application.h"

#include <BApplicationServer>

#include <QDebug>
#include <QDir>
#include <QHash>
#include <QObject>
#include <QString>
#include <QStringList>

int main(int argc, char **argv)
{
    static const QString AppName = "Cute LA2";
    QString home = QDir::home().dirName();
    BApplicationServer s(9970 + qHash(home) % 10, AppName + "0" + home);
    int ret = 0;
    if (!s.testServer()) {
        Application app(argc, argv, AppName, "Andrey Bogdanov");
        //QObject::connect(&s, SIGNAL(messageReceived(QStringList)), &app, SLOT(messageReceived(QStringList)));
        s.listen();
        ret = app.exec();
    } else {
        s.sendMessage(argc, argv);
    }
    return ret;
}
