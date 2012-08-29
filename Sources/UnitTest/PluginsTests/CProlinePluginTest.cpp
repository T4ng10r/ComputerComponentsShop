#include <PluginsTests/CProlinePluginTest.h>
#include <QtCore/QPluginLoader>
#include <QtCore/QUrl>
#include <ShopInterface.h>
#include <QMetaType>
#include <string>


ShopInterface * iProlineShop = NULL;
const unsigned int ciMiliSecondInSecond(1000); //in s
const unsigned int ciSingleTimeout(1000); //in s
const unsigned int ciTimeoutsCount(10); //in s
const unsigned int ciPriceTimeout(30); //in s
const QString strIntelProduct("Intel Core i5-2500K 3,3 GHz");
const QString strIntelSearchURLProduct("http://www.proline.pl/?prodq=Intel+Core+i5-2500K+3,3+GHz");
const QString strIntelURLProduct("http://www.proline.pl/?p=CORE+I5-2500K");
const QString strNoctuaProduct("Noctua NH-D14");
const QString strGigabyteProduct("Gigabyte GA-990FXA-UD3 s.AM3+");
const QString strGigabyteURLProduct("http://www.proline.pl/?p=GIGA+GA-990FXA-UD3");
const QString strGigabyteIncorrectURLProduct("http://www.proline.pl/?p=GIGA+GA-990FXA");
const QString strRasburoGAPProduct("Rasurbo GAP 656 650W");

void CProlinePluginTest::initTestCase()
{
	QVERIFY(iProlineShop==NULL);
	QDir pluginsDir;
	pluginsDir = QDir(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
	if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
		pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
	if (pluginsDir.dirName() == "MacOS") {
		pluginsDir.cdUp();
		pluginsDir.cdUp();
		pluginsDir.cdUp();
	}
#endif
	pluginsDir.cd("plugins");

	foreach (QString fileName, pluginsDir.entryList(QDir::Files)) 
	{
		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader.instance();
		if (plugin) 
		{
			ShopInterface * iShop = qobject_cast<ShopInterface *>(plugin);
			if (iShop && iShop->shopName()=="Proline") 
			{

				iProlineShop = iShop;
				break;
			}
		}
	}
	QVERIFY(iProlineShop!=NULL);
	qRegisterMetaType<CompPriceData>("CompPriceData");
}
void CProlinePluginTest::testPluginName()
{
	QString strName = iProlineShop->shopName();
	QVERIFY(strName=="Proline");
}
void CProlinePluginTest::testPluginIcon()
{
	QIcon stIcon;
	QVERIFY(stIcon.isNull());
	stIcon = iProlineShop->getShopIcon();
	QVERIFY(!stIcon.isNull());
}
void CProlinePluginTest::testPluginURL()
{
	QString strName = iProlineShop->getShopURL();
	QVERIFY(strName=="http://www.proline.pl");
}
void CProlinePluginTest::testPluginFindItem_SearchPageWithOnlyOneResult()
{
	int iCount = 0;
	QSignalSpy spy(iProlineShop, SIGNAL(priceSearchedFinished(CompPriceData)));
	iProlineShop->getProductData(strIntelProduct);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for PluginShop response");
			break;
		}
	}
	QList<QVariant> arguments = spy.takeFirst();
	CompPriceData stCompPriceData;
	stCompPriceData = arguments.at(0).value<CompPriceData>();
	QCOMPARE(stCompPriceData.eSearchResult,SR_COMPFOUND);
	QVERIFY(stCompPriceData.dPrice!=0);
	QVERIFY(!stCompPriceData.strName.isEmpty());
}
void CProlinePluginTest::testPluginFindItem_WithOldCorrectURL()
{
	int iCount = 0;
	QSignalSpy spy(iProlineShop, SIGNAL(priceSearchedFinished(CompPriceData)));
	iProlineShop->getProductData(strIntelProduct,strIntelURLProduct);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for PluginShop response");
			break;
		}
	}
	QList<QVariant> arguments = spy.takeFirst();
	CompPriceData stCompPriceData;
	stCompPriceData = arguments.at(0).value<CompPriceData>();
	QVERIFY(stCompPriceData.eSearchResult==SR_COMPFOUND);
	QVERIFY(stCompPriceData.dPrice!=0);
	QVERIFY(!stCompPriceData.strName.isEmpty());
}
void CProlinePluginTest::testPluginFindItem_ParsingOneSearchSide()
{
	int iCount =0;
	QSignalSpy spy(iProlineShop, SIGNAL(priceSearchedFinished(CompPriceData)));
	iProlineShop->getProductData(strNoctuaProduct);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for PluginShop response");
			break;
		}
	}
	QList<QVariant> arguments = spy.takeFirst();
	CompPriceData stCompPriceData;
	stCompPriceData = arguments.at(0).value<CompPriceData>();
	QVERIFY(stCompPriceData.eSearchResult==SR_COMPFOUND);
	QVERIFY(stCompPriceData.dPrice!=0);
	QVERIFY(stCompPriceData.strName==strNoctuaProduct);
}
void CProlinePluginTest::testPluginFindItem_NotFound()
{
	int iCount =0;
	QSignalSpy spy(iProlineShop, SIGNAL(priceSearchedFinished(CompPriceData)));
	iProlineShop->getProductData(strGigabyteProduct,strGigabyteURLProduct);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for PluginShop response");
			break;
		}
	}
	QList<QVariant> arguments = spy.takeFirst();
	CompPriceData stCompPriceData;
	stCompPriceData = arguments.at(0).value<CompPriceData>();
	QVERIFY(stCompPriceData.eSearchResult==SR_COMPFOUND);
	QVERIFY(stCompPriceData.dPrice!=0);
	QVERIFY(stCompPriceData.strName==strGigabyteProduct);
}
void CProlinePluginTest::testPluginFindItem_NotFoundButSearched()
{
	int iCount =0;
	QSignalSpy spy(iProlineShop, SIGNAL(priceSearchedFinished(CompPriceData)));
	iProlineShop->getProductData(strGigabyteProduct,strGigabyteIncorrectURLProduct);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for PluginShop response");
			break;
		}
	}
	QList<QVariant> arguments = spy.takeFirst();
	CompPriceData stCompPriceData;
	stCompPriceData = arguments.at(0).value<CompPriceData>();
	//QVERIFY(stCompPriceData.dPrice!=0);
	//QVERIFY(stCompPriceData.strName==strCompName);
	//QVERIFY(stCompPriceData.eSearchResult==SR_COMPFOUND);
}
void CProlinePluginTest::testPluginFindItem_SearchRasburoGAPProduct()
{
	int iCount = 0;
	QSignalSpy spy(iProlineShop, SIGNAL(priceSearchedFinished(CompPriceData)));
	iProlineShop->getProductData(strRasburoGAPProduct);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for PluginShop response");
			break;
		}
	}
	QList<QVariant> arguments = spy.takeFirst();
	CompPriceData stCompPriceData;
	stCompPriceData = arguments.at(0).value<CompPriceData>();
	QVERIFY(stCompPriceData.eSearchResult==SR_COMPFOUND);
	QVERIFY(stCompPriceData.dPrice!=0);
	QVERIFY(!stCompPriceData.strName.isEmpty());
}
