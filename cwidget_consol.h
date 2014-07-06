#ifndef CWIDGET_CONSOL_H
#define CWIDGET_CONSOL_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class CToolBarHeader;
class QLabel;
class QPlainTextEdit;
QT_END_NAMESPACE

class CWidgetConsol : public QWidget
{
    Q_OBJECT
public:
    explicit CWidgetConsol(QWidget *parent = 0);

    QAction *actVisible(){return m_actVisible;}

public slots:

    void messageAppend(const QString &message);
    void messageSet(const QString &message);

    void executingOperation(const QString&);

private:

    CToolBarHeader  *m_header;
    QAction         *m_actVisible;
    QPlainTextEdit  *m_edit;

signals:

};

#endif // CWIDGET_CONSOL_H
