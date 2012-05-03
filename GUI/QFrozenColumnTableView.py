from PyQt4.QtGui import *
from PyQt4.QtCore import *

class QFrozenColumnTableView(QTableView):
    frozenColumnTableView = None
    frozenRowTableView = None
    def __init__(self,  model):
        QTableView.__init__(self,  None)
        self.frozenColumnTableView = QTableView(self)
        self.frozenRowTableView = QTableView(self)
        self.setModel(model)
        #self.init()
        QObject.connect(self.horizontalHeader(),SIGNAL("sectionResized(int,int,int)"), self.updateSectionWidth)
        QObject.connect(self.verticalHeader(),SIGNAL("sectionResized(int,int,int)"), self.updateSectionHeight)

        QObject.connect(self.frozenColumnTableView.verticalScrollBar(), SIGNAL("valueChanged(int)"),
                self.verticalScrollBar(), SLOT("setValue(int)"))
        QObject.connect(self.verticalScrollBar(), SIGNAL("valueChanged(int)"),
                self.frozenColumnTableView.verticalScrollBar(), SLOT("setValue(int)"))
        QObject.connect(self.frozenRowTableView.verticalScrollBar(), SIGNAL("valueChanged(int)"),
                self.verticalScrollBar(), SLOT("setValue(int)"))
        QObject.connect(self.verticalScrollBar(), SIGNAL("valueChanged(int)"),
                self.frozenRowTableView.verticalScrollBar(), SLOT("setValue(int)"))
                
    def resizeEvent(self,  event):
        QTableView.resizeEvent(self, event)
        self.updateFrozenTableGeometry()
    def moveCursor(self,  cursorAction, modifiers):
        current = QTableView.moveCursor(self,  cursorAction, modifiers)
        if cursorAction == QAbstractItemView.MoveLeft and current.column()>0 and visualRect(current).topLeft().x() < self.frozenColumnTableView.columnWidth(0):
            newValue = self.horizontalScrollBar().value() + self.visualRect(current).topLeft().x() - self.frozenColumnTableView.columnWidth(0)
            self.horizontalScrollBar().setValue(newValue);
        return current;
    def scrollTo (self,  index, hint = QTableView.EnsureVisible):
        if index.column()>0:
            QTableView.scrollTo(self, index, hint);
    def setModel(self, model):
        QTableView.setModel(self,  model)
        if self.frozenColumnTableView <> None:
            self.frozenColumnTableView.setModel(self.model())
        if self.frozenColumnTableView <> None:
            self.frozenRowTableView.setModel(self.model())
        self.init();

    def init(self):
        self.frozenColumnTableView.setModel(self.model())
        self.frozenColumnTableView.setFocusPolicy(Qt.NoFocus)
        self.frozenColumnTableView.verticalHeader().hide()
        self.frozenColumnTableView.horizontalHeader().setResizeMode(QHeaderView.Fixed)

        self.frozenRowTableView.setModel(self.model())
        self.frozenRowTableView.setFocusPolicy(Qt.NoFocus)
        self.frozenRowTableView.verticalHeader().setResizeMode(QHeaderView.Fixed)
        self.frozenRowTableView.horizontalHeader().hide()

        self.viewport().stackUnder(self.frozenRowTableView);
        self.viewport().stackUnder(self.frozenColumnTableView);

        self.frozenColumnTableView.setStyleSheet("QTableView { border: none;"
                                      "background-color: #8EDE21;"
                                      "selection-background-color: #999}") 
        self.frozenRowTableView.setStyleSheet("QTableView { border: none;"
                                      "background-color: #8EDE21;"
                                      "selection-background-color: #999}") 
        selectModel =  self.selectionModel()
        self.frozenColumnTableView.setSelectionModel(selectModel)
        self.frozenRowTableView.setSelectionModel(selectModel)
        for col in range(1, self.model().columnCount()):
            self.frozenColumnTableView.setColumnHidden(col, 1)
        for row in range(1, self.model().rowCount()):
            self.frozenRowTableView.setRowHidden(row, 1)

        self.frozenColumnTableView.setColumnWidth(0, self.columnWidth(0) )
        self.frozenColumnTableView.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff);
        self.frozenColumnTableView.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff);
        self.frozenColumnTableView.show();
        self.frozenRowTableView.setRowHeight(0, self.rowHeight(0) )
        self.frozenRowTableView.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff);
        self.frozenRowTableView.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff);
        self.frozenRowTableView.show();

        self.updateFrozenTableGeometry();

        self.setHorizontalScrollMode(QAbstractItemView.ScrollPerPixel);
        self.setVerticalScrollMode(QAbstractItemView.ScrollPerPixel);
        self.frozenColumnTableView.setVerticalScrollMode(QAbstractItemView.ScrollPerPixel);
        self.frozenRowTableView.setHorizontalScrollMode(QAbstractItemView.ScrollPerPixel);

    def updateFrozenTableGeometry(self):
        self.frozenColumnTableView.setGeometry( self.verticalHeader().width()+self.frameWidth(),
             self.frameWidth(), self.columnWidth(0),
             self.viewport().height()+self.horizontalHeader().height())
        self.frozenRowTableView.setGeometry( self.frameWidth(), self.horizontalHeader().height() + self.frameWidth(),  
             self.viewport().width()+self.horizontalHeader().width(),
             self.rowHeight(0));
             
    def updateSectionWidth(self,  logicalIndex,a, newSize):
        if logicalIndex==0:
            self.frozenColumnTableView.setColumnWidth(0,newSize);
        self.frozenRowTableView.setColumnWidth(logicalIndex, newSize);
        self.updateFrozenTableGeometry();
    def updateSectionHeight(self,  logicalIndex, a, newSize):
        if logicalIndex==0:
            self.frozenRowTableView.setRowHeight(0,newSize);
        self.frozenColumnTableView.setRowHeight(logicalIndex, newSize);
        self.updateFrozenTableGeometry();
 
