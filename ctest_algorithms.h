#ifndef CTEST_ALGORITHMS_H
#define CTEST_ALGORITHMS_H

#include "typedef.h"

class CTestAlgorithms
{
public:
    CTestAlgorithms();
    virtual ~CTestAlgorithms()= 0;

    virtual TStr point() = 0;

    virtual bool hasNextPoint() = 0;
    virtual bool hasPrevPoint() = 0;

    virtual bool isBeginPoint() = 0;
    virtual bool isEndPoint() = 0;

    virtual void beginPoint() = 0;
    virtual void endPoint() = 0;
    virtual void nextPoint() = 0;
    virtual void prevPoint() = 0;

    virtual void executPoint() = 0;
};


class CTestAlgBFRandom : public CTestAlgorithms
{
public:
    typedef enum {NumX,NumZ,NumDis,LenDisMin,LenDisMax} TNameParam;

    virtual TStr point();

    virtual bool hasNextPoint() {return hasNextX() || hasNextY() ? true : false;}
    virtual bool hasPrevPoint() {return hasPrevX() || hasPrevY() ? true : false;}

    virtual bool isBeginPoint() {return !hasPrevPoint() ? true : false;}
    virtual bool isEndPoint() {return !hasNextPoint() ? true : false;}

    virtual void beginPoint();
    virtual void endPoint();
    virtual bool nextPoint();
    virtual bool prevPoint();

    virtual void executPoint();

private:

    bool hasNextX() {return m_xp < m_xPoints.size() - 1 ? true : false;}
    bool hasNextY() {return m_yp < m_yPoints.size() - 1 ? true : false;}
    bool hasPrevX() {return m_xp > 0 ? true : false;}
    bool hasPrevY() {return m_yp > 0 ? true : false;}

    void setPoint(){
        setPoint(m_xPoints(m_xp),m_xParam);
        setPoint(m_yPoints(m_yp),m_yParam);
    }

    void setPoint(int value, TNameParam param);

    const TNameParam m_xParam;
    const TNameParam m_yParam;

    TVec m_xPoints;
    TVec m_yPoints;

    int m_numTest;

    int m_numX;
    int m_numZ;
    int m_numDisMax;
    int m_lenDisMin;
    int m_lenDisMax;

    int m_xp;
    int m_yp;

};

class CTestAlgBFFile : public CTestAlgorithms
{

};

#endif // CTEST_ALGORITHMS_H
