#ifndef CHECK_H
#define CHECK_H

#include <QString>

class Check
{
private:
    Check();
    Check(Check const&);
    void operator=(Check const&);
public:
    static bool isCorrectAdress(QString const& adress);
    static bool isCorrectPort(QString const& adress);
};

#endif // CHECK_H
