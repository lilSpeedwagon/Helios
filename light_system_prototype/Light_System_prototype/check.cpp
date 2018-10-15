#include "check.h"

bool Check::isCorrectAdress(QString const& adress) {
    int dotNumber = 0;
    for (int i = 0; i < adress.length(); i++)   {
        if (adress[i] == '.')
            dotNumber++;
    }
    if (dotNumber == 3) {
        int numberCount = 0;
        for (int i = 0; i < 3; i++) {
            bool ok;
            int num = adress.section('.', i, i).toInt(&ok);
            if (num >= 0 && num < 256 && ok)
                numberCount++;
        }
        if (numberCount == 3)
            return true;
    }
    return false;
}

//default is 8888
bool Check::isCorrectPort(QString const& port)  {
    return port.toInt();
}
