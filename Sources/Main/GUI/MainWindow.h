#ifndef _CCS_WINDOW_INCLUDE_
#define _CCS_WINDOW_INCLUDE_

#include <QtCore/QMutex>
#include <QtGui/QMainWindow>
#include <QtGui/QBoxLayout>
#include <QtGui/QToolBar>
#include <QtGui/QMenuBar>
#include <QtGui/QLabel>
#include <QtGui/QTableView>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeView>
#include <QtCore/QSignalMapper>

class CComputerConfView;
class CComputerConfModel;
class ProxySettingsGUI;
class FrozenTableView;

enum ERefreshingState
{
	E_RS_READY_TO_REFRESH,
	E_RS_REFRESH_IN_PROGRESSS,
	E_RS_REFRESH_STOPPED_WAITING_TO_CLEAN_UP
};
enum { MaxRecentFiles = 5 };


class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();

protected:
	void setupUI();
	void setupActions();
	void setConnections();
	void logConnection(QString strConnDesc,bool bResult);
	void closeEvent(QCloseEvent *);
	QString strippedName(const QString &fullFileName);
protected slots:
	void onStatusBarMsgChange(const QString &);
	void onProxySettings(bool);
	void onWaitingForDataThreadTermination();
	void onRefreshConf();
	void onRefreshingFinished();
	void onUpdateRecentFileActions(const QStringList &);
	void onOpenRecentFile();
protected:
	QWidget *					m_ptCentralWidget;
	QVBoxLayout *				m_ptVLayout;
	QToolBar *					m_ptToolBar;

	//////////////////////////////////////////////////////////////////////////
	QHBoxLayout *				m_ptHLayout;
	FrozenTableView *			m_ptCompConfView;
	QPushButton *				m_ptrAddComp;
	QPushButton *				m_ptrAddShop;
	ProxySettingsGUI *			m_ptrProxySettings;
	QSignalMapper *				m_ptrStatusBarSignalMapper;
	////////////////////////////////////////////////////////////////////////////
	QAction *					m_actionNewConf;
	QAction *					m_actionOpenConf;
	QAction *					m_actionSaveConf;
	QAction *					m_actionSaveAsConf;
	QAction *					m_actionRefreshConf;
	QAction *					m_actionProxySettings;
	//////////////////////////////////////////////////////////////////////////
	QMenu *						m_recentFilesMenu;
	QAction *					m_separatorAct;
	QAction *					m_aRecentFileActs[MaxRecentFiles];
	//////////////////////////////////////////////////////////////////////////
	QMutex						m_DataThreadTerminatedMutex;
	void * ptrDataThread;
	ERefreshingState 			m_eRefreshingState;
};
#endif //_CCS_WINDOW_INCLUDE_ 
