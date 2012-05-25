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
		########################################        
		self.assertTrue( QObject.connect(self.Model,  SIGNAL("headerDataChanged"),  self.connectionBox.slotSlot))
		self.assertTrue(self.Model.insertColumns(0, 1))
		self.connectionBox.assertSignalArrived("headerDataChanged()")
		self.assertEqual(self.connectionBox.args[0], Qt.Horizontal)
		self.assertEqual(self.connectionBox.args[1], 1) #first section which requires to be update
		self.assertEqual(self.connectionBox.args[2], 1) #first section which requires to be update
		self.assertEqual(self.Model.columnCount(), 2)
		self.assertEqual(self.Model.rowCount(), 1)
		self.assertEqual(len(self.Model.tPricesMatrix), 1)
		for iShopIndex in range(len(self.Model.tPricesMatrix)):
			self.assertEqual(len(self.Model.tPricesMatrix[iShopIndex]), 0,  "1.Testing %d column for row correctness" % (iShopIndex))
		del self.connectionBox
		########################################
		self.connectionBox=ConnectionBox()        
		self.assertTrue( QObject.connect(self.Model,  SIGNAL("headerDataChanged"),  self.connectionBox.slotSlot))
		self.assertTrue(self.Model.insertColumns(0, 1))
		self.connectionBox.assertSignalArrived("headerDataChanged()")
		self.assertEqual(self.connectionBox.args[0], Qt.Horizontal)
		self.assertEqual(self.connectionBox.args[1], 1)
		self.assertEqual(self.connectionBox.args[2], 2)
		self.assertEqual(self.Model.columnCount(), 3)
		self.assertEqual(self.Model.rowCount(), 1)
		self.assertEqual(len(self.Model.tPricesMatrix), 2)
		for iShopIndex in range(len(self.Model.tPricesMatrix)):
			self.assertEqual(len(self.Model.tPricesMatrix[iShopIndex]), 0,  "2.Testing %d column for row correctness" % (iShopIndex))
		del self.connectionBox
		########################################
		self.connectionBox=ConnectionBox()        
		self.assertTrue( QObject.connect(self.Model,  SIGNAL("headerDataChanged"),  self.connectionBox.slotSlot))
		self.assertTrue(self.Model.insertColumns(2, 5))
		self.connectionBox.assertSignalArrived("headerDataChanged()")
		self.assertEqual(self.connectionBox.args[0], Qt.Horizontal)
		self.assertEqual(self.connectionBox.args[1], 3)
		self.assertEqual(self.connectionBox.args[2], 7)
		self.assertEqual(self.Model.columnCount(), 8)
		self.assertEqual(self.Model.rowCount(), 1)
		self.assertEqual(len(self.Model.tPricesMatrix), 7)
		for iShopIndex in range(len(self.Model.tPricesMatrix)):
			self.assertEqual(len(self.Model.tPricesMatrix[iShopIndex]), 0,  "3.Testing %d column for row correctness" % (iShopIndex))

	def test_03_ModelRowColsCleaningModel(self):
		"""To test cleaning model after destruction """
		self.assertEqual(self.Model.rowCount(), 1)
		self.assertEqual(self.Model.columnCount(), 1)

	def test_04_RemovingColumns(self):
		self.assertEqual(self.Model.columnCount(), 1)
		self.assertEqual(self.Model.rowCount(), 1)
		self.assertTrue(self.Model.insertColumns(0, 8))
		self.assertEqual(self.Model.columnCount(), 9)
		self.assertEqual(self.Model.rowCount(), 1)
		self.assertEqual(len(self.Model.tPricesMatrix), 8)
		for iShopIndex in range(len(self.Model.tPricesMatrix)):
			self.assertEqual(len(self.Model.tPricesMatrix[iShopIndex]), 0,  "1.Testing %d column for row correctness" % (iShopIndex))
		del self.connectionBox
		########################################
		self.connectionBox=ConnectionBox()        
		self.assertTrue( QObject.connect(self.Model,  SIGNAL("headerDataChanged"),  self.connectionBox.slotSlot))
		self.assertTrue(self.Model.removeColumns(3, 1))        
		self.assertEqual(self.Model.columnCount(), 8)
		self.assertEqual(self.Model.rowCount(), 1)
		self.connectionBox.assertSignalArrived("headerDataChanged()")
		self.assertEqual(self.connectionBox.args[0], Qt.Horizontal)
		self.assertEqual(self.connectionBox.args[1], 4) #first section which requires to be update
		self.assertEqual(self.connectionBox.args[2], 7) #first section which requires to be update
		self.assertEqual(len(self.Model.tPricesMatrix), 7)
		for iShopIndex in range(len(self.Model.tPricesMatrix)):
			self.assertEqual(len(self.Model.tPricesMatrix[iShopIndex]), 0,  "2.Testing %d column for row correctness" % (iShopIndex))
		del self.connectionBox
		########################################
		self.connectionBox=ConnectionBox()        
		self.assertTrue( QObject.connect(self.Model,  SIGNAL("headerDataChanged"),  self.connectionBox.slotSlot))
		self.assertTrue(self.Model.removeColumns(2, 3))        
		self.assertEqual(self.Model.columnCount(), 5)
		self.assertEqual(self.Model.rowCount(), 1)
		self.connectionBox.assertSignalArrived("headerDataChanged()")
		self.assertEqual(self.connectionBox.args[0], Qt.Horizontal)
		self.assertEqual(self.connectionBox.args[1], 3) #first section which requires to be update
		self.assertEqual(self.connectionBox.args[2], 4) #first section which requires to be update
		self.assertEqual(len(self.Model.tPricesMatrix), 4)
		for iShopIndex in range(len(self.Model.tPricesMatrix)):
			self.assertEqual(len(self.Model.tPricesMatrix[iShopIndex]), 0,  "2.Testing %d column for row correctness" % (iShopIndex))

	def test_05_AddingRow(self):
		self.assertEqual(self.Model.columnCount(), 1)
		self.assertEqual(self.Model.rowCount(), 1)
		self.Model.insertColumns(0, 2)
		########################################        
		self.assertTrue( QObject.connect(self.Model,  SIGNAL("headerDataChanged"),  self.connectionBox.slotSlot))
		self.assertTrue(self.Model.insertRows(0, 1))
		self.connectionBox.assertSignalArrived("headerDataChanged()")
		self.assertEqual(self.connectionBox.args[0], Qt.Vertical)
		self.assertEqual(self.connectionBox.args[1], 1)
		self.assertEqual(self.connectionBox.args[2], 1)
		self.assertEqual(self.Model.columnCount(), 3)
		self.assertEqual(self.Model.rowCount(), 2)
		self.assertEqual(len(self.Model.tPricesMatrix), 2)
		for iShopIndex in range(len(self.Model.tPricesMatrix)):
			self.assertEqual(len(self.Model.tPricesMatrix[iShopIndex]), 1,  "1.Testing %d column for row correctness" % (iShopIndex))
		del self.connectionBox
		########################################        
		self.connectionBox=ConnectionBox()        
		self.assertTrue( QObject.connect(self.Model,  SIGNAL("headerDataChanged"),  self.connectionBox.slotSlot))
		self.assertTrue(self.Model.insertRows(1, 4))
		self.connectionBox.assertSignalArrived("headerDataChanged()")
		self.assertEqual(self.connectionBox.args[0], Qt.Vertical)
		self.assertEqual(self.connectionBox.args[1], 2)
		self.assertEqual(self.connectionBox.args[2], 5)
		self.assertEqual(self.Model.columnCount(), 3)
		self.assertEqual(self.Model.rowCount(), 6)
		self.assertEqual(len(self.Model.tPricesMatrix), 2)
		for iShopIndex in range(len(self.Model.tPricesMatrix)):
			self.assertEqual(len(self.Model.tPricesMatrix[iShopIndex]), 5,  "1.Testing %d column for row correctness" % (iShopIndex))
	def test_06_RemovingRows(self):
		self.assertEqual(self.Model.columnCount(), 1)
		self.assertEqual(self.Model.rowCount(), 1)
		self.assertTrue(self.Model.insertColumns(0, 4))
		self.assertTrue(self.Model.insertRows(0, 8))
		self.assertEqual(self.Model.columnCount(), 5)
		self.assertEqual(self.Model.rowCount(), 9)
		self.assertEqual(len(self.Model.tPricesMatrix), 4)
		for iShopIndex in range(len(self.Model.tPricesMatrix)):
			self.assertEqual(len(self.Model.tPricesMatrix[iShopIndex]), 8,  "1.Testing %d column for row correctness" % (iShopIndex))
		del self.connectionBox
		########################################
		self.connectionBox=ConnectionBox()        
		self.assertTrue( QObject.connect(self.Model,  SIGNAL("headerDataChanged"),  self.connectionBox.slotSlot))
		self.assertTrue(self.Model.removeRows(3, 2))        
		self.assertEqual(self.Model.columnCount(), 5)
		self.assertEqual(self.Model.rowCount(), 7)
		self.connectionBox.assertSignalArrived("headerDataChanged()")
		self.assertEqual(self.connectionBox.args[0], Qt.Vertical)
		self.assertEqual(self.connectionBox.args[1], 4) #first section which requires to be update
		self.assertEqual(self.connectionBox.args[2], 6) #last section which requires to be update
		self.assertEqual(len(self.Model.tPricesMatrix), 4)
		for iShopIndex in range(len(self.Model.tPricesMatrix)):
			self.assertEqual(len(self.Model.tPricesMatrix[iShopIndex]), 6,  "2.Testing %d column for row correctness" % (iShopIndex))
		del self.connectionBox
		########################################
		
