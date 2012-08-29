#ifndef _CCS_SHOP_PLUGIN_BASE_INCLUDE_
#define _CCS_SHOP_PLUGIN_BASE_INCLUDE_
#include <ShopInterface.h>
#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QThread>
#include <QtNetwork/QAuthenticator>
#include <QtNetwork/QNetworkReply>
#include <QtXml/QXmlAttributes>
#include <map>
#include <set>

#define PRINT_LOADED_PAGE_CONTENT
//#define USE_WEBKIT
#ifdef USE_WEBKIT
#define LOAD_WITH_WEBKIT
#endif
#define USE_XML_PARSER

class ShopPluginBasePrivate;
#ifdef USE_WEBKIT
class QWebPage;
#endif

enum EPageLoadingPhase
{
	PAGETYPE_NONE,
	PAGETYPE_ERROR,
	PAGETYPE_PRODUCT_OLD_URL,
	PAGETYPE_SEARCH,
	PAGETYPE_SEARCH_NEXT_PAGE,
	PAGETYPE_PRODUCT
};

//CompName, URL
typedef std::map<QString, QUrl >  SearchResult;

//selector 
typedef std::vector<QString >  SelectorsList;	//

//class responsible for downloading HTML page contents and handling signal-slots
class ShopPluginBase : public ShopInterface
#ifdef USE_XML_PARSER
	, public QXmlDefaultHandler
#endif
{
	friend class ShopPluginBasePrivate;
	Q_OBJECT
	Q_INTERFACES(ShopInterface)
public:
	ShopPluginBase();
	~ShopPluginBase();
	void getProductData(const QString & strCompName, const QString & strOldURL = QString() );
	QString	getCurrentlyProcessedComponent() const;
	virtual QString getSearchProductPageURL(const QString &strCompName);
protected slots:
	void onPageDownloadFinished(const QString &);
	//void onUpdateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
	void onPageLoadingProgress(qint64 bytesRead, qint64 totalBytes);
	void onPageLoadingError(const QString strFaultReason);

	//void 	onAuthenticationRequired ( QNetworkReply * reply, QAuthenticator * authenticator );
	//void 	onNetworkAccessibleChanged ( QNetworkAccessManager::NetworkAccessibility accessible );
	//void 	onProxyAuthenticationRequired ( const QNetworkProxy & proxy, QAuthenticator * authenticator );

protected:
	void run();
	//for debug purposes - print currently loaded HTML page
	void printPageContent(const QString &strHTML);
	//
	void prepareToNewSearch();
	//method called for children to clean their previous searching values
	virtual void prepareHTMLParserForNewSearch()=0;
	//tests if single component with name as parameter fits to searched pattern
	bool testSingleComponentName(QString strFoundComponentName);
	EPageLoadingPhase getPageLoadingPhase();

	//////////////////////////////////////////////////////////////////////////
	virtual QUrl createSearchURL(const QString & strName = QString()) = 0;
	//creates link to next page with search results, 
	virtual QUrl createSearchNextPageURL(unsigned int uPageNr);
	//creates link to given product page, provided URL 
	virtual QUrl createProductURL(const QString & strUrlPart) = 0;
	virtual void parseSearchProductPage(SearchResult & stResult,bool & bNextPage)=0;
	virtual void parseProductPage() = 0;
	//////////////////////////////////////////////////////////////////////////
	//test if currently parsed page is without search result
	bool isNotFoundPage(){  return m_bNoResultPage;	}
	//test if currently parsed page is a product page
	bool isSingleProductPage(){	return m_bSingleProductPage;	}
	//test if currently parsed page is a search product page
	bool isSearchProductPage(){	return m_bSearchProductPage;	}
	//test if currently parsed page is a Refresh page
	bool isRefreshNeeded(){		return m_bIsRefresNeeded;	}
	//test if currently parsed page is a page with multiple search pages
	bool isLoadingNextSearchPage(){		return m_bLoadNextPage;	}

	//////////////////////////////////////////////////////////////////////////
	//selectors list used to test if parsed HTML page is a product page
	virtual SelectorsList produktPageTestSelectors()=0;
	//selectors used to detect HTML TAG containing Frame with product price - after that PriceSelectors will find proper price
	virtual SelectorsList produktPriceCardSelectors()=0;
	//selectors used to detect HTML TAG containing price - used only INSIDE PriceCard
	virtual SelectorsList produktPriceSelectors()=0;
	//selectors list used to test if parsed HTML page is a search page
	virtual SelectorsList searchPageTestSelectors()=0;
	//selectors used to detect HTML TAG containing link to single product from search results
	virtual SelectorsList searchPageProductsSelectors()=0;
	virtual SelectorsList notFoundPageSelectors()=0;
	virtual SelectorsList refreshPageSelectors();
	virtual SelectorsList nextSearchPageSelectors()=0;

	//what to do when page will be not found
	void pageNotFound();
	//virtual void SinglePageFound(){};
	//////////////////////////////////////////////////////////////////////////

#ifdef USE_XML_PARSER
	//XML parsing functions
	virtual bool startElement( const QString &a, const QString & localName, const QString & qName, const QXmlAttributes & atts ){ return false;};
	virtual bool endElement ( const QString & namespaceURI, const QString & localName, const QString & qName ){ return false;};
	virtual bool characters ( const QString & ch ){ return false;};
#endif
#ifdef USE_WEBKIT
	QWebPage * getWebPage();
#endif
protected:
	QString					m_strComponentName;
	ShopPluginBasePrivate *	m_ptrPriv;
	CompPriceData			m_stCompData;
	//////////////////////////////////////////////////////////////////////////
	bool					m_bNoResultPage;
	//flag - is currently loaded/parsed page a product page
	bool					m_bSingleProductPage;
	//flag - is currently loaded/parsed page a product search page with results
	bool					m_bSearchProductPage;
	//flag - is currently loaded/parsed page a refresh/redirect request
	bool					m_bIsRefresNeeded;
	//flag - search page contain multiple pages with result
	bool					m_bLoadNextPage;

	QString					m_strRefreshURL;
	//////////////////////////////////////////////////////////////////////////
	SelectorsList			m_ProductPageSelectors;
	SelectorsList			m_PriceProductCardSelectors;
	SelectorsList			m_PriceProductSelectors;
	SelectorsList			m_SearchPageSelectors;
	SelectorsList			m_SearchPageProduktSelectors;
	SelectorsList			m_NotFoundPageSelectors;
	SelectorsList			m_RefreshPageSelectors;
	SelectorsList			m_NextSearchPageSelectors;
};
#endif //_CCS_SHOP_PLUGIN_BASE_INCLUDE_
