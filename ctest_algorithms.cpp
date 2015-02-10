#include "ctest_algorithms.h"

//CTestAlgorithms::CTestAlgorithms()
//{
//}
////------------------------------------------------------------


//CTestAlgorithms::~CTestAlgorithms()
//{
//}
////------------------------------------------------------------


//void CTestAlgBFRandom::beginPoint()
//{
//    m_xp = 0;
//    m_yp = 0;

//    setPoint();
//}

//void CTestAlgBFRandom::endPoint()
//{
//    m_xp = m_xPoints.size() - 1;
//    m_yp = m_yPoints.size() - 1;

//    setPoint();
//}
////------------------------------------------------------------


//bool CTestAlgBFRandom::nextPoint()
//{
//    if (hasNextX()) {
//        ++m_xp;
//    } else {
//        if (hasNextY()) {
//            ++m_yp;
//            m_xp = 0;
//        } else {
//            return false;
//        }
//    }

//    setPoint();
//    return true;
//}
////------------------------------------------------------------


//bool CTestAlgBFRandom::prevPoint()
//{
//    if (hasPrevX()) {
//        --m_xp;
//    } else {
//        if (hasPrevY()) {
//            --m_yp;
//            m_xp = m_xPoints.size() - 1;
//        } else {
//            return false;
//        }
//    }

//    setPoint();
//    return true;
//}
////------------------------------------------------------------


//void CTestAlgBFRandom::executPoint()
//{
//    for (int i = 0; i < m_numTest; ++i){

//    }
//}
////------------------------------------------------------------


//void CTestAlgBFRandom::setPoint(int value,TNameParam param)
//{
//    switch (param) {
//    case NumX:
//       m_numX = value;
//        break;
//    case NumZ:
//       m_numZ = value;
//        break;
//    case NumDis:
//       m_numDisMax = value;
//        break;
//    case LenDisMin:
//       m_lenDisMin = value;
//        break;
//    case LenDisMax:
//       m_lenDisMax = value;
//    }
//}
////------------------------------------------------------------
