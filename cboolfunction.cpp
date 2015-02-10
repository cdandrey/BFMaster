#include "cboolfunction.h"
#include "ccombinasion.h"

#include <QTime>

void CBoolFunction::init(const TParam &p)
{
    // acceptable value >= 2
    m_numX = p.numX < 2 ? 2 : p.numX;

    // acceptable value [2 ... m_numX]
    m_numZ = (p.numZ > m_numX) ||
             (p.numZ < 2) ? m_numX : p.numZ;

    // acceptable value >= 1
    m_numDisMax = p.numDisMax < 1 ? 1 : p.numDisMax;

    // acceptable value [2 ... m_numX]
    m_lenDisMin = (p.lenDisMin < 2) ||
                  (p.lenDisMin > m_numX)
                   ? 2 : p.lenDisMin;

    // acceptable value [m_lenDisMin ... m_numX]
    m_lenDisMax = (p.lenDisMax < m_lenDisMin)||
                  (p.lenDisMin > m_numX)
                  ? m_numX : p.lenDisMax;

    //
    m_dis.clear();
    m_var.clear();
    m_satdata.clear();
}
//-------------------------------------------------------------------


void CBoolFunction::init(const TLLst &cnf)
{
    m_numDisMax = cnf.size();

    // find m_numX, m_lenDisMin, m_lenDisMax and append X in disjunction
    m_numX = 0;
    m_lenDisMin = cnf.at(0).size();
    m_lenDisMax = 0;

    m_dis = TLLst();

    for (int i = 0; i < cnf.size(); ++i) {

        TLst idis;

        foreach (int x, cnf.at(i)) {

            if (x > 0) {

                if (x > m_numX)
                    m_numX = x;

                --x;    // in function numeric var is begin with 0

                idis << x;
            }
        }

        if (cnf.at(i).size() > m_lenDisMax)
            m_lenDisMax = cnf.at(i).size();

        if (cnf.at(i).size() < m_lenDisMin)
            m_lenDisMin = cnf.at(i).size();

        m_dis << idis;
    }

    // find m_numZ and append Z in m_dis
    m_numZ = 0;

    int delta = m_numX - 1;

    for (int i = 0; i < cnf.size(); ++i) {

        foreach (int z, cnf.at(i)) {

            if (z < 0) {

                z = static_cast<int>(qFabs(z));
                if (z > m_numZ)
                    m_numZ = z;

                m_dis[i] << (z + delta);

            }
        }

        qSort(m_dis[i]);
    }

    // create m_var
    int numXZ = m_numX + m_numZ;

    m_var = TLLst();
    for (int i = 0; i < numXZ; ++i)
        m_var << TLst();

    for (int i = 0; i < m_dis.size(); ++i)
        foreach (int x, m_dis.at(i)) {
            m_var[x] << i;
        }

    for (int i = 0; i < m_dis.size(); ++i)
        qSort(m_dis[i]);

    for (int i = 0; i < m_var.size(); ++i)
        qSort(m_var[i]);

    m_satdata.clear();
}
//---------------------------------------------------------------


void CBoolFunction::genBegin()
{
    qsrand(QTime::currentTime().msec());

    m_dis.clear();
    m_var.clear();

    int n = m_numX * 2;
    for (int i = 0; i < n; ++i)
        m_var << TLst();

    m_satdata.clear();

    QSet<TByt> createComb;  // список всех созданных комбинаций
    TLByt      lstBitComb;  // список уникальных комбинаций в двоичном представлении

    // создаем минимально возможное количество дизьюнктов

    int nXZ = m_numX + m_numZ;

    TVec used(m_numX * 2,0);    // вектор флагов использования переменной в выражении
    used.reserve(m_numX * 2);


    TLst nX;    // номера переменных
    TLst nZ;    // номера противоположных переменных

    nX.reserve(m_numX);
    for (int i = 0; i < m_numX; ++i)
        nX << i;

    nZ.reserve(m_numZ);
    for (int i = m_numX; i < nXZ; ++i)
        nZ << i;

    int maxPossiblLen = m_numX; // максимальная длина дизьюнкта которую можно сформировать из оставшихся переменных

    while (!nX.isEmpty() || !nZ.isEmpty()) {

        int rndVar;

        TByt bitComb(n,'0');
        TLst decComb;

        while (decComb.size() < m_lenDisMax) {

            if (nX.size() == 1 || (nZ.isEmpty() && !nX.isEmpty())) {
                rndVar = nX.at(0);
            }
            else if (nZ.size() == 1 || (nX.isEmpty() && !nZ.isEmpty())) {
                rndVar = nZ.at(0);
            }
            else {
                // берем случайный номер пока он или его противоположный
                // не присутсвует в комбинации или пока он небыл уже использован в предыдущих
                // комбинациях если количество оставшихся переменных позволяет
                // сформировать комбинацию максимального размера
                do {

                    rndVar = qrand() % nXZ;    // случайный номер [0..n)

                } while (bitComb.at(rndVar) == '1' ||
                         bitComb.at(varNot(rndVar)) == '1' ||
                         ((used.at(rndVar) == 1) && maxPossiblLen >= m_lenDisMax));
            }

            used[rndVar] = 1;
            bitComb[rndVar] = '1';
            decComb << rndVar;
            if (rndVar < m_numX)
                nX.removeOne(rndVar);
            else
                nZ.removeOne(rndVar);

            if (used.at(varNot(rndVar)) == 1 ||
                (rndVar < m_numX && rndVar > m_numZ - 1))
                --maxPossiblLen;
        }

        if (createComb.contains(bitComb))
            continue;

        addDis(decComb);
        lstBitComb << bitComb;
        createComb << bitComb;
    }
}
//---------------------------------------------------------------


void CBoolFunction::generate(ull_t possibleCombinations)
{
    qsrand(QTime::currentTime().msec());

    m_dis.clear();
    m_var.clear();

    int n = m_numX * 2;
    for (int i = 0; i < n; ++i)
        m_var << TLst();

    m_satdata.clear();

    // список всех созданных комбинаций
    QSet<QByteArray> createComb;

    // список уникальных комбинаций в двоичном представлении
    QList<QByteArray> lstBitComb;

    // создаем минимально возможное количество дизьюнктов

    int nXZ = m_numX + m_numZ;

    // вектор флагов использования переменной в выражении
    TVec used(m_numX * 2,0);
    used.reserve(m_numX * 2);

    // номера переменных
    TLst nX;
    nX.reserve(m_numX);
    for (int i = 0; i < m_numX; ++i)
        nX << i;

    // номера противоположных переменных
    TLst nZ;
    nZ.reserve(m_numZ);
    for (int i = m_numX; i < nXZ; ++i)
        nZ << i;

    // максимальная длина дизьюнкта которую можно сформировать из оставшихся переменных
    int maxPossiblLen = m_numX;

    while (!nX.isEmpty() || !nZ.isEmpty()) {

        int rndVar;

        TByt bitComb(n,'0');
        TLst decComb;

        while (decComb.size() < m_lenDisMax) {

            if (nX.size() == 1 || (nZ.isEmpty() && !nX.isEmpty())) {
                rndVar = nX.at(0);
            }
            else if (nZ.size() == 1 || (nX.isEmpty() && !nZ.isEmpty())) {
                rndVar = nZ.at(0);
            }
            else {
                // берем случайный номер пока он или его противоположный
                // не присутсвует в комбинации или пока он небыл уже использован в предыдущих
                // комбинациях если количество оставшихся переменных позволяет
                // сформировать комбинацию максимального размера
                do {

                    rndVar = qrand() % nXZ;    // случайный номер [0..n)

                } while (bitComb.at(rndVar) == '1' ||
                         bitComb.at(varNot(rndVar)) == '1' ||
                         ((used.at(rndVar) == 1) && maxPossiblLen >= m_lenDisMax));
            }

            used[rndVar] = 1;
            bitComb[rndVar] = '1';
            decComb << rndVar;
            if (rndVar < m_numX)
                nX.removeOne(rndVar);
            else
                nZ.removeOne(rndVar);

            if (used.at(varNot(rndVar)) == 1 ||
                (rndVar < m_numX && rndVar > m_numZ - 1))
                --maxPossiblLen;
        }

//        if (createComb.contains(bitComb))
//            continue;

        addDis(decComb);
        lstBitComb << bitComb;
        createComb << bitComb;
    }

    // продолжаем создавать дизьюнкты функции до заданного количества

    // если возможное количество комбинаций дизьюнктов не заданно - определяем его
    if (possibleCombinations == 0){

        // максимальное число комбинаций переменных в дизъюнктах
        // без учета всех требований булевой функции равно сумме числа сочетаний
//        CCombinasion comb(nXZ);
//        for (int k = m_lenDisMin; k <= m_lenDisMax; ++k)
//            possibleCombinations += comb.numberCombinasion(k);
        possibleCombinations = 1000000;
    }

    // diffMaxMin - используется для смещения диапозона
    // случайных чисел в границы [m_lenDisMin,m_lenDisMax]
    // +1  - для того что бы m_lenDisMax входил в диапозон
    int diffMaxMin = m_lenDisMax - m_lenDisMin + 1;

    ull_t numCreateComb = static_cast<ull_t>(createComb.size());

    while (numCreateComb < possibleCombinations && numDis() < numDisMax()) {

        // получаем случайное количество переменных
        // для нового дизъюнкта в пределах [m_lenDisMin,m_lenDisMax]
        int rndVarDis = (qrand() % diffMaxMin ) + m_lenDisMin;

        // случайный номер переменной
        int rndVar = 0;

        // двоичное представление комбинации: индекс - номер переменной,
        // значение - 0 переменной нет в комбинации, 1 - есть
        // размер задаем двойной от m_cntVar что бы не выйти за пределы
        // вектора если m_cntNotVar < m_cntVar
        TByt bitComb(n,'0');

        // десятичное представление комбинации, множество номеров переменных
        TLst decComb;

        // создаем комбинацию переменных
        while (decComb.size() < rndVarDis) {

            do {
                rndVar = qrand() % nXZ;    // случайный номер [0..n)
            } while (bitComb.at(rndVar) == '1');

            bitComb[rndVar] = '1';
            decComb << rndVar;
        }

        // проверяем создавалась ли уже полученная комбинация
        // если да то переходим к созданию новой комбинации
        if (createComb.contains(bitComb))
            continue;

        createComb << bitComb;

        numCreateComb = static_cast<ull_t>(createComb.size());

        // проверяем содержит ли комбинация противоположные переменные
        bool isConflict = false;

        foreach (int x, decComb)
            if (bitComb.at(varNot(x)) == '1') {
                isConflict = true;
                break;
            }

        if (isConflict)
            continue;

        // проверяем поглощается ли новая комбинация уже
        // добавленными в функцию дизъюнктами
        bool isAbsorb = false;

        for (int i = 0; i < m_dis.size(); ++i) {

            // комбинации одинаковый длины не могут поглотить друг друга
            if (m_dis.at(i).size() == decComb.size())
                continue;

            if (m_dis.at(i).size() > decComb.size())
                isAbsorb = absorb(decComb,bitComb,lstBitComb.at(i));
            else
                isAbsorb = absorb(m_dis.at(i),lstBitComb.at(i),bitComb);

            if (isAbsorb)
                break;
        }

        // если новая комбинация не поглащается и не поглащает уже созданных
        // добавляем ее в список комбинаций и в список дизъюнктов
        if (!isAbsorb) {
            addDis(decComb);
            lstBitComb << bitComb;
        }
    }
}
//-------------------------------------------------------------------


int CBoolFunction::multiply(int x, int &r_restNumDis, TStr &r_state, TLst &r_seq)
{
    int quantity = 0;

    // удаляем дизьюнкты сожедржащие x
    foreach (int di, m_var.at(x)) {

        quantity += m_dis.at(di).size();

        // удаляем ссылку на дизюнкт у других переменных
        foreach (int xj, m_dis.at(di))
            m_var[xj].removeOne(di);

        m_dis[di].clear();
        --r_restNumDis;
    }

    // удаляем из функции переменные противоположные x
    ++quantity;

    int z = varNot(x);
    TLst disZ(m_var.at(z));

    m_var[z].clear();

    foreach (int di, disZ) {

        m_dis[di].removeOne(z);

        ++quantity;
        if (m_dis.at(di).size() == 1) {

            int xRest = m_dis[di].takeFirst();
            int zRest = varNot(xRest);
            m_var[xRest].removeOne(di);
            --r_restNumDis;

            ++quantity;
            if (!r_seq.contains(xRest))
                r_seq << xRest;

            ++quantity;
            if (r_seq.contains(zRest)) {
                r_state = Unsatisfiable;
                return quantity;
            }
        }
    }

    ++quantity;
    if (r_restNumDis == 0)
        r_state = Satisfiable;

    return quantity;
}
//-------------------------------------------------------------------


CBoolFunction::TParam CBoolFunction::param() const
{
    TParam p;
    p.numX = m_numX;
    p.numZ = m_numZ;
    p.numDisMax = numDisMax();
    p.lenDisMin = m_lenDisMin;
    p.lenDisMax = m_lenDisMax;

    return p;
}
//-------------------------------------------------------------------


inline bool CBoolFunction::absorb(const TLst &lessDecDis,
                                  const TByt &lessBitDis,
                                  const TByt &mostBitDis)
{
    foreach(int i,lessDecDis){
        if(lessBitDis.at(i) != mostBitDis.at(i))
            return false;
    }

    return true;
}
//-------------------------------------------------------------------


inline void CBoolFunction::addDis(TLst &dis)
{
    qSort(dis);

    foreach (int x, dis)
        m_var[x] << m_dis.size();

    m_dis << dis;
}
//-------------------------------------------------------------------
