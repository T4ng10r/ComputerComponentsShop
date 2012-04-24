from PyQt4.QtCore.Qt import *

#class ModelUserRoles:
#    RowInd_Remove=,
#    RowInd_Active,
#    RowInd_CompName,
#    RowInd_CompCode,
#    RowInd_BestPrice,
#    RowInd_Coun
    
class ComputerConfModelRoles:
	Role_RowActive = Qt.UserRole+1
	Role_ComponentName = Qt.UserRole+2
	Role_ComponentCode = Qt.UserRole+3
	Role_RowBestPrice = Qt.UserRole+4
	Role_ShopURL = Qt.UserRole+5
	Role_ShopName = Qt.UserRole+6
	Role_ShopNamesList = Qt.UserRole+7
	Role_ShopNamesIconsList = Qt.UserRole+8
	Role_CompShopSearchResult = Qt.UserRole+9
	Role_CompShopPrice = Qt.UserRole+10
	Role_CompShopURL = Qt.UserRole+11
	Role_ShopColumnActive = Qt.UserRole+12
