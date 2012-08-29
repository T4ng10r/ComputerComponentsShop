#ifndef _CCS_SHOP_INTERFACE_INCLUDE_
#define _CCS_SHOP_INTERFACE_INCLUDE_
#include <QtCore/QString> 
#include <QtCore/QThread> 
#include <QtGui/QIcon> 
#include "CompPriceData.h"

enum ESearchProgress
{
	ETEST_OLD_URL,
	ESEARCHING_PRODUCT,
	ELOAD_PAGE
};

class ShopInterface : public QThread
{
	Q_OBJECT
public:
	virtual ~ShopInterface() {}

	virtual QString		pluginName() const = 0;
	virtual QString		shopName() const = 0;
	virtual QIcon		getShopIcon() const = 0;
	//returns URL to eShop
	virtual QString		getShopURL() const = 0;
	//returns name of currently searched product
	virtual QString		getCurrentlyProcessedComponent() const = 0;
	//returns URL to HTML product search page
	virtual QString		getSearchProductPageURL(const QString &strCompName) = 0;
	//search for product data
	//starts Thread
	virtual void		getProductData(const QString & strCompName, const QString & strOldURL = QString() ) = 0;
signals:
	void priceSearchedFinished(CompPriceData stData);
	void priceSearchProgress(QString ,ESearchProgress, int );	//component name, component progress
};
Q_DECLARE_INTERFACE(ShopInterface, "ShopInterface/1.0");

#endif //_CCS_SHOP_INTERFACE_INCLUDE_