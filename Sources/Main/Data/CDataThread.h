#ifndef _CCS_DATA_THREAD_INCLUDE_
#define _CCS_DATA_THREAD_INCLUDE_
#include <QtCore/QThread>
#include <vector>
#include <Plugins/CompPriceData.h>
#include <Data/ProxySettings.h>
#include <QtGui/QStandardItemModel>

class CComputerConfModel;
class CDataThreadPrivate;
class ShopInterface;

class CDataThread : public QThread
{
	Q_OBJECT
	friend class CDataThreadPrivate;
	CDataThread(void);
	static CDataThread* pInstance_;
public:
	~CDataThread(void);
	static CDataThread* getInstance();
	void run();
	CComputerConfModel * getConfigurationModel();
	QStandardItemModel * getRefreshModel();
	const ProxySettings & getProxySettings();
	void setProxySettings(const ProxySettings &);
	std::vector<ShopInterface *> getPlugins();
	QString getCurrentFile();
	void onUpdateRecentFileActions();
signals:
	void refreshingFinished();
	void onStatusBarMsg(const QString & );
	void updateRecentFilesAction(const QStringList &);
public slots:
	void onAddShop();
	void onAddComp();
	void onRemComp(int iRow);
	void onRemShop(int iCol);
	void onNewConf();
	void onOpenConf(const QString strFilename=0);
	void onSaveConf();
	void onSaveAsConf();
	void onRefreshConf();
	void onStopRefreshingConf();
	void onOpenLinkInExternalBrowser(const QModelIndex & stIndex);
	void onOpenSearchInExternalBrowser(const QModelIndex & stIndex);
	void onResetFieldCache(const QModelIndex & stIndex);
protected:
	CDataThreadPrivate * m_ptrPriv;
	CComputerConfModel * m_ptrModel;
};
#endif //_CCS_DATA_THREAD_INCLUDE_
