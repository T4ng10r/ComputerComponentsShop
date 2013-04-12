#include <PluginsTests/LoadNetworkObjectTest.h>
#include <Network/LoadNetworkObject.h>

const unsigned int ciSingleTimeout(1000); //in ms
const unsigned int ciTimeoutsCount(5); //in s
//Q_DECLARE_METATYPE(Qt::Orientation)
//Q_DECLARE_METATYPE(QModelIndex)
#define TIMEOUT_INFO  QString("Timeout")
#define ERROR_INFO  QString("Timeout")

//////////////////////////////////////////////////////////////////////////
const QString strSimplePageUrl("http://www.google.pl");
const QString strIncorrectUrl("http://www.asdfa.pl");
const QString strIntelSearchURLProduct("http://www.proline.pl/?prodq=Intel+Core+i5-2500K+3,3+GHz");

void LoadNetworkObjectTest::init()
{
	m_ptrShopPluginNetwork = QSharedPointer<LoadNetworkObject>(new LoadNetworkObject());
}
void LoadNetworkObjectTest::cleanup()
{
//	delete m_ptrModel;
}
void LoadNetworkObjectTest::testSimplePageLoad()
{
	int iCount = 0;
	QSignalSpy spy(m_ptrShopPluginNetwork.data(), SIGNAL(netObjectLoadingFinished(const QByteArray &)));
	m_ptrShopPluginNetwork->loadNetworkObject(strSimplePageUrl);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,QString("Timeout waiting for ShopPluginNetwork response reading '%1'").arg(strSimplePageUrl).toAscii());
			break;
		}
	}
	QList<QVariant> arguments = spy.takeFirst();
	const QByteArray& strPage = arguments.at(0).value<QByteArray>();
	QVERIFY(!strPage.isEmpty());
}
void LoadNetworkObjectTest::testProlineIntelPageLoad()
{
	int iCount = 0;
	QSignalSpy spy(m_ptrShopPluginNetwork.data(), SIGNAL(netObjectLoadingFinished(const QByteArray &)));
	m_ptrShopPluginNetwork->loadNetworkObject(strIntelSearchURLProduct);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,QString("Timeout waiting for ShopPluginNetwork response reading '%1'").arg(strIntelSearchURLProduct).toAscii());
			break;
		}
	}
	QList<QVariant> arguments = spy.takeFirst();
	const QByteArray&  strPage = arguments.at(0).value<QByteArray>();
	QVERIFY(!strPage.isEmpty());
}
void LoadNetworkObjectTest::testNotExistingPageLoad()
{
	int iCount = 0;
	QSignalSpy spy(m_ptrShopPluginNetwork.data(), SIGNAL(netObjectLoadingFailed(const QString )));
	m_ptrShopPluginNetwork->loadNetworkObject(strIncorrectUrl);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,QString("Timeout waiting for ShopPluginNetwork response reading '%1'").arg(strSimplePageUrl).toAscii());
			break;
		}
	}
	QList<QVariant> arguments = spy.takeFirst();
	QString strFaultName = arguments.at(0).value<QString>();
	QCOMPARE(strFaultName,QString(TIMEOUT_INFO));
}
