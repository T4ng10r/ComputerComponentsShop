#ifndef _CCS_SHOP_HEADER_VIEW_INCLUDE_
#define _CCS_SHOP_HEADER_VIEW_INCLUDE_
#include <GUI/CustomHeaderView.h>

class CShopHeaderView : public CCustomHeaderView
{
public:
	CShopHeaderView(QWidget *parent = 0): CCustomHeaderView(Qt::Horizontal,parent){};
	virtual CCustomHeaderViewEntry* createEntryObject(QWidget *parent);
};

#endif //_CCS_SHOP_HEADER_VIEW_INCLUDE_
