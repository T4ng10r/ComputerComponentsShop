#include <PluginsTests/CProlineParserTest.h>
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
//////////////////////////////////////////////////////////////////////////
const QString strTestPagesDir("TestPages");
const QString strTestParsingSinglePage1("page_Proline_AMD Phenom II 965 BOX_SinglePage.html");
const QString strTestParsingSingleSearchPage("page_Proline_Gigabyte 1155 GA-Z77_SingleSearchPage.html");
const QString strTestParsingNotFoundPage("page_Proline_Wieloryb_NotFoundPage.html");


void CProlinePluginExtraTest::resetParser()
{
	m_bSingleProductPage=false;
	m_bSearchProductPage=false;
	m_bNoResultPage=false;
	m_bIsRefresNeeded=false;
	m_bLoadNextPage=false;
	prepareToNewSearch();
}
bool CProlinePluginExtraTest::isNotFoundPage()
{
	return m_bNoResultPage;
}
bool CProlinePluginExtraTest::isSingleProductPage()
{
	return m_bSingleProductPage;
}
bool CProlinePluginExtraTest::isSearchProductPage()
{
	return m_bSearchProductPage;
}
bool CProlinePluginExtraTest::isRefreshNeeded()
{
	return m_bIsRefresNeeded;
}
bool CProlinePluginExtraTest::isLoadingNextSearchPage()
{
	return m_bLoadNextPage;
}
//////////////////////////////////////////////////////////////////////////
void CProlineParserTest::initTestCase(){}
//////////////////////////////////////////////////////////////////////////
void CProlineParserTest::performHTMLCleanup(QString & strHTML )
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
void CProlineParserTest::testPluginParsing_ProductPage()
{
	QFile stFile(strTestPagesDir+QDir::separator()+strTestParsingSinglePage1);
	QVERIFY(stFile.open(QIODevice::ReadOnly | QIODevice::Text));
	QString strTestFile = stFile.readAll();
	stFile.close();
	//////////////////////////////////////////////////////////////////////////
	performHTMLCleanup(strTestFile);
	CProlinePluginExtraTest stProlineTest;
	QXmlSimpleReader		m_stXMLReader;
	QXmlInputSource			m_stXMLInputSource;

	stProlineTest.resetParser();

	m_stXMLInputSource.setData(strTestFile);
	m_stXMLReader.setContentHandler(&stProlineTest);
	m_stXMLReader.setErrorHandler(&stProlineTest);  
	bool bResult = m_stXMLReader.parse(&m_stXMLInputSource);
	QVERIFY(bResult);
	//////////////////////////////////////////////////////////////////////////
	QVERIFY(!stProlineTest.isNotFoundPage());
	QVERIFY(stProlineTest.isSingleProductPage());
	QVERIFY(!stProlineTest.isSearchProductPage());
	QVERIFY(!stProlineTest.isRefreshNeeded());
	QVERIFY(!stProlineTest.isLoadingNextSearchPage());

	stProlineTest.parseProductPage();
	QCOMPARE(double(stProlineTest.m_stCompData.dPrice),double(349));
	QCOMPARE(stProlineTest.m_stCompData.eSearchResult,SR_COMPFOUND);
}
void CProlineParserTest::testPluginParsing_NotFoundPage()
{
	QFile stFile(strTestPagesDir+QDir::separator()+strTestParsingNotFoundPage);
	QVERIFY(stFile.open(QIODevice::ReadOnly | QIODevice::Text));
	QString strTestFile = stFile.readAll();
	stFile.close();
	//////////////////////////////////////////////////////////////////////////
	performHTMLCleanup(strTestFile);
	CProlinePluginExtraTest stProlineTest;
	QXmlSimpleReader		m_stXMLReader;
	QXmlInputSource			m_stXMLInputSource;

	stProlineTest.resetParser();

	m_stXMLInputSource.setData(strTestFile);
	m_stXMLReader.setContentHandler(&stProlineTest);
	m_stXMLReader.setErrorHandler(&stProlineTest);  
	bool bResult = m_stXMLReader.parse(&m_stXMLInputSource);
	QVERIFY(bResult);
	//////////////////////////////////////////////////////////////////////////
	QVERIFY(stProlineTest.isNotFoundPage());
	QVERIFY(!stProlineTest.isSingleProductPage());
	QVERIFY(!stProlineTest.isSearchProductPage());
	QVERIFY(!stProlineTest.isRefreshNeeded());
	QVERIFY(!stProlineTest.isLoadingNextSearchPage());
}
void CProlineParserTest::testPluginParsing_SingleSearchPage()
{
	QFile stFile(strTestPagesDir+QDir::separator()+strTestParsingSingleSearchPage);
	QVERIFY(stFile.open(QIODevice::ReadOnly | QIODevice::Text));
	QString strTestFile = stFile.readAll();
	stFile.close();
	//////////////////////////////////////////////////////////////////////////
	performHTMLCleanup(strTestFile);
	CProlinePluginExtraTest stProlineTest;
	QXmlSimpleReader		m_stXMLReader;
	QXmlInputSource			m_stXMLInputSource;

	stProlineTest.resetParser();

	m_stXMLInputSource.setData(strTestFile);
	m_stXMLReader.setContentHandler(&stProlineTest);
	m_stXMLReader.setErrorHandler(&stProlineTest);  
	bool bResult = m_stXMLReader.parse(&m_stXMLInputSource);
	QVERIFY(bResult);
	//////////////////////////////////////////////////////////////////////////
	QVERIFY(!stProlineTest.isNotFoundPage());
	QVERIFY(!stProlineTest.isSingleProductPage());
	QVERIFY(stProlineTest.isSearchProductPage());
	QVERIFY(!stProlineTest.isRefreshNeeded());
	QVERIFY(!stProlineTest.isLoadingNextSearchPage());
}
