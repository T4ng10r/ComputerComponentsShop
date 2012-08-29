#include <GUI/ComponentDelegate.h>
#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include <Data/Enumerations.h>

const int ciSeperator(2);
const int ciMinHeight(40);

void CComponentDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	//draw background
	int total = 100;
	bool bCellRefreshing = index.data(Role_CellIsRefreshing).toBool();
	int iProgress = index.data(Role_CellRefreshProgress).toInt();
	if (!bCellRefreshing)
	{
		QStyledItemDelegate::paint(painter, option, index);
		return;
	}
	QStyleOptionViewItem opt = option;
	opt.rect.setHeight(opt.rect.height()/2-ciSeperator);
	QStyledItemDelegate::paint(painter, opt, index);


	QStyleOptionViewItemV4 opt4 = option;
	const QWidget *widget = NULL;//QStyledItemDelegatePrivate::widget(option);
	QStyle *style = widget ? widget->style() : QApplication::style();
	style->drawControl(QStyle::CE_ItemViewItem, &opt4, painter, widget);

	QStyleOptionProgressBar progressBarOption;
	progressBarOption.rect = option.rect;
	progressBarOption.rect.setY(progressBarOption.rect.y()+option.rect.height()/2-ciSeperator);
	progressBarOption.rect.setHeight(option.rect.height()/2);
	progressBarOption.minimum = 0;
	progressBarOption.maximum = total;
	progressBarOption.progress = iProgress;
	progressBarOption.text = QString();
	progressBarOption.textVisible = false;

	QApplication::style()->drawControl(QStyle::CE_ProgressBar, & progressBarOption, painter);
}
QSize CComponentDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	QRect stDelegateRect = option.rect;
	return QSize(stDelegateRect.width(),qMax(ciMinHeight,stDelegateRect.height()));
}

void drawCell(QPainter *painter, const QStyleOptionViewItemV4 &option, const QModelIndex &index)
{
	////Q_Q(QTableView);
	//QStyleOptionViewItemV4 opt = option;

	//if (index == hover)
	//	opt.state |= QStyle::State_MouseOver;
	//if (option.state & QStyle::State_Enabled) {
	//	QPalette::ColorGroup cg;
	//	if ((model->flags(index) & Qt::ItemIsEnabled) == 0) {
	//		opt.state &= ~QStyle::State_Enabled;
	//		cg = QPalette::Disabled;
	//	} else {
	//		cg = QPalette::Normal;
	//	}
	//	opt.palette.setCurrentColorGroup(cg);
	//}

	//if (index == q->currentIndex()) {
	//	const bool focus = (q->hasFocus() || viewport->hasFocus()) && q->currentIndex().isValid();
	//	if (focus)
	//		opt.state |= QStyle::State_HasFocus;
	//}

	//QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewRow, &opt, painter, q);

	//q->itemDelegate(index)->paint(painter, opt, index);
}