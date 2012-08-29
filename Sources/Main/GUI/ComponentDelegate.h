#include <QtGui/QStyledItemDelegate>

class CComponentDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	explicit CComponentDelegate(QObject *parent = 0) : QStyledItemDelegate(parent)
	{}

	void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
	virtual QSize CComponentDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};