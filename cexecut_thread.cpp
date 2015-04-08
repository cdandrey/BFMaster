#include "cexecut_thread.h"
#include "cexecut_object.h"

CExecutThread::CExecutThread(QObject *parent)
    : QThread(parent),
      m_queue(QueueExeObject())
{
}
//---------------------------------------------------------------


CExecutThread::CExecutThread(QObject *parent, CExecutObject *obj)
    : QThread(parent),
      m_queue(QueueExeObject())
{
    if (obj)
        m_queue.enqueue(obj);
}
//---------------------------------------------------------------


CExecutThread::CExecutThread(QObject *parent, const QueueExeObject &queue)
    : QThread(parent),
      m_queue(QueueExeObject())
{
    while (queue.size() > m_queue.size())
        if (queue.head())
            m_queue.enqueue(queue.head());
}
//---------------------------------------------------------------


CExecutThread::~CExecutThread()
{
}
//---------------------------------------------------------------


QString CExecutThread::progress() const
{
    return m_queue.isEmpty() ? tr("Очередь выполняемых процессов пустая.")
                               : m_queue.head()->progress();
}
//---------------------------------------------------------------


void CExecutThread::reset(const QueueExeObject &queue)
{
    m_queue = queue;
}
//---------------------------------------------------------------


void CExecutThread::run()
{
    while (!m_queue.isEmpty()) {
        emit executOperation(m_queue.head()->progressDescription());
        if (m_queue.head()->run())
            emit successfull();
        emit message(m_queue.dequeue()->progressFinished());
    }
}
//---------------------------------------------------------------


void CExecutThread::on_terminated()
{
    m_queue.head()->terminated();
}
//---------------------------------------------------------------


