#include "ShopPluginBase.h"
#include "ShopPluginBase_p.h"
#include <QtXml/QDomDocument>
#include <Tools/qtTools.h>

const unsigned int ciSinglePageLoadTime(20);
const unsigned int ciWholePluginLoadingTime(30);

ShopPluginBasePrivate::ShopPluginBasePrivate(ShopPluginBase * ptrPublic):
m_ptrPublic(ptrPublic), m_ePageLoadingPhase(PAGETYPE_NONE),
m_uCurrentPage(1),m_strCurrentURL(""),m_ptrNetworkObjectLoader(new LoadNetworkObject)
{
#ifdef USE_XML_PARSER
	m_stXMLInputSource.setData(QString(""));
	m_stXMLReader.setContentHandler(m_ptrPublic);
	m_stXMLReader.setErrorHandler(m_ptrPublic);  
#endif
	bool bResult(false);
	bResult = QObject::connect(m_ptrNetworkObjectLoader.get(), SIGNAL(netObjectLoadingFinished(const QByteArray &)),	m_ptrPublic, SLOT(onPageDownloadFinished(const QByteArray &)));
	logConnection("ShopPluginBasePrivate","'m_stNetworkObjectLoader::netObjectLoadingFinished' with 'm_ptrPublic::onPageDownloadFinished'", bResult);
	bResult = QObject::connect(m_ptrNetworkObjectLoader.get(), SIGNAL(netObjectLoadingFailed(const QString)),	m_ptrPublic, SLOT(onPageLoadingError(const QString)));
	logConnection("ShopPluginBasePrivate","'m_stNetworkObjectLoader::netObjectLoadingFailed' with 'm_ptrPublic::onPageLoadingError'", bResult);
	bResult = QObject::connect(m_ptrNetworkObjectLoader.get(), SIGNAL(netObjectLoadingProgress(qint64, qint64)),	m_ptrPublic, SLOT(onPageLoadingProgress(qint64, qint64)));
	logConnection("ShopPluginBasePrivate","'m_stNetworkObjectLoader::netObjectLoadingProgress' with 'm_ptrPublic::onPageLoadingProgress'", bResult);
}
void ShopPluginBasePrivate::loadNextPage()
{
	m_ePageLoadingPhase = PAGETYPE_SEARCH_NEXT_PAGE;
	++m_uCurrentPage;
	QUrl stUrl = m_ptrPublic->createSearchNextPageURL(m_uCurrentPage);
	m_ptrNetworkObjectLoader->loadNetworkObject(stUrl.toString());
}
void ShopPluginBasePrivate::processSearchPageLoad()
{
	printLog(eDebugLogLevel, eDebug, QString("Search page loaded for '%1'").arg(m_ptrPublic->m_strComponentName));
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
	printLog(eDebugLogLevel, eDebug, makeLogString(QString("Found %1 on shop search site").arg(m_stResult.size())));
	int i=1;
	for(SearchResult::const_iterator iterRes=m_stResult.begin();
		iterRes!=m_stResult.end();++iterRes,++i)
		printLog(eDebugLogLevel, eDebug, QString("Component (%1): '%2'").arg(i).arg(iterRes->first));
	QUrl stUrl;
	if (m_stResult.size()>1)
	{
		stUrl = findBestLink(m_stResult);
		printLog(eDebugLogLevel, eDebug, QString("Best match (URL=%2)").arg(stUrl.toString()));

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
		printLog(eDebugLogLevel, eDebug, QString("Only one result (URL=%2)").arg(stUrl.toString()));
	}
	else if (m_stResult.size()==0)
	{
		m_ptrPublic->m_stCompData.eSearchResult = SR_COMPNOTFOUND;
		printLog(eDebugLogLevel, eDebug, makeLogString(QString("No matching components found")));
		emit m_ptrPublic->priceSearchedFinished(m_ptrPublic->m_stCompData);
		//m_ptrPublic->afterSearchCleanup();
		return;
	}
	++m_iProcessingPhase;
	m_ePageLoadingPhase = PAGETYPE_PRODUCT;
	m_ptrPublic->m_stCompData.strCompURL =stUrl.toString();
	m_strCurrentURL = stUrl.toString();
	m_ptrNetworkObjectLoader->loadNetworkObject(stUrl.toString());
}
void ShopPluginBasePrivate::processProductPageLoad()
{
	printLog(eDebugLogLevel, eDebug, QString("Component page loaded for '%2'").arg(m_ptrPublic->m_strComponentName));
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
ShopPluginBase::ShopPluginBase():m_ptrPriv(new ShopPluginBasePrivate(this)), 
m_bNoResultPage(false),m_bSingleProductPage(false),m_bSearchProductPage(false)
{
	m_ProductPageSelectors.clear();
};
ShopPluginBase::~ShopPluginBase()
{
	//delete m_ptrPriv;
}
QString	ShopPluginBase::getCurrentlyProcessedComponent() const
{
	return m_strComponentName;
}
void ShopPluginBase::run()
{
	if (m_ptrPriv->thread()!=this->thread())
	{
		m_ptrPriv->moveToThread(this);
	}
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
	QFile stFile(QString("logs/")+pageName);
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
	}
	if (0==m_PriceProductCardSelectors.size())
	{
		m_PriceProductCardSelectors=	produktPriceCardSelectors();
	}
	if (0==m_PriceProductSelectors.size())
	{
		m_PriceProductSelectors=		produktPriceSelectors();
	}
	if (0==m_SearchPageSelectors.size())
	{
		m_SearchPageSelectors =			searchPageTestSelectors();
	}
	if (0==m_SearchPageProduktSelectors.size())
	{
		m_SearchPageProduktSelectors =	searchPageProductsSelectors();
	}
	if (0==m_NotFoundPageSelectors.size())
	{
		m_NotFoundPageSelectors =		notFoundPageSelectors();
	}
	if (0==m_RefreshPageSelectors.size())
	{
		m_RefreshPageSelectors =		refreshPageSelectors();
	}
	if (0==m_NextSearchPageSelectors.size())
	{
		m_NextSearchPageSelectors =		nextSearchPageSelectors();
	}
	if (0==m_ProductImageSelectors.size())
	{
		m_ProductImageSelectors =		productImageSelectors();
	}

	prepareHTMLParserForNewSearch();
}
void ShopPluginBase::getProductData(const QString &strCompName, const QString & strOldURL )
{
	m_strComponentName = strCompName;
	prepareToNewSearch();
	m_stCompData.strName = strCompName;
	printLog(eDebugLogLevel, eDebug, QString("//--------------------//"));
	printLog(eDebugLogLevel, eDebug, m_ptrPriv->makeLogString(QString("New product search - '%1'").arg(strCompName)));
	QUrl stUrl;
	if (strOldURL.isEmpty())
	{
		printLog(eDebugLogLevel, eDebug, m_ptrPriv->makeLogString("Start loading product data"));
		m_ptrPriv->m_ePageLoadingPhase = PAGETYPE_SEARCH;
		stUrl = createSearchURL();
		m_stCompData.strCompSearchURL = stUrl.toString();
		emit priceSearchProgress(strCompName,ESEARCHING_PRODUCT, 1);
	}
	else
	{
		printLog(eDebugLogLevel, eDebug, m_ptrPriv->makeLogString("Start loading old URL based product data"));
		m_ptrPriv->m_ePageLoadingPhase = PAGETYPE_PRODUCT_OLD_URL;
		stUrl = strOldURL;
		m_stCompData.strCompURL = stUrl.toString();
		emit priceSearchProgress(strCompName,ETEST_OLD_URL, 1);
	}
	++(m_ptrPriv->m_iProcessingPhase);
	m_ptrPriv->m_strCurrentURL = stUrl.toString();
	m_ptrPriv->m_ptrNetworkObjectLoader->loadNetworkObject(stUrl.toString());
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
	printLog(eDebugLogLevel, eDebug, "Product not found.");
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
void ShopPluginBase::onPageDownloadFinished(const QByteArray & aContent)
{
	QString strContent(aContent);
	onPageDownloadFinished(strContent);
}
void ShopPluginBase::onPageDownloadFinished(const QString & strPageContent)
{
	printLog(eDebugLogLevel, eDebug, "Product downloading finished.");
	m_ptrPriv->m_strPageHTML = strPageContent;
	m_ptrPriv->performHTMLCleanup();
	//printPageContent(m_ptrPriv->m_strPageHTML);
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
		printLog(eDebugLogLevel, eDebug, "Product loading failed.");
		emit priceSearchedFinished(m_stCompData);
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	if (isRefreshNeeded())
	{
		m_ptrPriv->m_ptrNetworkObjectLoader->loadNetworkObject(m_strRefreshURL);
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
#endif
}
void ShopPluginBase::onPageLoadingProgress(qint64 bytesRead, qint64 totalBytes)
{
	//printDebugLog(m_ptrPriv->makeLogString(QString("Update data read progress (%1/%2)").arg(bytesRead).arg(totalBytes)));
}
void ShopPluginBase::onPageLoadingError(const QString strFaultReason)
{
	if (strFaultReason==TIMEOUT_INFO)
	{
		printLog(eErrorLogLevel, eDebug, m_ptrPriv->makeLogString(QString("Refreshing timeout reached")));
		m_stCompData.eSearchResult=SR_LOADINGTIMEOUT;
		printLog(eDebugLogLevel, eDebug, "Product loading timeout.");
		emit priceSearchedFinished(m_stCompData);
		return;
	}
	else if (strFaultReason==ERROR_INFO)
	{
		printLog(eErrorLogLevel, eDebug, m_ptrPriv->makeLogString(QString("Network downloading failed")));
		m_stCompData.eSearchResult=SR_LOADINGFAIL;
		emit priceSearchedFinished(m_stCompData);
		return;
	}
	else
	{

	}
}
void ShopPluginBase::loadSelectorsFromXML(const QString & strPluginName)
{
	read_xml("plugins/"+strPluginName.toStdString(), m_ptrPriv->pt);
	QDomDocument doc;
	QFile file(QString("plugins/")+strPluginName);
	if (!file.open(QIODevice::ReadOnly))
		return;
	if (!doc.setContent(&file)) {
		file.close();
		return;
	}
	file.close();
	QDomElement mainDocElem = doc.documentElement();
	QDomNode singleSelectorNode = mainDocElem.firstChild();
	while(!singleSelectorNode.isNull()) 
	{
		QString nodeName = singleSelectorNode.nodeName();
		QDomElement e = singleSelectorNode.toElement(); // try to convert the node to an element.
		QString strTag = e.attribute("tag");
		QString strAttrName = e.attribute("attrName");
		QString strAttrValue = e.attribute("attrValue");

		SelectorsList stSelectorsList;
		stSelectorsList.push_back(strTag);
		stSelectorsList.push_back(strAttrName);
		stSelectorsList.push_back(strAttrValue);
		if (nodeName=="SINGLE_PRODUCT_PAGE")
		{
			m_ProductPageSelectors = stSelectorsList;
		}
		else if (nodeName=="PRODUCT_PRICE_CARD")
		{
			m_PriceProductCardSelectors = stSelectorsList;
		}
		else if (nodeName=="PRODUCT_PRICE")
		{
			m_PriceProductSelectors = stSelectorsList;
		}
		else if (nodeName=="PRODUCT_SEARCH_LIST_TEST")
		{
			m_SearchPageSelectors =	stSelectorsList;
		}
		else if (nodeName=="SINGLE_ENTRY_ON_PRODUCT_SEARCH_LIST")
		{
			m_SearchPageProduktSelectors = stSelectorsList;
		}
		else if (nodeName=="PRODUCT_NOT_FOUND")
		{
			m_NotFoundPageSelectors = stSelectorsList;
		}
		else if (nodeName=="PRODUCT_SEARCH_LIST_NEXT_PAGE")
		{
			m_NextSearchPageSelectors = stSelectorsList;
		}
		else if (nodeName=="PRODUCT_IMAGE")
		{
			m_ProductImageSelectors = stSelectorsList;
		}
		else if (nodeName=="REFRESH_PAGE")
		{
			m_RefreshPageSelectors = stSelectorsList;
		}
		singleSelectorNode = singleSelectorNode.nextSibling();
	}
}
