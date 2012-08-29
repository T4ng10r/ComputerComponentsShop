#ifndef _CCS_COMP_HEADER_ENTRY_INCLUDE_
#define _CCS_COMP_HEADER_ENTRY_INCLUDE_
#include <GUI/CustomHeaderViewEntry.h>
//#include <QtGui/QHeaderView>

class CComponentHeaderEntryPrivate;

class CComponentHeaderEntry : public CCustomHeaderViewEntry
{
	Q_OBJECT
public:
	CComponentHeaderEntry(QWidget *parent = 0);
	~CComponentHeaderEntry();
	void updateData();
	void setSectionIndex(int iSection);
//signals:
//	void removeComponent(int iSection);
protected slots:
	void onActiveClicked(bool);
	void onRemoveClicked(bool);
	void onComponentCodeChanged();
	void onCompNameEdited(QString);
protected:
	CComponentHeaderEntryPrivate *	m_ptrPriv;
};

#endif //_CCS_COMP_HEADER_ENTRY_INCLUDE_
