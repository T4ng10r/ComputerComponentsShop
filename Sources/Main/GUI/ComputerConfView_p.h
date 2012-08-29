#include <QtGui/QStandardItemModel>
#include <QtGui/QGridLayout>
#include <QtGui/QScrollArea>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>
#include <QtCore/QAbstractItemModel>

class CComputerConfView;

class CComputerConfViewPrivate : public QObject
{
	Q_OBJECT
public:
	CComputerConfViewPrivate(CComputerConfView * ptrPublic);
	void setupUI();
	void setRowData();
	void initialize();
	void removeLayoutWidgets();

	void appendShop();
	void removeShop(int iCol);
	void appendRow();
	void removeRow(int iRow);

protected slots:
	void onRowActivation( int state );
	void onShopActivation( int state );
	void onCompNameEditFinished();
	void onCompCodeEditFinished();
	void onCompRowRemove();
	void onAddComponent();
	void onAddShop();
public:
	QAbstractItemModel *	m_ptrModel;
//	////ScrollArea storing
	QScrollArea *	m_ptrScrollArea;
	QWidget *		m_ptrScrollAreaWidgetContents;
	QGridLayout *	m_ptrGridLayout;
//
//	RaceStatsSelector *	m_ptrSelector;
//	QStandardItemModel *	modelRaceBest;
//
	CComputerConfView * m_ptrPublic;
};
//////////////////////////////////////////////////////////////////////////
//CComputerConfViewPrivate::CComputerConfViewPrivate(CComputerConfView * ptrPublic)
//{
//	m_ptrPublic = ptrPublic;
//	setupUI();
//	initialize();
//}
//void CComputerConfViewPrivate::removeLayoutWidgets()
//{
//	QWidget * ptrWidget = NULL;
//	while(m_ptrGridLayout->count())
//	{
//		ptrWidget = NULL;
//		QLayoutItem * ptrLItem = m_ptrGridLayout->itemAt(m_ptrGridLayout->count()-1);
//		if (ptrLItem->widget())
//		{
//			ptrWidget = ptrLItem->widget();
//			m_ptrGridLayout->removeWidget(ptrWidget);
//			//m_ptrGridLayout->count();
//		}
//		else
//			m_ptrGridLayout->removeItem(ptrLItem);
//	}
//}
//void CComputerConfViewPrivate::initialize()
//{
//	removeLayoutWidgets();
//	QWidget * ptrEmptyWidget = new QWidget;
//	m_ptrGridLayout->addWidget(ptrEmptyWidget,0,0,ciHeaderRowCount,ciHeaderColCount);
//	appendShop();
//	appendRow();
//}
//void CComputerConfViewPrivate::appendShop()
//{
//	QCheckBox * ptrCheckBox = new QCheckBox;
//	ptrCheckBox->setChecked(true);
//	connect(ptrCheckBox,SIGNAL(stateChanged(int)), this, SLOT(onShopActivation(int)));
//	m_ptrGridLayout->addWidget(ptrCheckBox,0,m_ptrGridLayout->columnCount()-1);
//	QComboBox * ptrComboBox = new QComboBox;
//	m_ptrGridLayout->addWidget(ptrComboBox,1,m_ptrGridLayout->columnCount()-1);
//}
//void CComputerConfViewPrivate::removeShop(int iCol)
//{
//
//}
//void CComputerConfViewPrivate::appendRow()
//{
//	QCheckBox * ptrCheckBox = new QCheckBox;
//	ptrCheckBox->setChecked(true);
//	connect(ptrCheckBox,SIGNAL(stateChanged(int)), this, SLOT(onRowActivation(int)));
//	m_ptrGridLayout->addWidget(ptrCheckBox,m_ptrGridLayout->rowCount()-1,0);
//	QLabel * ptrCompName = new QLabel;
//	m_ptrGridLayout->addWidget(ptrCompName,m_ptrGridLayout->rowCount()-1,1);
//	QLabel * ptrCompCode = new QLabel;
//	m_ptrGridLayout->addWidget(ptrCompCode,m_ptrGridLayout->rowCount()-1,2);
//	QLabel * ptrBestPrice = new QLabel;
//	m_ptrGridLayout->addWidget(ptrBestPrice,m_ptrGridLayout->rowCount()-1,3);
//}
//void CComputerConfViewPrivate::removeRow(int iRow)
//{
//
//}
//void CComputerConfViewPrivate::setupUI()
//{
//	m_ptrPublic->setObjectName(QString::fromUtf8("ComputerConfView"));
//	//////////////////////////////////////////////////////////////////////////
//	QHBoxLayout * ptrHorizontalLayout = new QHBoxLayout(m_ptrPublic);
//	ptrHorizontalLayout->setContentsMargins(1, 1, 1, 1);
//	ptrHorizontalLayout->setSpacing(0);
//	ptrHorizontalLayout->setObjectName(QString::fromUtf8("ptrHorizontalLayout"));
//	//////////////////////////////////////////////////////////////////////////
//	m_ptrScrollArea = new QScrollArea(m_ptrPublic);
//	m_ptrScrollArea->setObjectName(QString::fromUtf8("m_ptrScrollArea"));
//	m_ptrScrollArea->setFrameShadow(QFrame::Plain);
//	m_ptrScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//	m_ptrScrollArea->setWidgetResizable(true);
//
//	m_ptrScrollAreaWidgetContents = new QWidget();
//	m_ptrScrollAreaWidgetContents->setObjectName(QString::fromUtf8("m_ptrScrollAreaWidgetContents"));
//	m_ptrScrollArea->setWidget(m_ptrScrollAreaWidgetContents);
//	//////////////////////////////////////////////////////////////////////////
//	//m_ptrGridLayout = new QGridLayout;
//	m_ptrGridLayout = new QGridLayout(m_ptrScrollAreaWidgetContents);
//	m_ptrGridLayout->setContentsMargins(1, 1, 1, 1);
//	m_ptrGridLayout->setSpacing(0);
//	m_ptrGridLayout->setObjectName(QString::fromUtf8("ptrGridLayout"));
//	//delete m_ptrScrollAreaWidgetContents->layout();
//	//m_ptrScrollAreaWidgetContents->setLayout(m_ptrGridLayout);
//
//	ptrHorizontalLayout->addWidget(m_ptrScrollArea);
//}
//void CComputerConfViewPrivate::setRowData()
//{
//
//}
////////////////////////////////////////////////////////////////////////////
//void CComputerConfViewPrivate::onRowActivation( int state )
//{
//
//}
//void CComputerConfViewPrivate::onShopActivation( int state )
//{
//
//}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
