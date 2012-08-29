#include <Data/Enumerations.h>
#include <GUI/ComputerConfView.h>
#include <GUI/ComputerConfView_p.h>
#include <QtGui/QStandardItemModel>
#include <QtGui/QGridLayout>
#include <QtGui/QScrollArea>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QIcon>
#include <QtCore/QAbstractItemModel>

CComputerConfViewPrivate::CComputerConfViewPrivate(CComputerConfView * ptrPublic)
{
	m_ptrPublic = ptrPublic;
	setupUI();
	initialize();
}
void CComputerConfViewPrivate::removeLayoutWidgets()
{
	QWidget * ptrWidget = NULL;
	while(m_ptrGridLayout->count())
	{
		ptrWidget = NULL;
		QLayoutItem * ptrLItem = m_ptrGridLayout->itemAt(m_ptrGridLayout->count()-1);
		if (ptrLItem->widget())
		{
			ptrWidget = ptrLItem->widget();
			m_ptrGridLayout->removeWidget(ptrWidget);
			//m_ptrGridLayout->count();
		}
		else
			m_ptrGridLayout->removeItem(ptrLItem);
	}
}
void CComputerConfViewPrivate::initialize()
{
	removeLayoutWidgets();
	QWidget * ptrEmptyWidget = new QWidget;
	m_ptrGridLayout->addWidget(ptrEmptyWidget,0,0,ciHeaderRowCount,ciHeaderColCount);
	m_ptrGridLayout->setColumnMinimumWidth(0,25);
	m_ptrGridLayout->setRowMinimumHeight(0,25);
}
void CComputerConfViewPrivate::appendShop()
{
	int iCol = m_ptrGridLayout->columnCount();
	//
	QHBoxLayout * ptrHLayout = new QHBoxLayout;

	QCheckBox * ptrCheckBox = new QCheckBox;
	ptrCheckBox->setChecked(true);
	ptrCheckBox->setProperty("column",iCol-ciHeaderColCount);
	//connect(ptrCheckBox,SIGNAL(stateChanged(int)), this, SLOT(onShopActivation(int)));
	ptrHLayout->addWidget(ptrCheckBox);

	QPushButton * ptrRemShop = new QPushButton;
	ptrRemShop->setIcon(QIcon(":/images/minus.png"));
	ptrRemShop->setFlat(true);
	ptrRemShop->setProperty("column",iCol-ciHeaderColCount);
	//connect(ptrRemShop,SIGNAL(clicked ( bool )), this, SLOT(onCompRowRemove()));
	ptrHLayout->addWidget(ptrRemShop);

	m_ptrGridLayout->addLayout(ptrHLayout,0,iCol,Qt::AlignCenter);
	//
	QComboBox * ptrComboBox = new QComboBox;
	ptrRemShop->setProperty("column",iCol-ciHeaderColCount);
	m_ptrGridLayout->addWidget(ptrComboBox,1,iCol);
}
void CComputerConfViewPrivate::removeShop(int iCol)
{

}
void CComputerConfViewPrivate::appendRow()
{
	//usun ostatni element rozpychajacy
	int iRow = m_ptrGridLayout->rowCount();
	for(int iCol = 0;iCol<m_ptrGridLayout->columnCount();++iCol)
	{
		//QLayoutItem * ptrItem = m_ptrGridLayout->itemAtPosition(iRow,iCol);
	}
	//////////////////////////////////////////////////////////////////////////
	QPushButton * ptrRemRow = new QPushButton;
	ptrRemRow->setIcon(QIcon(":/images/minus.png"));
	ptrRemRow->setFlat(true);
	ptrRemRow->setProperty("row",iRow-ciHeaderRowCount);
	connect(ptrRemRow,SIGNAL(clicked ( bool )), this, SLOT(onCompRowRemove()));
	m_ptrGridLayout->addWidget(ptrRemRow,iRow,RowInd_Remove,Qt::AlignCenter);
	//////////////////////////////////////////////////////////////////////////
	QCheckBox * ptrCheckBox = new QCheckBox;
	ptrCheckBox->setChecked(true);
	ptrCheckBox->setProperty("row",iRow-ciHeaderRowCount);
	connect(ptrCheckBox,SIGNAL(stateChanged(int)), this, SLOT(onRowActivation(int)));
	m_ptrGridLayout->addWidget(ptrCheckBox,iRow,RowInd_Active,Qt::AlignCenter);
	//////////////////////////////////////////////////////////////////////////
	//QLabel * ptrCompName = new QLabel;
	QLineEdit * ptrCompName = new QLineEdit;
	ptrCompName->setProperty("row",iRow-ciHeaderRowCount);
	connect(ptrCompName,SIGNAL(editingFinished ()), this, SLOT(onCompNameEditFinished()));
	m_ptrGridLayout->addWidget(ptrCompName,iRow,RowInd_CompName);
	//////////////////////////////////////////////////////////////////////////
	//QLabel * ptrCompCode = new QLabel;
	QLineEdit * ptrCompCode = new QLineEdit;
	ptrCompCode->setProperty("row",iRow-ciHeaderRowCount);
	connect(ptrCompCode,SIGNAL(editingFinished ()), this, SLOT(onCompCodeEditFinished()));
	m_ptrGridLayout->addWidget(ptrCompCode,iRow,RowInd_CompCode);
	//////////////////////////////////////////////////////////////////////////
	QLabel * ptrBestPrice = new QLabel;
	m_ptrGridLayout->addWidget(ptrBestPrice,iRow,RowInd_BestPrice);
	//////////////////////////////////////////////////////////////////////////
	for(int iCol = 0;iCol<m_ptrGridLayout->columnCount();++iCol)
	{
		QSpacerItem * ptrItem = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding);
		m_ptrGridLayout->addItem(ptrItem,iRow+1,iCol);
	}
}
void CComputerConfViewPrivate::removeRow(int iRow)
{

}
void CComputerConfViewPrivate::setupUI()
{
	m_ptrPublic->setObjectName(QString::fromUtf8("ComputerConfView"));
	//////////////////////////////////////////////////////////////////////////
	QVBoxLayout * ptrHorizontalLayout = new QVBoxLayout(m_ptrPublic);
	ptrHorizontalLayout->setContentsMargins(1, 1, 1, 1);
	ptrHorizontalLayout->setSpacing(0);
	ptrHorizontalLayout->setObjectName(QString::fromUtf8("ptrHorizontalLayout"));
	//////////////////////////////////////////////////////////////////////////
	m_ptrScrollArea = new QScrollArea(m_ptrPublic);
	m_ptrScrollArea->setObjectName(QString::fromUtf8("m_ptrScrollArea"));
	m_ptrScrollArea->setFrameShadow(QFrame::Plain);
	m_ptrScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_ptrScrollArea->setWidgetResizable(true);

	m_ptrScrollAreaWidgetContents = new QWidget();
	m_ptrScrollAreaWidgetContents->setObjectName(QString::fromUtf8("m_ptrScrollAreaWidgetContents"));
	m_ptrScrollArea->setWidget(m_ptrScrollAreaWidgetContents);
	//////////////////////////////////////////////////////////////////////////
	m_ptrGridLayout = new QGridLayout(m_ptrScrollAreaWidgetContents);
	m_ptrGridLayout->setContentsMargins(1, 1, 1, 1);
	m_ptrGridLayout->setSpacing(4);
	m_ptrGridLayout->setObjectName(QString::fromUtf8("ptrGridLayout"));

	ptrHorizontalLayout->addWidget(m_ptrScrollArea,10);
	//////////////////////////////////////////////////////////////////////////
	QHBoxLayout * ptrHButtonsLayout = new QHBoxLayout;
	
	QSpacerItem * ptrItem = new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding);
	ptrHButtonsLayout->addItem(ptrItem);
	QPushButton * ptrAddRow = new QPushButton;
	ptrAddRow->setIcon(QIcon(":/images/plus.png"));
	ptrAddRow->setText("Component");
	connect(ptrAddRow,SIGNAL(clicked ( bool )), this, SLOT(onAddComponent()));
	ptrHButtonsLayout->addWidget(ptrAddRow);

	QPushButton * ptrAddShop = new QPushButton;
	ptrAddShop->setIcon(QIcon(":/images/plus.png"));
	ptrAddShop->setText("Shop");
	connect(ptrAddShop,SIGNAL(clicked ( bool )), this, SLOT(onAddShop()));
	ptrHButtonsLayout->addWidget(ptrAddShop);

	ptrHorizontalLayout->addLayout(ptrHButtonsLayout);
}
void CComputerConfViewPrivate::setRowData()
{

}
//////////////////////////////////////////////////////////////////////////
void CComputerConfViewPrivate::onRowActivation( int state )
{
	QCheckBox * ptrRowAct = dynamic_cast<QCheckBox*>(sender());
	int iIndex = m_ptrGridLayout->indexOf(ptrRowAct);
	//
}
void CComputerConfViewPrivate::onShopActivation( int state )
{
	QCheckBox * ptrShopAct = dynamic_cast<QCheckBox*>(sender());
	int iIndex = m_ptrGridLayout->indexOf(ptrShopAct);
	//
}
void CComputerConfViewPrivate::onCompNameEditFinished()
{
	QCheckBox * ptrRowAct = dynamic_cast<QCheckBox*>(sender());
	int iIndex = m_ptrGridLayout->indexOf(ptrRowAct);
}
void CComputerConfViewPrivate::onCompCodeEditFinished()
{
	QCheckBox * ptrShopAct = dynamic_cast<QCheckBox*>(sender());
	int iIndex = m_ptrGridLayout->indexOf(ptrShopAct);
}
void CComputerConfViewPrivate::onCompRowRemove()
{
	QCheckBox * ptrShopAct = dynamic_cast<QCheckBox*>(sender());
	int iIndex = m_ptrGridLayout->indexOf(ptrShopAct);
}
void CComputerConfViewPrivate::onAddComponent()
{
	QAbstractItemModel * ptrModel = m_ptrPublic->model();
	ptrModel->insertRow(-1);
}
void CComputerConfViewPrivate::onAddShop()
{
	QAbstractItemModel * ptrModel = m_ptrPublic->model();
	ptrModel->insertColumn(-1);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CComputerConfView::CComputerConfView(QWidget *parent) : QAbstractItemView(parent)
{
	m_ptrPriv = new CComputerConfViewPrivate(this);
	//setSelectionMode(QAbstractItemView::SingleSelection);
	//m_ptrPriv->setConnections();
	//rowsInserted ( const QModelIndex & parent, int start, int end )

}
CComputerConfView::~CComputerConfView()
{
	delete m_ptrPriv;
}
void CComputerConfView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{

	//QAbstractItemView::dataChanged(topLeft, bottomRight);
	//const QAbstractItemModel * ptrModel = topLeft.model();
	//if (!bottomRight.isValid())
	//{
	//	if (topLeft.column()==0 && topLeft.row()==14)
	//		return;
	//	if (topLeft.column()==2 && topLeft.row()<7)
	//	{
	//		if (topLeft.row()==0)
	//		{
	//			RaceStatsSelector * ptrRaceSelector = m_ptrPriv->m_ptrSelector;
	//			QStringList strRaceList = ptrModel->data(topLeft,RacesListRole).toStringList();
	//			ptrRaceSelector->setRaceList(strRaceList);
	//			QStringList strFractionList = ptrModel->data(topLeft,FractionsListRole).toStringList();
	//			ptrRaceSelector->setFractionList(strFractionList);
	//			QString strRace = ptrModel->data(topLeft,RaceChosenRole).toString();
	//			ptrRaceSelector->setCurrentRace(strRace);
	//			QString strFraction = ptrModel->data(topLeft,FractionChosenRole).toString();
	//			ptrRaceSelector->setCurrentFraction(strFraction);
	//		}
	//	}
	//	else
	//	{
	//		//process only one element
	//		RaceStatsDisplay * guiItem = getResearchGUIItem(topLeft.column(),topLeft.row());
	//		RaceStatsModelItem modelItem = ptrModel->data(topLeft,RaceItemRole).value<RaceStatsModelItem>();
	//		guiItem->setStatsValue(modelItem.iStatsValue);
	//		guiItem->setStatsValueVisibility(modelItem.bShowStatsValue);
	//		guiItem->setStatsName(modelItem.strName);
	//	}
	//}
	//else
	//{
	//	for(int iColumn=topLeft.column();iColumn<=bottomRight.column();++iColumn)
	//	{
	//		int iRowCount(bottomRight.row());
	//		QModelIndex parent(ptrModel->index(0,iColumn));
	//		int iRows = ptrModel->rowCount(parent)-1;
	//		if (iRowCount>iRows)
	//			iRowCount=iRows;
	//		for(int iRow=topLeft.row();iRow<=iRowCount;++iRow)
	//		{
	//			if (iColumn==0 && iRow==14)
	//				continue;
	//			else if (iColumn==2 && iRow<7)
	//			{
	//				if (iRow==0)
	//				{
	//					RaceStatsSelector * ptrRaceSelector = m_ptrPriv->m_ptrSelector;
	//					QModelIndex stIndex = model()->index(iRow,iColumn);
	//					QStringList strRaceList = ptrModel->data(stIndex,RacesListRole).toStringList();
	//					ptrRaceSelector->setRaceList(strRaceList);
	//					QStringList strFractionList = ptrModel->data(stIndex,FractionsListRole).toStringList();
	//					ptrRaceSelector->setFractionList(strFractionList);
	//					QString strRace = ptrModel->data(topLeft,RaceChosenRole).toString();
	//					ptrRaceSelector->setCurrentRace(strRace);
	//					QString strFraction = ptrModel->data(topLeft,FractionChosenRole).toString();
	//					ptrRaceSelector->setCurrentFraction(strFraction);
	//				}
	//			}
	//			else
	//			{
	//				RaceStatsDisplay * guiItem = getResearchGUIItem(iColumn,iRow);
	//				QModelIndex stIndex = model()->index(iRow,iColumn);
	//				RaceStatsModelItem modelItem = ptrModel->data(stIndex,RaceItemRole).value<RaceStatsModelItem>();
	//				guiItem->setStatsValue(modelItem.iStatsValue);
	//				guiItem->setStatsValueVisibility(modelItem.bShowStatsValue);
	//				guiItem->setStatsName(modelItem.strName);
	//				guiItem->setSeparator(modelItem.bSeparator);
	//			}
	//		}
	//	}	
	//}
}
void CComputerConfView::setModel(QAbstractItemModel *ptrModel)
{
	m_ptrPriv->initialize();
	for(int iIndex=0;iIndex<ptrModel->rowCount();++iIndex)
	{
		m_ptrPriv->appendRow();
	}
	for(int iIndex=0;iIndex<ptrModel->columnCount();++iIndex)
	{
		m_ptrPriv->appendShop();
	}
	
	//


	//QAbstractItemView::setModel(ptrModel);
	//m_ptrPriv->m_ptrModel = ptrModel;
	//m_ptrPriv->removeLayoutWidgets();
	//QModelIndex parent = ptrModel->index(0,0);
	//int iRowCounts = ptrModel->rowCount(parent)-1;
	//for(int i=0;i<iRowCounts;++i)
	//{
	//	RaceStatsDisplay * disp = new RaceStatsDisplay();
	//	disp->setCoords(i,0);
	//	m_ptrPriv->m_ptrLeftRaces->insertWidget(-1, disp);
	//}
	//m_ptrPriv->m_ptrLeftRaces->addStretch();
	////////////////////////////////////////////////////////////////////////////
	//m_ptrPriv->removeLayoutWidgets();
	//parent = ptrModel->index(0,1);
	//iRowCounts = ptrModel->rowCount(parent);
	//for(int i=0;i<iRowCounts;++i)
	//{
	//	RaceStatsDisplay * disp = new RaceStatsDisplay();
	//	disp->setCoords(i,1);
	//	m_ptrPriv->m_ptrMiddleRaces->insertWidget(-1, disp);
	//}
	//m_ptrPriv->m_ptrMiddleRaces->addStretch();
	////////////////////////////////////////////////////////////////////////////
	//m_ptrPriv->removeLayoutWidgets();
	//parent = ptrModel->index(0,2);
	//iRowCounts = ptrModel->rowCount(parent);
	//RaceStatsSelector * selector = new RaceStatsSelector();
	//m_ptrPriv->m_ptrRightRaces->insertWidget(-1, selector);
	//m_ptrPriv->m_ptrSelector = selector;
	//QObject::connect(selector,SIGNAL(raceComboChanged(const QString & )),this, SLOT(OnRaceComboChanged(const QString & )));
	//QObject::connect(selector,SIGNAL(fractionComboChanged(const QString &)),this, SLOT(OnFractionComboChanged(const QString & )));
	//QObject::connect(selector,SIGNAL(showValuesToggled(bool )),this, SLOT(onShowValuesToggled(bool )));
	//QObject::connect(selector,SIGNAL(showBestCombToggled(bool )),this, SLOT(onShowBestCombToggled(bool )));

	//for(int i=7;i<iRowCounts;++i)
	//{
	//	RaceStatsDisplay * disp = new RaceStatsDisplay();
	//	disp->setCoords(i,2);
	//	m_ptrPriv->m_ptrRightRaces->insertWidget(-1, disp);
	//}
	//m_ptrPriv->m_ptrRightRaces->addStretch();
}
void CComputerConfView::performRetranslateUI()
{
	//m_ptrPriv->m_ptrSelector->performRetranslateUI();
}
void CComputerConfView::rowsInserted ( const QModelIndex & parent, int start, int end )
{
	int i=1;
}
//////////////////////////////////////////////////////////////////////////
