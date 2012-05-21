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
    
    def test_02_AddingColumns(self):
        self.assertEqual(self.Model.columnCount(), 1)
        self.assertEqual(self.Model.rowCount(), 1)
        self.assertEqual( QObject.connect(self.Model,  SIGNAL("headerDataChanged"),  self.connectionBox.slotSlot), 1)
        self.Model.insertColumns(0, 1)
        self.connectionBox.assertSignalArrived("headerDataChanged()")
        self.assertEqual(self.connectionBox.args[0], Qt.Horizontal)
        self.assertEqual(self.connectionBox.args[1], 0)
        self.assertEqual(self.connectionBox.args[2], 1)
        self.assertEqual(self.Model.columnCount(), 2)
        self.assertEqual(self.Model.rowCount(), 1)
        del self.connectionBox
        self.connectionBox=ConnectionBox()        
        self.assertEqual( QObject.connect(self.Model,  SIGNAL("headerDataChanged"),  self.connectionBox.slotSlot), 1)
        self.Model.insertColumns(0, 1)
        self.connectionBox.assertSignalArrived("headerDataChanged()")
        self.assertEqual(self.connectionBox.args[0], Qt.Horizontal)
        self.assertEqual(self.connectionBox.args[1], 0)
        self.assertEqual(self.connectionBox.args[2], 2)
        self.assertEqual(self.Model.columnCount(), 3)
        self.assertEqual(self.Model.rowCount(), 1)
        del self.connectionBox
        self.connectionBox=ConnectionBox()        
        self.assertEqual( QObject.connect(self.Model,  SIGNAL("headerDataChanged"),  self.connectionBox.slotSlot), 1)
        self.Model.insertColumns(2, 5)
        self.connectionBox.assertSignalArrived("headerDataChanged()")
        self.assertEqual(self.connectionBox.args[0], Qt.Horizontal)
        self.assertEqual(self.connectionBox.args[1], 0)
        self.assertEqual(self.connectionBox.args[2], 7)
        self.assertEqual(self.Model.columnCount(), 8)
        self.assertEqual(self.Model.rowCount(), 1)
    
    def test_03_ModelRowColsCleaningModel(self):
        """To test cleaning model after destruction """
        self.assertEqual(self.Model.rowCount(), 1)
        self.assertEqual(self.Model.columnCount(), 1)
        
    def test_04_ModelDataChangeEmited(self):
        pass
