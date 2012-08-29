#include <Data/CDataThread.h>
#include <Data/ComputerConfModel.h>
#include <GUI/MainWindow.h>
#include <GUI/ComputerConfView.h>
#include <GUI/ProxySettingsGUI.h>
#include <GUI/FrozenTableView.h>
#include <QtGui/QStatusBar>
#include <QtGui/QMenuBar>
#include <QtCore/QFileInfo>

const QSize ciSize(700,450);

void printGUICreationLog(const QString & strLog);
void printSlotsConnectionLog(const QString & strLog);

MainWindow::MainWindow() : QMainWindow(), m_eRefreshingState(E_RS_READY_TO_REFRESH)
{
	setupUI();
	setupActions();
	CDataThread::getInstance()->start();
	setConnections();
	CDataThread::getInstance()->onUpdateRecentFileActions();
	m_ptrProxySettings = new ProxySettingsGUI;
	m_ptrProxySettings->hide();

	m_ptCompConfView->setModel(CDataThread::getInstance()->getConfigurationModel());
	m_ptCompConfView->setMinimumWidth(ciSize.width());
	m_ptCompConfView->setMinimumHeight(ciSize.height());
	statusBar()->showMessage("Ready");
}
MainWindow::~MainWindow()
{
	m_DataThreadTerminatedMutex.lock();
	delete m_ptrProxySettings;
	m_DataThreadTerminatedMutex.unlock();
	delete m_ptrStatusBarSignalMapper;
}
void MainWindow::closeEvent(QCloseEvent *)
{
	m_DataThreadTerminatedMutex.lock();
	bool bResult = connect(CDataThread::getInstance(), SIGNAL(terminated()) ,
		this, SLOT(onWaitingForDataThreadTermination()));
	bResult = connect(CDataThread::getInstance(), SIGNAL(finished()) ,
		this, SLOT(onWaitingForDataThreadTermination()));
	ptrDataThread = CDataThread::getInstance();
	CDataThread::getInstance()->quit();
}
void MainWindow::onWaitingForDataThreadTermination()
{
	m_DataThreadTerminatedMutex.unlock();
	static_cast<CDataThread*>(ptrDataThread)->deleteLater();
}
void MainWindow::setupUI()
{
	m_ptCentralWidget = new QWidget;
	setCentralWidget(m_ptCentralWidget);

	m_ptVLayout = new QVBoxLayout;
	delete m_ptCentralWidget->layout();
	m_ptCentralWidget->setLayout(m_ptVLayout);

	//m_ptToolBar = new QToolBar(this);
	//m_ptToolBar->setObjectName("m_ptToolBar");
	//m_ptToolBar->setMaximumHeight(25);

	//m_ptVLayout->addWidget(m_ptToolBar);
	//////////////////////////////////
	m_ptCompConfView = new FrozenTableView(this);
	m_ptCompConfView->setObjectName("CompConfView");

	m_ptVLayout->addWidget(m_ptCompConfView);
	//////////////////////////////////////////////////////////////////////////
	m_ptHLayout = new QHBoxLayout;

	m_ptrAddComp = new QPushButton(this);
	m_ptrAddComp->setIcon(QIcon(":/images/plus.png"));
	m_ptrAddComp->setText("Item");

	m_ptrAddShop = new QPushButton(this);
	m_ptrAddShop->setIcon(QIcon(":/images/plus.png"));
	m_ptrAddShop->setText("Shop");

	m_ptHLayout->addStretch(10);
	m_ptHLayout->addWidget(m_ptrAddComp);
	m_ptHLayout->addWidget(m_ptrAddShop);
	//////////////////////////////////////////////////////////////////////////
	m_ptVLayout->addLayout(m_ptHLayout);

	statusBar()->show();
}
void MainWindow::logConnection(QString strConnDesc,bool bResult)
{
	QString strDebug = "Connection "+strConnDesc;
	if (bResult)	strDebug+=" SUCCESS";	else	strDebug+=" FAIL";
	printSlotsConnectionLog(strDebug);
	Q_ASSERT_X(bResult==true,"MainWindow::setConnections",QString("Connect %1").arg(strConnDesc).toAscii());
}
void MainWindow::setConnections()
{
	bool bResult = false;
	m_ptrStatusBarSignalMapper = new QSignalMapper;
	m_ptrStatusBarSignalMapper->setMapping(m_actionNewConf,QString("New configuration"));
	m_ptrStatusBarSignalMapper->setMapping(m_actionOpenConf,QString("Opening configuration from file"));
	m_ptrStatusBarSignalMapper->setMapping(m_actionSaveConf,QString("Saving configuration to file"));
	m_ptrStatusBarSignalMapper->setMapping(m_actionRefreshConf,QString("Refreshing configuration"));
	bResult = connect(m_actionNewConf, SIGNAL(triggered(bool)) , m_ptrStatusBarSignalMapper, SLOT(map()));
	bResult = connect(m_actionOpenConf, SIGNAL(triggered(bool)) , m_ptrStatusBarSignalMapper, SLOT(map()));
	bResult = connect(m_actionSaveConf, SIGNAL(triggered(bool)) , m_ptrStatusBarSignalMapper, SLOT(map()));
	bResult = connect(m_actionRefreshConf, SIGNAL(triggered(bool)) , m_ptrStatusBarSignalMapper, SLOT(map()));
	bResult = connect(m_ptrStatusBarSignalMapper, SIGNAL(mapped(const QString &)), this, SLOT(onStatusBarMsgChange(const QString &)));


	bResult = connect(m_actionNewConf, SIGNAL(triggered(bool)) ,
		CDataThread::getInstance(), SLOT(onNewConf()));
	QString strConnDesc = "'m_actionNewConf::toggled' with 'CDataThread::onNewConf'";
	logConnection(strConnDesc,bResult);

	bResult = connect(m_actionRefreshConf, SIGNAL(triggered(bool)) ,
		this, SLOT(onRefreshConf()));
	strConnDesc = "'m_actionRefreshConf::toggled' with 'this::onRefreshConf'";
	logConnection(strConnDesc,bResult);

	bResult = connect(CDataThread::getInstance(), SIGNAL(refreshingFinished()) ,
		this, SLOT(onRefreshingFinished()));
	strConnDesc = "'CDataThread::getInstance::refreshingFinished' with 'this::onRefreshingFinished'";
	logConnection(strConnDesc,bResult);
	/////

	bResult = connect(m_actionOpenConf, SIGNAL(triggered(bool)),
		CDataThread::getInstance(), SLOT(onOpenConf()));
	strConnDesc = "'m_actionOpenConf::toggled' with 'CDataThread::onOpenConf'";
	logConnection(strConnDesc,bResult);

	bResult = connect(m_actionSaveConf, SIGNAL(triggered(bool)),
		CDataThread::getInstance(), SLOT(onSaveConf()));
	strConnDesc = "'m_actionSaveConf::toggled' with 'CDataThread::onSaveConf'";
	logConnection(strConnDesc,bResult);

	bResult = connect(m_actionSaveAsConf, SIGNAL(triggered(bool)),
		CDataThread::getInstance(), SLOT(onSaveAsConf()));
	strConnDesc = "'m_actionSaveAsConf::toggled' with 'CDataThread::onSaveAsConf'";
	logConnection(strConnDesc,bResult);

	bResult = connect(m_actionProxySettings, SIGNAL(triggered(bool)) ,
		this, SLOT(onProxySettings(bool)));
	strConnDesc = "'m_actionProxySettings::toggled' with 'this::onProxySettings'";
	logConnection(strConnDesc,bResult);

	//////////////////////////////////////////////////////////////////////////
	bResult = connect(m_ptrAddComp, SIGNAL(clicked(bool)) ,
		CDataThread::getInstance(), SLOT(onAddComp()));
	strConnDesc = "'m_ptrAddComp::clicked' with 'CDataThread::onAddComp'";
	logConnection(strConnDesc,bResult);
	
	bResult = connect(m_ptrAddShop, SIGNAL(clicked(bool)) ,
		CDataThread::getInstance(), SLOT(onAddShop()));
	strConnDesc = "'m_ptrAddShop::clicked' with 'CDataThread::onAddShop'";
	logConnection(strConnDesc,bResult);
	//////////////////////////////////////////////////////////////////////////
	bResult = connect(m_ptCompConfView, SIGNAL(removeShop(int)) ,
		CDataThread::getInstance(), SLOT(onRemShop(int)));
	strConnDesc = "'m_ptCompConfView::removeShop' with 'CDataThread::onRemShop'";
	logConnection(strConnDesc,bResult);
	//////////////////////////////////////////////////////////////////////////
	bResult = connect(m_ptCompConfView, SIGNAL(removeComponent(int)) ,
		CDataThread::getInstance(), SLOT(onRemComp(int)));
	strConnDesc = "'m_ptCompConfView::removeComponent' with 'CDataThread::onRemComp'";
	logConnection(strConnDesc,bResult);
	////////////////////////////////////////////////////////////////////////////
	bResult = connect(m_ptCompConfView, SIGNAL(onOpenLink(QModelIndex)) ,
		CDataThread::getInstance(), SLOT(onOpenLinkInExternalBrowser(QModelIndex)));
	strConnDesc = "'m_ptCompConfView::onOpenLink' with 'CDataThread::onOpenLinkInExternalBrowser'";
	logConnection(strConnDesc,bResult);
	////////////////////////////////////////////////////////////////////////////
	bResult = connect(m_ptCompConfView, SIGNAL(onOpenSearchPage(QModelIndex)) ,
		CDataThread::getInstance(), SLOT(onOpenSearchInExternalBrowser(QModelIndex)));
	strConnDesc = "'m_ptCompConfView::onOpenSearchPage' with 'CDataThread::onOpenSerachInExternalBrowser'";
	logConnection(strConnDesc,bResult);
	////////////////////////////////////////////////////////////////////////////
	bResult = connect(m_ptCompConfView, SIGNAL(resetFieldCache(QModelIndex)) ,
		CDataThread::getInstance(), SLOT(onResetFieldCache(QModelIndex)));
	strConnDesc = "'m_ptCompConfView::onOpenSearchPage' with 'CDataThread::onOpenSerachInExternalBrowser'";
	logConnection(strConnDesc,bResult);
	////////////////////////////////////////////////////////////////////////////
	bResult = connect(CDataThread::getInstance(), SIGNAL(onStatusBarMsg(const QString & )) ,
		this , SLOT(onStatusBarMsgChange(const QString &)));
	strConnDesc = "'CDataThread::getInstance()::onStatusBarMsg' with 'this::onStatusBarMsgChange'";
	logConnection(strConnDesc,bResult);
	////////////////////////////////////////////////////////////////////////////
	bResult = connect(CDataThread::getInstance(), SIGNAL(updateRecentFilesAction(const QStringList &)) ,
		this , SLOT(onUpdateRecentFileActions(const QStringList &)));
	strConnDesc = "'CDataThread::getInstance()::updateRecentFilesAction' with 'this::onUpdateRecentFileActions'";
	logConnection(strConnDesc,bResult);
}
void MainWindow::setupActions()
{
	m_actionNewConf = menuBar()->addAction("New");
	m_actionNewConf->setShortcut(QKeySequence::New);
	m_actionNewConf->setToolTip("");
	menuBar()->addSeparator();
	////////////////////////////////////////////////////////////////////////////
	m_recentFilesMenu = menuBar()->addMenu("Open");
	m_actionOpenConf = m_recentFilesMenu->addAction("Open");
	m_actionOpenConf->setShortcut(QKeySequence::Open);
	m_actionOpenConf->setToolTip("");
	m_recentFilesMenu->addSeparator();
	////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < MaxRecentFiles; ++i) {	
		m_aRecentFileActs[i] = m_recentFilesMenu->addAction("");
		m_aRecentFileActs[i]->setVisible(false);
		connect(m_aRecentFileActs[i], SIGNAL(triggered()),
			this, SLOT(onOpenRecentFile()));
	}
	//////////////////////////////////////////////////////////////////////////
	m_actionSaveConf = menuBar()->addAction("Save");
	m_actionSaveConf->setShortcut(QKeySequence::Save);
	m_actionSaveConf->setToolTip("");
	////////////////////////////////////////////////////////////////////////////
	m_actionSaveAsConf = menuBar()->addAction("Save as");
	m_actionSaveAsConf->setShortcut(QKeySequence::SaveAs);
	m_actionSaveAsConf->setToolTip("");
	////////////////////////////////////////////////////////////////////////////
	menuBar()->addSeparator();
	m_actionRefreshConf = menuBar()->addAction("Refresh");
	m_actionRefreshConf->setShortcut(QKeySequence::Refresh);
	m_actionRefreshConf->setToolTip("");
	////////////////////////////////////////////////////////////////////////////
	menuBar()->addSeparator();
	m_actionProxySettings= menuBar()->addAction("Proxy");
	m_actionProxySettings->setToolTip("");
}
void MainWindow::onProxySettings(bool)
{
	m_ptrProxySettings->show();
}
void MainWindow::onStatusBarMsgChange(const QString &strMsg)
{
	statusBar()->showMessage(strMsg,3*1000);
}
void MainWindow::onRefreshConf()
{
	if (m_eRefreshingState == E_RS_READY_TO_REFRESH)
	{
		m_eRefreshingState = E_RS_REFRESH_IN_PROGRESSS;
		m_actionRefreshConf->setText("Stop");
		CDataThread::getInstance()->onRefreshConf();
	}
	else if (m_eRefreshingState == E_RS_REFRESH_IN_PROGRESSS)
	{
		m_eRefreshingState = E_RS_REFRESH_STOPPED_WAITING_TO_CLEAN_UP;
		m_actionRefreshConf->setDisabled(true);
		CDataThread::getInstance()->onStopRefreshingConf();
	}
}
void MainWindow::onRefreshingFinished()
{
	m_eRefreshingState = E_RS_READY_TO_REFRESH;
	m_actionRefreshConf->setText("Refresh");
	m_actionRefreshConf->setDisabled(false);
	statusBar()->showMessage("Refreshing finished");
}
void MainWindow::onUpdateRecentFileActions(const QStringList & files)
{
	setWindowFilePath(CDataThread::getInstance()->getCurrentFile());

	int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

	for (int i = 0; i < numRecentFiles; ++i) {
		QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
		m_aRecentFileActs[i]->setText(text);
		m_aRecentFileActs[i]->setData(files[i]);
		m_aRecentFileActs[i]->setVisible(true);
	}
	for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
		m_aRecentFileActs[j]->setVisible(false);
}
QString MainWindow::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}
void MainWindow::onOpenRecentFile()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		CDataThread::getInstance()->onOpenConf(action->data().toString());
}
