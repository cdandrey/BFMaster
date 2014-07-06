#include "cbf_view.h"

const TStr CBFView::descriptView = QObject::tr("Общая информация функции");
const TStr CBFView::descriptViewDIMACS = QObject::tr("Представление функции в формате DIMACS");
const TStr CBFView::descriptViewDis = QObject::tr("Список дизъюнктов функции");
const TStr CBFView::descriptViewDisSort = QObject::tr("Упорядоченный список дизъюнктов функции");
const TStr CBFView::descriptViewVar = QObject::tr("Список переменных функции");
const TStr CBFView::descriptViewVarSort = QObject::tr("Упорядоченный список переменных функции");

CBFView::CBFView(CBoolFunction *f,const TStr &name,const TStr &path)
    : m_bf(f),m_name(name),m_pathFile(path)
{
}
//--------------------------------------------------


CBFView::CBFView(CBFView *bf)
{
    setBoolFunction(bf->boolFunction());
    setBoolFunctionName(bf->boolFunctionName());
    setPathFile(bf->pathFile());
}
//--------------------------------------------------


CBFView::~CBFView()
{
}
//--------------------------------------------------


TStr CBFView::view() const
{
    TStr html = "";

    // table parameters of boolean function

    html += TStr("<table %1>").arg(TableBorderStyle);
    html += TStr("<tr><td colspan = '2'>%1</td></tr>")
                .arg(QObject::tr("Параметры булевой функции"));

    html += TStr("<tr><td %1><b>%2</b></td><td %3>%4</td></tr>")
                .arg(BgColorC).arg(QObject::tr("Имя функции"))
                .arg(BgColorE).arg(boolFunctionName());

    html += TStr("<tr><td %1><b>%2</b></td><td %3>%4</td></tr>")
                .arg(BgColorC).arg(QObject::tr("Количество переменных"))
                .arg(BgColorE).arg(boolFunction()->numX());

    html += TStr("<tr><td %1><b>%2</b></td><td %3>%4</td></tr>")
                .arg(BgColorC).arg(QObject::tr("Количество противоположных переменных"))
                .arg(BgColorE).arg(boolFunction()->numZ());

    html += TStr("<tr><td %1><b>%2</b></td><td %3>%4</td></tr>")
                .arg(BgColorC).arg(QObject::tr("Количество дизьюнктов"))
                .arg(BgColorE).arg(boolFunction()->numDis());

    html += TStr("<tr><td %1><b>%2</b></td><td %3>%4</td></tr>")
                .arg(BgColorC).arg(QObject::tr("Минимальное количество переменных в дизьюнкте"))
                .arg(BgColorE).arg(boolFunction()->lenDisMin());

    html += TStr("<tr><td %1><b>%2</b></td><td %3>%4</td></tr>")
                .arg(BgColorC).arg(QObject::tr("Максимальное количество переменных в дизьюнкте"))
                .arg(BgColorE).arg(boolFunction()->lenDisMax());

    return html += "</table>";
}
//--------------------------------------------------


TStr CBFView::viewDIMACS() const
{
    TStr str = TStr("p cnf %1 %2\n")
            .arg(boolFunction()->numX())
            .arg(boolFunction()->numDis());

    const TLLst &dis = boolFunction()->dis();

    for (int i = 0; i < dis.size(); ++i) {

        foreach (int x, dis.at(i))
            str += TStr("%1 ").arg(boolFunction()->varDIMACS(x));

        str += "0\n";
    }

    return str;
}
//--------------------------------------------------


TStr CBFView::viewDis() const
{
    int num = boolFunction()->numDis();
    int numView = num > 1000 ? 1000 : num; // number view disjunction

    TStr html = "";

    if (numView < num)
        html = QObject::tr("<p><i> Функция большая"
             ", выводится только первые тысячу дизьюнктов</i></p>");

    html += TStr("<table %1 width='100%'>").arg(TableBorderStyle);
    html += TStr("<tr><td colspan = '3'>%1</td></tr>")
            .arg(descriptViewDis);

    // write disjunction in rows of table
    TStr trColor = "";
    for (int i = 0; i < numView; ++i) {

        trColor = i % 2 > 0 ? BgColorE : BgColorF;

        html += TStr("<tr><td width='3%'>%1</td><td %2 width='7%'><b>%3</b> (%4)</td><td %5>")
                    .arg(i + 1).arg(BgColorC).arg(htmlDis(i))
                    .arg(boolFunction()->dis().at(i).size()).arg(trColor);

        foreach (int x, boolFunction()->dis().at(i))
            html += TStr("%1 ").arg(htmlVar(x));

        html += "</td></tr>";
    }

    return html += "</table>";
}
//--------------------------------------------------


TStr CBFView::viewDisSort() const
{
    TVLst lstSort(numericSort(boolFunction()->dis(),boolFunction()->lenDisMax() + 1));

    int num = boolFunction()->numDis();
    int numView = num > 1000000 ? 1000 : num; // number view disjunction

    TStr html = "";

    if (numView < num)
        html = QObject::tr("<p><i> Функция большая"
             ", выводится только первые тысячу дизьюнктов</i></p>");

    html += TStr("<table %1 width='100%'>").arg(TableBorderStyle);
    html += TStr("<tr><td colspan = '3'>%1</td></tr>")
            .arg(descriptViewDisSort);

    // write disjunction in rows of table
    int count = 0;
    for (int i = 1; i < lstSort.size(); ++i) {

        qSort(lstSort[i]);

        foreach (int d, lstSort.at(i)) {

            TStr trColor = count % 2 > 0 ? BgColorE : BgColorF;

            html += TStr("<tr><td  width='3%'>%1</td><td %2 width='7%'><b>%3</b> (%4)</td><td %5>")
                        .arg(count + 1).arg(BgColorC).arg(htmlDis(d))
                        .arg(boolFunction()->dis().at(d).size()).arg(trColor);

            foreach (int x, boolFunction()->dis().at(d))
                html += TStr("%1 ").arg(htmlVar(x));

            html += "</td></tr>";

            ++count;

            if (count == numView)
                return html += "</table>";
        }
    }

    return html += "</table>";
}
//--------------------------------------------------


TStr CBFView::viewVar() const
{
    int num = boolFunction()->numXZ();
    int numView = num > 100000 ? 100 : num; // number view variables

    TStr html = "";

    if (numView < num)
        html = QObject::tr("<p><i> Функция большая"
             ", выводится только первые сто переменных</i></p>");

    html += TStr("<table %1 width='100%'>").arg(TableBorderStyle);
    html += TStr("<tr><td colspan = '3'>%1</td></tr>")
            .arg(descriptViewVar);

    // write variable in rows of table
    for (int i = 0; i < numView; ++i) {

        TStr trColor = i % 2 > 0 ? BgColorE : BgColorF;

        html += TStr("<tr><td  width='3%'>%1</td><td %2 width='7%'><b>%3 </b>(%4)</td><td %5>")
                    .arg(i + 1).arg(BgColorC).arg(htmlVar(i))
                    .arg(boolFunction()->var().at(i).size()).arg(trColor);

        foreach (int d, boolFunction()->var().at(i))
            html += TStr("%1 ").arg(htmlDis(d));

        html += "</td></tr>";
    }

    return html += "</table>";
}
//--------------------------------------------------


TStr CBFView::viewVarSort() const
{
    TVLst lstSort(numericSort(boolFunction()->var()));

    int num = boolFunction()->numXZ();
    int numView = num > 100 ? 100 : num; // number view variables

    TStr html = "";

    if (numView < num)
        html = QObject::tr("<p><i> Функция большая"
             ", выводится только первые сто переменных</i></p>");

    html += TStr("<table %1 width='100%'>").arg(TableBorderStyle);
    html += TStr("<tr><td colspan = '3'>%1</td></tr>")
            .arg(descriptViewVarSort);

    // write variable in rows of table
    int count = 0;
    for (int i = 1; i < lstSort.size(); ++i) {

        qSort(lstSort[i]);

        foreach (int x, lstSort.at(i)) {

            TStr trColor = count % 2 > 0 ? BgColorE : BgColorF;

            html += TStr("<tr><td  width='3%'>%1</td><td %2 width='7%'><b>%3 </b>(%4)</td><td %5>")
                        .arg(count + 1).arg(BgColorC).arg(htmlVar(x))
                        .arg(boolFunction()->var().at(x).size()).arg(trColor);

            foreach (int d, boolFunction()->var().at(x))
                html += TStr("%1 ").arg(htmlDis(d));

            html += "</td></tr>";

            ++count;

            if (count == numView)
                return html += "</table>";
        }
    }

    return html += "</table>";
}
//--------------------------------------------------


TStr CBFView::viewSat() const
{
    TStr html = "";

    //table satisfiability of the boolean function

    html += TStr("<table %1>").arg(TableBorderStyle);
    html += TStr("<tr><td colspan = '5'>%1</td></tr>")
                .arg(QObject::tr("Выполнимость булевой функции"));

    html += TStr("<tr %1><td width = '20%'><b>%2</b></td>")
                .arg(BgColorC)
                .arg(QObject::tr("Название метода"));

    html += TStr("<td width = '20%'><b>%2</b></td>")
                .arg(QObject::tr("Состояние"));

    html += TStr("<td width = '40%'><b>%1</b></td>")
                .arg(QObject::tr("Наборы выполнимости"));

    html += TStr("<td width = '10%'><b>%1</b></td>")
                .arg(QObject::tr("Количество операций"));

    html += TStr("<td width = '10%'><b>%1</b></td></tr>")
                .arg(QObject::tr("Время, мс"));

    TMapSatData satdata = boolFunction()->satdata();
    TMapSatData::const_iterator it = satdata.cbegin();

    TStr tmp = "";

    for (;it != satdata.cend(); ++it){

        tmp += TStr("<tr><td %1>%2</td>")
                    .arg(BgColorE).arg(it.value()->description);

        tmp += TStr("<td %1>%2</td>")
                    .arg(BgColorE).arg(it.value()->state);

        TStr sequence = "";
        foreach (int val, it.value()->sequence)
            sequence += TStr("%1 ").arg(htmlVar(val));

        tmp += TStr("<td %1>%2</td>")
                    .arg(BgColorE).arg(sequence);

        tmp += TStr("<td %1>%2</td>")
                    .arg(BgColorE).arg(it.value()->quantity);

        tmp += TStr("<td %1>%2</td></tr>")
                    .arg(BgColorC).arg(it.value()->time / 1000000);
    }

    if (tmp.isEmpty())
        tmp = TStr("<tr %1><td colspan = '5'><i>%2</i></td></tr>")
                    .arg(BgColorE)
                    .arg(QObject::tr("Проверка функции на выполнимость не производилась."));

    html += tmp;

    return html += "</table>";
}
//--------------------------------------------------


inline TStr CBFView::htmlVar(int var) const
{
    int numVar = boolFunction()->numX();

    return var < numVar ? TStr("X<sub>%1</sub>").arg(var + 1) :
                          TStr("Z<sub>%1</sub>").arg(var - numVar + 1);
}
//--------------------------------------------------


inline TStr CBFView::htmlDis(int dis) const
{
    return TStr("D<sub>%1</sub>").arg(dis + 1);
}
//--------------------------------------------------


TVLst CBFView::numericSort(const TLLst &lst, int len) const
{
    if (len == 0) {
        for (int i = 0; i < lst.size(); ++i)
            if (len < lst.at(i).size())
                len = lst.at(i).size();

        ++len;
    }

    TVLst lstSort(len,TLst());
    lstSort.reserve(len);

    for (int i = 0; i < lst.size(); ++i)
        lstSort[lst.at(i).size()] << i;

    return lstSort;
}
//--------------------------------------------------
