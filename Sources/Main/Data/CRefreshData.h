#ifndef _CCS_REFRESH_DATA_INCLUDE_
#define _CCS_REFRESH_DATA_INCLUDE_
#include <QtCore/QObject>
#include <Plugins/CompPriceData.h>
#include <Plugins/ShopInterface.h>

class CRefreshDataPrivate;

class CRefreshData : public QObject
{
	Q_OBJECT
	friend class CRefreshDataPrivate;
public:
	CRefreshData(void);
	~CRefreshData(void);
signals:
	void refreshingFinished();
public slots:
	//transfer shops and theirs components identificators (names) to internal structures
	void onRefreshConf();
	void onStopRefreshConf();
	void onPriceSearchedFinished(CompPriceData stData);
	void onPriceSearchProgress(QString ,ESearchProgress, int );	//component name, component progress
	void onShopPluginTerminated();	//component name, component progress
protected:
	CRefreshDataPrivate * m_ptrPriv;
};
#endif //_CCS_REFRESH_DATA_INCLUDE_
