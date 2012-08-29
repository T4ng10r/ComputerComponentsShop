#ifndef _CCS_COMP_HEADER_VIEW_INCLUDE_
#define _CCS_COMP_HEADER_VIEW_INCLUDE_
#include <GUI/CustomHeaderView.h>

class CComponentHeaderView : public CCustomHeaderView
{
public:
	CComponentHeaderView(QWidget *parent = 0): CCustomHeaderView(Qt::Vertical,parent){};
	virtual CCustomHeaderViewEntry* createEntryObject(QWidget *parent);
};
#endif //_CCS_COMP_HEADER_VIEW_INCLUDE_
