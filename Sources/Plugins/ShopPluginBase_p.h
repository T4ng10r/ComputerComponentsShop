#include <Network/LoadNetworkObject.h>
#ifdef USE_XML_PARSER
#include <QtXml/QXmlDefaultHandler>
#include <tidy.h>
#include <buffio.h>
#endif

#include <QtCore/QTimer>
#include <QtCore/QString>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <map>

enum EPageLoadingPhase
{
	PAGETYPE_NONE,
	PAGETYPE_ERROR,
	PAGETYPE_PRODUCT_OLD_URL,
	PAGETYPE_SEARCH,
	PAGETYPE_SEARCH_NEXT_PAGE,
	PAGETYPE_PRODUCT
};

class ShopPluginBase;
using boost::property_tree::ptree;

class ShopPluginBasePrivate : public QObject
{
public:
	ShopPluginBasePrivate(ShopPluginBase * prtPublic);
	//////////////////////////////////////////////////////////////////////////
	void processSearchPageLoad();
	void processProductPageLoad();
	void loadNextPage();
	QUrl findBestLink(SearchResult stResult);
	void performHTMLCleanup();
	const QString  makeLogString(const QString &strLogText);
public:
	ShopPluginBase *		m_ptrPublic;
	//class loading HTML pages
	LoadNetworkObject		m_stNetworkObjectLoader;
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
	ptree pt;
	//////////////////////////////////////////////////////////////////////////
#ifdef USE_XML_PARSER
	QXmlSimpleReader		m_stXMLReader;
	QXmlInputSource			m_stXMLInputSource;
#endif
};
