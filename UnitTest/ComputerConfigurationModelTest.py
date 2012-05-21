import os
import sys
import unittest
import time
import threading
from PyQt4 import QtCore
from ConnectionBox import *

### testing Slot/Signal example
### http://www.commandprompt.com/community/pyqt/x5255.htm 
try:
	from Data.ComputerConfigurationModel import ComputerConfModel
except ImportError:	
	sys.path.append(os.path.join(os.path.dirname(__file__),".."))
	sys.path.append(os.path.dirname(__file__))
from Data.ComputerConfigurationModel import ComputerConfModel
from Data.ShopPluginsStubs import *

class ComputerConfigurationModelTest(unittest.TestCase):
    """Class for UnitTest of Model"""
    def setUp(self):
        self.Model = ComputerConfModel()
        self.connectionBox=ConnectionBox()

    def tearDown(self):
        del self.Model
        self.connectionBox=None
    
    def test_01_emptyModelRowColsCount(self):
        self.assertEqual(self.Model.rowCount(), 1)
        self.assertEqual(self.Model.columnCount(), 1)
    
    def test_02_ModelRowColsCount(self):
        self.assertEqual(self.Model.columnCount(), 1)
        self.assertEqual(self.Model.rowCount(), 1)
        pKomputronikShopPluginStub = KomputronikShopPluginStub
        self.assertEqual(self.Model.addShopPlugin(pKomputronikShopPluginStub), 1)
        self.assertEqual(self.Model.columnCount(), 2)
        self.assertEqual(self.Model.rowCount(), 1)
        pProlineShopPluginStub = ProlineShopPluginStub
        self.assertEqual(self.Model.addShopPlugin(pProlineShopPluginStub), 1)
        self.assertEqual(self.Model.columnCount(), 3)
        self.assertEqual(self.Model.rowCount(), 1)
    
    def test_03_ModelRowColsCleaningModel(self):
        """To test cleaning model after destruction """
        self.assertEqual(self.Model.rowCount(), 1)
        self.assertEqual(self.Model.columnCount(), 1)
        
    def test_04_ModelDataChangeEmited(self):
        self.assertEqual( QObject.connect(self.Model,  SIGNAL("dataChanged"),  self.connectionBox.slotSlot), 1)
        self.assertEqual(self.Model.columnCount(), 1)
        self.assertEqual(self.Model.rowCount(), 1)
        pKomputronikShopPluginStub = KomputronikShopPluginStub
        self.assertEqual(self.Model.addShopPlugin(pKomputronikShopPluginStub), 1)
        self.assertEqual(self.Model.columnCount(), 2)
        self.assertEqual(self.Model.rowCount(), 1)
        self.connectionBox.assertSignalArrived("dataChanged(QModelIndex,QModelIndex)")

#	#jak przetestowac czy opener zostal prawidlowo przygotowany?
#	def test_01_openerBuild(self):
#		prevOpener = urllib2._opener
#		self.assertEqual(prevOpener,None)
#		HTTPConnector.prepareHttpOpener()
#		self.assertNotEqual(prevOpener,urllib2._opener)
#	def test_02_GetSimpleWWWPage(self):
#		TEST_SITE="http://www.onet.pl"
#		self.assertEqual(urllib2._opener,None,'Test if there\'s no HTTP opener set')
#		HTTPConnector.prepareHttpOpener()
#		self.assertNotEqual(urllib2._opener,None,'Test if HTTP opener is set')
#		req = urllib2.Request(TEST_SITE)
#		self.assertNotEqual(req, None)
#		page = urllib2.urlopen(req)
#		self.assertNotEqual(page, None)
#		#f = open('testFile.html','w')
#		#f.write(page.read())
#		#.decode('cp1250')
#		#f.close()
