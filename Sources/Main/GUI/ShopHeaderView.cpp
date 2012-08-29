#include <GUI/ShopHeaderView.h>
#include <GUI/ShopHeaderEntry.h>

CCustomHeaderViewEntry* CShopHeaderView::createEntryObject(QWidget *parent)
{
	return new CShopHeaderEntry(parent);
};