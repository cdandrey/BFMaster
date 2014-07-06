#include <QTime>
#include "cbf_generate.h"
#include "cbf_view.h"


TStr CBFGenerate::progress() const
{
    if (this->isFinished())
        return "";

    return tr("осталось комбинаций: %1 - cозданно дизъюнктов: %2")
            .arg(m_combinations - m_createCombinations)
            .arg(m_createDis);
}
//-----------------------------------------------------------------------


TStr CBFGenerate::progressFinished() const
{
    TStr str = "";

    if (m_stopped) {

        str = tr("Функция не созданна. Процесс был остановлен.");

    } else {

        str = tr("cозданно дизъюнктов - %1").arg(m_createDis);

        if (m_createDis < m_numDisMax)

            str = tr("cоздано максимально возможное количество дизьюнктов - %1")
                                .arg(m_createDis);

        else if (m_createDis > m_numDisMax)

            str = tr("cоздано минимально возможное количество дизьюнктов - %1")
                                .arg(m_createDis);

        str += tr("\nфункция %1 создана успешно.\n")
                    .arg(m_bfv->boolFunctionName());
    }

    return str;
}
//-----------------------------------------------------------------------


TStr CBFGenerate::progressDescription() const
{
    return TStr("%1 %2 ")
            .arg(tr("Создается функция"))
            .arg(m_bfv->boolFunctionName());
}
//-----------------------------------------------------------------------


void CBFGenerate::run()
{
    qsrand(QTime::currentTime().msec());

    int numX = m_bfv->boolFunction()->numX();
    int numZ = m_bfv->boolFunction()->numZ();
    m_numDisMax = m_bfv->boolFunction()->numDisMax();
    int lenDisMin = m_bfv->boolFunction()->lenDisMin();
    int lenDisMax = m_bfv->boolFunction()->lenDisMax();

    TLLst dis;
    TLLst cnf;

    int n = numX * 2;

    // список всех созданных комбинаций
    QSet<TByt> createComb;

    // список уникальных комбинаций в двоичном представлении
    TLByt lstBitComb;

    // создаем минимально возможное количество дизьюнктов

    int nXZ = numX + numZ;

    // вектор флагов использования переменной в выражении
    TVec used(numX * 2,0);
    used.reserve(numX * 2);

    // номера переменных
    TLst nX;
    nX.reserve(numX);
    for (int i = 0; i < numX; ++i)
        nX << i;

    // номера противоположных переменных
    TLst nZ;
    nZ.reserve(numZ);
    for (int i = numX; i < nXZ; ++i)
        nZ << i;

    // максимальная длина дизьюнкта которую можно сформировать из оставшихся переменных
    int maxPossiblLen = numX;

    while (!nX.isEmpty() || !nZ.isEmpty()) {

        int rndVar;

        TByt bitComb(n,'0');
        TLst decComb;
        TLst cnfComb;

        while (decComb.size() < lenDisMax) {

            if (nX.size() == 1 || (nZ.isEmpty() && !nX.isEmpty())) {
                rndVar = nX.at(0);
            }
            else if (nZ.size() == 1 || (nX.isEmpty() && !nZ.isEmpty())) {
                rndVar = nZ.at(0);
            }
            else {
                do {

                    rndVar = qrand() % nXZ;    // случайный номер [0..n)

                } while (bitComb.at(rndVar) == '1' ||
                         bitComb.at(m_bfv->boolFunction()->varNot(rndVar)) == '1' ||
                         ((used.at(rndVar) == 1) && maxPossiblLen >= lenDisMax));
            }

            used[rndVar] = 1;
            bitComb[rndVar] = '1';
            decComb << rndVar;
            cnfComb << m_bfv->boolFunction()->varDIMACS(rndVar);
            if (rndVar < numX)
                nX.removeOne(rndVar);
            else
                nZ.removeOne(rndVar);

            if (used.at(m_bfv->boolFunction()->varNot(rndVar)) == 1 ||
                (rndVar < numX && rndVar > numZ - 1))
                --maxPossiblLen;
        }

        if (createComb.contains(bitComb))
            continue;

        dis << decComb;
        cnf << cnfComb;
        m_createDis = dis.size();
        lstBitComb << bitComb;
        createComb << bitComb;
    }

    // продолжаем создавать дизьюнкты функции до заданного количества

    // определяем возможное количество комбинаций дизьюнктов
    TVVecd matrix(nXZ + 1,TVecd(nXZ + 1,0));
    fillMatrixCombinations(matrix);
    for (int k = lenDisMin; k <= lenDisMax; ++k)
        m_combinations += static_cast<double>(matrix.at(nXZ).at(k));

    // diffMaxMin - используется для смещения диапозона
    // случайных чисел в границы [m_lenDisMin,m_lenDisMax]
    // +1  - для того что бы m_lenDisMax входил в диапозон
    int diffMaxMin = lenDisMax - lenDisMin + 1;

    m_createCombinations = static_cast<double>(createComb.size());

    while (m_createCombinations < m_combinations && m_createDis < m_numDisMax) {

        if (m_stopped)
            return;

        // получаем случайное количество переменных
        // для нового дизъюнкта в пределах [m_lenDisMin,m_lenDisMax]
        int rndVarDis = (qrand() % diffMaxMin ) + lenDisMin;

        // случайный номер переменной
        int rndVar = 0;

        TByt bitComb(n,'0');

        // десятичное представление комбинации, множество номеров переменных
        TLst decComb;
        TLst cnfComb;

        // создаем комбинацию переменных
        while (decComb.size() < rndVarDis) {

            do {
                rndVar = qrand() % nXZ;    // случайный номер [0..n)
            } while (bitComb.at(rndVar) == '1');

            bitComb[rndVar] = '1';
            decComb << rndVar;
            cnfComb << m_bfv->boolFunction()->varDIMACS(rndVar);
        }

        // проверяем создавалась ли уже полученная комбинация
        // если да то переходим к созданию новой комбинации
        if (createComb.contains(bitComb))
            continue;

        createComb << bitComb;

        m_createCombinations = static_cast<double>(createComb.size());

        // проверяем содержит ли комбинация противоположные переменные
        bool isConflict = false;

        foreach (int x, decComb)
            if (bitComb.at(m_bfv->boolFunction()->varNot(x)) == '1') {
                isConflict = true;
                break;
            }

        if (isConflict)
            continue;

        // проверяем поглощается ли новая комбинация уже
        // добавленными в функцию дизъюнктами
        bool isAbsorb = false;

        for (int i = 0; i < dis.size(); ++i) {

            // комбинации одинаковый длины не могут поглотить друг друга
            if (dis.at(i).size() == decComb.size())
                continue;

            if (dis.at(i).size() > decComb.size())
                isAbsorb = absorb(decComb,bitComb,lstBitComb.at(i));
            else
                isAbsorb = absorb(dis.at(i),lstBitComb.at(i),bitComb);

            if (isAbsorb)
                break;
        }

        // если новая комбинация не поглащается и не поглащает уже созданных
        // добавляем ее в список комбинаций и в список дизъюнктов
        if (!isAbsorb) {
            dis << decComb;
            cnf << cnfComb;
            m_createDis = dis.size();
            lstBitComb << bitComb;
        }
    }

    m_bfv->boolFunction()->init(cnf);

    emit successfull(m_bfv);
}
//-----------------------------------------------------------------------


inline bool CBFGenerate::absorb(const TLst &lessDecDis,
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


void CBFGenerate:: fillMatrixCombinations(TVVecd &c)
{
    int n = c.size();

    for(int i = 0; i < n; ++i) {

       c[i][0] = 1;
       c[i][i] = 1;

       for(int j = 1; j < i; ++j)
           c[i][j] = c[i-1][j-1] + c[i-1][j];
    }
}
//-------------------------------------------------------------------


