#include <PluginsTests/CKomputronikParserTest.h>
#include <QtCore/QPluginLoader>
#include <QtCore/QMetaType>
#include <ShopInterface.h>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QXmlInputSource>
#include <QXmlSimpleReader>
#include <string>
#include <tidy.h>
#include <buffio.h>

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
const QString strTestParsingSinglePage1("page_Komputronik_AMD Phenom II 965_SinglePage.html");
const QString strTestParsingNotFoundPage("page_Komputronik_Wieloryb_NotFoundPage.html");
const QString strTestParsingSingleSearchPage("page_Komputronik_Corsair DDR3_SingleSearchPage.html");


void CKomputronikPluginExtraTest::resetParser()
{
	m_bSingleProductPage=false;
	m_bSearchProductPage=false;
	m_bNoResultPage=false;
	m_bIsRefresNeeded=false;
	m_bLoadNextPage=false;
	prepareToNewSearch();
}
bool CKomputronikPluginExtraTest::isNotFoundPage()
{
	return m_bNoResultPage;
}
bool CKomputronikPluginExtraTest::isSingleProductPage()
{
	return m_bSingleProductPage;
}
bool CKomputronikPluginExtraTest::isSearchProductPage()
{
	return m_bSearchProductPage;
}
bool CKomputronikPluginExtraTest::isRefreshNeeded()
{
	return m_bIsRefresNeeded;
}
bool CKomputronikPluginExtraTest::isLoadingNextSearchPage()
{
	return m_bLoadNextPage;
}
//////////////////////////////////////////////////////////////////////////
void CKomputronikParserTest::initTestCase(){}
//////////////////////////////////////////////////////////////////////////
void CKomputronikParserTest::performHTMLCleanup(QString & strHTML )
{
	TidyBuffer output = {0};
	TidyBuffer errbuf = {0};
	int rc = -1;
	bool ok;

	TidyDoc tdoc = tidyCreate();                     // Initialize "document"

	ok = tidyOptSetBool( tdoc, TidyXhtmlOut, yes );  // Convert to XHTML
	if ( ok )
		rc = tidySetErrorBuffer( tdoc, &errbuf );      // Capture diagnostics
	if ( rc >= 0 )
		rc = tidyParseString( tdoc, strHTML.toAscii() );           // Parse the input
	if ( rc >= 0 )
		rc = tidyCleanAndRepair( tdoc );               // Tidy it up!
	if ( rc >= 0 )
		rc = tidyRunDiagnostics( tdoc );               // Kvetch
	if ( rc > 1 )                                    // If error, force output.
		rc = ( tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1 );
	if ( rc >= 0 )
		rc = tidySaveBuffer( tdoc, &output );          // Pretty Print
	strHTML = (char*)output.bp;
}
//////////////////////////////////////////////////////////////////////////
void CKomputronikParserTest::testPluginParsing_Refresh()
{
	//wczytac tresc strony HTML 
	QFile stFile(strTestPagesDir+QDir::separator()+strTestParsingRefresh);
	QVERIFY(stFile.open(QIODevice::ReadOnly | QIODevice::Text));
	QString strTestFile = stFile.readAll();
	stFile.close();
	//////////////////////////////////////////////////////////////////////////
	performHTMLCleanup(strTestFile);
	CKomputronikPluginExtraTest stKomputronikTest;
	QXmlSimpleReader		m_stXMLReader;
	QXmlInputSource			m_stXMLInputSource;

	stKomputronikTest.resetParser();

	m_stXMLInputSource.setData(strTestFile);
	m_stXMLReader.setContentHandler(&stKomputronikTest);
	m_stXMLReader.setErrorHandler(&stKomputronikTest);  
	bool bResult = m_stXMLReader.parse(&m_stXMLInputSource);
	QVERIFY(bResult);
	//////////////////////////////////////////////////////////////////////////
	QVERIFY(!stKomputronikTest.isNotFoundPage());
	QVERIFY(!stKomputronikTest.isSingleProductPage());
	QVERIFY(!stKomputronikTest.isSearchProductPage());
	QVERIFY(stKomputronikTest.isRefreshNeeded());
	QVERIFY(!stKomputronikTest.isLoadingNextSearchPage());
}
void CKomputronikParserTest::testPluginParsing_ProductPage()
{
	QFile stFile(strTestPagesDir+QDir::separator()+strTestParsingSinglePage1);
	QVERIFY(stFile.open(QIODevice::ReadOnly | QIODevice::Text));
	QString strTestFile = stFile.readAll();
	stFile.close();
	//////////////////////////////////////////////////////////////////////////
	performHTMLCleanup(strTestFile);
	CKomputronikPluginExtraTest stKomputronikTest;
	QXmlSimpleReader		m_stXMLReader;
	QXmlInputSource			m_stXMLInputSource;

	stKomputronikTest.resetParser();

	m_stXMLInputSource.setData(strTestFile);
	m_stXMLReader.setContentHandler(&stKomputronikTest);
	m_stXMLReader.setErrorHandler(&stKomputronikTest);  
	bool bResult = m_stXMLReader.parse(&m_stXMLInputSource);
	QVERIFY(bResult);
	//////////////////////////////////////////////////////////////////////////
	QVERIFY(!stKomputronikTest.isNotFoundPage());
	QVERIFY(stKomputronikTest.isSingleProductPage());
	QVERIFY(!stKomputronikTest.isSearchProductPage());
	QVERIFY(!stKomputronikTest.isRefreshNeeded());
	QVERIFY(!stKomputronikTest.isLoadingNextSearchPage());

	stKomputronikTest.parseProductPage();
	QCOMPARE(double(stKomputronikTest.m_stCompData.dPrice),double(389));
	QCOMPARE(stKomputronikTest.m_stCompData.eSearchResult,SR_COMPFOUND);
}
void CKomputronikParserTest::testPluginParsing_NotFoundPage()
{
	QFile stFile(strTestPagesDir+QDir::separator()+strTestParsingNotFoundPage);
	QVERIFY(stFile.open(QIODevice::ReadOnly | QIODevice::Text));
	QString strTestFile = stFile.readAll();
	stFile.close();
	//////////////////////////////////////////////////////////////////////////
	performHTMLCleanup(strTestFile);
	CKomputronikPluginExtraTest stKomputronikTest;
	QXmlSimpleReader		m_stXMLReader;
	QXmlInputSource			m_stXMLInputSource;

	stKomputronikTest.resetParser();

	m_stXMLInputSource.setData(strTestFile);
	m_stXMLReader.setContentHandler(&stKomputronikTest);
	m_stXMLReader.setErrorHandler(&stKomputronikTest);  
	bool bResult = m_stXMLReader.parse(&m_stXMLInputSource);
	QVERIFY(bResult);
	//////////////////////////////////////////////////////////////////////////
	QVERIFY(stKomputronikTest.isNotFoundPage());
	QVERIFY(!stKomputronikTest.isSingleProductPage());
	QVERIFY(stKomputronikTest.isSearchProductPage());
	QVERIFY(!stKomputronikTest.isRefreshNeeded());
	QVERIFY(stKomputronikTest.isLoadingNextSearchPage());
}
void CKomputronikParserTest::testPluginParsing_SingleSearchPage()
{
	QFile stFile(strTestPagesDir+QDir::separator()+strTestParsingSingleSearchPage);
	QVERIFY(stFile.open(QIODevice::ReadOnly | QIODevice::Text));
	QString strTestFile = stFile.readAll();
	stFile.close();
	//////////////////////////////////////////////////////////////////////////
	performHTMLCleanup(strTestFile);
	CKomputronikPluginExtraTest stKomputronikTest;
	QXmlSimpleReader		m_stXMLReader;
	QXmlInputSource			m_stXMLInputSource;

	stKomputronikTest.resetParser();

	m_stXMLInputSource.setData(strTestFile);
	m_stXMLReader.setContentHandler(&stKomputronikTest);
	m_stXMLReader.setErrorHandler(&stKomputronikTest);  
	bool bResult = m_stXMLReader.parse(&m_stXMLInputSource);
	QVERIFY(bResult);
	//////////////////////////////////////////////////////////////////////////
	QVERIFY(!stKomputronikTest.isNotFoundPage());
	QVERIFY(!stKomputronikTest.isSingleProductPage());
	QVERIFY(stKomputronikTest.isSearchProductPage());
	QVERIFY(!stKomputronikTest.isRefreshNeeded());
	QVERIFY(stKomputronikTest.isLoadingNextSearchPage());
}
