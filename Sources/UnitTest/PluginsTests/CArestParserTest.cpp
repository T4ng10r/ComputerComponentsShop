#include <PluginsTests/CArestParserTest.h>
#include <QtCore/QMetaType>
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
const QString strTestParsingSinglePage1("page_Arest_AMD PHENOM II X4 965_SinglePage.html" );
const QString strTestParsingSingleSearchPage("page_Arest_AMD PHENOM X4_SingleSearchPage.html");
const QString strTestParsingNotFoundPage("page_Arest_Wieloryb_NotFound.html");


void CArestPluginExtraTest::resetParser()
{
	m_bSingleProductPage=false;
	m_bSearchProductPage=false;
	m_bNoResultPage=false;
	m_bIsRefresNeeded=false;
	m_bLoadNextPage=false;
	prepareToNewSearch();
}
bool CArestPluginExtraTest::isNotFoundPage()
{
	return m_bNoResultPage;
}
bool CArestPluginExtraTest::isSingleProductPage()
{
	return m_bSingleProductPage;
}
bool CArestPluginExtraTest::isSearchProductPage()
{
	return m_bSearchProductPage;
}
bool CArestPluginExtraTest::isRefreshNeeded()
{
	return m_bIsRefresNeeded;
}
bool CArestPluginExtraTest::isLoadingNextSearchPage()
{
	return m_bLoadNextPage;
}
//////////////////////////////////////////////////////////////////////////
void CArestParserTest::initTestCase(){}
//////////////////////////////////////////////////////////////////////////
void CArestParserTest::performHTMLCleanup(QString & strHTML )
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
void CArestParserTest::testPluginParsing_ProductPage()
{
	QFile stFile(strTestPagesDir+QDir::separator()+strTestParsingSinglePage1);
	QVERIFY(stFile.open(QIODevice::ReadOnly | QIODevice::Text));
	QString strTestFile = stFile.readAll();
	stFile.close();
	//////////////////////////////////////////////////////////////////////////
	performHTMLCleanup(strTestFile);
	CArestPluginExtraTest	stArestTest;
	QXmlSimpleReader		m_stXMLReader;
	QXmlInputSource			m_stXMLInputSource;

	stArestTest.resetParser();

	m_stXMLInputSource.setData(strTestFile);
	m_stXMLReader.setContentHandler(&stArestTest);
	m_stXMLReader.setErrorHandler(&stArestTest);  
	bool bResult = m_stXMLReader.parse(&m_stXMLInputSource);
	QVERIFY(bResult);
	//////////////////////////////////////////////////////////////////////////
	QVERIFY(!stArestTest.isNotFoundPage());
	QVERIFY(stArestTest.isSingleProductPage());
	QVERIFY(!stArestTest.isSearchProductPage());
	QVERIFY(!stArestTest.isRefreshNeeded());
	QVERIFY(!stArestTest.isLoadingNextSearchPage());

	stArestTest.parseProductPage();
	QCOMPARE(double(stArestTest.m_stCompData.dPrice),double(405));
	QCOMPARE(stArestTest.m_stCompData.eSearchResult,SR_COMPFOUND);
}
void CArestParserTest::testPluginParsing_NotFoundPage()
{
	QFile stFile(strTestPagesDir+QDir::separator()+strTestParsingNotFoundPage);
	QVERIFY(stFile.open(QIODevice::ReadOnly | QIODevice::Text));
	QString strTestFile = stFile.readAll();
	stFile.close();
	//////////////////////////////////////////////////////////////////////////
	performHTMLCleanup(strTestFile);
	CArestPluginExtraTest stArestTest;
	QXmlSimpleReader		m_stXMLReader;
	QXmlInputSource			m_stXMLInputSource;

	stArestTest.resetParser();

	m_stXMLInputSource.setData(strTestFile);
	m_stXMLReader.setContentHandler(&stArestTest);
	m_stXMLReader.setErrorHandler(&stArestTest);  
	bool bResult = m_stXMLReader.parse(&m_stXMLInputSource);
	QVERIFY(bResult);
	//////////////////////////////////////////////////////////////////////////
	QVERIFY(stArestTest.isNotFoundPage());
	QVERIFY(!stArestTest.isSingleProductPage());
	QVERIFY(!stArestTest.isSearchProductPage());
	QVERIFY(!stArestTest.isRefreshNeeded());
	QVERIFY(!stArestTest.isLoadingNextSearchPage());
}
void CArestParserTest::testPluginParsing_SingleSearchPage()
{
	QFile stFile(strTestPagesDir+QDir::separator()+strTestParsingSingleSearchPage);
	QVERIFY(stFile.open(QIODevice::ReadOnly | QIODevice::Text));
	QString strTestFile = stFile.readAll();
	stFile.close();
	//////////////////////////////////////////////////////////////////////////
	performHTMLCleanup(strTestFile);
	CArestPluginExtraTest stArestTest;
	QXmlSimpleReader		m_stXMLReader;
	QXmlInputSource			m_stXMLInputSource;

	stArestTest.resetParser();

	m_stXMLInputSource.setData(strTestFile);
	m_stXMLReader.setContentHandler(&stArestTest);
	m_stXMLReader.setErrorHandler(&stArestTest);  
	bool bResult = m_stXMLReader.parse(&m_stXMLInputSource);
	QVERIFY(bResult);
	//////////////////////////////////////////////////////////////////////////
	QVERIFY(!stArestTest.isNotFoundPage());
	QVERIFY(!stArestTest.isSingleProductPage());
	QVERIFY(stArestTest.isSearchProductPage());
	QVERIFY(!stArestTest.isRefreshNeeded());
	QVERIFY(!stArestTest.isLoadingNextSearchPage());
}
