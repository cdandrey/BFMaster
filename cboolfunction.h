#ifndef CBOOLFUNCTION_H
#define CBOOLFUNCTION_H

#include "typedef.h"

class CBoolFunction
{
public:

    typedef QSharedPointer<CBoolFunction> TSPtr;

    typedef struct CBoolFunctionParam {

        CBoolFunctionParam() :
            numX(2),numZ(2),numDisMax(1),
            lenDisMin(2),lenDisMax(2) {}

        CBoolFunctionParam(int nX,int nZ,int nMaxDis,int lMin,int lMax) :
            numX(nX),numZ(nZ),numDisMax(nMaxDis),
            lenDisMin(lMin),lenDisMax(lMax) {}

        int numX;           // количество переменных в функции
        int numZ;           // количество противоположных переменных в функции
        int numDisMax;      // максимально возможное количество дизьюнктов в функции
        int lenDisMin;      // минимальное количество переменных в дизьюнктах
        int lenDisMax;      // максимальное количество переменных в дизъюнктах

    } TParam;

    explicit CBoolFunction(int numX = 3,int numDisMax = 6) {
        init(CBoolFunctionParam(numX,0,numDisMax,0,0));
    }

    explicit CBoolFunction(const TParam &p) {
        init(p);
    }

    CBoolFunction(const CBoolFunction &f);

    explicit CBoolFunction(const TLLst &cnf){
        init(cnf);
    }

    const CBoolFunction& operator=(const CBoolFunction &rhs);

    void init(const CBoolFunction::TParam & p);
    void init(const TLLst &cnf);
    
    int varNot(int x) const{
        return x < m_numX ? x + m_numX : x - m_numX;
    }

    int varDIMACS(int x) const{
        return x < m_numX ? (x + 1) : (m_numX - x - 1);
    }

    void generate(double possibleCombinations = 0);
    int multiply(int x,int &r_restNumDis,TStr &r_state,TLst &r_seq);


    int numDis() const {return m_dis.size();}
    int numDisMax() const {return m_numDisMax;}
    int numVar() const {return m_var.size();}

    int numXZ() const {return m_numX + m_numZ;}
    int numX() const {return m_numX;}
    int numZ() const {return m_numZ;}
    int lenDisMin() const {return m_lenDisMin;}
    int lenDisMax() const {return m_lenDisMax;}

    TParam param() const;
    const TLLst& dis() const {return m_dis;}
    const TLLst& var() const {return m_var;}

    const TMapSatData& satdata() const {return m_satdata;}
    void addSatdata(TStr name,TSatData *satdata){
        m_satdata.insert(name,satdata);
    }

private:

    int m_numX;
    int m_numZ;
    int m_numDisMax;
    int m_lenDisMin;
    int m_lenDisMax;

    TLLst m_dis;
    TLLst m_var;

    TMapSatData m_satdata;

    inline bool absorb(const TLst &lessDecDis,
                       const TByt &lessBitDis,
                       const TByt &mostBitDis);

    inline void addDis(TLst &dis);

    void fillMatrixCombinations(TVVecd &c);
};

#endif // CBOOLFUNCTION_H
