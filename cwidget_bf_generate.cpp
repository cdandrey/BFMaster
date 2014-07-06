#include "cwidget_bf_generate.h"

#include <QAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QTime>
#include <QVBoxLayout>

#include "cbf_view.h"
#include "ctoolbar_header.h"

CWidgetBFGenerate::CWidgetBFGenerate(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("QLineEdit {font-size: 12px; border-radius: 3px;}"
                  "QLabel {font-size: 16px;}"
                  "QSpinBox {font-size: 16px;}");

    m_header = new CToolBarHeader(tr("Имя"));

    m_actNewName = new QAction(QIcon(":/ico/bfgen_name_refresh.png"),tr("Задать новое имя функции"),this);
    m_lineName = new QLineEdit(this);
    m_lineName->addAction(m_actNewName,QLineEdit::TrailingPosition);
    m_lineName->setMaximumHeight(20);
    m_lineName->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    triggered_actNewName();

    m_actGenerate = new QAction(QIcon(":/ico/bfgen_gen.png"),tr("Генерировать функцию"),this);
    m_actGenerate->setShortcut(TStr("F6"));

    m_actRegenerate = new QAction(QIcon(":/ico/bfgen_regen.png"),tr("Регенерировать функцию"),this);
    m_actRegenerate->setShortcut(QKeySequence::Refresh);
    m_actRegenerate->setEnabled(false);

    m_actRemove = new QAction(QIcon(":/ico/bfgen_del.png"),tr("Удалить функцию"),this);
    m_actRemove->setShortcut(QKeySequence::Delete);
    m_actRemove->setEnabled(false);

    QList<QAction*> lst;
    lst << m_actGenerate << m_actRegenerate << m_actRemove;

    m_header->insertWidget(m_header->actHint(),m_lineName);
    m_header->insertSpace(m_header->actHint(),16);
    m_header->insertActions(m_header->actHint(),lst);
    m_header->insertStretch(m_header->actHint());

    m_spinX = newSpin(3,2);
    m_spinZ = newSpin(3,2);
    m_spinD = newSpin(3,1);
    m_spinMin = newSpin(2,2);
    m_spinMax = newSpin(3,2);

    QLabel *labelX	= new QLabel(tr("X"),this);
    QLabel *labelZ	= new QLabel(tr("Z"),this);
    QLabel *labelD	= new QLabel(tr("D"),this);
    QLabel *labelMin = new QLabel(tr("K<sub>min</sub>"),this);
    QLabel *labelMax = new QLabel(tr("K<sub>max</sub>"),this);

    labelX->setBuddy(m_spinX);
    labelZ->setBuddy(m_spinZ);
    labelD->setBuddy(m_spinD);
    labelMin->setBuddy(m_spinMin);
    labelMax->setBuddy(m_spinMax);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(labelX);
    hbox->addWidget(m_spinX);
    hbox->addSpacing(20);
    hbox->addWidget(labelZ);
    hbox->addWidget(m_spinZ);
    hbox->addSpacing(20);
    hbox->addWidget(labelD);
    hbox->addWidget(m_spinD);
    hbox->addSpacing(20);
    hbox->addWidget(labelMin);
    hbox->addWidget(m_spinMin);
    hbox->addSpacing(20);
    hbox->addWidget(labelMax);
    hbox->addWidget(m_spinMax);
    hbox->addStretch(100);

    QVBoxLayout *vbox = new QVBoxLayout(this);

    vbox->setMenuBar(m_header);
    vbox->addLayout(hbox);

    m_bfv = NULL;

    connect(m_actNewName,SIGNAL(triggered()),this,SLOT(triggered_actNewName()));
    connect(m_actGenerate,SIGNAL(triggered()),this,SLOT(triggered_actGenerate()));
    connect(m_actRegenerate,SIGNAL(triggered()),this,SLOT(triggered_actRegenerate()));
    connect(m_actRemove,SIGNAL(triggered()),this,SLOT(triggered_actRemove()));

    m_actVisible = new QAction(tr("Отображать окно создания случайной функции"),this);
    m_actVisible->setCheckable(true);

    connect(m_header->actHint(),SIGNAL(triggered(bool)),this,SLOT(hide()));
    connect(m_actVisible,SIGNAL(toggled(bool)),this,SLOT(setVisible(bool)));
    connect(m_header->actHint(),SIGNAL(triggered(bool)),m_actVisible,SLOT(setChecked(bool)));
}
//------------------------------------------------------------------


void CWidgetBFGenerate::on_set(CBFView *bfv)
{
    if (!m_bfv)
        m_bfv = bfv;
    else if (m_bfv->boolFunctionName() != bfv->boolFunctionName())
        m_bfv = bfv;

    m_lineName->setText(m_bfv->boolFunctionName());
    m_spinX->setValue(m_bfv->boolFunction()->numX());
    m_spinZ->setValue(m_bfv->boolFunction()->numZ());
    m_spinD->setValue(m_bfv->boolFunction()->numDisMax());
    m_spinMin->setValue(m_bfv->boolFunction()->lenDisMin());
    m_spinMax->setValue(m_bfv->boolFunction()->lenDisMax());

    if (m_actGenerate->isEnabled()) {
        m_actRegenerate->setEnabled(true);
        m_actRemove->setEnabled(true);
    }
}
//------------------------------------------------------------------


void CWidgetBFGenerate::on_locked()
{
    m_actGenerate->setEnabled(false);
    m_actRegenerate->setEnabled(false);
    m_actRemove->setEnabled(false);
}
//------------------------------------------------------------------


void CWidgetBFGenerate::on_unlocked()
{
    m_actGenerate->setEnabled(true);
    m_actRegenerate->setEnabled(true);
    m_actRemove->setEnabled(true);
}
//------------------------------------------------------------------


void CWidgetBFGenerate::on_generateLocked()
{
    m_actGenerate->setEnabled(false);
    m_actRegenerate->setEnabled(false);
    m_actRemove->setIcon(QIcon(":/ico/bfgen_stop.png"));
    m_actRemove->setText(tr("Прервать генерацию функции"));
    disconnect(m_actRemove,SIGNAL(triggered()),this,SLOT(triggered_actRemove()));
    connect(m_actRemove,SIGNAL(triggered()),this,SIGNAL(stopped()));
}
//------------------------------------------------------------------


void CWidgetBFGenerate::on_generateUnlocked()
{
    m_actGenerate->setEnabled(true);
    m_actRegenerate->setEnabled(true);
    m_actRemove->setIcon(QIcon(":/ico/bfgen_del.png"));
    m_actRemove->setText(tr("Удалить функцию"));
    disconnect(m_actRemove,SIGNAL(triggered()),this,SIGNAL(stopped()));
    connect(m_actRemove,SIGNAL(triggered()),this,SLOT(triggered_actRemove()));
}
//------------------------------------------------------------------


QSpinBox *CWidgetBFGenerate::newSpin(int val, int min, int max)
{
    QSpinBox *spin = new QSpinBox(this);
    spin->setMinimum(min);
    spin->setMaximum(max);
    spin->setValue(val);

    return spin;
}
//------------------------------------------------------------------


void CWidgetBFGenerate::triggered_actGenerate()
{
    CBoolFunction::TParam p;
    p.numX = m_spinX->value();
    p.numZ = m_spinZ->value();
    p.numDisMax = m_spinD->value();
    p.lenDisMin = m_spinMin->value();
    p.lenDisMax = m_spinMax->value();

    if (m_bfv == NULL ||
        m_bfv->boolFunctionName() == m_lineName->text() ||
        m_lineName->text().isEmpty())
    {
        triggered_actNewName();
    }

    m_bfv = new CBFView(new CBoolFunction(p),m_lineName->text());

    m_actRegenerate->setEnabled(true);
    m_actRemove->setEnabled(true);

    emit generate(m_bfv);
}
//------------------------------------------------------------------


void CWidgetBFGenerate::triggered_actRegenerate()
{
    if (m_bfv == NULL)
        return;

    CBoolFunction::TParam p;
    p.numX = m_spinX->value();
    p.numZ = m_spinZ->value();
    p.numDisMax = m_spinD->value();
    p.lenDisMin = m_spinMin->value();
    p.lenDisMax = m_spinMax->value();

    m_bfv->boolFunction()->init(p);
    emit generate(m_bfv);
}
//------------------------------------------------------------------


void CWidgetBFGenerate::triggered_actRemove()
{
    if (m_bfv == NULL)
        return;

    CBFView *bfv = m_bfv;
    m_bfv = NULL;

    m_actRegenerate->setEnabled(false);
    m_actRemove->setEnabled(false);

    emit remove(bfv->boolFunctionName());
    emit remove(bfv);
}
//------------------------------------------------------------------


void CWidgetBFGenerate::triggered_actNewName()
{
    m_lineName->setText(TStr("BFUNC_%1")
                        .arg(QDateTime::currentDateTime().toString("hh_mm_ss_zzz")));
}
//------------------------------------------------------------------
