#include "cexecut_thread.h"
#include "cexecut_object.h"

CExecutThread::CExecutThread(QObject *parent, CExecutObject *obj) :
    QThread(parent),
    m_obj(obj)
{
}
//---------------------------------------------------------------


CExecutThread::~CExecutThread()
{
}
//---------------------------------------------------------------


void CExecutThread::reset(CExecutObject *obj)
{
    m_obj = obj;
}
//---------------------------------------------------------------


void CExecutThread::run()
{
    emit message(m_obj->progressDescription());
    if (m_obj->run())
        emit successfull();
    emit message(m_obj->progressFinished());

}
//---------------------------------------------------------------


void CExecutThread::on_terminated()
{
    m_obj->terminated();
}
//---------------------------------------------------------------


