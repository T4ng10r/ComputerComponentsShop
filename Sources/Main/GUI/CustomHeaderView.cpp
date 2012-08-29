#include <GUI/CustomHeaderView.h>
#include <GUI/CustomHeaderViewEntry.h>
#include <Data/Enumerations.h>

const unsigned int ciEntrySeparator(10);
void printDebugLog(const QString & strLog);

class CCustomHeaderViewPrivate
{
public:
	CCustomHeaderViewPrivate(CCustomHeaderView * ptrPublic);
	~CCustomHeaderViewPrivate();
	void createSectionWidget();
	void removeSectionWidget();
	inline size_t sectionsCount()
	{	return m_vSectionsWidget.size();	}
	void checkSectionSize(CCustomHeaderViewEntry *);
	void setSectionsValues();
public:
	std::vector<CCustomHeaderViewEntry * >		m_vSectionsWidget;
	CCustomHeaderView *			m_ptrPublic;
	QSize						m_sectionSize;
};
CCustomHeaderViewPrivate::CCustomHeaderViewPrivate(CCustomHeaderView * ptrPublic):m_ptrPublic(ptrPublic){}
CCustomHeaderViewPrivate::~CCustomHeaderViewPrivate()
{
	while(sectionsCount())
	{
		removeSectionWidget();
	}
}
void CCustomHeaderViewPrivate::createSectionWidget()
{
	CCustomHeaderViewEntry * ptrEntry = m_ptrPublic->createEntryObject(m_ptrPublic->viewport());
	ptrEntry->setModel(m_ptrPublic->model());
	ptrEntry->move(QPoint(ciEntrySeparator/2,ciEntrySeparator/2));
	ptrEntry->show();
	checkSectionSize(ptrEntry);
	if (sectionsCount())
	{
		CCustomHeaderViewEntry * ptrPrevEntry = m_vSectionsWidget.back();
		QPoint ptPos = ptrPrevEntry->pos();
		if (m_ptrPublic->orientation()==Qt::Horizontal)
			ptPos+=QPoint(m_sectionSize.width(),0);
		else
			ptPos+=QPoint(0,m_sectionSize.height());
		ptrEntry->move(ptPos);
	}
	m_vSectionsWidget.push_back(ptrEntry);
	setSectionsValues();
	QObject::connect(ptrEntry, SIGNAL(removeEntry(int)), m_ptrPublic, SIGNAL(removeEntry(int)));
	ptrEntry->updateData();
}
void CCustomHeaderViewPrivate::checkSectionSize(CCustomHeaderViewEntry * ptrEntry)
{
	int iMinWidth = ptrEntry->minimumWidth();
	int iMinHeight = ptrEntry->minimumHeight();
	QSize stSizeHint = ptrEntry->sizeHint();
	if (!iMinWidth) iMinWidth = stSizeHint.width();
	if (!iMinHeight) iMinHeight = stSizeHint.height();
	iMinWidth+=ciEntrySeparator;
	iMinHeight+=ciEntrySeparator;
	bool bChange=false;

	if (iMinWidth>m_sectionSize.width())
	{
		m_sectionSize.rwidth() = iMinWidth;
		m_ptrPublic->setMinimumWidth(iMinWidth);
		if (m_ptrPublic->orientation()==Qt::Horizontal)
		{
			m_ptrPublic->setDefaultSectionSize(iMinWidth);
			m_ptrPublic->setMinimumSectionSize(iMinWidth);
		}
		bChange=true;
	}
	if (iMinHeight>m_sectionSize.height())
	{
		m_sectionSize.rheight() = iMinHeight;
		m_ptrPublic->setMinimumHeight(iMinHeight);
		if (m_ptrPublic->orientation()==Qt::Vertical)
		{
			m_ptrPublic->setMinimumSectionSize(iMinHeight);
			m_ptrPublic->setDefaultSectionSize(iMinHeight);
		}
		bChange=true;
	}
	if (bChange)
		emit m_ptrPublic->defaultSectionSizeChanged();
}
void CCustomHeaderViewPrivate::removeSectionWidget()
{
	CCustomHeaderViewEntry * ptrEntry = m_vSectionsWidget.back();
	m_vSectionsWidget.pop_back();
	delete ptrEntry;
	setSectionsValues();
	if (!sectionsCount())
		m_sectionSize = QSize(-1,-1);
}
void CCustomHeaderViewPrivate::setSectionsValues()
{
	int i=0;
	for(std::vector<CCustomHeaderViewEntry * >::iterator iterSect = m_vSectionsWidget.begin();
		iterSect!=m_vSectionsWidget.end();++iterSect,++i)
	{
		(*iterSect)->setSectionIndex(i);
		//#if defined VIEW_WITH_FROZEN_COLUMN
		//	if (i==0 && m_ptrPublic->orientation()==Qt::Horizontal)
		//		(*iterSect)->hide();
		//#endif
		//#if defined VIEW_WITH_FROZEN_ROW
		//	if (i==0 && m_ptrPublic->orientation()==Qt::Vertical)
		//		(*iterSect)->hide();
		//#endif
	}
}
//////////////////////////////////////////////////////////////////////////
CCustomHeaderView::CCustomHeaderView(Qt::Orientation orientation,QWidget *parent): QHeaderView(orientation, parent)
{
	m_ptrPriv = new CCustomHeaderViewPrivate(this);
	//////////////////////////////////////////////////////////////////////////
	connect(this, SIGNAL(sectionResized ( int , int , int )),this,SLOT(onSectionResized(int,int,int)));
	connect(this, SIGNAL(sectionCountChanged ( int , int )),this,SLOT(onSectionCountChanged(int , int)));
}
CCustomHeaderView::~CCustomHeaderView()
{
	delete m_ptrPriv;
}
void CCustomHeaderView::setModel(QAbstractItemModel * ptrModel)
{
	QHeaderView::setModel(ptrModel);
	int iCount = orientation()==Qt::Horizontal?ptrModel->columnCount():ptrModel->rowCount();
	for(int i=m_ptrPriv->sectionsCount();i<iCount;++i)
		m_ptrPriv->createSectionWidget();
	for(int iX=0;iX<iCount;++iX)
	{
		CCustomHeaderViewEntry * ptrEntry = m_ptrPriv->m_vSectionsWidget[iX];
		ptrEntry->setModel(ptrModel);
		ptrEntry->updateData();
	}
}
//////////////////////////////////////////////////////////////////////////
void CCustomHeaderView::headerDataChanged ( Qt::Orientation orientation, int logicalFirst, int logicalLast )
{
	if (orientation!=this->orientation())	return;
	for(int iX=logicalFirst;iX<=logicalLast;++iX)
	{
		CCustomHeaderViewEntry * ptrEntry = m_ptrPriv->m_vSectionsWidget[iX];
		ptrEntry->updateData();
	}
}
void CCustomHeaderView::onSectionResized(int section, int oldSize, int newSize)
{
	int iPrevSectionSize(-1);
	QSize sEntrySize = sectionSizeFromContents(section);
	CCustomHeaderViewEntry * ptrPrevEntry;
	int iCount  = m_ptrPriv->sectionsCount();
	if (section < iCount)
	{
		//get resized section size
		CCustomHeaderViewEntry * ptrEntry = m_ptrPriv->m_vSectionsWidget[section];
		if (orientation()==Qt::Horizontal)
			sEntrySize = QSize(newSize-ciEntrySeparator,m_ptrPriv->m_sectionSize.height()-ciEntrySeparator);
		else
			sEntrySize = QSize(m_ptrPriv->m_sectionSize.width()-ciEntrySeparator,newSize-ciEntrySeparator);
		ptrEntry->setFixedSize(sEntrySize);
		ptrPrevEntry = ptrEntry;
		//if (section==0)
		//	iPrevSectionSize = sEntrySize.width();
	}
	else
		return;
	//change position of all NEXT widgets
	for(int i = section+1;i<iCount;i++)
	{
		sEntrySize = ptrPrevEntry->minimumSize();
		QPoint ptPos = ptrPrevEntry->pos();
		if (iPrevSectionSize==-1)
		{
			if (orientation()==Qt::Horizontal)
				ptPos.rx()+=sEntrySize.width()+ciEntrySeparator;
			else
				ptPos.ry()+=sEntrySize.height()+ciEntrySeparator;
		}
		else
		{
			if (orientation()==Qt::Horizontal)
				ptPos.rx()+=iPrevSectionSize+ciEntrySeparator;
			else
				ptPos.ry()+=iPrevSectionSize+ciEntrySeparator;
			iPrevSectionSize=-1;
		}
		CCustomHeaderViewEntry * ptrEntry = m_ptrPriv->m_vSectionsWidget[i];
		ptrEntry->move(ptPos);
		ptrPrevEntry = ptrEntry;
	}
}
void CCustomHeaderView::onSectionCountChanged ( int oldCount, int newCount )
{
	if (m_ptrPriv->sectionsCount()==newCount)
		return;
	if ((size_t)newCount>(m_ptrPriv->sectionsCount()))
	{
		for(size_t i=m_ptrPriv->sectionsCount();i<(size_t)newCount;++i)
		{
			m_ptrPriv->createSectionWidget();
		}
	}
	else if ((m_ptrPriv->sectionsCount())>(size_t)newCount)
	{
		for(size_t i=(size_t)newCount;i<m_ptrPriv->sectionsCount();++i)
		{
			m_ptrPriv->removeSectionWidget();
		}
	}
}
void CCustomHeaderView::sectionsAboutToBeRemoved ( const QModelIndex & parent, int logicalFirst, int logicalLast ) 
{
	//remove selected sections
	for(int iX=logicalFirst;iX<=logicalLast;++iX)
	{
		CCustomHeaderViewEntry * ptrEntry = m_ptrPriv->m_vSectionsWidget[logicalFirst];
		m_ptrPriv->m_vSectionsWidget.erase(m_ptrPriv->m_vSectionsWidget.begin()+logicalFirst);
		//ptrEntry
		delete ptrEntry;
		int  stSize = sectionSize(0);
		onSectionResized(0,stSize,stSize);
		m_ptrPriv->setSectionsValues();
	}
	if (m_ptrPriv->sectionsCount()==1)
	{
		CCustomHeaderViewEntry * ptrEntry = m_ptrPriv->m_vSectionsWidget.back();
		ptrEntry->move(QPoint(ciEntrySeparator/2,ciEntrySeparator/2));
	}

}