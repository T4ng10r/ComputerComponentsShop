#ifndef _CCS_ENUMERATIONS_INCLUDE_
#define _CCS_ENUMERATIONS_INCLUDE_

#include <QtCore/Qt>

//
enum CComputerConfModelRoles
{
	Role_RowActive = Qt::UserRole+1,	//
	Role_ComponentName,	//component name
	Role_ComponentNameInShop,	//component name
	Role_ComponentCode,	//component code
	Role_RowBestPrice,	//best price for given component
	Role_ShopURL,		//SHOP URL
	Role_ShopName,		//shop name
	Role_ShopNamesList,			//list of available shops names
	Role_ShopNamesIconsList,
	Role_CompShopSearchResult,	//result of component search
	Role_CompShopPrice,	//price of selected component in given shop
	Role_CompShopURL,	//URL to selected component in given shop
	Role_CompShopSearchURL,	//URL to selected component in given shop
	Role_ShopColumnActive,
	Role_CellIsRefreshing,
	Role_CellRefreshProgress,
	Role_CellIcon
};
#endif //_CCS_ENUMERATIONS_INCLUDE_
