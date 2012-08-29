#include <GUI/ComponentHeaderEntry.h>
#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#include <QtGui/QBoxLayout>
#include <Data/Enumerations.h>

const QSize cPushButtonSize(QSize(30,24));
const unsigned int ciLineEditSize(250);
const unsigned int ciMinimumHeigth(24);
const unsigned int ciEntrySeparator(10);

class CComponentHeaderEntryPrivate
{
public:
	CComponentHeaderEntryPrivate(CComponentHeaderEntry * ptrPublic);
	void setupUI();
public:
	bool				m_bFirstSeparatorSet;
	QHBoxLayout *		m_ptrMainLayout;
	QCheckBox *			m_ptrActiveCheck;
	QPushButton *		m_ptrRemoveEntry;
	QLineEdit *			m_ptrCompName;
	QLineEdit *			m_ptrCompCode;
	int					m_iSection;
	CComponentHeaderEntry *	m_ptrPublic;
};
CComponentHeaderEntryPrivate::CComponentHeaderEntryPrivate(CComponentHeaderEntry * ptrPublic):m_ptrPublic(ptrPublic),m_bFirstSeparatorSet(false)
{
	m_ptrPublic->setAutoFillBackground(true);
	setupUI();
}
void CComponentHeaderEntryPrivate::setupUI()
{
	m_ptrMainLayout = new QHBoxLayout(m_ptrPublic);
	m_ptrMainLayout->setMargin(0);
	m_ptrMainLayout->margin();
	m_ptrMainLayout->spacing();

	m_ptrRemoveEntry = new QPushButton;
	QObject::connect(m_ptrRemoveEntry, SIGNAL(clicked ( bool)), m_ptrPublic, SLOT(onRemoveClicked(bool)));
	m_ptrRemoveEntry->setIcon(QIcon(":/images/minus.png"));
	m_ptrRemoveEntry->setMaximumSize(cPushButtonSize);
	m_ptrMainLayout->addWidget(m_ptrRemoveEntry);

	m_ptrActiveCheck = new QCheckBox;
	QObject::connect(m_ptrActiveCheck, SIGNAL(clicked ( bool)), m_ptrPublic, SLOT(onActiveClicked(bool)));
	m_ptrMainLayout->addWidget(m_ptrActiveCheck);

	m_ptrCompName = new QLineEdit;
	m_ptrCompName->setMinimumWidth(ciLineEditSize);
	QObject::connect(m_ptrCompName, SIGNAL(textEdited(QString)), m_ptrPublic, SLOT(onCompNameEdited(QString)));
	m_ptrMainLayout->addWidget(m_ptrCompName);

	m_ptrCompCode = new QLineEdit;
	m_ptrCompCode->hide();
	m_ptrMainLayout->addWidget(m_ptrCompCode);
}
//////////////////////////////////////////////////////////////////////////
CComponentHeaderEntry::CComponentHeaderEntry(QWidget *parent):CCustomHeaderViewEntry(parent)
{
	m_ptrPriv = new CComponentHeaderEntryPrivate(this);
	setMinimumHeight(ciMinimumHeigth);
}
CComponentHeaderEntry::~CComponentHeaderEntry()
{
	delete m_ptrPriv;
}
void CComponentHeaderEntry::setSectionIndex(int iSection)
{
	m_ptrPriv->m_iSection = iSection;
	if (iSection==0)
	{
		if (false==m_ptrPriv->m_bFirstSeparatorSet)
		{
			m_ptrPriv->m_ptrMainLayout->insertSpacing(0,cPushButtonSize.width()+m_ptrPriv->m_ptrMainLayout->spacing());
			m_ptrPriv->m_bFirstSeparatorSet=true;
		}
		m_ptrPriv->m_ptrRemoveEntry->hide();
		m_ptrPriv->m_ptrCompName->hide();
		m_ptrPriv->m_ptrCompCode->hide();
	}
}
void CComponentHeaderEntry::updateData()
{
	bool bActive = m_ptrModel->headerData(m_ptrPriv->m_iSection,Qt::Vertical,Role_RowActive).toBool();
	m_ptrPriv->m_ptrActiveCheck->setChecked(bActive);
	QString strCompName = m_ptrModel->headerData(m_ptrPriv->m_iSection,Qt::Vertical,Role_ComponentName).toString();
	if (strCompName!=m_ptrPriv->m_ptrCompName->text())
		m_ptrPriv->m_ptrCompName->setText(strCompName);
}
void CComponentHeaderEntry::onActiveClicked(bool bVal)
{
	m_ptrModel->setHeaderData(m_ptrPriv->m_iSection,Qt::Vertical,bVal,Role_RowActive);
}
void CComponentHeaderEntry::onRemoveClicked(bool)
{
	emit removeEntry(m_ptrPriv->m_iSection);
}
void CComponentHeaderEntry::onComponentCodeChanged(){}
void CComponentHeaderEntry::onCompNameEdited(QString strNewCompName)
{
	m_ptrModel->setHeaderData(m_ptrPriv->m_iSection,Qt::Vertical,strNewCompName,Role_ComponentName);
}
