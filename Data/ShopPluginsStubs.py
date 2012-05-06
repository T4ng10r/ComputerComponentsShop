from Data.ShopPluginInterface import *

class ProlineShopPluginStub(ShopPluginInterface):
    def pluginName(self): return QString('ProlineShopPlugin')
    def shopName(self): return QString('Proline')
    def getShopIcon(self): return QIcon()
    def getShopUrl(self): return QString('http://www.proline.pl')

class KomputronikShopPluginStub(ShopPluginInterface):
    def pluginName(self): return QString('KomputronikShopPlugin')
    def shopName(self): return QString('Komputronik')
    def getShopIcon(self): return QIcon()
    def getShopUrl(self): return QString('http://www.komputronik.pl')

class ArestShopPluginStub(ShopPluginInterface):
    def __init__(self):
        pass
