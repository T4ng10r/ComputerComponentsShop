#include "ArestShop.h"
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebElementCollection>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebPage>
#include <QtCore/QFile>
#include <map>

const QString cstrHomePage("http://www.arest.pl/");
const QString cstrSearchPrefix("http://www.arest.pl/szukaj/%1//");

int giProgress=1;
namespace ShopPluginLoggers
{
	void createLoggers(const QString & strName);
	void printErrorLog(const QString & strLog);
	void printDebugLog(const QString & strLog);
}

namespace Arest
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

ArestShopPlugin::ArestShopPlugin()
{
	ShopPluginLoggers::createLoggers(shopName());
}
ArestShopPlugin::~ArestShopPlugin(){}
QString ArestShopPlugin::pluginName() const
{
	return "ArestShopPlugin";
}
QString ArestShopPlugin::shopName() const
{
	return QString("Arest");
}
QIcon ArestShopPlugin::getShopIcon() const
{
	return QIcon(":/images/arest.ico");
}
QString ArestShopPlugin::getShopURL() const
{	
	return cstrHomePage;
}
//////////////////////////////////////////////////////////////////////////
void ArestShopPlugin::prepareHTMLParserForNewSearch()
{

}
SelectorsList ArestShopPlugin::produktPageTestSelectors()
{
	SelectorsList stSelectorsList;
	//h1 class=nazwatowaru
	stSelectorsList.push_back("h1");
	stSelectorsList.push_back("class");
	stSelectorsList.push_back("nazwatowaru");
	return stSelectorsList;
};
SelectorsList ArestShopPlugin::produktPriceCardSelectors()
{
	SelectorsList stSelectorsList;
	//table class="towar"
	stSelectorsList.push_back("table");
	stSelectorsList.push_back("class");
	stSelectorsList.push_back("towar");
	return stSelectorsList;
}
SelectorsList ArestShopPlugin::produktPriceSelectors()
{
	SelectorsList stSelectorsList;
	//span class="price"
	stSelectorsList.push_back("span");
	stSelectorsList.push_back("class");
	stSelectorsList.push_back("price");
	return stSelectorsList;
}
SelectorsList ArestShopPlugin::searchPageTestSelectors()
{
	SelectorsList stSelectorsList;
	//td class="grupa_cs"
	stSelectorsList.push_back("td");
	stSelectorsList.push_back("class");
	stSelectorsList.push_back("grupa_cs");
	return stSelectorsList;
};
SelectorsList ArestShopPlugin::searchPageProductsSelectors()
{
	SelectorsList stSelectorsList;
	//table class="pricelist"
	stSelectorsList.push_back("td");
	stSelectorsList.push_back("class");
	stSelectorsList.push_back("grupa_cs");
	return stSelectorsList;
}
SelectorsList ArestShopPlugin::notFoundPageSelectors()
{
	SelectorsList stSelectorsList;
	//div class="searchfailed"
	stSelectorsList.push_back("div");
	stSelectorsList.push_back("class");
	stSelectorsList.push_back("searchfailed");
	return stSelectorsList;
}
SelectorsList ArestShopPlugin::nextSearchPageSelectors()
{
	SelectorsList stSelectorsList;
	stSelectorsList.push_back("");
	stSelectorsList.push_back("");
	stSelectorsList.push_back("");
	return stSelectorsList;
}
//////////////////////////////////////////////////////////////////////////
QUrl ArestShopPlugin::createSearchURL(const QString &  strName)
{
	QString strUrl(QString(cstrSearchPrefix).arg(strName.isEmpty()?m_strComponentName:strName));
	return strUrl;
}
QUrl ArestShopPlugin::createSearchNextPageURL(unsigned int uPageNr)
{
	//QByteArray strCompNameArray(m_strComponentName.toAscii());
	//QString strUrl(cstrSearchPrefix + strCompNameArray.toBase64() + 
	//	cstrSearchCategory+QString(cstrNextPageSuffix).arg(uPageNr));
	//return strUrl;
	return QUrl();
}
QUrl ArestShopPlugin::createProductURL(const QString & strUrlPart)
{
	//QString strUrl(stProductUrl.toString());
	//strUrl.replace(QString("//"),QString("/"));
	//return strUrl;
	return QUrl();
}
bool ArestShopPlugin::isNotFoundPage()
{
#ifdef USE_WEBKIT
	QWebFrame * ptrFrame = getWebPage()->mainFrame();
	QWebElementCollection notFoundPage = ptrFrame->findAllElements("div[class=searchfailed]");
	if (notFoundPage.count())
		return true;
#endif
	return false;
}
bool ArestShopPlugin::isSingleProductPage()
{
#ifdef USE_WEBKIT
	QWebFrame * ptrFrame = getWebPage()->mainFrame();
	QWebElementCollection productPage = ptrFrame->findAllElements("h1[class=nazwatowaru]");
	if (productPage.count())
		return true;
#endif
	return false;
}
void ArestShopPlugin::NotFoundPage()
{
#ifdef USE_WEBKIT
	QWebFrame * ptrFrame = getWebPage()->mainFrame();
	m_stCompData.dPrice = 0;
	m_stCompData.strCompURL = ptrFrame->url().toString();
	m_stCompData.eSearchResult=SR_COMPNOTFOUND;
	emit priceSearchedFinished(m_stCompData);
	afterSearchCleanup();
#endif
}
//void ArestShopPlugin::SinglePageFound()
//{
//#ifdef USE_WEBKIT
//	QWebFrame * ptrFrame = getWebPage()->mainFrame();
//	parseProductPage(m_stCompData);
//	m_stCompData.strCompURL = ptrFrame->url().toString();
//	emit priceSearchedFinished(m_stCompData);
//	productFoundFinish();
//#endif
//};
void ArestShopPlugin::parseSearchProductPage(SearchResult & stResult,bool & bNextPage)
{
	//run XML search page parse

	bNextPage=false;
#ifdef USE_WEBKIT
	QWebFrame * ptrFrame = getWebPage()->mainFrame();
	//////////////////////////////////////////////////////////////////////////
	QWebElementCollection tableProdRows = ptrFrame->findAllElements("table[class=pricelist]");
	
	for(int iIndex=0;iIndex<tableProdRows.count();++iIndex)
	{
		QWebElement prodNameHeader = tableProdRows.at(iIndex).findFirst("td[class=nazwa]");
		if (prodNameHeader.isNull())
			continue;
		QWebElement productLink = prodNameHeader.findFirst("a");
		QUrl stProductURL = productLink.attribute("href");
		//////////////////////////////////////////////////////////////////////////
		QString strName = productLink.toPlainText();
		stResult.insert(std::make_pair(strName,stProductURL));
	}
	//////////////////////////////////////////////////////////////////////////
	QWebElement tablePageNavi = ptrFrame->findFirstElement("li[class=next]");
	if (tablePageNavi.isNull())
	{
		bNextPage=false;
		return;
	}
	bNextPage=true;
#endif
	stResult.insert(Arest::mSearchResult.begin(),Arest::mSearchResult.end());
	bNextPage=m_bLoadNextPage;
}
void ArestShopPlugin::parseProductPage()
{
#ifdef USE_WEBKIT
	QWebFrame * ptrFrame = getWebPage()->mainFrame();
	printPageContent(ptrFrame->toHtml());
	QWebElement elCompFullName = ptrFrame->findFirstElement("h1[class=nazwatowaru]");
	QWebElement elPrice = ptrFrame->findFirstElement("span[class=price]"); //div class="altFontFace price"
	QString strPrice = elPrice.toPlainText();
	for(int i = 0;i<strPrice.size();i++)
	{
		if (strPrice.at(i).isLetterOrNumber())
			continue;
		strPrice = strPrice.left(i);
		break;
	}
	m_stCompData.dPrice = strPrice.toDouble();
	m_stCompData.eSearchResult = SR_COMPFOUND;
	m_stCompData.strCompURL = getWebPage()->mainFrame()->url().toString();
#endif
#ifdef USE_XML_PARSER
	m_stCompData.dPrice = Arest::dPrice;
	m_stCompData.eSearchResult = SR_COMPFOUND;
#endif
}
#ifdef USE_XML_PARSER
bool ArestShopPlugin::startElement( const QString &a, const QString & localName, const QString & qName, const QXmlAttributes & atts )
{
	QString strElementName = qName.toLower();
	if (m_ProductPageSelectors[0]==strElementName && m_ProductPageSelectors[2]==atts.value(m_ProductPageSelectors[1]))
	{
		m_bSingleProductPage=true;
		return true;
	}
	if (m_bSingleProductPage && m_PriceProductCardSelectors[0]==strElementName && m_PriceProductCardSelectors[2]==atts.value(m_PriceProductCardSelectors[1]))
	{
		Arest::bPriceCardRead=true;
		return true;
	}
	if (Arest::bPriceCardRead && m_PriceProductSelectors[0]==strElementName && m_PriceProductSelectors[2]==atts.value(m_PriceProductSelectors[1]))
	{
		Arest::bProductPriceTagOpen=true;
		return true;
	}
	if (m_SearchPageSelectors[0]==strElementName && m_SearchPageSelectors[2]==atts.value(m_SearchPageSelectors[1]))
	{
		m_bSearchProductPage=true;
		return true;
	}
	if (m_SearchPageProduktSelectors[0]==strElementName && m_SearchPageProduktSelectors[2]==atts.value(m_SearchPageProduktSelectors[1]))
	{
		Arest::bSearchPageProduktTagOpen=true;
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
	if (Arest::bSearchPageProduktTagOpen && "a"==strElementName )
	{
		Arest::strHrefLink=atts.value("href");
		return true;
	}
	return true;
}
bool ArestShopPlugin::endElement ( const QString & namespaceURI, const QString & localName, const QString & qName )
{
	if (Arest::bSearchPageProduktTagOpen && qName.toLower()=="a")
	{
		Arest::bSearchPageProduktTagOpen=false;
		Arest::mSearchResult[Arest::strTagText]=QUrl(Arest::strHrefLink);
	}
	return true;
}
bool ArestShopPlugin::characters ( const QString & ch )
{
	if (Arest::bProductPriceTagOpen)
	{
		Arest::bProductPriceTagOpen=false;
		//int iPos = ch.indexOf(" z");
		//if (iPos!=-1)
		{
			QString strPrice = ch/*.left(iPos)*/;
			Arest::dPrice = strPrice.toDouble();
			Arest::bProductPriceTagOpen=false;
			Arest::bPriceCardRead=false;
			return true;
		}
	}
	if (Arest::bSearchPageProduktTagOpen && !ch.isEmpty())
	{
		Arest::strTagText=ch;
		return true;
	}

	return true;
}
#endif
#ifndef WITHOUT_INTERFACE 
Q_EXPORT_PLUGIN2("ArestShopPlugin",ArestShopPlugin);
#endif
