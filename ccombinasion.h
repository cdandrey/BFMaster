#ifndef CCOMBINASION_H
#define CCOMBINASION_H

#include <QList>

class CCombinasion
{
//public memeber functions
public:

    typedef unsigned long long ull;
    typedef QList<ull>         TLull;
    typedef QList<TLull>       TLLull;

    explicit CCombinasion(unsigned n);

    ull numberCombinasion(unsigned m) const {return (m > m_num) ? 0 : m_numsComb.at(m_num).at(m);}

    ull numberCombinasion(unsigned n,unsigned m) const {return (n > m_num) || (m > m_num) ? 0 : m_numsComb.at(n).at(m);}

    static TLLull numbersCombinasion(unsigned n);

//private member functions
private:
    CCombinasion();

//private members
private:

    unsigned m_num;
    TLLull m_numsComb;
};

#endif // CCOMBINASION_H
