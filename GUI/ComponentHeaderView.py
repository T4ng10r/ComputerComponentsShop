from PyQt4.QtGui import *
from PyQt4.QtCore import *



class ComponentHeaderView(QHeaderView):
	def __init__(self,  parent=None):
		QHeaderView.__init__(self, parent)
	def setModel(self,  model):
		pass
	def  removeComponent(self,  iSection):
		pass
	def headerDataChanged(self,  orientation, logicalFirst, logicalLast):
		pass
	def onSectionCouintChanged(self,  int1,  int2):
		pass
	def onSectionResized(self,  int1,  int2,  int3):
		pass