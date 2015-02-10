#ifndef CIMPL_OBJECT_H
#define CIMPL_OBJECT_H

//class CBoolFormula;

template<class T>
class CImplObject
{
public:
    explicit CImplObject(T *obj);
    CImplObject(const CImplObject &other);
    CImplObject operator=(const CImplObject &other);

private:

    T *m_obj;

    CImplObject();
};

//template<>
//class CImplObject<CBoolFormula>
//{

//private:

//    CBoolFormula *m_obj;
//};

#endif // CIMPL_OBJECT_H
