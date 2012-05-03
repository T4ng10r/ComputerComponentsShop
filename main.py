from PyQt4.QtCore import *
from PyQt4.QtGui import *
from GUI.QFrozenColumnTableView import *
from GUI.ComponentHeaderItem import *
from ComputerConfigurationParser import ConfigurationXMLParser
import sys

app = QApplication(sys.argv)

parser = ConfigurationXMLParser("ComputerConfShop.xml")
parser.parseConfigurationFile()

model = QStandardItemModel (len(parser.listComps), len(parser.listShops)+1)
#for shop in parser.listShops:
    
view = QFrozenColumnTableView(model)
view.show();

sys.exit(app.exec_())
