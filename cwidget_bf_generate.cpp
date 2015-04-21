#include <QAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QTime>
#include <QVBoxLayout>

#include "cwidget_bf_generate.h"
#include "cbool_formula.h"
#include "cexecut_object.h"
#include "ctoolbar_header.h"

CWidgetBFGenerate::CWidgetBFGenerate(QWidget *parent)
    : QWidget(parent),
      m_name(""),
      m_bf(NULL),
      m_exeObj(new CExecutGenerateFormula()),
      m_isTriggeredGenerate(false)
{
    m_header = new CToolBarHeader(tr("Имя"));

    m_actNewName = new QAction(QIcon(":/ico/bfgen_name_refresh.png"),tr("Задать новое имя функции"),this);
    m_lineName = new QLineEdit(this);
    m_lineName->addAction(m_actNewName,QLineEdit::TrailingPosition);
    m_lineName->setMaximumHeight(20);
    m_lineName->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    triggered_actNewName();

    m_actGenerate = new QAction(QIcon("://ico/bfgen_gen.png"),tr("Генерировать функцию"),this);
    m_actGenerate->setShortcut(QString("F6"));

    m_actRegenerate = new QAction(QIcon("://ico/bfgen_regen.png"),tr("Регенерировать функцию"),this);
    m_actRegenerate->setShortcut(QKeySequence::Refresh);
    m_actRegenerate->setEnabled(false);

    m_actRemove = new QAction(QIcon(":/ico/bfgen_del.png"),tr("Удалить функцию"),this);
    m_actRemove->setShortcut(QKeySequence::Delete);
    m_actRemove->setEnabled(false);
    m_actRemove->setData(true);

    m_header->insertWidget(m_header->actHint(),m_lineName);
    m_header->insertSpace(m_header->actHint(),16);
    m_header->insertAction(m_header->actHint(),m_actGenerate);
    m_header->insertAction(m_header->actHint(),m_actRegenerate);
    m_header->insertAction(m_header->actHint(),m_actRemove);
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


CWidgetBFGenerate::~CWidgetBFGenerate()
{
    delete m_exeObj;
}
//------------------------------------------------------------------


void CWidgetBFGenerate::on_changeRemoveStop(bool change)
{
    if (change) {
        m_actRemove->setIcon(QIcon("://ico/bfgen_stop.png"));
        m_actRemove->setText(tr("Прервать генерацию функции"));
        disconnect(m_actRemove,SIGNAL(triggered()),this,SLOT(triggered_actRemove()));
        connect(m_actRemove,SIGNAL(triggered()),this,SIGNAL(terminated()));
    } else {
        m_actRemove->setIcon(QIcon("://ico/bfgen_del.png"));
        m_actRemove->setText(tr("Удалить функцию"));
        connect(m_actRemove,SIGNAL(triggered()),this,SLOT(triggered_actRemove()));
        disconnect(m_actRemove,SIGNAL(triggered()),this,SIGNAL(terminated()));
    }
}
//------------------------------------------------------------------


void CWidgetBFGenerate::on_generate()
{
    if (m_bf == NULL || m_name.isEmpty())
        return;

    m_exeObj->resetObject(m_name,m_bf);

    emit execut(QList<CExecutObject*>({m_exeObj}));
}
//------------------------------------------------------------------


void CWidgetBFGenerate::on_locked()
{
    m_lineName->setEnabled(false);
    m_actNewName->setEnabled(false);

    m_actGenerate->setEnabled(false);
    m_actRegenerate->setEnabled(false);

    m_actRemove->setEnabled(m_isTriggeredGenerate);
    on_changeRemoveStop(m_isTriggeredGenerate);

    m_actVisible->setEnabled(false);

    m_spinX->setEnabled(false);
    m_spinZ->setEnabled(false);
    m_spinD->setEnabled(false);
    m_spinMin->setEnabled(false);
    m_spinMax->setEnabled(false);
}
//------------------------------------------------------------------


void CWidgetBFGenerate::on_unlocked()
{
    on_resetTriggered();

    m_lineName->setEnabled(true);
    m_actNewName->setEnabled(true);

    m_actGenerate->setEnabled(true);
    m_actRegenerate->setEnabled(true);

    m_actRemove->setEnabled(true);
    on_changeRemoveStop(false);

    m_actVisible->setEnabled(true);

    m_spinX->setEnabled(true);
    m_spinZ->setEnabled(true);
    m_spinD->setEnabled(true);
    m_spinMin->setEnabled(true);
    m_spinMax->setEnabled(true);
}
//------------------------------------------------------------------


void CWidgetBFGenerate::on_set(const QString &name, CBoolFormula *bf)
{
    if (m_name != name) {

        m_bf = bf;
        m_name = name;

        if (m_bf == NULL || m_name.isEmpty())
            return;

        m_lineName->setText(m_name);

        m_spinX->setValue(m_bf->numLit(true));
        m_spinZ->setValue(m_bf->numLit(false));
        m_spinD->setValue(m_bf->numClaus());
        m_spinMin->setValue(m_bf->numLenClaus(true));
        m_spinMax->setValue(m_bf->numLenClaus(false));

        if (m_actGenerate->isEnabled()) {
            m_actRegenerate->setEnabled(true);
            m_actRemove->setEnabled(true);
        }
    }
}
//------------------------------------------------------------------


void CWidgetBFGenerate::on_resetTriggered()
{
    m_isTriggeredGenerate = false;
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
    m_isTriggeredGenerate = true;

    BFParam p;
    p.numLit = m_spinX->value();
    p.numNotLit = m_spinZ->value();
    p.numClaus = m_spinD->value();
    p.minLenClaus = m_spinMin->value();
    p.maxLenClaus = m_spinMax->value();

    m_bf = new CBoolFormula(p);

    if (m_name == m_lineName->text() || m_lineName->text().isEmpty())
        triggered_actNewName();

    m_name = m_lineName->text();

    emit append(m_name,m_bf);
}
//------------------------------------------------------------------


void CWidgetBFGenerate::triggered_actRegenerate()
{
    if (m_bf == NULL || m_name.isEmpty())
        return;

    m_isTriggeredGenerate = true;

    BFParam p;
    p.numLit = m_spinX->value();
    p.numNotLit = m_spinZ->value();
    p.numClaus = m_spinD->value();
    p.minLenClaus = m_spinMin->value();
    p.maxLenClaus = m_spinMax->value();

    m_bf->setParam(p);

    if (m_lineName->text().isEmpty())
        triggered_actNewName();

    m_name = m_lineName->text();

    emit replace(m_name,m_bf);
}
//------------------------------------------------------------------


void CWidgetBFGenerate::triggered_actRemove()
{
    if (m_name.isEmpty())
        return;

    m_actRegenerate->setEnabled(false);
    m_actRemove->setEnabled(false);

    emit remove(m_name);
}
//------------------------------------------------------------------


void CWidgetBFGenerate::triggered_actNewName()
{
    m_lineName->setText(QString("BFUNC_%1")
                        .arg(QDateTime::currentDateTime().toString("hh_mm_ss_zzz")));
}
//------------------------------------------------------------------
