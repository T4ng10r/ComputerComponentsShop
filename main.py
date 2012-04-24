from PyQt4.QtCore import *
from PyQt4.QtGui import *
from QFrozenColumnTableView import *
from ComponentHeaderItem import *
import sys

app = QApplication(sys.argv)
model = QStandardItemModel (8, 3)
view = QFrozenColumnTableView(model)
view.show();

a = ComponentHeaderItem()
a.show()
sys.exit(app.exec_())
