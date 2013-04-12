#ifndef _LOAD_NETWORK_OBJECT_PRIVATE_INCLUDE_
#define _LOAD_NETWORK_OBJECT_PRIVATE_INCLUDE_
#include <QtGui/QWidget>
#include <QtCore/QTimer>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QAuthenticator>
#include <boost/utility.hpp>

class LoadNetworkObject;

class LoadNetworkObjectPrivate : public QObject, boost::noncopyable
{
	Q_OBJECT
public:
	~LoadNetworkObjectPrivate(){};
	LoadNetworkObjectPrivate(LoadNetworkObject * ptrPublic);
	void afterLoadingCleanup();
	void beforeLoadingCleanup();
public:
	LoadNetworkObject *			m_ptrPublic;
	QNetworkAccessManager		m_NetAccesMgm;
	QNetworkReply *				m_ptrReply;
	//content of loaded network object
	QByteArray					m_stNetObjectByteArray;
	//downloaded page size
	long long 					m_iPageSize;
	//timer used to limit download time
	QTimer						m_stTimer;
	QString						m_strURL;

public slots:
	void onNetObjectDownloadFinished();
	//part/packet of document read
	void onHttpReadyRead();
	//when during downloading error occurs
	void onHttpErrorReply(QNetworkReply::NetworkError);
	//downloading progress
	void onDownloadProgress(qint64,qint64);
	//used to get downloaded document size
	void onMetaDataChanged();
	//////////////////////////////////////////////////////////////////////////
	void onAuthenticationRequired ( QNetworkReply * reply, QAuthenticator * authenticator );
	void onNetworkAccessibleChanged ( QNetworkAccessManager::NetworkAccessibility accessible );
	void onProxyAuthenticationRequired ( const QNetworkProxy & proxy, QAuthenticator * authenticator );
	//////////////////////////////////////////////////////////////////////////
	//if waiting time till page download expires
	void onNetObjectLoadingTimeout();
};
#endif //_LOAD_NETWORK_OBJECT_PRIVATE_INCLUDE_
