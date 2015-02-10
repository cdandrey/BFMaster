#include "cimpl_object.h"

template<class T>
CImplObject<T>::CImplObject()
{
}
//---------------------------------------------------------------


template<class T>
CImplObject<T>::CImplObject(T *obj) :
    m_obj(obj)
{
}
//---------------------------------------------------------------


template<class T>
CImplObject<T>::CImplObject(const CImplObject &other)
{
    if (*this == other)
        return;


}
//---------------------------------------------------------------
