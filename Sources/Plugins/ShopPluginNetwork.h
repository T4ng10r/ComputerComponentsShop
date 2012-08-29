#ifndef _CCS_SHOP_PLUGIN_NETWORK_INCLUDE_
#define _CCS_SHOP_PLUGIN_NETWORK_INCLUDE_
#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QSharedPointer>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QAuthenticator>
#include <QtNetwork/QSslError>

#define PRINT_LOADED_PAGE_CONTENT
//#define USE_WEBKIT
#ifdef USE_WEBKIT
#define LOAD_WITH_WEBKIT
#endif
#define USE_XML_PARSER

#define TIMEOUT_INFO  QString("Timeout")
#define ERROR_INFO  QString("Timeout")

class ShopPluginNetworkPrivate;

//class responsible for downloading HTML page contents and handling signal-slots
class ShopPluginNetwork : public QObject
{
	Q_OBJECT
	friend class ShopPluginNetworkPrivate;
public:
	ShopPluginNetwork();
	~ShopPluginNetwork();
public slots:
	//start loading selected page
	void loadPage(const QString &strUrl);
//////////////////////////////////////////////////////////////////////////
protected slots:
	void onPageDownloadFinished();
	//if waiting time till page download expires
	void onPageLoadingTimeout();
	//part/packet of document read
	void onHttpReadyRead();
	//when during downloading error occurs
	void onHttpErrorReply(QNetworkReply::NetworkError);
	//downloading progress
	void onDownloadProgress(qint64,qint64);
	//used to get downloaded document size
	void onMetaDataChanged();
	//////////////////////////////////////////////////////////////////////////
	void 	onAuthenticationRequired ( QNetworkReply * reply, QAuthenticator * authenticator );
	void 	onNetworkAccessibleChanged ( QNetworkAccessManager::NetworkAccessibility accessible );
	void 	onProxyAuthenticationRequired ( const QNetworkProxy & proxy, QAuthenticator * authenticator );

	//////////////////////////////////////////////////////////////////////////
signals:
	void pageLoadingFinished(const QString &strPageContent);
	void pageLoadingFailed(const QString strFaultReason);
	void pageLoadingProgress(qint64 bytesRead, qint64 totalBytes);
protected:
	void loadingPreparation();
	void afterLoadingCleanup();
	//ShopPluginNetworkPrivate *	m_ptrPriv;
protected:
	QSharedPointer<ShopPluginNetworkPrivate> m_ptrPriv;
};
#endif //_CCS_SHOP_PLUGIN_NETWORK_INCLUDE_
