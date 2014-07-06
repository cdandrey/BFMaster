#ifndef CTOOLBAR_HEADER_H
#define CTOOLBAR_HEADER_H

#include <QToolBar>

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
QT_END_NAMESPACE

class CToolBarHeader : public QToolBar
{
    Q_OBJECT
public:
    explicit CToolBarHeader(QString title = "",QWidget *parent = 0);

    QAction *actHint(){return m_actHint;}

    void insertSpace(QAction *before, int width);
    void insertStretch(QAction *before);

    QString title() const;

private:

    QAction *m_actHint;
    QLabel  *m_label;

signals:

public slots:

    void setTitle(const QString&);

};

#endif // CTOOLBAR_HEADER_H
