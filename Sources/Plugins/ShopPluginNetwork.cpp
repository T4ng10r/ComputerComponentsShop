#include <ShopPluginNetwork.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QtCore/QTimer>


const unsigned int ciMilisecondsInSecond(1000);
const unsigned int ciSinglePageLoadTime(7*ciMilisecondsInSecond);

namespace ShopPluginLoggers
{
	void printErrorLog(const QString & strLog);
	void printDebugLog(const QString & strLog);
}

class ShopPluginNetworkPrivate
{
public:
	ShopPluginNetworkPrivate(ShopPluginNetwork * ptrPublic);
public:
	ShopPluginNetwork *			m_ptrPublic;
	QNetworkAccessManager		m_NetAccesMgm;
	QNetworkReply *				m_ptrReply;
	//content of loaded HTML page
	QString						m_strPageHTML;
	//downloaded page size
	long long 					m_iPageSize;
	//timer used to limit download time
	QTimer						m_stTimer;
	QString						m_strURL;
};
//////////////////////////////////////////////////////////////////////////

ShopPluginNetworkPrivate::ShopPluginNetworkPrivate(ShopPluginNetwork * ptrPublic):m_ptrPublic(ptrPublic),m_ptrReply(NULL)
{
	m_stTimer.setSingleShot(true);
	QObject::connect(&m_stTimer, SIGNAL(timeout()), m_ptrPublic, SLOT(onPageLoadingTimeout()));

	QObject::connect(&m_NetAccesMgm, SIGNAL(authenticationRequired(QNetworkReply *, QAuthenticator *)), m_ptrPublic, SLOT(onAuthenticationRequired(QNetworkReply *, QAuthenticator *)));
	QObject::connect(&m_NetAccesMgm, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)), m_ptrPublic, SLOT(onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)));
	QObject::connect(&m_NetAccesMgm, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy & , QAuthenticator *)), m_ptrPublic, SLOT(onProxyAuthenticationRequired (const QNetworkProxy & , QAuthenticator *)));
}
//////////////////////////////////////////////////////////////////////////
ShopPluginNetwork::ShopPluginNetwork()
{
	m_ptrPriv = QSharedPointer<ShopPluginNetworkPrivate>(new ShopPluginNetworkPrivate(this));
}
ShopPluginNetwork::~ShopPluginNetwork()
{
	//delete m_ptrPriv.data();
}
void ShopPluginNetwork::loadPage(const QString &strUrl)
{
	ShopPluginLoggers::printDebugLog(QString("Start HTML page loading (URL='%1')").arg(strUrl));
	m_ptrPriv->m_strURL = strUrl;
	loadingPreparation();
#ifdef LOAD_WITH_WEBKIT
	connect(&m_stWebPage, SIGNAL(loadFinished (bool)), this, SLOT(onPageDownloadFinished()));
	m_stWebPage.mainFrame()->load(stUrl);
#else
	m_ptrPriv->m_strPageHTML.clear();
	m_ptrPriv->m_ptrReply = m_ptrPriv->m_NetAccesMgm.get(QNetworkRequest(QUrl(strUrl)));

	QObject::connect(m_ptrPriv->m_ptrReply, SIGNAL(finished()), this, SLOT(onPageDownloadFinished()));
	QObject::connect(m_ptrPriv->m_ptrReply, SIGNAL(readyRead()),this, SLOT(onHttpReadyRead()));
	QObject::connect(m_ptrPriv->m_ptrReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(onDownloadProgress(qint64,qint64)));
	QObject::connect(m_ptrPriv->m_ptrReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onHttpErrorReply(QNetworkReply::NetworkError)));
	QObject::connect(m_ptrPriv->m_ptrReply, SIGNAL(metaDataChanged()), this, SLOT(onMetaDataChanged()));
#endif
	m_ptrPriv->m_stTimer.start(ciSinglePageLoadTime);
}
void ShopPluginNetwork::loadingPreparation()
{
	m_ptrPriv->m_iPageSize=0;
}
void ShopPluginNetwork::afterLoadingCleanup()
{
#ifdef LOAD_WITH_WEBKIT
	disconnect(&(m_ptrPriv->m_stWebPage), 0,0,0);
#else
#endif
	m_ptrPriv->m_stTimer.stop();
}
//////////////////////////////////////////////////////////////////////////
void ShopPluginNetwork::onHttpReadyRead()
{
	m_ptrPriv->m_strPageHTML.append(m_ptrPriv->m_ptrReply->readAll());
}
void ShopPluginNetwork::onPageDownloadFinished()
{
	afterLoadingCleanup();
#ifndef LOAD_WITH_WEBKIT
	//m_ptrPriv->testLoadingSuccessfullnes(m_ptrPriv->m_ptrReply);
	disconnect(m_ptrPriv->m_ptrReply, 0,0,0);
	m_ptrPriv->m_ptrReply->close();
	m_ptrPriv->m_ptrReply->deleteLater();
#else
	disconnect(&(m_ptrPriv->m_stWebPage), 0,0,0);
#endif
	emit pageLoadingFinished(m_ptrPriv->m_strPageHTML);
}
void ShopPluginNetwork::onDownloadProgress(qint64 iBytesRead,qint64 iTotalBytes)
{
	if (iTotalBytes<=0 && m_ptrPriv->m_iPageSize>0)
		iTotalBytes = m_ptrPriv->m_iPageSize;
	emit pageLoadingProgress(iBytesRead, iTotalBytes);
}
void ShopPluginNetwork::onMetaDataChanged()
{
	long long size = m_ptrPriv->m_ptrReply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
	m_ptrPriv->m_iPageSize=size;
}
void ShopPluginNetwork::onPageLoadingTimeout()
{
	emit pageLoadingFailed(TIMEOUT_INFO);
}
void ShopPluginNetwork::onHttpErrorReply(QNetworkReply::NetworkError errorNbr)
{
	ShopPluginLoggers::printErrorLog(QString("HTTP Loading error :'%1' (URL='%2')").arg(errorNbr).arg(m_ptrPriv->m_strURL));
	emit pageLoadingFailed(ERROR_INFO);
}

//////////////////////////////////////////////////////////////////////////
void 	ShopPluginNetwork::onAuthenticationRequired ( QNetworkReply * reply, QAuthenticator * authenticator )
{
	int a=1;
}
void 	ShopPluginNetwork::onNetworkAccessibleChanged ( QNetworkAccessManager::NetworkAccessibility accessible )
{
	int a=1;
}
void 	ShopPluginNetwork::onProxyAuthenticationRequired ( const QNetworkProxy & proxy, QAuthenticator * authenticator )
{
	int a=1;
}
