#ifndef _SHOP_PLUGIN_BASE_MOCK_
#define _SHOP_PLUGIN_BASE_MOCK_
#include <ShopPluginBase.h>
#include "Mocks/LoadNetworkObjectMock.h"
#include <gmock/gmock.h>

class ShopPluginBaseTest;

class ShopPluginBaseInheriterMock : public ShopPluginBase
{
public:
	ShopPluginBaseInheriterMock();
	MOCK_METHOD0(prepareHTMLParserForNewSearch, void());
	MOCK_METHOD1(createSearchURL, QUrl(const QString & strName));
	MOCK_METHOD1(createProductURL, QUrl(const QString & strUrlPart));
	MOCK_METHOD2(parseSearchProductPage, void(SearchResult & stResult,bool & bNextPage));
	MOCK_METHOD0(parseProductPage,void());
	MOCK_METHOD0(produktPageTestSelectors, SelectorsList ());
	MOCK_METHOD0(produktPriceCardSelectors,SelectorsList ());
	MOCK_METHOD0(produktPriceSelectors, SelectorsList());
	MOCK_METHOD0(searchPageTestSelectors, SelectorsList());
	MOCK_METHOD0(searchPageProductsSelectors, SelectorsList());
	MOCK_METHOD0(notFoundPageSelectors, SelectorsList());
	MOCK_METHOD0(nextSearchPageSelectors, SelectorsList());
	MOCK_METHOD4(startElement, bool(const QString &, const QString &, const QString &, const QXmlAttributes & atts ));;
	MOCK_METHOD3(endElement, bool(const QString &, const QString &, const QString &));;
	MOCK_METHOD1(characters, bool(const QString &));;

	MOCK_CONST_METHOD0(pluginName, QString());
	MOCK_CONST_METHOD0(shopName, QString());
	MOCK_CONST_METHOD0(getShopIcon, QIcon());
	MOCK_CONST_METHOD0(getShopURL, QString());

	void call_onPageDownloadFinished(const QByteArray & stContent)
	{
		onPageDownloadFinished(stContent);
	};
public:
	LoadNetworkObjectMock			stLoadNetworkObjectMock;
protected:
	void testFunctionProt(){}

	friend class ShopPluginBaseTest;
};
#endif
