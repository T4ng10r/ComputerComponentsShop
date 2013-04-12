#ifndef _LOAD_NETWORK_OBJECT_INCLUDE_
#define _LOAD_NETWORK_OBJECT_INCLUDE_
#include <QtCore/QObject>
#include <QtGui/QWidget>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QAuthenticator>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
//#include <boost/utility.hpp>

class LoadNetworkObjectPrivate;

class LoadNetworkObject : public QObject //, boost::noncopyable
{
	Q_OBJECT
	friend class LoadNetworkObjectPrivate;
public:
	LoadNetworkObject();
	~LoadNetworkObject();
	void operator=(const LoadNetworkObject & stCopy);
	void loadNetworkObject(const QString &strObjectUrl);
	void loadPostNetworkObject(QUrl stUrl, const std::map<QString, QString> &mPostData);
signals:
	void netObjectLoadingFinished(const QByteArray &strPageContent);
	void netObjectLoadingFailed(const QString strFaultReason);
	void netObjectLoadingProgress(qint64 bytesRead, qint64 totalBytes);
protected:
	//boost::scoped_ptr<LoadNetworkObjectPrivate> m_ptrPriv;
	boost::shared_ptr<LoadNetworkObjectPrivate> m_ptrPriv;
};
#endif //_LOAD_NETWORK_OBJECT_INCLUDE_
