#include "ccombinasion.h"

CCombinasion::CCombinasion(unsigned n):
    m_num(n),
    m_numsComb(numbersCombinasion(m_num))
{
}
//---------------------------------------------------------------


CCombinasion::TLLull CCombinasion::numbersCombinasion(unsigned n)
{
    TLull lstInit;
    for(unsigned i = 0; i <= n; ++i)
        lstInit << 0;

    TLLull c;
    for(unsigned i = 0; i <= n; ++i)
        c << lstInit;

    for(unsigned i = 0; i <= n; ++i) {

       c[i][0] = 1;
       c[i][i] = 1;

       for(unsigned j = 1; j < i; ++j)
           c[i][j] = c.at(i-1).at(j-1) + c.at(i-1).at(j);
    }

    return c;
}
//---------------------------------------------------------------
