#ifndef _CCS_SHOP_HEADER_ENTRY_INCLUDE_
#define _CCS_SHOP_HEADER_ENTRY_INCLUDE_
#include <GUI/CustomHeaderViewEntry.h>

class CShopHeaderEntryPrivate;

class CShopHeaderEntry : public CCustomHeaderViewEntry
{
	Q_OBJECT
public:
	CShopHeaderEntry(QWidget *parent = 0);
	void updateData();
	void setSectionIndex(int iSection);
	void retranslateUI();
//signals:
//	void removeShop(int iSection);
protected slots:
	void onActiveClicked(bool);
	void onRemoveClicked(bool);
	void onShopChanged(const QString & strVal);
protected:
	void setActive(bool bActive);
	void setShop(QString strShopName);
	void setShopList(QStringList strShopsList);
	void setShopIcons(QVariantList lIconsList);
protected:
	CShopHeaderEntryPrivate *	m_ptrPriv;
};

#endif //_CCS_SHOP_HEADER_ENTRY_INCLUDE_
