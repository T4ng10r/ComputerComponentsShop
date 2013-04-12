#include <ShopPluginBase.cpp>
#include <gmock/gmock.h>

class ShopPluginBaseInheriterMock : public ShopPluginBase
{
	friend class ShopPluginBaseTest;
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

	MOCK_CONST_METHOD0(pluginName, QString());
	MOCK_CONST_METHOD0(shopName, QString());
	MOCK_CONST_METHOD0(getShopIcon, QIcon());
	MOCK_CONST_METHOD0(getShopURL, QString());
public:
	LoadNetworkObjectMock			stLoadNetworkObjectMock;
};

ShopPluginBaseInheriterMock::ShopPluginBaseInheriterMock()
{
	m_ptrPriv->m_stNetworkObjectLoader = stLoadNetworkObjectMock;
}
