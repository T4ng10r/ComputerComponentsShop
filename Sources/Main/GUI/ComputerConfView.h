#ifndef _CCS_VIEW_INCLUDE_
#define _CCS_VIEW_INCLUDE_
#include <QtGui/QAbstractItemView>

class CComputerConfViewPrivate;

class CComputerConfView : public QAbstractItemView
{
	Q_OBJECT
public:
	CComputerConfView(QWidget *parent = 0);
	~CComputerConfView();
public:
	QRect visualRect(const QModelIndex &index) const { return QRect(); }	//inherited
	void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible){}	//inherited
	QModelIndex indexAt(const QPoint &point) const { return QModelIndex(); }	//inherited
	void setModel(QAbstractItemModel *model);
	void performRetranslateUI();
protected slots:
	void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
	void rowsInserted ( const QModelIndex & parent, int start, int end );
protected:
	void setupUI();

	//bool edit(const QModelIndex &index, EditTrigger trigger, QEvent *event);
	virtual QModelIndex moveCursor(CursorAction cursorAction,
		Qt::KeyboardModifiers modifiers) { return QModelIndex(); };

	virtual int horizontalOffset() const { return 0; }
	virtual int verticalOffset() const  { return 0; }

	virtual bool isIndexHidden(const QModelIndex &index) const { return false; }
	virtual void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command) {}
	virtual QRegion visualRegionForSelection(const QItemSelection &selection) const { return QRegion(); }
protected:
	CComputerConfViewPrivate *	m_ptrPriv;
};
#endif //_CCS_VIEW_INCLUDE_
