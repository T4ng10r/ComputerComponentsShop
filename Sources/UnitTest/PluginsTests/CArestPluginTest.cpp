#include <PluginsTests/CArestPluginTest.h>
#include <QtCore/QPluginLoader>
#include <ShopInterface.h>
#include <QMetaType>
#include <string>

ShopInterface * iArestShop = NULL;
const unsigned int ciMiliSecondInSecond(1000); //in s
const unsigned int ciSingleTimeout(1*ciMiliSecondInSecond); //in ms
const unsigned int ciTimeoutsCount(32); //in s
const unsigned int ciPriceTimeout(32); //in s
const QString strIntelProduct("AMD Phenom II 965 Black Edition");
const QString strAMDProductSearchPage("AMD Phenom II 965 Black Edition");
const QString strNoctuaProduct("Noctua NH-D14");
const QString strGigabyteProduct("Gigabyte GA-990FXA-UD3");
const QString strCorsairProduct("Corsair DDR3 2x 4GB 1866MHz CL9 Vengeance");
const QString strProductNotFound("Wieloryb");
const QString strTacensRadixProduct("Tacens Radix V 650W 85");
const QString strGigabyteURLProduct("http://www.proline.pl/?p=GIGA+GA-990FXA-UD3");
const QString strGigabyteIncorrectURLProduct("http://www.proline.pl/?p=GIGA+GA-990FXA");

void CArestPluginTest::initTestCase()
{
	QVERIFY(iArestShop==NULL);
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
			if (iShop && iShop->shopName()=="Arest") 
			{

				iArestShop = iShop;
				break;
			}
		}
	}
	QVERIFY(iArestShop!=NULL);
	qRegisterMetaType<CompPriceData>("CompPriceData");
	//setProxy();
}
void CArestPluginTest::testPluginName()
{
	QString strName = iArestShop->shopName();
	QVERIFY(strName=="Arest");
}
void CArestPluginTest::testPluginIcon()
{
	QIcon stIcon;
	QVERIFY(stIcon.isNull());
	stIcon = iArestShop->getShopIcon();
	QVERIFY(!stIcon.isNull());
}
void CArestPluginTest::testPluginURL()
{
	QString strName = iArestShop->getShopURL();
	QVERIFY(strName=="http://www.arest.pl/");
}
//////////////////////////////////////////////////////////////////////////
void CArestPluginTest::testPluginFindItem_WithoutParsingSearchPages()
{
	int iCount =0;
	QSignalSpy spy(iArestShop, SIGNAL(priceSearchedFinished(CompPriceData)));
	iArestShop->getProductData(strAMDProductSearchPage);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for ArestShop response");
			break;
		}
	}
	QList<QVariant> arguments = spy.takeFirst();
	CompPriceData stCompPriceData = arguments.at(0).value<CompPriceData>();
	QVERIFY(stCompPriceData.eSearchResult==SR_COMPFOUND);
	QVERIFY(stCompPriceData.dPrice!=0);
	QVERIFY(!stCompPriceData.strName.isEmpty());
}
void CArestPluginTest::testPluginFindItem_ParsingOneSearchSide()
{
	int iCount =0;
	QSignalSpy spy(iArestShop, SIGNAL(priceSearchedFinished(CompPriceData)));
	iArestShop->getProductData(strCorsairProduct);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for ArestShop response");
			break;
		}
	}
	QList<QVariant> arguments = spy.takeFirst();
	CompPriceData stCompPriceData = arguments.at(0).value<CompPriceData>();
	QVERIFY(stCompPriceData.eSearchResult==SR_COMPFOUND);
	QVERIFY(stCompPriceData.dPrice!=0);
	QVERIFY(!stCompPriceData.strName.isEmpty());
}
void CArestPluginTest::testPluginFindItem_NotFound()
{
	return;
	int iCount =0;
	QSignalSpy spy(iArestShop, SIGNAL(priceSearchedFinished(CompPriceData)));
	iArestShop->getProductData(strProductNotFound);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for ArestShop response");
			break;
		}
	}
	QList<QVariant> arguments = spy.takeFirst();
	CompPriceData stCompPriceData = arguments.at(0).value<CompPriceData>();
	QVERIFY(stCompPriceData.eSearchResult==SR_COMPNOTFOUND);
	QVERIFY(stCompPriceData.dPrice==0);
	QVERIFY(!stCompPriceData.strName.isEmpty());
}
void CArestPluginTest::testPluginFindItem_NotFoundButSearched()
{
	return;
	int iCount =0;
	QSignalSpy spy(iArestShop, SIGNAL(priceSearchedFinished(CompPriceData)));
	iArestShop->getProductData(strTacensRadixProduct);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for ArestShop response");
			break;
		}
	}
	QList<QVariant> arguments = spy.takeFirst();
	CompPriceData stCompPriceData = arguments.at(0).value<CompPriceData>();
	QVERIFY(stCompPriceData.eSearchResult==SR_COMPNOTFOUND);
	QVERIFY(stCompPriceData.dPrice==0);
	QVERIFY(!stCompPriceData.strName.isEmpty());
}
void CArestPluginTest::testPluginFindItem_FoundSearchedMultiplePages()
{
	return;
	int iCount =0;
	QSignalSpy spy(iArestShop, SIGNAL(priceSearchedFinished(CompPriceData)));
	iArestShop->getProductData(strIntelProduct);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for ArestShop response");
			break;
		}
	}
	QList<QVariant> arguments = spy.takeFirst();
	CompPriceData stCompPriceData = arguments.at(0).value<CompPriceData>();
	QVERIFY(stCompPriceData.eSearchResult==SR_COMPFOUND);
	QVERIFY(stCompPriceData.dPrice!=0);
	QVERIFY(!stCompPriceData.strName.isEmpty());
}
void CArestPluginTest::testPluginFindItem_Test1()
{
	return;
	int iCount =0;
	QSignalSpy spy(iArestShop, SIGNAL(priceSearchedFinished(CompPriceData)));
	iArestShop->getProductData("Wieloryb");
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for ArestShop response");
			break;
		}
	}
	QList<QVariant> arguments = spy.takeFirst();
	CompPriceData stCompPriceData = arguments.at(0).value<CompPriceData>();
	QVERIFY(stCompPriceData.eSearchResult==SR_COMPFOUND);
	QVERIFY(stCompPriceData.dPrice!=0);
	QVERIFY(!stCompPriceData.strName.isEmpty());
}
