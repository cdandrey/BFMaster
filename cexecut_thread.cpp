#include "cexecut_thread.h"
#include "cexecut_object.h"

//CExecutThread::CExecutThread(QObject *parent)
//    : QThread(parent),
//      m_lstObj(QList<CExecutObject *>())
//{
//}
//---------------------------------------------------------------


//CExecutThread::CExecutThread(QObject *parent, const QList<CExecutObject *> &lstObj)
//    : QThread(parent),
//      m_lstObj(lstObj)
//{
//}
//---------------------------------------------------------------


QString CExecutThread::progress() const
{
    return m_lstObj.isEmpty() ? "" : m_lstObj.first()->progress();
}
//---------------------------------------------------------------


void CExecutThread::run()
{
    try {
        while (!m_lstObj.isEmpty()) {

            emit executOperation(m_lstObj.first()->progressDescription());
            emit timerWork(100U);

            m_lstObj.first()->run();

            emit timerWork(0U);
            emit executOperation("");
            emit message(m_lstObj.takeFirst()->progressFinished());
        }
    } catch (...) {
        emit timerWork(0U);
        emit executOperation("");
        emit message(tr("ОШИБКА! При выполнении процесса произошла ошибка."));
    }
}
//---------------------------------------------------------------


void CExecutThread::on_terminated()
{
    m_lstObj.first()->terminated();
}
//---------------------------------------------------------------


