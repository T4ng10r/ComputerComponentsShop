#ifndef _CCS_CUSTOM_HEADER_VIEW_INCLUDE_
#define _CCS_CUSTOM_HEADER_VIEW_INCLUDE_
#include <QtGui/QHeaderView>

class CCustomHeaderViewPrivate;
class CCustomHeaderViewEntry;

class CCustomHeaderView : public QHeaderView
{
	friend class CCustomHeaderViewPrivate;
	Q_OBJECT
public:
	CCustomHeaderView(Qt::Orientation orientation,QWidget *parent = 0);
	~CCustomHeaderView();
	void setModel(QAbstractItemModel *model);
	virtual CCustomHeaderViewEntry* createEntryObject(QWidget *parent)=0;
signals:
	void removeEntry(int iSection);
	void defaultSectionSizeChanged();
protected slots:
	//Updates the changed header sections with the given orientation, 
	// from logicalFirst to logicalLast inclusive.
	void headerDataChanged ( Qt::Orientation orientation, int logicalFirst, int logicalLast );
	//This signal is emitted when the number of sections changes, 
	//i.e., when sections are added or deleted. 
	// The original count is specified by oldCount, and the new count by newCount.
	void onSectionCountChanged( int , int );
	//The section's logical number is specified by logicalIndex, 
	//the old size by oldSize, and the new size by newSize.
	void onSectionResized(int section, int oldSize, int newSize);
	void sectionsAboutToBeRemoved ( const QModelIndex & parent, int logicalFirst, int logicalLast ) ;
protected:
	CCustomHeaderViewPrivate *	m_ptrPriv;
};

#endif //_CCS_CUSTOM_HEADER_VIEW_INCLUDE_
