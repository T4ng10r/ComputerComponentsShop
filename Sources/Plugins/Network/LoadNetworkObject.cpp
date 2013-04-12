#include <Network/LoadNetworkObject.h>
#include <Network/LoadNetworkObjectPrivate.h>
#include <QtNetwork/QNetworkAccessManager>

#include <Tools/loggers.h>

const unsigned int ciMilisecondsInSecond(1000);
const unsigned int ciSinglePageLoadTime(10*ciMilisecondsInSecond);
#define TIMEOUT_INFO  QString("Timeout")
#define ERROR_INFO  QString("Timeout")

#define POST_PAIR_DELIMITER  char('&')
#define POST_VALUE_DELIMITER  char('=')

LoadNetworkObjectPrivate::LoadNetworkObjectPrivate(LoadNetworkObject * ptrPublic):m_ptrPublic(ptrPublic)
{
	m_stTimer.setSingleShot(true);
	QObject::connect(&m_stTimer, SIGNAL(timeout()), this, SLOT(onNetObjectLoadingTimeout()));

	QObject::connect(&m_NetAccesMgm, SIGNAL(authenticationRequired(QNetworkReply *, QAuthenticator *)), this, SLOT(onAuthenticationRequired(QNetworkReply *, QAuthenticator *)));
	QObject::connect(&m_NetAccesMgm, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)), this, SLOT(onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)));
	QObject::connect(&m_NetAccesMgm, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy & , QAuthenticator *)), this, SLOT(onProxyAuthenticationRequired (const QNetworkProxy & , QAuthenticator *)));
}
void LoadNetworkObjectPrivate::beforeLoadingCleanup()
{
	m_iPageSize=0;
	m_stNetObjectByteArray.clear();
}
void LoadNetworkObjectPrivate::afterLoadingCleanup()
{
	m_stTimer.stop(); 
}
//////////////////////////////////////////////////////////////////////////
void LoadNetworkObjectPrivate::onHttpReadyRead()
{
	m_stNetObjectByteArray.append(m_ptrReply->readAll());
}
void LoadNetworkObjectPrivate::onNetObjectDownloadFinished()
{
	printLog(eDebugLogLevel, eNetwork, "onNetObjectDownloadFinished: network request finished");
	afterLoadingCleanup();
	//m_ptrPriv->testLoadingSuccessfullnes(m_ptrPriv->m_ptrReply);
	disconnect(m_ptrReply, 0,0,0);
	//m_ptrPriv->m_ptrReply->close();
	m_ptrReply->deleteLater();

	if(m_ptrReply->error() == QNetworkReply::NoError) {
		// Get the http status code
		int v = m_ptrReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (v >= 200 && v < 300) // Success
		{
			// Here we got the final reply 
			m_stNetObjectByteArray.size();
			emit m_ptrPublic->netObjectLoadingFinished(m_stNetObjectByteArray);
			return;
		} 
		else if (v >= 300 && v < 400) // Redirection
		{
			// Get the redirection url
			QUrl newUrl = m_ptrReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
			// Because the redirection url can be relative, 
			// we have to use the previous one to resolve it 
			newUrl = m_ptrReply->url().resolved(newUrl);
			m_ptrPublic->loadNetworkObject(newUrl.toString());
			return; // to keep the manager for the next request
		} 
	} 
	emit m_ptrPublic->netObjectLoadingFailed("");
}
void LoadNetworkObjectPrivate::onDownloadProgress(qint64 iBytesRead,qint64 iTotalBytes)
{
	if (iTotalBytes<=0 && m_iPageSize>0)
		iTotalBytes = m_iPageSize;
	printLog(eDebugLogLevel, eNetwork, QString("onDownloadProgress: %1/%2").arg(iBytesRead).arg(iTotalBytes));
	emit m_ptrPublic->netObjectLoadingProgress(iBytesRead, iTotalBytes);
}
void LoadNetworkObjectPrivate::onMetaDataChanged()
{
	long long size = m_ptrReply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
	m_iPageSize=size;
}
void LoadNetworkObjectPrivate::onNetObjectLoadingTimeout()
{
	printLog(eDebugLogLevel, eNetwork, "onNetObjectLoadingTimeout");
	emit m_ptrPublic->netObjectLoadingFailed(TIMEOUT_INFO);
}
void LoadNetworkObjectPrivate::onHttpErrorReply(QNetworkReply::NetworkError errorNbr)
{
	printLog(eDebugLogLevel, eNetwork, "onHttpErrorReply");
	//printErrorLog(QString("HTTP Loading error :'%1' (URL='%2')").arg(errorNbr).arg(m_ptrPriv->m_strURL));
	emit m_ptrPublic->netObjectLoadingFailed(ERROR_INFO);
}
//////////////////////////////////////////////////////////////////////////
void 	LoadNetworkObjectPrivate::onAuthenticationRequired ( QNetworkReply * reply, QAuthenticator * authenticator )
{
	printLog(eDebugLogLevel, eNetwork, "onAuthenticationRequired");
	int a=1;
}
void 	LoadNetworkObjectPrivate::onNetworkAccessibleChanged ( QNetworkAccessManager::NetworkAccessibility accessible )
{
	printLog(eDebugLogLevel, eNetwork, "onNetworkAccessibleChanged");
	int a=1;
}
void 	LoadNetworkObjectPrivate::onProxyAuthenticationRequired ( const QNetworkProxy & proxy, QAuthenticator * authenticator )
{
	printLog(eDebugLogLevel, eNetwork, "onProxyAuthenticationRequired");
	int a=1;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LoadNetworkObject::LoadNetworkObject():m_ptrPriv(new LoadNetworkObjectPrivate(this)), QObject(NULL)
{
}
LoadNetworkObject::~LoadNetworkObject()
{
}
void LoadNetworkObject::loadNetworkObject(const QString &strObjectUrl)
{
	printLog(eDebugLogLevel, eNetwork, "loadNetworkObject: sending network request");
	m_ptrPriv->beforeLoadingCleanup();
	m_ptrPriv->m_strURL = strObjectUrl;
	m_ptrPriv->m_ptrReply = m_ptrPriv->m_NetAccesMgm.get(QNetworkRequest(QUrl(strObjectUrl)));

	QObject::connect(m_ptrPriv->m_ptrReply, SIGNAL(finished()), m_ptrPriv.get(), SLOT(onNetObjectDownloadFinished()));
	QObject::connect(m_ptrPriv->m_ptrReply, SIGNAL(readyRead()),m_ptrPriv.get(), SLOT(onHttpReadyRead()));
	QObject::connect(m_ptrPriv->m_ptrReply, SIGNAL(downloadProgress(qint64,qint64)), m_ptrPriv.get(), SLOT(onDownloadProgress(qint64,qint64)));
	QObject::connect(m_ptrPriv->m_ptrReply, SIGNAL(error(QNetworkReply::NetworkError)), m_ptrPriv.get(), SLOT(onHttpErrorReply(QNetworkReply::NetworkError)));
	QObject::connect(m_ptrPriv->m_ptrReply, SIGNAL(metaDataChanged()), m_ptrPriv.get(), SLOT(onMetaDataChanged()));
	m_ptrPriv->m_stTimer.start(ciSinglePageLoadTime);
	printLog(eDebugLogLevel, eNetwork, "loadNetworkObject: network request send");
}
void LoadNetworkObject::loadPostNetworkObject( QUrl stUrl, const std::map<QString, QString> &mPostData)
{
	printLog(eDebugLogLevel, eNetwork, 
		QString("loadPostNetworkObject: sending network POST request (URL='%1'").arg(stUrl.toString()));
	//printLog(eDebugLogLevel, eNetwork, QString("loadPostNetworkObject: sending network POST request (URL='%1'").arg(stUrl.toString()));
	m_ptrPriv->beforeLoadingCleanup();
	QNetworkRequest request(stUrl);
	request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

	QByteArray stPostData;
	QUrl params;
	params.setQueryDelimiters(POST_VALUE_DELIMITER, POST_PAIR_DELIMITER);

	for(std::map<QString, QString>::const_iterator iter=mPostData.begin();iter!=mPostData.end();++iter)
	{
		params.addQueryItem(iter->first, iter->second);
	}
	stPostData = params.encodedQuery();
	//stPostData = params.queryItems();
	//////////////////////////////////////////////////////////////////////////
	m_ptrPriv->m_ptrReply = m_ptrPriv->m_NetAccesMgm.post(request, stPostData);

	QObject::connect(m_ptrPriv->m_ptrReply, SIGNAL(finished()), this, SLOT(onNetObjectDownloadFinished()));
	QObject::connect(m_ptrPriv->m_ptrReply, SIGNAL(readyRead()),this, SLOT(onHttpReadyRead()));
	QObject::connect(m_ptrPriv->m_ptrReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(onDownloadProgress(qint64,qint64)));
	QObject::connect(m_ptrPriv->m_ptrReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onHttpErrorReply(QNetworkReply::NetworkError)));
	QObject::connect(m_ptrPriv->m_ptrReply, SIGNAL(metaDataChanged()), this, SLOT(onMetaDataChanged()));
	m_ptrPriv->m_stTimer.start(ciSinglePageLoadTime);
	printLog(eDebugLogLevel, eNetwork, "loadPostNetworkObject: network POST request send");
}

void LoadNetworkObject::operator=(const LoadNetworkObject & stCopy)
{

}
