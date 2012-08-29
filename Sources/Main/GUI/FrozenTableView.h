#ifndef _CCS_FROZEN_TABLE_VIEW_INCLUDE_
#define _CCS_FROZEN_TABLE_VIEW_INCLUDE_

#include <QtGui/QTableView>
#include <QtGui/QHeaderView>

class FrozenTableViewPrivate;

class FrozenTableView : public QTableView
{
	Q_OBJECT

public:
	FrozenTableView(QWidget *parent = 0);
	~FrozenTableView();
	virtual void setModel(QAbstractItemModel * pModel);

signals:
	void removeShop(int);
	void removeComponent(int);
	void onOpenLink( const QModelIndex & index );
	void onOpenSearchPage( const QModelIndex & index );
	void resetFieldCache( const QModelIndex & index );
protected slots:
	void onRowsColumnsCountChanged(const QModelIndex & parent, int start, int end);
	void onComponentHeadersSectionSizeChanged();
	void onShopsHeadersSectionSizeChanged();
	void onCustomContextMenuRequested ( const QPoint & pos );
	void onGoToProductPageCustomMenu();
	void onGoToSearchPageCustomMenu();
	void onResetFieldCache();
	void onRefreshCellData();
protected:
	virtual void resizeEvent(QResizeEvent *event);
	virtual QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
	void scrollTo (const QModelIndex & index, ScrollHint hint = EnsureVisible);
private:
	FrozenTableViewPrivate *	m_ptrPriv;

	private slots:
		void updateSectionWidth(int logicalIndex,int, int newSize);
		void updateSectionHeight(int logicalIndex, int, int newSize);
		void onSetValue(int);

};
#endif //_CCS_FROZEN_TABLE_VIEW_INCLUDE_
