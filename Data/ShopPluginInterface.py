import sys
from PyQt4.QtCore import QString,  QThread
from PyQt4.QtGui import QIcon

class ShopPluginInterface:
    def __init__(self): pass 
    def pluginName(self): return QString()
    def shopName(self): return QString()
    def getShopIcon(self): return QIcon()
    def getShopUrl(self): return QString()
    def getProductData(self,  sProductName,  sOldUrl = QString): pass
    #void priceSearchedFinished(CompPriceData stData);
	#void priceSearchProgress(QString ,int );	//component name, component progress
    
#Q_DECLARE_INTERFACE(ShopInterface, "ShopInterface/1.0");
