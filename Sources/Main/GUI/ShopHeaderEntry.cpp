#include <GUI/ShopHeaderEntry.h>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>
#include <QtGui/QBoxLayout>
#include <Data/Enumerations.h>

class CShopHeaderEntryPrivate
{
public:
	CShopHeaderEntryPrivate(CShopHeaderEntry * ptrPublic);
	void setupUI();
public:
	QVBoxLayout *		m_ptrMainLayout;
	QCheckBox *			m_ptrActiveCheck;
	QPushButton *		m_ptrRemoveEntry;
	QComboBox *			m_ptrShopData;
	CShopHeaderEntry *	m_ptrPublic;
	int					m_iSection;
};

CShopHeaderEntryPrivate::CShopHeaderEntryPrivate(CShopHeaderEntry * ptrPublic):m_ptrPublic(ptrPublic)
{
	m_ptrPublic->setAutoFillBackground(true);
	setupUI();
}
void CShopHeaderEntryPrivate::setupUI()
{
	m_ptrMainLayout = new QVBoxLayout(m_ptrPublic);
	m_ptrMainLayout->setMargin(0);

	QHBoxLayout * ptrHLayout = new QHBoxLayout;
	m_ptrActiveCheck = new QCheckBox;
	QObject::connect(m_ptrActiveCheck, SIGNAL(clicked ( bool)), m_ptrPublic, SLOT(onActiveClicked(bool)));
	
	ptrHLayout->addWidget(m_ptrActiveCheck,0,Qt::AlignCenter);
	
	m_ptrRemoveEntry = new QPushButton;
	QObject::connect(m_ptrRemoveEntry, SIGNAL(clicked ( bool)), m_ptrPublic, SLOT(onRemoveClicked(bool)));
	m_ptrRemoveEntry->setIcon(QIcon(":/images/minus.png"));
	m_ptrRemoveEntry->setMaximumSize(QSize(30,24));

	ptrHLayout->addWidget(m_ptrRemoveEntry,0,Qt::AlignCenter);
	m_ptrMainLayout->addLayout(ptrHLayout);
	//////////////////////////////////////////////////////////////////////////
	m_ptrShopData = new QComboBox;
	QObject::connect(m_ptrShopData, SIGNAL(activated ( const QString & )), 
		m_ptrPublic, SLOT(onShopChanged(const QString &)));
	
	m_ptrMainLayout->addWidget(m_ptrShopData);
}
//////////////////////////////////////////////////////////////////////////
CShopHeaderEntry::CShopHeaderEntry(QWidget *parent): CCustomHeaderViewEntry(parent)
{
	m_ptrPriv = new CShopHeaderEntryPrivate(this);
	retranslateUI();
	setMinimumWidth(110);
}
void CShopHeaderEntry::setActive(bool bActive)
{
	m_ptrPriv->m_ptrActiveCheck->setChecked(bActive);
}
void CShopHeaderEntry::setShop(QString strShopName)
{
	int iIndex = m_ptrPriv->m_ptrShopData->findText(strShopName);
	m_ptrPriv->m_ptrShopData->setCurrentIndex(iIndex);
}
void CShopHeaderEntry::setShopList(QStringList strShopsList)
{
	QString strCurrentShop = m_ptrPriv->m_ptrShopData->currentText();
	m_ptrPriv->m_ptrShopData->clear();
	m_ptrPriv->m_ptrShopData->addItems(strShopsList);
	int iIndex = m_ptrPriv->m_ptrShopData->findText(strCurrentShop);
	m_ptrPriv->m_ptrShopData->setCurrentIndex(iIndex);
}
void CShopHeaderEntry::setShopIcons(QVariantList lIconsList)
{
	int iIndex=1;
	for(QList<QVariant>::const_iterator iterIcons = lIconsList.begin();
		iterIcons != lIconsList.end();++iterIcons,++iIndex)
		m_ptrPriv->m_ptrShopData->setItemIcon(iIndex,iterIcons->value<QIcon>());
}
void CShopHeaderEntry::setSectionIndex(int iSection)
{
	m_ptrPriv->m_iSection=iSection;
	if (iSection==0)
	{
		m_ptrPriv->m_ptrRemoveEntry->hide();
		m_ptrPriv->m_ptrShopData->hide();
	}
}
void CShopHeaderEntry::updateData()
{
	setActive(m_ptrModel->headerData(m_ptrPriv->m_iSection,Qt::Horizontal,Role_ShopColumnActive).toBool());
	setShopList(m_ptrModel->headerData(m_ptrPriv->m_iSection,Qt::Horizontal,Role_ShopNamesList).toStringList());
	setShopIcons(m_ptrModel->headerData(m_ptrPriv->m_iSection,Qt::Horizontal,Role_ShopNamesIconsList).toList());
	setShop(m_ptrModel->headerData(m_ptrPriv->m_iSection,Qt::Horizontal,Role_ShopName).toString());
}
void CShopHeaderEntry::retranslateUI()
{
	m_ptrPriv->m_ptrActiveCheck->setToolTip("Activates/Deactivated column");
	m_ptrPriv->m_ptrRemoveEntry->setToolTip("Removes column with shop");
	m_ptrPriv->m_ptrShopData->setToolTip("");

}
//////////////////////////////////////////////////////////////////////////
void CShopHeaderEntry::onActiveClicked(bool bVal)
{
	m_ptrModel->setHeaderData(m_ptrPriv->m_iSection,Qt::Horizontal,bVal,Role_ShopColumnActive);
}
void CShopHeaderEntry::onRemoveClicked(bool bVal)
{
	emit removeEntry(m_ptrPriv->m_iSection);
}
void CShopHeaderEntry::onShopChanged(const QString & strVal)
{
	m_ptrModel->setHeaderData(m_ptrPriv->m_iSection,Qt::Horizontal,strVal,Role_ShopName);
}
