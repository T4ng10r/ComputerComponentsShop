#include <GUI/FrozenTableView.h>
#include <GUI/ComponentDelegate.h>
#include <QtGui/QHeaderView>
#include <QtGui/QScrollBar>
#include <QtGui/QMenu>
#include <QtGui/QFont>

void printDebugLog(const QString & strLog);
void printSlotsConnectionLog(const QString & strLog);

#include <GUI/ShopHeaderView.h>
#include <GUI/ComponentHeaderView.h>

class FrozenTableViewPrivate
{
public:
	FrozenTableViewPrivate(FrozenTableView * ptrPublic);
	~FrozenTableViewPrivate();
	void init();
	void initModel(QAbstractItemModel * pModel);
	void logConnection(QString strConnDesc,bool bResult);
	void updateFrozenTableGeometry();
	void updateRowsHeigth();
	void updateColumnsWidth();
	void setConnections();
public:
	FrozenTableView *			m_ptrPublic;
	CShopHeaderView *			m_ptrShopHeader;
	CComponentHeaderView *		m_ptrComponentHeader;
	CComponentDelegate *		m_ptrComponentDelegate;
	QMenu *						m_ptrContextMenu;

#ifdef VIEW_WITH_FROZEN_COLUMN
	QTableView *				frozenTableColumnView;
#endif
#ifdef VIEW_WITH_FROZEN_ROW
	QTableView *				frozenTableRowView;
#endif
};
FrozenTableViewPrivate::FrozenTableViewPrivate(FrozenTableView * ptrPublic)
{
	m_ptrPublic = ptrPublic;
	init();
	m_ptrComponentDelegate = new CComponentDelegate;
	m_ptrPublic->setItemDelegate(m_ptrComponentDelegate);
	updateFrozenTableGeometry();
	setConnections();
}
FrozenTableViewPrivate::~FrozenTableViewPrivate()
{
	delete frozenTableColumnView;
	delete frozenTableRowView;
}
void FrozenTableViewPrivate::init()
{
	m_ptrShopHeader = new CShopHeaderView;
	m_ptrPublic->setHorizontalHeader(m_ptrShopHeader);
	m_ptrComponentHeader = new CComponentHeaderView;
	m_ptrPublic->setVerticalHeader(m_ptrComponentHeader);
	//////////////////////////////////////////////////////////////////////////
	frozenTableColumnView = new QTableView(m_ptrPublic);
	frozenTableRowView = new QTableView(m_ptrPublic);
	//////////////////////////////////////////////////////////////////////////
	frozenTableColumnView->setFocusPolicy(Qt::NoFocus);
	frozenTableColumnView->verticalHeader()->hide();
	frozenTableColumnView->horizontalHeader()->hide();
	frozenTableColumnView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);

	frozenTableRowView->setFocusPolicy(Qt::NoFocus);
	frozenTableRowView->horizontalHeader()->hide();
	frozenTableRowView->verticalHeader()->hide();
	frozenTableRowView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
	//////////////////////////////////////////////////////////////////////////
	m_ptrPublic->viewport()->stackUnder(frozenTableRowView);
	m_ptrPublic->viewport()->stackUnder(frozenTableColumnView);
	//////////////////////////////////////////////////////////////////////////
	frozenTableColumnView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	frozenTableColumnView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	frozenTableRowView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	frozenTableRowView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	frozenTableColumnView->setColumnWidth(0, m_ptrPublic->columnWidth(0) );
	frozenTableRowView->setRowHeight(0, m_ptrPublic->rowHeight(0) );
	//////////////////////////////////////////////////////////////////////////
	m_ptrPublic->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_ptrPublic->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	frozenTableColumnView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	frozenTableRowView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	//////////////////////////////////////////////////////////////////////////
}
void FrozenTableViewPrivate::initModel(QAbstractItemModel * pModel)
{
	frozenTableColumnView->setModel(pModel);
	frozenTableRowView->setModel(pModel);

	//frozenTableColumnView->setSelectionModel(m_ptrPublic->selectionModel());
	//frozenTableRowView->setSelectionModel(m_ptrPublic->selectionModel());

	for(int iCol=1; iCol<pModel->columnCount(); iCol++)
		frozenTableColumnView->setColumnHidden(iCol, true);
	for(int iRow=1; iRow<pModel->rowCount(); iRow++)
		frozenTableRowView->setRowHidden(iRow, true);

	updateRowsHeigth();
	updateColumnsWidth();
	updateFrozenTableGeometry();
}
void FrozenTableViewPrivate::updateRowsHeigth()
{
	int iRowCount = m_ptrPublic->model()->rowCount();
	for(int iRow = 0;iRow<iRowCount;++iRow)
	{
		int iHeight = m_ptrPublic->rowHeight(iRow);
		if (iRow==0)
			frozenTableRowView->setRowHeight(0,iHeight);
		frozenTableColumnView->setRowHeight(iRow,iHeight);
	}
}
void FrozenTableViewPrivate::updateColumnsWidth()
{
	frozenTableRowView->setRowHeight(0,m_ptrPublic->rowHeight(0));
	int iColCount = m_ptrPublic->model()->columnCount();
	for(int iCol = 0;iCol<iColCount;++iCol)
	{
		int iWidth = m_ptrPublic->columnWidth(iCol);
		if (iCol==0)
			frozenTableColumnView->setColumnWidth(0,iWidth);
		frozenTableRowView->setColumnWidth(iCol,iWidth);
	}
}
void FrozenTableViewPrivate::updateFrozenTableGeometry()
{
	int iX = m_ptrPublic->verticalHeader()->width();
	int iY = m_ptrPublic->horizontalHeader()->height();
	int iWidth = m_ptrPublic->columnWidth(0);
	int iHeigth = m_ptrPublic->verticalHeader()->height()+m_ptrPublic->frameWidth();
	frozenTableColumnView->setGeometry( iX, iY, iWidth, iHeigth );
	iWidth = m_ptrPublic->horizontalHeader()->width()+m_ptrPublic->frameWidth();
	iHeigth = m_ptrPublic->rowHeight(0);
	frozenTableRowView->setGeometry( iX, iY, iWidth, iHeigth  );
}
void FrozenTableViewPrivate::logConnection(QString strConnDesc,bool bResult)
{
	QString strDebug = "Connection "+strConnDesc;
	if (bResult)	strDebug+=" SUCCESS";	else	strDebug+=" FAIL";
	printSlotsConnectionLog(strDebug);
	Q_ASSERT_X(bResult==true,"FrozenTableViewPrivate::setConnections",QString("Connect %1").arg(strConnDesc).toAscii());
}
void FrozenTableViewPrivate::setConnections()
{
	bool bResult=false;
	bResult = QObject::connect(m_ptrPublic->horizontalHeader(),SIGNAL(sectionResized(int,int,int)), m_ptrPublic,
		SLOT(updateSectionWidth(int,int,int)));
	QString strConnDesc = "'horizontalHeader::sectionResized' with 'updateSectionWidth'";
	logConnection(strConnDesc,bResult);
	//////////////////////////////////////////////////////////////////////////
	bResult = QObject::connect(m_ptrPublic->verticalHeader(),SIGNAL(sectionResized(int,int,int)), m_ptrPublic,
		SLOT(updateSectionHeight(int,int,int)));
	strConnDesc = "'verticalHeader::sectionResized' with 'updateSectionHeight'";
	logConnection(strConnDesc,bResult);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	bResult = QObject::connect(m_ptrPublic->verticalHeader(),SIGNAL(defaultSectionSizeChanged()), m_ptrPublic,
		SLOT(onComponentHeadersSectionSizeChanged()));
	strConnDesc = "'verticalHeader::defaultSectionSizeChanged' with 'onComponentHeadersSectionSizeChanged'";
	logConnection(strConnDesc,bResult);
	//////////////////////////////////////////////////////////////////////////
	bResult = QObject::connect(m_ptrPublic->horizontalHeader(),SIGNAL(defaultSectionSizeChanged()), m_ptrPublic,
		SLOT(onShopsHeadersSectionSizeChanged()));
	strConnDesc = "'verticalHeader::defaultSectionSizeChanged' with 'onShopsHeadersSectionSizeChanged'";
	logConnection(strConnDesc,bResult);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	bResult = QObject::connect(m_ptrPublic->verticalScrollBar(), SIGNAL(valueChanged(int)),
		frozenTableColumnView->verticalScrollBar(), SLOT(setValue(int)));
	strConnDesc = "'public::verticalScrollBar' with 'setValue'";
	logConnection(strConnDesc,bResult);
	//////////////////////////////////////////////////////////////////////////
	bResult = QObject::connect(m_ptrPublic->horizontalScrollBar(), SIGNAL(valueChanged(int)),
		frozenTableRowView->horizontalScrollBar(), SLOT(setValue(int)));
	strConnDesc = "'public::horizontalScrollBar' with 'setValue'";
	logConnection(strConnDesc,bResult);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	bResult = QObject::connect(frozenTableColumnView->verticalScrollBar(), SIGNAL(valueChanged(int)),
		m_ptrPublic->verticalScrollBar(), SLOT(setValue(int)));
	strConnDesc = "'verticalScrollBar' with 'public::verticalScrollBar::setValue'";
	logConnection(strConnDesc,bResult);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	bResult = QObject::connect(m_ptrShopHeader, SIGNAL(removeEntry(int)),
		m_ptrPublic, SIGNAL(removeShop(int)));
	strConnDesc = "'m_ptrShopHeader::removeShop' with 'removeShop'";
	logConnection(strConnDesc,bResult);
	//////////////////////////////////////////////////////////////////////////
	bResult = QObject::connect(m_ptrComponentHeader, SIGNAL(removeEntry(int)) ,
		m_ptrPublic, SIGNAL(removeComponent(int)));
	strConnDesc = "'m_ptrComponentHeader::removeComponent' with 'removeComponent'";
	logConnection(strConnDesc,bResult);
	//////////////////////////////////////////////////////////////////////////
	bResult = QObject::connect(m_ptrPublic, SIGNAL(doubleClicked (QModelIndex)),
		m_ptrPublic, SIGNAL(onOpenLink(QModelIndex)));
	strConnDesc = "'public:doubleClicked' with signal 'onOpenLink'";
	logConnection(strConnDesc,bResult);
	//////////////////////////////////////////////////////////////////////////
	bResult = QObject::connect(m_ptrPublic, SIGNAL(customContextMenuRequested (QPoint)),
		m_ptrPublic, SLOT(onCustomContextMenuRequested(QPoint)));
	strConnDesc = "'public:customContextMenuRequested' with signal 'onCustomContextMenuRequested'";
	logConnection(strConnDesc,bResult);
}
FrozenTableView::FrozenTableView(QWidget *parent):QTableView(parent)
{
	m_ptrPriv = new FrozenTableViewPrivate(this);
	setContextMenuPolicy(Qt::CustomContextMenu);
	//connect the headers and scrollbars of both tableviews together

}
FrozenTableView::~FrozenTableView()
{
	delete m_ptrPriv;
}
void FrozenTableView::setModel(QAbstractItemModel * pModel)
{
	QTableView::setModel(pModel);
	m_ptrPriv->initModel(pModel);
	bool bResult = QObject::connect(pModel, SIGNAL(columnsInserted (QModelIndex,int,int)) ,
		this, SLOT(onRowsColumnsCountChanged(QModelIndex,int,int)));
	QString strConnDesc = "'pModel::columnsInserted' with 'onRowsColumnsCountChanged'";
	m_ptrPriv->logConnection(strConnDesc,bResult);
	//////////////////////////////////////////////////////////////////////////
	bResult = QObject::connect(pModel, SIGNAL(columnsRemoved  (QModelIndex,int,int)) ,
		this, SLOT(onRowsColumnsCountChanged(QModelIndex,int,int)));
	strConnDesc = "'pModel::columnsRemoved' with 'onRowsColumnsCountChanged'";
	m_ptrPriv->logConnection(strConnDesc,bResult);
	bResult = QObject::connect(pModel, SIGNAL(rowsInserted(QModelIndex,int,int)) ,
		this, SLOT(onRowsColumnsCountChanged(QModelIndex,int,int)));
	strConnDesc = "'pModel::rowsInserted' with 'onRowsColumnsCountChanged'";
	m_ptrPriv->logConnection(strConnDesc,bResult);
	//////////////////////////////////////////////////////////////////////////
	bResult = QObject::connect(pModel, SIGNAL(rowsRemoved  (QModelIndex,int,int)) ,
		this, SLOT(onRowsColumnsCountChanged(QModelIndex,int,int)));
	strConnDesc = "'pModel::rowsRemoved' with 'onRowsColumnsCountChanged'";
	m_ptrPriv->logConnection(strConnDesc,bResult);
}
void FrozenTableView::updateSectionWidth(int logicalIndex, int, int newSize)
{
	m_ptrPriv->frozenTableRowView->setColumnWidth(logicalIndex, newSize);
	if(logicalIndex==0){
		m_ptrPriv->frozenTableColumnView->setColumnWidth(0,newSize);
	}
	m_ptrPriv->updateFrozenTableGeometry();
}
void FrozenTableView::updateSectionHeight(int logicalIndex, int oldSize, int newSize)
{
	m_ptrPriv->frozenTableColumnView->setRowHeight(logicalIndex, newSize);
	if(logicalIndex==0)
	{
		m_ptrPriv->frozenTableRowView->setRowHeight(0, newSize);
	}
	m_ptrPriv->updateFrozenTableGeometry();
}
void FrozenTableView::resizeEvent(QResizeEvent * event)
{
	QTableView::resizeEvent(event);
	m_ptrPriv->updateFrozenTableGeometry();
}
QModelIndex FrozenTableView::moveCursor(CursorAction cursorAction,
										  Qt::KeyboardModifiers modifiers)
{
	QModelIndex current = QTableView::moveCursor(cursorAction, modifiers);

	//if(cursorAction == MoveLeft && current.column()>0
	//	&& visualRect(current).topLeft().x() < frozenTableView->columnWidth(0) ){

	//		const int newValue = horizontalScrollBar()->value() + visualRect(current).topLeft().x()
	//			- frozenTableView->columnWidth(0);
	//		horizontalScrollBar()->setValue(newValue);
	//}
	return current;
}
void FrozenTableView::scrollTo (const QModelIndex & index, ScrollHint hint){
	if(index.column()>0)
		QTableView::scrollTo(index, hint);
}

void FrozenTableView::onSetValue(int ii)
{
	printDebugLog(QString("SetValue1: toSetVal(%1) current(%2)").arg(ii)
		.arg(m_ptrPriv->frozenTableColumnView->verticalScrollBar()->value())
		);
	m_ptrPriv->frozenTableColumnView->verticalScrollBar()->setValue(ii);
	printDebugLog(QString("SetValue2: toSetVal(%1) current(%2)").arg(ii)
		.arg(m_ptrPriv->frozenTableColumnView->verticalScrollBar()->value())
		);
	//verticalScrollBar()
	//printDebugLog(QString("SetValue: val(%1)").arg(ii));
}
void FrozenTableView::onRowsColumnsCountChanged(const QModelIndex & parent, int start, int end)
{
	m_ptrPriv->updateFrozenTableGeometry();
}
void FrozenTableView::onComponentHeadersSectionSizeChanged()
{
	m_ptrPriv->frozenTableColumnView->verticalHeader()->setDefaultSectionSize(m_ptrPriv->m_ptrComponentHeader->minimumSectionSize());
	m_ptrPriv->frozenTableRowView->verticalHeader()->setDefaultSectionSize(m_ptrPriv->m_ptrComponentHeader->minimumSectionSize());
}
void FrozenTableView::onShopsHeadersSectionSizeChanged()
{
	m_ptrPriv->frozenTableRowView->horizontalHeader()->setDefaultSectionSize(m_ptrPriv->m_ptrShopHeader->minimumSectionSize());
	m_ptrPriv->frozenTableColumnView->horizontalHeader()->setDefaultSectionSize(m_ptrPriv->m_ptrShopHeader->minimumSectionSize());
}
void FrozenTableView::onCustomContextMenuRequested ( const QPoint & pos )
{
	QMenu *menu = new QMenu;
	QModelIndex cell = this->indexAt(pos);
	QPoint stPos(cell.row(),cell.column());
	if(cell.isValid())
	{
		QAction * ptrProdPage = menu->addAction("Go to product page",this,SLOT(onGoToProductPageCustomMenu()));
		QFont stFont = ptrProdPage->font();
		stFont.setBold(true);
		ptrProdPage->setFont(stFont);
		ptrProdPage->setIconVisibleInMenu(false);
		ptrProdPage->setData(stPos);
		//////////////////////////////////////////////////////////////////////////
		QAction * ptrSearchPage = menu->addAction("Go to search page",this,SLOT(onGoToSearchPageCustomMenu()));
		ptrSearchPage->setIconVisibleInMenu(false);
		ptrSearchPage->setData(stPos);
		//////////////////////////////////////////////////////////////////////////
		QAction * ptrRefreshCell = menu->addAction("Refresh cell",this,SLOT(onRefreshCellData()));
		ptrRefreshCell->setIconVisibleInMenu(false);
		ptrRefreshCell->setData(stPos);
		//////////////////////////////////////////////////////////////////////////
		menu->addSeparator();
		QAction * ptrResetCache = menu->addAction("Reset field",this,SLOT(onResetFieldCache()));
		ptrResetCache->setIconVisibleInMenu(false);
		ptrResetCache->setData(stPos);
		menu->exec(this->viewport()->mapToGlobal(pos));
	}
}
void FrozenTableView::onGoToProductPageCustomMenu()
{
	QAction * ptrProductPage = qobject_cast<QAction *>(sender());
	QPoint sPos = ptrProductPage->data().toPoint();
	QModelIndex stIndex = model()->index(sPos.x(),sPos.y());
	emit onOpenLink(stIndex);
}
void FrozenTableView::onGoToSearchPageCustomMenu()
{
	QAction * ptrSearchPage = qobject_cast<QAction *>(sender());
	QPoint sPos = ptrSearchPage->data().toPoint();
	QModelIndex stIndex = model()->index(sPos.x(),sPos.y());
	emit onOpenSearchPage( stIndex );
}
void FrozenTableView::onResetFieldCache()
{
	QAction * ptrSearchPage = qobject_cast<QAction *>(sender());
	QPoint sPos = ptrSearchPage->data().toPoint();
	QModelIndex stIndex = model()->index(sPos.x(),sPos.y());
	emit resetFieldCache( stIndex );
}
void FrozenTableView::onRefreshCellData()
{

}
