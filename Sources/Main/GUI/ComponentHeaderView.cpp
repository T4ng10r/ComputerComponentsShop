#include <GUI/ComponentHeaderView.h>
#include <GUI/ComponentHeaderEntry.h>

CCustomHeaderViewEntry* CComponentHeaderView::createEntryObject(QWidget *parent)
{
	return new CComponentHeaderEntry(parent);
};