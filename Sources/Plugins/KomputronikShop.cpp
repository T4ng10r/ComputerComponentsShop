#include "KomputronikShop.h"
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebElementCollection>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebPage>
#include <QtCore/QFile>
#include <map>

const QString cstrHomePage("http://www.komputronik.pl/");
const QString cstrSearchPrefix("http://www.komputronik.pl/search/pl/run/showProducts/1/searchString/");
const QString cstrSearchCategory("/category/9409/?view=2");
const QString cstrNextPageSuffix("?sb=&sd=&p=%1&sort=0&by=&sa=&vg=0");
//////////////////////////////////////////////////////////////////////////
namespace Komputronik
{
	//flag - frame with product code detected
	bool			bPriceCardRead=false;
	//flag
	//flag - HTML tag with price as a value open
	bool			bProductPriceTagOpen=false;
	bool			bSearchPageProduktTagOpen=false;
	QString			strTagText;
	QString			strHrefLink;
	SearchResult	mSearchResult;
	double			dPrice(0);
}

KomputronikShopPlugin::KomputronikShopPlugin()
{
	createLoggers(shopName());
}
KomputronikShopPlugin::~KomputronikShopPlugin()
{
	//delete m_ptrPriv;
}
QString KomputronikShopPlugin::pluginName() const
{
	return "KomputronikShopPlugin";
}
QString KomputronikShopPlugin::shopName() const
{
	return QString("Komputronik");
}
QIcon KomputronikShopPlugin::getShopIcon() const
{
	return QIcon(":/images/komputronik.png");
}
QString KomputronikShopPlugin::getShopURL() const
{	
	return cstrHomePage;
}
//////////////////////////////////////////////////////////////////////////
QUrl KomputronikShopPlugin::createSearchURL(const QString &  strName)
{
	QByteArray strCompNameArray = strName.isEmpty()?m_strComponentName.toAscii():strName.toAscii();
	QString strUrl(cstrSearchPrefix + strCompNameArray.toBase64() + 
		cstrSearchCategory);
	return strUrl;
}
QUrl KomputronikShopPlugin::createSearchNextPageURL(unsigned int uPageNr)
{
	QByteArray strCompNameArray(m_strComponentName.toAscii());
	QString strUrl(cstrSearchPrefix + strCompNameArray.toBase64() + 
		cstrSearchCategory+QString(cstrNextPageSuffix).arg(uPageNr));
	return strUrl;
}
QUrl KomputronikShopPlugin::createProductURL(const QString & strUrlPart)
{
	QString strUrl = strUrlPart;
	strUrl.replace(QString("//"),QString("/"));
	return strUrl;
}
//////////////////////////////////////////////////////////////////////////
void KomputronikShopPlugin::prepareHTMLParserForNewSearch()
{
	Komputronik::bProductPriceTagOpen=false;
	Komputronik::bSearchPageProduktTagOpen=false;
	Komputronik::strHrefLink="";
	Komputronik::strTagText="";
	Komputronik::mSearchResult.clear();
	Komputronik::dPrice=0;
}
SelectorsList KomputronikShopPlugin::produktPageTestSelectors()
{
	SelectorsList stSelectorsList;
	//div class=mainColumnContainer advancedSearchResultsContainer
	stSelectorsList.push_back("div");
	stSelectorsList.push_back("class");
	stSelectorsList.push_back("mainColumnContainer productFullViewContainer");
	return stSelectorsList;
};
SelectorsList KomputronikShopPlugin::produktPriceCardSelectors()
{
	SelectorsList stSelectorsList;
	//div class=pricing
	stSelectorsList.push_back("div");
	stSelectorsList.push_back("class");
	stSelectorsList.push_back("pricing internetPrice");
	return stSelectorsList;
}
SelectorsList KomputronikShopPlugin::produktPriceSelectors()
{
	SelectorsList stSelectorsList;
	//div class="altFontFace price"
	stSelectorsList.push_back("div");
	stSelectorsList.push_back("class");
	stSelectorsList.push_back("altFontFace price");
	return stSelectorsList;
}
SelectorsList KomputronikShopPlugin::searchPageTestSelectors()
{
	SelectorsList stSelectorsList;
	//div class="mainColumnContainer productsListContainer"
	stSelectorsList.push_back("div");
	stSelectorsList.push_back("class");
	stSelectorsList.push_back("mainColumnContainer productsListContainer");
	return stSelectorsList;
};
SelectorsList KomputronikShopPlugin::searchPageProductsSelectors()
{
	SelectorsList stSelectorsList;
	//div class=name
	stSelectorsList.push_back("div");
	stSelectorsList.push_back("class");
	stSelectorsList.push_back("name");
	return stSelectorsList;
}
SelectorsList KomputronikShopPlugin::notFoundPageSelectors()
{
	SelectorsList stSelectorsList;
	//div class="noProductsFound"
	stSelectorsList.push_back("div");
	stSelectorsList.push_back("class");
	stSelectorsList.push_back("noProductsFound");
	return stSelectorsList;
}
SelectorsList KomputronikShopPlugin::nextSearchPageSelectors()
{
	SelectorsList stSelectorsList;
	//li class="next"
	stSelectorsList.push_back("li");
	stSelectorsList.push_back("class");
	stSelectorsList.push_back("next");
	return stSelectorsList;
}
//////////////////////////////////////////////////////////////////////////
#ifndef USE_XML_PARSER
bool KomputronikShopPlugin::isNotFoundPage()
{
#ifdef USE_WEBKIT
	QWebFrame * ptrFrame = getWebPage()->mainFrame();
	QWebElementCollection notFoundPage = ptrFrame->findAllElements("div[class=noProductsFound]");
	if (notFoundPage.count())
		return true;
#endif
	return false;
}
bool KomputronikShopPlugin::isSingleProductPage()
{
#ifdef USE_WEBKIT
	QWebFrame * ptrFrame = getWebPage()->mainFrame();
	QWebElementCollection productPage = ptrFrame->findAllElements("div[class=\"product fullView\"]");
	if (productPage.count())
		return true;
#endif
	return false;
}
bool KomputronikShopPlugin::isSearchProductPage()
{
	return true;
}
#endif
void KomputronikShopPlugin::parseSearchProductPage(SearchResult & stResult,bool & bNextPage)
{
#ifdef USE_WEBKIT
	bNextPage=false;
	QWebFrame * ptrFrame = getWebPage()->mainFrame();
	//////////////////////////////////////////////////////////////////////////
	QWebElementCollection tableProdRows = ptrFrame->findAllElements("div[class=\"product compactView\"]");
	for(int iIndex=0;iIndex<tableProdRows.count();++iIndex)
	{
		QWebElement prodNameHeader = tableProdRows.at(iIndex).findFirst("div[class=name]");
		if (prodNameHeader.isNull())
			continue;
		QWebElement productLink = prodNameHeader.findFirst("a");
		QUrl stProductURL = productLink.attribute("href");
		//////////////////////////////////////////////////////////////////////////
		QString strName = productLink.attribute("title");
		stResult.insert(std::make_pair(strName,stProductURL));
	}
	//////////////////////////////////////////////////////////////////////////
	QWebElement tablePageNavi = ptrFrame->findFirstElement("li[class=next]");
	if (tablePageNavi.isNull())
	{
		bNextPage=false;
		return;
	}
#endif
	stResult.insert(Komputronik::mSearchResult.begin(),Komputronik::mSearchResult.end());
	bNextPage=m_bLoadNextPage;
}
void KomputronikShopPlugin::parseProductPage()
{
#ifdef USE_WEBKIT
	QWebFrame * ptrFrame = getWebPage()->mainFrame();
	printPageContent(ptrFrame->toHtml());
	QWebElement elCompFullName = ptrFrame->findFirstElement("div[class=\"name \"]");//div class="name"
	QWebElement elPrice = ptrFrame->findFirstElement("div[class=\"altFontFace price\"]"); //div class="altFontFace price"
	QString strPrice = elPrice.toPlainText();
	int iPos = strPrice.indexOf(" z");
	if (iPos!=-1)
	{
		strPrice = strPrice.left(iPos);
		m_stCompData.dPrice = strPrice.toDouble();
	}
	m_stCompData.eSearchResult = SR_COMPFOUND;
	m_stCompData.strCompURL = getWebPage()->mainFrame()->url().toString();
#endif
#ifdef USE_XML_PARSER
	m_stCompData.dPrice = Komputronik::dPrice;
	m_stCompData.eSearchResult = SR_COMPFOUND;
	printLog(eDebugLogLevel, eDebug,"Product found.");
#endif
}
#ifdef USE_XML_PARSER
bool KomputronikShopPlugin::startElement( const QString &a, const QString & localName, const QString & qName, const QXmlAttributes & atts )
{
	QString strElementName = qName.toLower();
	if (m_ProductPageSelectors[0]==strElementName && m_ProductPageSelectors[2]==atts.value(m_ProductPageSelectors[1]))
	{
		m_bSingleProductPage=true;
		return true;
	}
	if (m_bSingleProductPage && m_PriceProductCardSelectors[0]==strElementName && m_PriceProductCardSelectors[2]==atts.value(m_PriceProductCardSelectors[1]))
	{
		Komputronik::bPriceCardRead=true;
		return true;
	}
	if (Komputronik::bPriceCardRead && m_PriceProductSelectors[0]==strElementName && m_PriceProductSelectors[2]==atts.value(m_PriceProductSelectors[1]))
	{
		Komputronik::bProductPriceTagOpen=true;
		return true;
	}
	if (m_SearchPageSelectors[0]==strElementName && m_SearchPageSelectors[2]==atts.value(m_SearchPageSelectors[1]))
	{
		m_bSearchProductPage=true;
		return true;
	}
	if (m_SearchPageProduktSelectors[0]==strElementName && m_SearchPageProduktSelectors[2]==atts.value(m_SearchPageProduktSelectors[1]))
	{
		Komputronik::bSearchPageProduktTagOpen=true;
		return true;
	}
	if (m_NotFoundPageSelectors[0]==strElementName && m_NotFoundPageSelectors[2]==atts.value(m_NotFoundPageSelectors[1]))
	{
		m_bNoResultPage=true;
		return true;
	}
	if (m_NextSearchPageSelectors[0]==strElementName && m_NextSearchPageSelectors[2]==atts.value(m_NextSearchPageSelectors[1]))
	{
		m_bLoadNextPage=true;
		return true;
	}
	if (m_RefreshPageSelectors[0]==strElementName && m_RefreshPageSelectors[2]==atts.value(m_RefreshPageSelectors[1]))
	{
		//get destination URL
		QString strValue = atts.value("content");
		int iPos = strValue.indexOf("url=");
		m_strRefreshURL = strValue.remove(0,iPos+4);
		m_bIsRefresNeeded=true;
		return true;
	}
	if (Komputronik::bSearchPageProduktTagOpen && "a"==strElementName )
	{
		Komputronik::strHrefLink=atts.value("href");
		return true;
	}
	return true;
}
bool KomputronikShopPlugin::endElement ( const QString & namespaceURI, const QString & localName, const QString & qName )
{
	if (Komputronik::bSearchPageProduktTagOpen && qName.toLower()=="a")
	{
		Komputronik::bSearchPageProduktTagOpen=false;
		Komputronik::mSearchResult[Komputronik::strTagText]=QUrl(Komputronik::strHrefLink);
	}
	return true;
}
bool KomputronikShopPlugin::characters ( const QString & ch )
{
	if (Komputronik::bProductPriceTagOpen && ch.indexOf(" z")>0 )
	{
		Komputronik::bProductPriceTagOpen=false;
		int iPos = ch.indexOf(" z");
		if (iPos!=-1)
		{
			QString strPrice = ch.left(iPos);
			Komputronik::dPrice = strPrice.toDouble();
			Komputronik::bProductPriceTagOpen=false;
			Komputronik::bPriceCardRead=false;
			return true;
		}
	}
	if (Komputronik::bSearchPageProduktTagOpen && !ch.isEmpty())
	{
		Komputronik::strTagText=ch;
		return true;
	}

	return true;
}
#endif
#ifndef WITHOUT_INTERFACE 
Q_EXPORT_PLUGIN2("KomputronikShopPlugin",KomputronikShopPlugin);
#endif
