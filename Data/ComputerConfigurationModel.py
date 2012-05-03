#pragma once
from PyQt4.QtCore import *
from GUI.ModelEnumerations import *

class ComponentEntry:
    """Class contains data about single row"""
    def __init__(self):
        self.bActive=0
        self.strName=QString
        self.strCode=QString
        self.dBestPrice=0.0

class ShopEntry:
    """Class contains data about single column"""
    iRowActive=True
    def __init__(self):
        self.bActive=0
        self.strName=QString
        self.strShopURL=QString
        self.strAvailableShopsName=[]
        self.lShopIconsList=QVariantList
        self.dComponentPriceSum=0.0;


class ComputerConfModel(QAbstractTableModel):
    lShopsEntries = {}
    lComponentsEntries = {}
    def __init__(self, parent = None):
        QAbstractTableModel.__init__(self, parent)
#        item = ComputerConfModelEntry
#        self.itemsTable= [item] * 3 #lista elementow 
#        self.availableShops = []
#        self.shopsSelectedForColumns = []
    def index(self,  row, column, parent):
        if row<0 or row>=rowCount() or column<0 or column>=columnCount():
            return QModelIndex();
        return self.createIndex(row, column);
    def parent(self,  child):
        return QModelIndex();
    def rowCount(self, parent = QModelIndex()):
        return  len(self.lShopsEntries)+1 # minimum
    def columnCount(self, parent = QModelIndex()):
        return len(self.lComponentsEntries)+1 
    def addShop(self,  ptrPlugin):
        
        pass
void CComputerConfModel::addShopPlugin(ShopInterface * ptrPlugin)
{
	if (!ptrPlugin)	return;
	QString strShopName = ptrPlugin->shopName();
	m_ptrPriv->mPlugins.insert(std::make_pair(strShopName,ptrPlugin));
	m_ptrPriv->processHeaders();
	QModelIndex stTopLeft = index(0,0);
	QModelIndex stBottomRight = index(rowCount()-1,columnCount()-1);
	emit dataChanged(stTopLeft,stBottomRight);
}        
#    def data(self, index, role = Qt.DisplayRole):
#        iRow = index.row()
#        iColumn = index.column()
#        if iRow<0 or iRow>self.iRowCount-1:
#            return QVariant
#        if iColumn<0 or iColumn>self.iColumnCount-1:
#            return QVariant
#        item = self.itemsTable[iRow]
#        if iRow == ModelRowEnum.ROW_SHOP_NAME:
#            if role == Qt.DisplayRole:
#                return item
#            pass
#        elif iRow == ModelRowEnum.ROW_SHOP_ACTIVE:
#            pass
#        else:
#            if index.column()==ModelColumnEnum.ITEM_ACTIVE:
#                return item.iRowActive
#            elif index.column()==ModelColumnEnum.ITEM_NAME:
#                return item.sItemName
#            elif index.column()==ModelColumnEnum.ITEM_CODE:
#                return item.sItemCode
#            elif index.column()>=ModelColumnEnum.ITEM_COUNT:
#                return item.shops[index.column()-2]
#        return QVariant
#
#    def setData(self, index, value, role = Qt.DisplayRole):
#        return True

#	~CRaceStatsModel(void);
#	void onRaceChanged(const QString & strRaceName);
#	void onFractionChanged(const QString & strFractionName);
#	void onXWarsValuesLoaded();


