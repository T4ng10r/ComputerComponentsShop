#include <PluginsTests/CKomputronikPluginTest.h>
#include <QtCore/QPluginLoader>
#include <QtCore/QMetaType>
#include <ShopInterface.h>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtXML/QXmlInputSource>
#include <QtXML/QXmlSimpleReader>
#include <string>
#include <tidy.h>
#include <buffio.h>

static ShopInterface * iKomputronikShop = NULL;
const unsigned int ciMiliSecondInSecond(1000); //in s
const unsigned int ciSingleTimeout(1*ciMiliSecondInSecond); //in ms
const unsigned int ciTimeoutsCount(32); //in s
//////////////////////////////////////////////////////////////////////////
/// testing constans
const QString strIntelProduct("AMD Phenom II 965 Black Edition");
const QString strAMDProductSearchPage("AMD Phenom II 965 Black Edition");
const QString strNoctuaProduct("Noctua NH-D14");
const QString strGigabyteProduct("Gigabyte GA-990FXA-UD3");
const QString strCorsairProduct("Corsair DDR3 2x 4GB 1866MHz CL9 Vengeance");
const QString strProductNotFound("Wieloryb");
const QString strTacensRadixProduct("Tacens Radix V 650W 85");
const QString strGigabyteURLProduct("http://www.proline.pl/?p=GIGA+GA-990FXA-UD3");
const QString strGigabyteIncorrectURLProduct("http://www.proline.pl/?p=GIGA+GA-990FXA");
//////////////////////////////////////////////////////////////////////////
const QString strTestPagesDir("TestPages");
const QString strTestParsingRefresh("page_Komputronik_QNAP TS-212_Refresh.html");
//////////////////////////////////////////////////////////////////////////
void CKomputronikPluginTest::initTestCase()
{
	QVERIFY(iKomputronikShop==NULL);
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
			if (iShop && iShop->shopName()=="Komputronik") 
			{

				iKomputronikShop = iShop;
				break;
			}
		}
	}
	QVERIFY(iKomputronikShop!=NULL);
	qRegisterMetaType<CompPriceData>("CompPriceData");
	//setProxy();
}
void CKomputronikPluginTest::testPluginName()
{
	QString strName = iKomputronikShop->shopName();
	QVERIFY(strName=="Komputronik");
}
void CKomputronikPluginTest::testPluginIcon()
{
	QIcon stIcon;
	QVERIFY(stIcon.isNull());
	stIcon = iKomputronikShop->getShopIcon();
	QVERIFY(!stIcon.isNull());
}
void CKomputronikPluginTest::testPluginURL()
{
	QString strName = iKomputronikShop->getShopURL();
	QVERIFY(strName=="http://www.komputronik.pl/");
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CKomputronikPluginTest::testPluginFindItem_WithoutParsingSearchPages()
{
	int iCount =0;
	QSignalSpy spy(iKomputronikShop, SIGNAL(priceSearchedFinished(CompPriceData)));
	iKomputronikShop->getProductData(strGigabyteProduct);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for KomputronikShop response");
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
void CKomputronikPluginTest::testPluginFindItem_ParsingOneSearchSide()
{
	int iCount =0;
	QSignalSpy spy(iKomputronikShop, SIGNAL(priceSearchedFinished(CompPriceData)));
	iKomputronikShop->getProductData(strCorsairProduct);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for KomputronikShop response");
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
void CKomputronikPluginTest::testPluginFindItem_NotFound()
{
	int iCount =0;
	QSignalSpy spy(iKomputronikShop, SIGNAL(priceSearchedFinished(CompPriceData)));
	iKomputronikShop->getProductData(strProductNotFound);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for KomputronikShop response");
			break;
		}
	}
	QList<QVariant> arguments = spy.takeFirst();
	CompPriceData stCompPriceData;
	stCompPriceData = arguments.at(0).value<CompPriceData>();
	QVERIFY(stCompPriceData.eSearchResult==SR_COMPNOTFOUND);
	QVERIFY(stCompPriceData.dPrice==0);
	QVERIFY(!stCompPriceData.strName.isEmpty());
}
void CKomputronikPluginTest::testPluginFindItem_NotFoundButSearched()
{
	int iCount =0;
	QSignalSpy spy(iKomputronikShop, SIGNAL(priceSearchedFinished(CompPriceData)));
	iKomputronikShop->getProductData(strTacensRadixProduct);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for KomputronikShop response");
			break;
		}
	}
	QList<QVariant> arguments = spy.takeFirst();
	CompPriceData stCompPriceData;
	stCompPriceData = arguments.at(0).value<CompPriceData>();
	QVERIFY(stCompPriceData.eSearchResult==SR_COMPNOTFOUND);
	QVERIFY(stCompPriceData.dPrice==0);
	QVERIFY(!stCompPriceData.strName.isEmpty());
}
void CKomputronikPluginTest::testPluginFindItem_FoundSearchedMultiplePages()
{
	int iCount =0;
	QSignalSpy spy(iKomputronikShop, SIGNAL(priceSearchedFinished(CompPriceData)));
	iKomputronikShop->getProductData(strIntelProduct);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for KomputronikShop response");
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
void CKomputronikPluginTest::testPluginFindItem_LoadingQNAP_TS212()
{
	int iCount =0;
	QSignalSpy spy(iKomputronikShop, SIGNAL(priceSearchedFinished(CompPriceData)));
	iKomputronikShop->getProductData("QNAP TS-212");
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for KomputronikShop response");
			break;
		}
	}
	QList<QVariant> arguments = spy.takeFirst();
	CompPriceData stCompPriceData;
	stCompPriceData = arguments.at(0).value<CompPriceData>();
	QVERIFY(stCompPriceData.eSearchResult==SR_COMPFOUND);
	QVERIFY(stCompPriceData.dPrice==739);
	QVERIFY(!stCompPriceData.strName.isEmpty());
}
