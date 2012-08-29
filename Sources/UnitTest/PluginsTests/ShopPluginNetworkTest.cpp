#include <PluginsTests/ShopPluginNetworkTest.h>
#include <ShopPluginNetwork.h>

const unsigned int ciSingleTimeout(1000); //in ms
const unsigned int ciTimeoutsCount(5); //in s
//Q_DECLARE_METATYPE(Qt::Orientation)
//Q_DECLARE_METATYPE(QModelIndex)

//////////////////////////////////////////////////////////////////////////
const QString strSimplePageUrl("http://www.google.pl");
const QString strIncorrectUrl("http://www.asdfa.pl");
const QString strIntelSearchURLProduct("http://www.proline.pl/?prodq=Intel+Core+i5-2500K+3,3+GHz");

void ShopPluginNetworkTest::init()
{
	m_ptrShopPluginNetwork = QSharedPointer<ShopPluginNetwork>(new ShopPluginNetwork());
}
void ShopPluginNetworkTest::cleanup()
{
//	delete m_ptrModel;
}
void ShopPluginNetworkTest::testSimplePageLoad()
{
	int iCount = 0;
	QSignalSpy spy(m_ptrShopPluginNetwork.data(), SIGNAL(pageLoadingFinished(const QString &)));
	m_ptrShopPluginNetwork->loadPage(strSimplePageUrl);
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
	QString strPage = arguments.at(0).value<QString>();
	QVERIFY(!strPage.isEmpty());
}
void ShopPluginNetworkTest::testProlineIntelPageLoad()
{
	int iCount = 0;
	QSignalSpy spy(m_ptrShopPluginNetwork.data(), SIGNAL(pageLoadingFinished(const QString &)));
	m_ptrShopPluginNetwork->loadPage(strIntelSearchURLProduct);
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
	QString strPage = arguments.at(0).value<QString>();
	QVERIFY(!strPage.isEmpty());
}
void ShopPluginNetworkTest::testNotExistingPageLoad()
{
	int iCount = 0;
	QSignalSpy spy(m_ptrShopPluginNetwork.data(), SIGNAL(pageLoadingFailed(const QString )));
	m_ptrShopPluginNetwork->loadPage(strIncorrectUrl);
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
	QCOMPARE(strFaultName,QString(ERROR_INFO));
}
