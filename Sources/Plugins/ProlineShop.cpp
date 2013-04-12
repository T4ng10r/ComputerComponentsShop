#include "ProlineShop.h"
#include <QtCore/QFile>
#include <map>
#include <string>

const QString cstrHomePage("http://www.proline.pl");
const QString cstrShopSuffix("/?prodq=");
const QString cstrPricessTableRegExp("<table class=\"cennik\">.*</table>");
const QString cstrProlineSelectorsFile("ProlineShopSelectors.xml");

namespace Proline
{
	//flag that single line with product on search page is open
	bool			bSearchProduktTagOpen=false;
	bool			bPriceCardRead=false;
	QString			strTagText;
	QString			strHrefLink;
	SearchResult	mSearchResult;
	double			dPrice(0);
}

//namespace ShopPluginLoggers
//{
//	void createLoggers(const QString & strName);
//	void printErrorLog(const QString & strLog);
//	void printDebugLog(const QString & strLog);
//}
//////////////////////////////////////////////////////////////////////////
class ProlineShopPlugin::ProlineShopPluginPrivate
{
public:
	ProlineShopPluginPrivate();
};

ProlineShopPlugin::ProlineShopPluginPrivate::ProlineShopPluginPrivate()
{
}
//////////////////////////////////////////////////////////////////////////
ProlineShopPlugin::ProlineShopPlugin()
{
	createLoggers(shopName());
	loadSelectorsFromXML(cstrProlineSelectorsFile);
}
ProlineShopPlugin::~ProlineShopPlugin(){}
QString ProlineShopPlugin::pluginName() const
{
	return "ProlineShopPlugin";
}
QString ProlineShopPlugin::shopName() const
{
	return QString("Proline");
}
QIcon ProlineShopPlugin::getShopIcon() const
{
	return QIcon(":/images/proline.png");
}
QString ProlineShopPlugin::getShopURL() const
{	
	return cstrHomePage;
}
void ProlineShopPlugin::prepareHTMLParserForNewSearch()
{
	Proline::bSearchProduktTagOpen=false;
	Proline::bPriceCardRead=false;
	Proline::strTagText="";
	Proline::strHrefLink="";
	Proline::mSearchResult.clear();
	Proline::dPrice=0;
}
SelectorsList ProlineShopPlugin::produktPageTestSelectors()
{
	SelectorsList stSelectorsList;
	//a[name="topprodid"]
	stSelectorsList.push_back("a");
	stSelectorsList.push_back("name");
	stSelectorsList.push_back("topprodid");
	return stSelectorsList;
};
SelectorsList ProlineShopPlugin::produktPriceCardSelectors()
{
	SelectorsList stSelectorsList;
	//div[class="cena_karta"]
	stSelectorsList.push_back("div");
	stSelectorsList.push_back("class");
	stSelectorsList.push_back("cena_karta");
	return stSelectorsList;
}
SelectorsList ProlineShopPlugin::produktPriceSelectors()
{
	SelectorsList stSelectorsList;
	//b[class="cena_b"]
	stSelectorsList.push_back("b");
	stSelectorsList.push_back("class");
	stSelectorsList.push_back("cena_b");
	return stSelectorsList;
}
SelectorsList ProlineShopPlugin::searchPageTestSelectors()
{
	SelectorsList stSelectorsList;
	//a[name="cennik"]
	stSelectorsList.push_back("a");
	stSelectorsList.push_back("name");
	stSelectorsList.push_back("cennik");
	return stSelectorsList;
};
SelectorsList ProlineShopPlugin::searchPageProductsSelectors()
{
	SelectorsList stSelectorsList;
	//a[class="produkt"]
	stSelectorsList.push_back("a");
	stSelectorsList.push_back("class");
	stSelectorsList.push_back("produkt");
	return stSelectorsList;
}
SelectorsList ProlineShopPlugin::notFoundPageSelectors()
{
	SelectorsList stSelectorsList;
	//a[class="produkt"]
	stSelectorsList.push_back("div");
	stSelectorsList.push_back("class");
	stSelectorsList.push_back("brak-wynikow");
	return stSelectorsList;
}
SelectorsList ProlineShopPlugin::nextSearchPageSelectors()
{
	SelectorsList stSelectorsList;
	//a[class="produkt"]
	stSelectorsList.push_back("");
	stSelectorsList.push_back("");
	stSelectorsList.push_back("");
	return stSelectorsList;
}
SelectorsList ProlineShopPlugin::productImageSelectors()
{
	SelectorsList stSelectorsList;
	//a[class="produkt"]
	stSelectorsList.push_back("img");
	stSelectorsList.push_back("name");
	stSelectorsList.push_back("foto_pic");
	return stSelectorsList;
}
//////////////////////////////////////////////////////////////////////////
void ProlineShopPlugin::NotFoundPage()
{}
QUrl ProlineShopPlugin::createSearchURL(const QString & strName)
{
	QString strUrl(cstrHomePage+cstrShopSuffix);
	QString strCompName = strName.isEmpty()?m_strComponentName:strName;
	strCompName.replace(QString(" "),QString("+"));
	return strUrl+strCompName;
}
QUrl ProlineShopPlugin::createProductURL(const QString & strUrlPart)
{
	QString strUrl(cstrHomePage+strUrlPart) ;
	return strUrl;
}
void ProlineShopPlugin::parseSearchProductPage(SearchResult & stResult,bool & bNextPage)
{
	stResult.insert(Proline::mSearchResult.begin(),Proline::mSearchResult.end());
	bNextPage=false;
}
void ProlineShopPlugin::parseProductPage()
{
	m_stCompData.dPrice = Proline::dPrice;
	m_stCompData.eSearchResult = SR_COMPFOUND;
	m_stCompData.strName = m_strComponentName;
	printLog(eDebugLogLevel, eDebug,"Product found.");
	//////////////////////////////////////////////////////////////////////////
	//stCompData.strCompURL = getWebPage()->mainFrame()->url().toString();
	//stResult.insert(mSearchResult.begin(),mSearchResult.end());
}
bool ProlineShopPlugin::startElement( const QString &a, const QString & localName, const QString & qName, const QXmlAttributes & atts )
{
	QString strElementName = qName.toLower();
	if (m_ProductPageSelectors[0]==strElementName && m_ProductPageSelectors[2]==atts.value(m_ProductPageSelectors[1]))
	{
		m_bSingleProductPage=true;
		return true;
	}
	if (m_PriceProductCardSelectors[0]==strElementName && m_PriceProductCardSelectors[2]==atts.value(m_PriceProductCardSelectors[1]))
	{
		Proline::bPriceCardRead=true;
		return true;
	}
	if (Proline::bPriceCardRead && m_PriceProductSelectors[0]==strElementName && m_PriceProductSelectors[2]==atts.value(m_PriceProductSelectors[1]))
	{
		Proline::dPrice = atts.value("content").toDouble();
		Proline::bPriceCardRead=false;
		return true;
	}
	if (m_SearchPageSelectors[0]==strElementName && m_SearchPageSelectors[2]==atts.value(m_SearchPageSelectors[1]))
	{
		m_bSearchProductPage=true;
		return true;
	}
	if (m_SearchPageProduktSelectors[0]==strElementName && m_SearchPageProduktSelectors[2]==atts.value(m_SearchPageProduktSelectors[1]))
	{
		Proline::bSearchProduktTagOpen=true;
		Proline::strHrefLink = atts.value("href");
		return true;
	}
	if (m_ProductImageSelectors[0]==strElementName && m_ProductImageSelectors[2]==atts.value(m_ProductImageSelectors[1]))
	{
		QString strImageURL = atts.value("src");
		return true;
	}
	if (m_NotFoundPageSelectors[0]==strElementName && m_NotFoundPageSelectors[2]==atts.value(m_NotFoundPageSelectors[1]))
	{
		m_bNoResultPage=true;
		return true;
	}
	return true;
}
bool ProlineShopPlugin::endElement ( const QString & namespaceURI, const QString & localName, const QString & qName )
{
	if (qName.toLower()=="span" && Proline::bSearchProduktTagOpen)
	{
		Proline::bSearchProduktTagOpen=false;
		QUrl strHrefUrl = createProductURL(Proline::strHrefLink);
		Proline::mSearchResult.insert(std::make_pair(Proline::strTagText,strHrefUrl));
	}

	return true;
}
bool ProlineShopPlugin::characters ( const QString & ch )
{
	if (Proline::bSearchProduktTagOpen)
	{
		Proline::strTagText = ch;
	}

	return true;
}
#ifndef WITHOUT_INTERFACE 
Q_EXPORT_PLUGIN2("ProlineShopPlugin",ProlineShopPlugin);
#endif
