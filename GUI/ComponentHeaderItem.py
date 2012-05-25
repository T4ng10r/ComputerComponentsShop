from PyQt4.QtGui import *
from PyQt4.QtCore import *


class ComponentHeaderItem(QWidget):
	m_Model = None #QAbstractItemModel *
	m_ActiveCheck = None #QCheckBox
	m_RemoveEntry = None #QPushButton
	m_CompName = None #QLineEdit *
	m_rCompCode = None #QLineEdit *
	m_rBestPrice = None #QLabel *
	m_iSection = 0
	def __init__(self,  parent = None) :
		QWidget.__init__(self,  parent)
		pMainLayout = QHBoxLayout(self)
		pMainLayout.setMargin(0)
		self.m_Remove = QPushButton()
		self.m_Remove.setMaximumSize( 30, 24)
		pMainLayout.addWidget(self.m_Remove) 
		self.m_ActiveCheck = QCheckBox()
		pMainLayout.addWidget(self.m_ActiveCheck)
		self.m_CompName = QLineEdit()
		self.m_CompName.setMinimumWidth(250)
		pMainLayout.addWidget(self.m_CompName) 
		QObject.connect(self.m_Remove, SIGNAL("clicked ( bool)"), self.onRemoveClicked)
		QObject.connect(self.m_ActiveCheck, SIGNAL("clicked ( bool)"), self.onActiveClicked)
		QObject.connect(self.m_CompName, SIGNAL("editingFinished ()"), self.onComponentNameEntered)

	def setModel(self,  model):
		m_Model = model

	def onRemoveClicked(self,  bRemove):
		self.emit(PYSIGNAL("removeComponent"), (1,  ))
		pass

	def onActiveClicked(self,  bRemove):
		#self.m_Model.setHeaderData(self.m_iSection,Qt.Vertical,bVal,Role_RowActive);
		pass

	def onComponentNameEntered(self):
		self.m_CompName.text()
		#self.m_Model.setHeaderData(self.m_iSection,Qt.Vertical,bVal,Role_RowActive);
		pass

