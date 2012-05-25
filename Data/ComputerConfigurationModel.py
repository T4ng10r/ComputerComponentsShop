#pragma once
from PyQt4.QtCore import *
from GUI.ModelEnumerations import *
from Data.ShopPluginInterface import *
from Data.SearchResultType import *

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

class PriceCell:
	strURL = QString()
	dPrice = 0.0
	bActive = 0;
	bBestPrice = 0;
	bWorsePrice = 0;
	eSearchResult = SearchResultType.SR_NONE

class ComputerConfModel(QAbstractTableModel):
	COLUMN_FIX = -1
	lShopsEntries = []
	lComponentsEntries = []
	lShopPlugins = {}
	iLastCol = 0
	tPricesMatrix = [[]] # Shops x Components
	def __init__(self, parent = None):
		QAbstractTableModel.__init__(self, parent)
#        item = ComputerConfModelEntry
#        self.itemsTable= [item] * 3 #lista elementow 
#        self.availableShops = []
#        self.shopsSelectedForColumns = []
	def __del__(self):
		del self.lShopsEntries[:]
		del self.lComponentsEntries[:]
		self.lShopPlugins.clear()
		del self.tPricesMatrix[:]

	def index(self,  row, column, parent):
		if row<0 or row>=self.rowCount() or column<0 or column>=self.columnCount():
			return QModelIndex();
		return self.createIndex(row, column);

	def parent(self,  child):
		return QModelIndex();

	def rowCount(self, parent = QModelIndex()):
		return  len(self.lComponentsEntries)+1 # minimum

	def columnCount(self, parent = QModelIndex()):
		return len(self.lShopsEntries)+1 

	def recalculateCellsValues(self):
		pass

	def insertColumns(self,  iColumn, iCount, parent=None):
		if  iColumn < 0 or iColumn > self.columnCount():
			return 0
		if  iCount==1:
			self.beginInsertColumns(QModelIndex(), iColumn, iColumn);
		else:
			self.beginInsertColumns(QModelIndex(), iColumn, iColumn+iCount);        
		for iPos in range(iCount):
			self.lShopsEntries.insert(iColumn, ShopEntry)
		self.iLastCol = len(self.lShopsEntries)
#    #ifdef VIEW_WITH_FROZEN_COLUMN
		self.iLastCol = self.iLastCol + 1
#    #endif
		#self.recreatePricesMatrix()
		for iShopIndex in range(iColumn, iColumn+iCount):
			self.tPricesMatrix.insert(iShopIndex, [])
			self.tPricesMatrix[iShopIndex]=[PriceCell]*len(self.lComponentsEntries)
		self.endInsertColumns()
		self.recalculateCellsValues()
		self.emit(SIGNAL("headerDataChanged"),Qt.Horizontal, iColumn+1,  self.columnCount()-1)
		return 1

	def insertRows(self,  iRow, iCount, parent=None): 
		if iRow < 0 or iRow > self.rowCount():
			return 0;
		if iCount == 1:
			self.beginInsertRows(QModelIndex(), iRow, iRow);
		else:
			self.beginInsertRows(QModelIndex(), iRow, iRow+iCount);
		#compItem = ComponentEntry
		for iComp in range(iRow, iRow+iCount):
			self.lComponentsEntries.insert(iComp,  ComponentEntry)

		for iCol in range(len(self.tPricesMatrix)):
			for iRowIndex in range(iRow,  iRow+iCount):
				self.tPricesMatrix[iCol].insert(iRowIndex, PriceCell)
		#m_ptrPriv->m_iLastRow = m_ptrPriv->vComponentsList.size();
		#ifdef VIEW_WITH_FROZEN_COLUMN
		#++m_ptrPriv->m_iLastRow;
		#endif
		self.endInsertRows()
		self.recalculateCellsValues()
		self.emit(SIGNAL("headerDataChanged"),Qt.Vertical, iRow+1,  self.rowCount()-1)
		return 1

	def removeColumns (self,  iColumn, iCount, parent = None):
		if iCount<=0 or iColumn >= self.columnCount():
			return 0
		if (iColumn+iCount) > self.columnCount():
			iCount= self.columnCount()+self.COLUMN_FIX-iColumn
		self.beginRemoveColumns(QModelIndex(),iColumn,iColumn+iCount+self.COLUMN_FIX);
		iStartPos = iColumn
		iEndPos = iStartPos + iCount
		#//////////////////////////////////////////////////////////////////////////
		del self.lShopsEntries[iStartPos:iEndPos]
		##
		del self.tPricesMatrix[iColumn:iColumn+iCount]
		self.iLastCol = len(self.lShopsEntries)
		#ifdef VIEW_WITH_FROZEN_COLUMN
		self.iLastCol = self.iLastCol + 1
		#endif
		self.endRemoveColumns()
		self.recalculateCellsValues();
		self.emit(SIGNAL("headerDataChanged"),Qt.Horizontal, iColumn+1,  self.columnCount()-1)
		return 1;

	def removeRows(self,  iRow, iCount, parent = None):
		if iCount<=0 or iRow >= self.rowCount():
			return 0
		if (iRow+iCount) > self.rowCount():
			iCount = self.rowCount()+self.COLUMN_FIX-iRow
		self.beginRemoveRows(QModelIndex(),iRow,iRow+iCount+self.COLUMN_FIX);
		iStartPos = iRow
		iEndPos = iStartPos + iCount
		del self.lComponentsEntries[iStartPos:iEndPos]
		for iCol in range(len(self.tPricesMatrix)):
			del self.tPricesMatrix[iCol][iRow:iRow+iCount]

		self.endRemoveColumns()
		self.emit(SIGNAL("headerDataChanged"),Qt.Vertical, iRow+1,  self.rowCount()-1)
		return 1

	def addShopPlugin(self,  ptrPlugin):
		if ptrPlugin == None: 
			return 0
		strShopName = ptrPlugin.shopName
		self.lShopPlugins[strShopName]=ptrPlugin
		#m_ptrPriv->processHeaders();
		stTopLeft = self.index(0,0, None);
		stBottomRight = self.index(self.rowCount()-1,self.columnCount()-1, None);
		self.emit(SIGNAL("dataChanged"),stTopLeft,stBottomRight)
		return 1
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

