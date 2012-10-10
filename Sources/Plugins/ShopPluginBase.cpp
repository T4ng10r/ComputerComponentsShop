#include "ShopPluginBase.h"
#include "ShopPluginNetwork.h"
#ifdef USE_WEBKIT
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebPage>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebElement>
#endif
#ifdef USE_XML_PARSER
#include <QtXml/QXmlDefaultHandler>
#include <tidy.h>
#include <buffio.h>
#endif

#include <QtCore/QTimer>
#include <list>
#include <map>
#include <string>

#include <QtCore/QDir>

#define LOGER_NAME QString("debug")
#define LOG_DIR QString("logs%1").arg(QDir::separator())


const unsigned int ciSinglePageLoadTime(20);
const unsigned int ciWholePluginLoadingTime(30);

namespace ShopPluginLoggers
{
	void printErrorLog(const QString & strLog);
	void printDebugLog(const QString & strLog);
}
#define LOG_DEBUG(logContent) ShopPluginLoggers::printDebugLog(makeLogString((logContent)))


class ShopPluginBasePrivate : public QObject
{
public:
	ShopPluginBasePrivate(ShopPluginBase * prtPublic);
	//////////////////////////////////////////////////////////////////////////
	void processSearchPageLoad();
	void processProductPageLoad();
	const QString  makeLogString(const QString &strLogText);
	void loadNextPage();
	QUrl findBestLink(SearchResult stResult);
	void performHTMLCleanup();
public:
	ShopPluginBase *		m_ptrPublic;
	//class loading HTML pages
	ShopPluginNetwork		m_stNetworkHTMLLoader;
	//////////////////////////////////////////////////////////////////////////
	EPageLoadingPhase		m_ePageLoadingPhase; 
	QString					m_strCurrentURL;
	unsigned int			m_iProcessingPhase;
	//during searching on sites with multiple pages of result hold currently loaded page index
	unsigned int			m_uCurrentPage;
	//result of search pages parse
	SearchResult			m_stResult;
	QString					m_strPageHTML;
	QTimer					m_stTimer;
	//////////////////////////////////////////////////////////////////////////
	QStringList				m_ComponentNamesTokens;
	int						m_iSearchedCompTokensCount;
	//////////////////////////////////////////////////////////////////////////
#ifdef USE_WEBKIT
	QWebPage				m_stWebPage;
#endif
	//////////////////////////////////////////////////////////////////////////
	//QNetworkAccessManager	m_NetAccesMgm;
	//QNetworkReply *			m_pReply;
	//int						httpGetId;
	//bool					httpRequestAborted;
	//////////////////////////////////////////////////////////////////////////
#ifdef USE_XML_PARSER
	QXmlSimpleReader		m_stXMLReader;
	QXmlInputSource			m_stXMLInputSource;
#endif
};
ShopPluginBasePrivate::ShopPluginBasePrivate(ShopPluginBase * ptrPublic):
m_ptrPublic(ptrPublic), m_ePageLoadingPhase(PAGETYPE_NONE),
m_uCurrentPage(1),m_strCurrentURL("")
{
#ifdef USE_XML_PARSER
	m_stXMLInputSource.setData(QString(""));
	m_stXMLReader.setContentHandler(m_ptrPublic);
	m_stXMLReader.setErrorHandler(m_ptrPublic);  
#endif
	QObject::connect(&m_stNetworkHTMLLoader, SIGNAL(pageLoadingFinished(const QString &)),	m_ptrPublic, SLOT(onPageDownloadFinished(const QString &)));
	QObject::connect(&m_stNetworkHTMLLoader, SIGNAL(pageLoadingFailed(const QString)),		m_ptrPublic, SLOT(onPageLoadingError(const QString)));
	QObject::connect(&m_stNetworkHTMLLoader, SIGNAL(pageLoadingProgress(qint64, qint64)),	m_ptrPublic, SLOT(onPageLoadingProgress(qint64, qint64)));
}
void ShopPluginBasePrivate::loadNextPage()
{
	m_ePageLoadingPhase = PAGETYPE_SEARCH_NEXT_PAGE;
	++m_uCurrentPage;
	QUrl stUrl = m_ptrPublic->createSearchNextPageURL(m_uCurrentPage);
	m_stNetworkHTMLLoader.loadPage(stUrl.toString());
}
void ShopPluginBasePrivate::processSearchPageLoad()
{
	LOG_DEBUG(QString("Search page loaded for '%1'").arg(m_ptrPublic->m_strComponentName));
	++m_iProcessingPhase;
	bool bNextPageSearch;
#if defined(USE_WEBKIT) && !defined(LOAD_WITH_WEBKIT)
	m_stWebPage.mainFrame()->setHtml(m_strPageHTML);
#endif
	m_ptrPublic->parseSearchProductPage(m_stResult,bNextPageSearch);
	if (bNextPageSearch)
	{
		loadNextPage();
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	ShopPluginLoggers::printDebugLog(makeLogString(QString("Found %1 on shop search site").arg(m_stResult.size())));
	int i=1;
	for(SearchResult::const_iterator iterRes=m_stResult.begin();
		iterRes!=m_stResult.end();++iterRes,++i)
		ShopPluginLoggers::printDebugLog(QString("Component (%1): '%2'").arg(i).arg(iterRes->first));
	QUrl stUrl;
	if (m_stResult.size()>1)
	{
		stUrl = findBestLink(m_stResult);
		LOG_DEBUG(QString("Best match (URL=%2)").arg(stUrl.toString()));
		if (stUrl.isEmpty())
		{
			m_ptrPublic->pageNotFound();
			emit m_ptrPublic->priceSearchedFinished(m_ptrPublic->m_stCompData);
			return;
		}
	}
	else if (m_stResult.size()==1)
	{
		stUrl = m_stResult.begin()->second;
		LOG_DEBUG(QString("Only one result (URL=%2)").arg(stUrl.toString()));
	}
	else if (m_stResult.size()==0)
	{
		m_ptrPublic->m_stCompData.eSearchResult = SR_COMPNOTFOUND;
		ShopPluginLoggers::printDebugLog(makeLogString(QString("No matching components found")));
		emit m_ptrPublic->priceSearchedFinished(m_ptrPublic->m_stCompData);
		//m_ptrPublic->afterSearchCleanup();
		return;
	}
	++m_iProcessingPhase;
	m_ePageLoadingPhase = PAGETYPE_PRODUCT;
	m_ptrPublic->m_stCompData.strCompURL =stUrl.toString();
	m_strCurrentURL = stUrl.toString();
	m_stNetworkHTMLLoader.loadPage(stUrl.toString());
}
void ShopPluginBasePrivate::processProductPageLoad()
{
	LOG_DEBUG(QString("Component page loaded for '%2'").arg(m_ptrPublic->m_strComponentName));
	//m_ptrPublic->printPageContent(m_strPageHTML);
	++m_iProcessingPhase;
#if defined(LOAD_WITH_WEBKIT) && !defined(USE_XML_PARSER)
	m_ptrPublic->m_stWebPage.mainFrame()->setHtml(m_ptrPublic->m_strPageHTML);
#endif
	m_ptrPublic->parseProductPage();
	emit m_ptrPublic->priceSearchedFinished(m_ptrPublic->m_stCompData);
	//m_ptrPublic->afterSearchCleanup();
}
const QString  ShopPluginBasePrivate::makeLogString(const QString & strLogText)
{
	return QString("[%1] %2").arg(m_iProcessingPhase).arg(strLogText);
}
QUrl ShopPluginBasePrivate::findBestLink(SearchResult stResult)
{
	QStringList stSearchedCompTokens = m_ptrPublic->m_strComponentName.toLower().split(" ");
	int iSearchedCompTokensCount = stSearchedCompTokens.count();
	int iCompTokensCount=0;
	std::map<QUrl, int> mCounter;
	//split found components name to tokens and store their counts 
	//////////////////////////////////////////////////////////////////////////
	for(SearchResult::const_iterator iterResult=stResult.begin();
		iterResult!=stResult.end();++iterResult)
	{
		std::pair<QUrl, int> stPair(iterResult->second,0);
		QString strFoundComponetCadidateName = iterResult->first;
		strFoundComponetCadidateName=strFoundComponetCadidateName.toLower();
		QStringList stFoundCandidateTokens = strFoundComponetCadidateName.split(" ");
		iCompTokensCount = stFoundCandidateTokens.size();
		for(int iCompTokensIndex = 0;iCompTokensIndex<iSearchedCompTokensCount;++iCompTokensIndex)
		{
			if (0>stFoundCandidateTokens.indexOf(stSearchedCompTokens.at(iCompTokensIndex)))	continue;
			(stPair.second)++;
		}
		if (stPair.second >= (iSearchedCompTokensCount/2))
			mCounter.insert(stPair);
	}
	//stored only those components which contains at last the same amount of tokens
	if (mCounter.size())
		return mCounter.begin()->first;
	return QUrl();
}
void ShopPluginBasePrivate::performHTMLCleanup()
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
		rc = tidyParseString( tdoc, m_strPageHTML.toAscii() );           // Parse the input
	if ( rc >= 0 )
		rc = tidyCleanAndRepair( tdoc );               // Tidy it up!
	if ( rc >= 0 )
		rc = tidyRunDiagnostics( tdoc );               // Kvetch
	if ( rc > 1 )                                    // If error, force output.
		rc = ( tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1 );
	if ( rc >= 0 )
		rc = tidySaveBuffer( tdoc, &output );          // Pretty Print
	m_strPageHTML = (char*)output.bp;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
ShopPluginBase::ShopPluginBase():m_bNoResultPage(false),m_bSingleProductPage(false),m_bSearchProductPage(false)
{
	m_ptrPriv = new ShopPluginBasePrivate(this);
#ifdef USE_WEBKIT
	QWebSettings::globalSettings()->setAttribute(QWebSettings::AutoLoadImages,false);
	QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptEnabled,false);
	QWebSettings::globalSettings()->setAttribute(QWebSettings::PrintElementBackgrounds,false);
#endif
	m_ProductPageSelectors.clear();
};
ShopPluginBase::~ShopPluginBase()
{
	delete m_ptrPriv;
}
#ifdef USE_WEBKIT
QWebPage * ShopPluginBase::getWebPage()
{
	return &(m_ptrPriv->m_stWebPage);
}
#endif
QString	ShopPluginBase::getCurrentlyProcessedComponent() const
{
	return m_strComponentName;
}
EPageLoadingPhase ShopPluginBase::getPageLoadingPhase()
{
	return m_ptrPriv->m_ePageLoadingPhase;
}
void ShopPluginBase::run()
{
	if (m_ptrPriv->thread()!=this->thread())
	{
		m_ptrPriv->moveToThread(this);
	}
#ifdef USE_WEBKIT
	if (m_ptrPriv->m_stWebPage.thread()!=this->thread())
	{
		m_ptrPriv->m_stWebPage.moveToThread(this);
	}
#endif
	exec();
}
bool ShopPluginBase::testSingleComponentName(QString strComponentNameToTest)
{
	int iComponentTokensCount(0);
	QStringList stCompToTestTokens = strComponentNameToTest.split(" ");
	int iSize(stCompToTestTokens.size());
	for(int iCompTok = 0;iCompTok<m_ptrPriv->m_iSearchedCompTokensCount;++iCompTok)
	{
		//find tokens in 
		if (0>stCompToTestTokens.indexOf(m_ptrPriv->m_ComponentNamesTokens.at(iCompTok)))	continue;
		iComponentTokensCount++;
	}
	if (iComponentTokensCount >= m_ptrPriv->m_iSearchedCompTokensCount)
		return true;
	return false;
}
void ShopPluginBase::printPageContent(const QString & strHTML)
{
#ifdef PRINT_LOADED_PAGE_CONTENT
#ifdef USE_LOG4QT
	QString pageName = QString("page_%1_%2_phase_%3.html").arg(shopName())
		.arg(m_strComponentName).arg(m_ptrPriv->m_iProcessingPhase);
	QFile stFile(LOG_DIR+pageName);
	stFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
	stFile.write(strHTML.toAscii());
	stFile.close();
#endif
#endif
}
QUrl ShopPluginBase::createSearchNextPageURL(unsigned int uPageNr)
{
	return QUrl();
}
void ShopPluginBase::prepareToNewSearch()
{
	m_stCompData.reset();
	m_ptrPriv->m_stTimer.start();
	m_ptrPriv->m_stResult.clear();
	m_ptrPriv->m_iProcessingPhase=1;
	m_ptrPriv->m_uCurrentPage=1;
	m_ptrPriv->m_ComponentNamesTokens = m_strComponentName.split(" ");
	m_ptrPriv->m_iSearchedCompTokensCount = m_ptrPriv->m_ComponentNamesTokens.count();
	m_bNoResultPage=false;
	m_bSingleProductPage=false;
	m_bSearchProductPage=false;
	if (0==m_ProductPageSelectors.size())
	{
		m_ProductPageSelectors =		produktPageTestSelectors();
		m_PriceProductCardSelectors=	produktPriceCardSelectors();
		m_PriceProductSelectors=		produktPriceSelectors();
		m_SearchPageSelectors =			searchPageTestSelectors();
		m_SearchPageProduktSelectors =	searchPageProductsSelectors();
		m_NotFoundPageSelectors =		notFoundPageSelectors();
		m_RefreshPageSelectors =		refreshPageSelectors();
		m_NextSearchPageSelectors =		nextSearchPageSelectors();
		m_ProductImageSelectors =		productImageSelectors();
	}

	prepareHTMLParserForNewSearch();
}
void ShopPluginBase::getProductData(const QString &strCompName, const QString & strOldURL )
{
	m_strComponentName = strCompName;
	prepareToNewSearch();
	m_stCompData.strName = strCompName;
	ShopPluginLoggers::printDebugLog(QString("//--------------------//"));
	ShopPluginLoggers::printDebugLog(m_ptrPriv->makeLogString(QString("New product search - '%1'").arg(strCompName)));
	QUrl stUrl;
	if (strOldURL.isEmpty())
	{
		ShopPluginLoggers::printDebugLog(m_ptrPriv->makeLogString("Start loading product data"));
		m_ptrPriv->m_ePageLoadingPhase = PAGETYPE_SEARCH;
		stUrl = createSearchURL();
		m_stCompData.strCompSearchURL = stUrl.toString();
		emit priceSearchProgress(strCompName,ESEARCHING_PRODUCT, 1);
	}
	else
	{
		ShopPluginLoggers::printDebugLog(m_ptrPriv->makeLogString("Start loading old URL based product data"));
		m_ptrPriv->m_ePageLoadingPhase = PAGETYPE_PRODUCT_OLD_URL;
		stUrl = strOldURL;
		m_stCompData.strCompURL = stUrl.toString();
		emit priceSearchProgress(strCompName,ETEST_OLD_URL, 1);
	}
	++(m_ptrPriv->m_iProcessingPhase);
	m_ptrPriv->m_strCurrentURL = stUrl.toString();
	m_ptrPriv->m_stNetworkHTMLLoader.loadPage(stUrl.toString());
	start();	//starting thread processing
}

QString ShopPluginBase::getSearchProductPageURL( const QString &strCompName )
{
	QUrl stUrl = createSearchURL(strCompName);
	return stUrl.toString();
}
void ShopPluginBase::pageNotFound()
{
#ifdef USE_WEBKIT
	QWebFrame * ptrFrame = getWebPage()->mainFrame();
	m_stCompData.strCompURL = ptrFrame->url().toString();
#endif
	m_stCompData.dPrice = 0;
	m_stCompData.eSearchResult=SR_COMPNOTFOUND;
	ShopPluginLoggers::printDebugLog("Product not found.");
}
SelectorsList ShopPluginBase::refreshPageSelectors()
{
	SelectorsList stSelectorsList;
	//a[class="produkt"]

	stSelectorsList.push_back("meta");
	stSelectorsList.push_back("http-equiv");
	stSelectorsList.push_back("refresh");
	return stSelectorsList;
}
//////////////////////////////////////////////////////////////////////////
void ShopPluginBase::onPageDownloadFinished(const QString & strPageContent)
{
	ShopPluginLoggers::printDebugLog("Product downloading finished.");
#ifndef LOAD_WITH_WEBKIT
	m_ptrPriv->m_strPageHTML = strPageContent;
	//disconnect(m_ptrPriv->m_pReply, 0,0,0);
	//m_ptrPriv->m_pReply->deleteLater();
#else
	disconnect(&(m_ptrPriv->m_stWebPage), 0,0,0);
#endif
	m_ptrPriv->performHTMLCleanup();
#ifdef LOAD_WITH_WEBKIT
	printPageContent(m_ptrPriv->m_stWebPage.mainFrame()->toHtml());
#else
	printPageContent(m_ptrPriv->m_strPageHTML);
#endif
#ifdef USE_XML_PARSER
	m_bSingleProductPage=false;
	m_bSearchProductPage=false;
	m_bNoResultPage=false;
	m_bIsRefresNeeded=false;
	m_bLoadNextPage=false;
	m_ptrPriv->m_stXMLInputSource.setData(m_ptrPriv->m_strPageHTML);

	bool bResult = m_ptrPriv->m_stXMLReader.parse(&m_ptrPriv->m_stXMLInputSource);
	if (!bResult)
	{
		m_stCompData.eSearchResult=SR_LOADINGFAIL;
		ShopPluginLoggers::printDebugLog("Product loading failed.");
		emit priceSearchedFinished(m_stCompData);
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	if (isRefreshNeeded())
	{
		m_ptrPriv->m_stNetworkHTMLLoader.loadPage(m_strRefreshURL);
		return;
	}
	else if (isNotFoundPage())
	{
		pageNotFound();
		emit priceSearchedFinished(m_stCompData);
		return;
	}
	else if (isSingleProductPage())
	{
		parseProductPage();
		emit priceSearchedFinished(m_stCompData);
		return;
	}
	//if we are here - this means this is search page - gather date and load next page
	m_ptrPriv->processSearchPageLoad();
#else

	//////////////////////////////////////////////////////////////////////////
	switch(m_ptrPriv->m_ePageLoadingPhase)
	{
	case PAGETYPE_ERROR:
		{
			m_ptrPriv->processErrorPageLoad();
			return;
		}
	case PAGETYPE_SEARCH:
	case PAGETYPE_SEARCH_NEXT_PAGE:
		{
			emit priceSearchProgress(m_strComponentName,ESEARCHING_PRODUCT, m_ptrPriv->m_uCurrentPage+1);
			m_ptrPriv->processSearchPageLoad();
			return;
		}
	case PAGETYPE_PRODUCT_OLD_URL:
	case PAGETYPE_PRODUCT:
		{
			emit priceSearchProgress(m_strComponentName,ELOAD_PAGE, 1);
			m_ptrPriv->processProductPageLoad();
			return;
		}
	}
#endif
}
void ShopPluginBase::onPageLoadingProgress(qint64 bytesRead, qint64 totalBytes)
{
	//ShopPluginLoggers::printDebugLog(m_ptrPriv->makeLogString(QString("Update data read progress (%1/%2)").arg(bytesRead).arg(totalBytes)));
}
void ShopPluginBase::onPageLoadingError(const QString strFaultReason)
{
	if (strFaultReason==TIMEOUT_INFO)
	{
		ShopPluginLoggers::printErrorLog(m_ptrPriv->makeLogString(QString("Refreshing timeout reached")));
		m_stCompData.eSearchResult=SR_LOADINGTIMEOUT;
		ShopPluginLoggers::printDebugLog("Product loading timeout.");
		emit priceSearchedFinished(m_stCompData);
		return;
	}
	else if (strFaultReason==ERROR_INFO)
	{
		ShopPluginLoggers::printErrorLog(m_ptrPriv->makeLogString(QString("Network downloading failed")));
		m_stCompData.eSearchResult=SR_LOADINGFAIL;
		emit priceSearchedFinished(m_stCompData);
		return;
	}
	else
	{

	}
}
