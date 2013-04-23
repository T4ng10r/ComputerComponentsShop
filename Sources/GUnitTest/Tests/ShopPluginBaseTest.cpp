#include "Tests/ShopPluginBaseTest.h"
#include "ShopPluginBase_p.h"
#include <boost/bind.hpp>
using ::testing::Return;
using ::testing::AtLeast; 
using ::testing::_; 
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;

ShopPluginBaseTest::ShopPluginBaseTest(){}
void ShopPluginBaseTest::SetUp()
{
	prepareSelectorsMocks();
	// 
	ON_CALL(stShopPluginBase, prepareHTMLParserForNewSearch())
		.WillByDefault(Return());
	ON_CALL(stShopPluginBase, createSearchURL(_))
		.WillByDefault(Return(QUrl("url")));
	ON_CALL(stShopPluginBase, startElement(_,_,_,_))
		.WillByDefault(Return(true));
	ON_CALL(stShopPluginBase, endElement(_,_,_))
		.WillByDefault(Return(true));
	ON_CALL(stShopPluginBase, characters(_))
		.WillByDefault(Return(true));
	//reaction to calling LoadNetworkObjectMoc
}
LoadNetworkObjectMock* ShopPluginBaseTest::getShopPluginBaseMock()
{
	return (LoadNetworkObjectMock*)stShopPluginBase.m_ptrPriv->m_ptrNetworkObjectLoader.get();
}

void ShopPluginBaseTest::prepareSelectorsMocks()
{
	QString strSelTestPage []={"a","class","test"};
	ON_CALL(stShopPluginBase, produktPageTestSelectors())
		.WillByDefault(Return(SelectorsList(strSelTestPage,strSelTestPage+sizeof(strSelTestPage) / sizeof(QString))));
	QString strSelPriceCard []={"a","class","test"};
	ON_CALL(stShopPluginBase, produktPriceCardSelectors())
		.WillByDefault(Return(SelectorsList(strSelPriceCard,strSelPriceCard+sizeof(strSelPriceCard) / sizeof(QString))));
	QString strSelPrice []={"a","class","test"};
	ON_CALL(stShopPluginBase, produktPriceSelectors())
		.WillByDefault(Return(SelectorsList(strSelPrice,strSelPrice+sizeof(strSelPrice) / sizeof(QString))));
	QString strSelSearchPage []={"a","class","test"};
	ON_CALL(stShopPluginBase, searchPageTestSelectors())
		.WillByDefault(Return(SelectorsList(strSelSearchPage,strSelSearchPage+sizeof(strSelSearchPage) / sizeof(QString))));
	QString strSelSearchPageProduct []={"a","class","test"};
	ON_CALL(stShopPluginBase, searchPageProductsSelectors())
		.WillByDefault(Return(SelectorsList(strSelSearchPageProduct,strSelSearchPageProduct+sizeof(strSelSearchPageProduct) / sizeof(QString))));
	QString strSelNotFoundPage []={"a","class","test"};
	ON_CALL(stShopPluginBase, notFoundPageSelectors())
		.WillByDefault(Return(SelectorsList(strSelNotFoundPage,strSelNotFoundPage+sizeof(strSelNotFoundPage) / sizeof(QString))));
	QString strSelNextPage []={"a","class","test"};
	ON_CALL(stShopPluginBase, nextSearchPageSelectors())
		.WillByDefault(Return(SelectorsList(strSelNextPage,strSelNextPage+sizeof(strSelNextPage) / sizeof(QString))));
}

TEST_F(ShopPluginBaseTest, General_test_1)
{
	EXPECT_CALL(stShopPluginBase, produktPageTestSelectors()).Times(AtLeast(1));
	EXPECT_CALL(stShopPluginBase, produktPriceCardSelectors()).Times(AtLeast(1));
	EXPECT_CALL(stShopPluginBase, produktPriceSelectors()).Times(AtLeast(1));
	EXPECT_CALL(stShopPluginBase, searchPageTestSelectors()).Times(AtLeast(1));
	EXPECT_CALL(stShopPluginBase, searchPageProductsSelectors()).Times(AtLeast(1));
	EXPECT_CALL(stShopPluginBase, notFoundPageSelectors()).Times(AtLeast(1));
	EXPECT_CALL(stShopPluginBase, nextSearchPageSelectors()).Times(AtLeast(1));
	EXPECT_CALL(stShopPluginBase, prepareHTMLParserForNewSearch()).Times(AtLeast(1));
	EXPECT_CALL(stShopPluginBase, createSearchURL(_)).Times(AtLeast(1));

	EXPECT_CALL(stShopPluginBase, startElement(_,_,_,_)).Times(AtLeast(1));
	EXPECT_CALL(stShopPluginBase, endElement(_,_,_)).Times(AtLeast(1));
	EXPECT_CALL(stShopPluginBase, characters(_)).Times(AtLeast(1));


	//EXPECT_CALL(stShopPluginBase, parseSearchProductPage(_,_)).Times(AtLeast(1));


	QByteArray stContent("<title>AMD X4 FX-4170 s.AM3+ BOX - ProLine.pl Najlepszy Internetowy Sklep Komputerowy Wroc³aw</title>");
	ON_CALL(*getShopPluginBaseMock(),loadNetworkObject(_))
		.WillByDefault(Invoke(boost::bind(&ShopPluginBaseInheriterMock::call_onPageDownloadFinished, &stShopPluginBase, stContent)));
		//.WillByDefault(InvokeWithoutArgs(&stShopPluginBase,&ShopPluginBaseInheriterMock::parseProductPage)); 
	stShopPluginBase.getProductData("TestComponent");
} 