
#ifndef _CCS_KOMPUTRONIK_SHOP_INCLUDE_
#define _CCS_KOMPUTRONIK_SHOP_INCLUDE_
#include <ShopInterface.h>
#include <ShopPluginBase.h>
#include <QtCore/QtPlugin>

class KomputronikShopPluginPrivate;

class KomputronikShopPlugin : /*public ShopInterface,*/ public ShopPluginBase
{
	Q_OBJECT
public:
	KomputronikShopPlugin();
	virtual ~KomputronikShopPlugin();
	virtual QString		shopName() const;
	virtual QIcon		getShopIcon() const;
	virtual QString		getShopURL() const;
	virtual QString		pluginName() const;
	virtual QUrl		createSearchURL(const QString & strName = QString());
	virtual QUrl		createProductURL(const QString & strUrlPart);
	virtual QUrl		createSearchNextPageURL(unsigned int uPageNr);
protected:
#ifndef USE_XML_PARSER
	virtual bool isNotFoundPage();
	virtual bool isSingleProductPage();
	virtual bool isSearchProductPage();
#endif
	virtual void parseSearchProductPage(SearchResult & stResult,bool & bNextPage);
	virtual void parseProductPage();
	virtual void prepareHTMLParserForNewSearch();
	virtual SelectorsList produktPageTestSelectors();
	virtual SelectorsList searchPageTestSelectors();
	virtual SelectorsList produktPriceCardSelectors();
	virtual SelectorsList produktPriceSelectors();
	virtual SelectorsList searchPageProductsSelectors();
	virtual SelectorsList notFoundPageSelectors();
	virtual SelectorsList nextSearchPageSelectors();

#ifdef USE_XML_PARSER
	//XML parsing functions
	virtual bool startElement( const QString &a, const QString & localName, const QString & qName, const QXmlAttributes & atts );
	virtual bool endElement ( const QString & namespaceURI, const QString & localName, const QString & qName );
	virtual bool characters ( const QString & ch );
#endif
};
#endif //_CCS_KOMPUTRONIK_SHOP_INCLUDE_
