#include <QtCore/QString>
#ifdef USE_LOG4QT
#include <log4qt/logmanager.h>
#include <log4qt/consoleappender.h>
#include <log4qt/fileappender.h>
#include <log4qt/logger.h>
#include <log4qt/ttcclayout.h>
using namespace Log4Qt;

TTCCLayout *		p_ConsoleLayout = NULL;
ConsoleAppender *	p_ConsoleAppender = NULL;
FileAppender *		p_FileDebugAppender = NULL;
FileAppender *		p_FileSlotConnAppender = NULL;
FileAppender *		p_FileGUIAppender = NULL;

#elif defined(USE_LOG4CPP)
#include	<log4cpp/Category.hh>
#include	<log4cpp/FileAppender.hh>
#include	<log4cpp/Win32DebugAppender.hh>
#include	<log4cpp/PatternLayout.hh>
#elif defined(USE_BOOSTLOG) && defined(HAS_BOOST)
#include <boost/logging/format/named_write.hpp>
typedef boost::logging::named_logger<>::type logger_type;

#define L_(lvl) BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_log_level(), lvl )

BOOST_DEFINE_LOG_FILTER(g_log_level, boost::logging::level::holder ) // holds the application log level
BOOST_DEFINE_LOG(g_l, logger_type)  //definuje loggera 

void test_mul_levels_one_logger() {
	// formatting    : time [idx] message \n
	// destinations  : console, file "out.txt" and debug window
	g_l()->writer().write("%time%($hh:$mm.$ss.$mili) [%idx%] |\n", "cout file(out.txt) debug");
	g_l()->mark_as_initialized();
}
#endif

#include <QtCore/QFile>
#include <QtCore/QDir>


#define LOG_DIR QString("logs%1").arg(QDir::separator())
//#define LOG_DIR QString(QString("logs")+QDir::separator())
#define DEBUG_LOG_FILE QString(LOG_DIR+QString("debug.log"))
#define SLOTS_LOG_FILE QString(LOG_DIR+QString("slotsConnection.log"))
#define GUI_LOG_FILE QString(LOG_DIR+QString("GUICreation.log"))

void createLoggers()
{

	//QFile stFile("log2.log");
	//stFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
	//stFile.write("createLoggers\n");
	//stFile.flush();

#ifdef USE_LOG4QT
	TTCCLayout *		p_FileLayout = NULL;
	p_FileLayout = new TTCCLayout();
	p_FileLayout->setName(QLatin1String("My Layout"));
	p_FileLayout->activateOptions();
	p_FileLayout->setThreadPrinting(false);
	p_FileLayout->setCategoryPrefixing(false);
	//stFile.write("p_FileLayout utworzone\n");
	//stFile.flush();

	p_ConsoleLayout = new TTCCLayout();
    p_ConsoleLayout->setName(QLatin1String("My Layout"));
    p_ConsoleLayout->activateOptions();
	p_ConsoleLayout->setThreadPrinting(false);
	p_ConsoleLayout->setCategoryPrefixing(false);


	// Create an appender
    p_ConsoleAppender = new ConsoleAppender(p_ConsoleLayout, ConsoleAppender::STDOUT_TARGET);
    p_ConsoleAppender->setName(QLatin1String("My Console Appender"));
    p_ConsoleAppender->activateOptions();

	//////////////////////////////////////////////////////////////////////////
	p_FileDebugAppender = new FileAppender(p_FileLayout, DEBUG_LOG_FILE);
	p_FileDebugAppender->setName(QLatin1String("My Debug Appender"));
	p_FileDebugAppender->activateOptions();
	//////////////////////////////////////////////////////////////////////////
    p_FileSlotConnAppender = new FileAppender(p_FileLayout, SLOTS_LOG_FILE);
    p_FileSlotConnAppender->setName(QLatin1String("My Appender"));
    p_FileSlotConnAppender->activateOptions();
	//////////////////////////////////////////////////////////////////////////
	p_FileGUIAppender = new FileAppender(p_FileLayout, GUI_LOG_FILE);
	p_FileGUIAppender->setName(QLatin1String("My KB Appender"));
	p_FileGUIAppender->activateOptions();
	//////////////////////////////////////////////////////////////////////////
	LogManager::logger("debug")->addAppender(p_ConsoleAppender);
	LogManager::logger("debug")->addAppender(p_FileDebugAppender);
	LogManager::logger("debug")->info("Log started");

	LogManager::logger("GUICreation")->addAppender(p_FileGUIAppender);
	LogManager::logger("GUICreation")->info("Log started");

    LogManager::logger("slotsConnection")->addAppender(p_FileSlotConnAppender);
	LogManager::logger("slotsConnection")->info("Log started");
	//////////////////////////////////////////////////////////////////////////
#elif defined(USE_LOG4CPP)
	log4cpp::PatternLayout* layout1 = new log4cpp::PatternLayout();
	layout1->setConversionPattern("%d %p %c %x: %m%n");

	log4cpp::Appender* debugAppender = new log4cpp::FileAppender("DebugAppender", LOG_DIR.toStdString()+"debug.log",false);
	debugAppender->setLayout(layout1);

	log4cpp::Category &debugCategory = log4cpp::Category::getInstance("debug");
	debugCategory.setAdditivity(false);
	debugCategory.setAppender(debugAppender);
	debugCategory.setPriority(log4cpp::Priority::INFO);

	//////////////////////////////////////////////////////////////////////////
	//log4cpp::PatternLayout* layout = new log4cpp::PatternLayout();
	//layout->setConversionPattern("%d %p %c %x: %m%n");
	log4cpp::Appender* GUICreationAppender = new log4cpp::FileAppender("GUICreationAppender", LOG_DIR.toStdString()+"GUICreation.log",false);
	GUICreationAppender->setLayout(layout1);
	log4cpp::Category &GUICreationCategory = log4cpp::Category::getInstance("GUICreation");
	GUICreationCategory.setAdditivity(false);
	GUICreationCategory.setAppender(GUICreationAppender);
	GUICreationCategory.setPriority(log4cpp::Priority::INFO);

	//////////////////////////////////////////////////////////////////////////
	log4cpp::Appender* slotsConnectionAppender = new log4cpp::FileAppender("SlotsConnectionAppender", LOG_DIR.toStdString()+"slotsConnection.log",false);
	slotsConnectionAppender->setLayout(layout1);

	log4cpp::Category &slotsConnectionCategory = log4cpp::Category::getInstance("slotsConnection");
	slotsConnectionCategory.setAdditivity(false);
	slotsConnectionCategory.setAppender(slotsConnectionAppender);
	slotsConnectionCategory.setPriority(log4cpp::Priority::INFO);
#elif defined(USE_BOOSTLOG) && defined(HAS_BOOST)
#endif
}
void destroyLoggers()
{
#ifdef USE_LOG4QT
	QList<Appender*> lApplist = LogManager::logger("debug")->appenders();
	LogManager::logger("debug")->removeAllAppenders();
	//for(QList<Appender*>::iterator iterApp = lApplist.begin();iterApp != lApplist.end();iterApp++)
	//{
	//	Appender* app = *iterApp;
	//	delete app;
	//}
	lApplist = LogManager::logger("GUICreation")->appenders();
	LogManager::logger("GUICreation")->removeAllAppenders();
	//for(QList<Appender*>::iterator iterApp = lApplist.begin();iterApp != lApplist.end();iterApp++)
	//{
	//	Appender* app = *iterApp;
	//	delete app;
	//}

	lApplist = LogManager::logger("slotsConnection")->appenders();
	LogManager::logger("slotsConnection")->removeAllAppenders();
	//for(QList<Appender*>::iterator iterApp = lApplist.begin();iterApp != lApplist.end();iterApp++)
	//{
	//	Appender* app = *iterApp;
	//	delete app;
	//}
	//delete p_ConsoleLayout;
	//delete p_ConsoleAppender;
	//delete p_FileDebugAppender;
	//delete p_FileDebugAppender;
	//delete p_FileSlotConnAppender;
	//delete p_FileGUIAppender;
#elif defined(USE_BOOSTLOG) && defined(HAS_BOOST)
#endif
}

void printInfoLog(const QString &strLog)
{
#ifdef USE_LOG4QT
	Log4Qt::LogManager::logger("debug")->info(strLog);
#elif defined(USE_LOG4CPP)
	log4cpp::Category::getInstance("debug").info(strLog.toStdString());
#elif defined(USE_BOOSTLOG) && defined(HAS_BOOST)
#endif
}
void printDebugLog(const QString &strLog)
{
#ifdef USE_LOG4QT
	Log4Qt::LogManager::logger("debug")->debug(strLog);
#elif defined(USE_LOG4CPP)
	log4cpp::Category::getInstance("debug").debug(strLog.toStdString());
#elif defined(USE_BOOSTLOG) && defined(HAS_BOOST)
#endif
}
void printWarnLog(const QString & strLog)
{
#ifdef USE_LOG4QT
	Log4Qt::LogManager::logger("debug")->warn(strLog);
#elif defined(USE_LOG4CPP)
	log4cpp::Category::getInstance("debug").warn(strLog.toStdString());
#elif defined(USE_BOOSTLOG) && defined(HAS_BOOST)
#endif
}
void printErrorLog(const QString & strLog)
{
#ifdef USE_LOG4QT
	Log4Qt::LogManager::logger("debug")->error(strLog);
#elif defined(USE_LOG4CPP)
	log4cpp::Category::getInstance("debug").error(strLog.toStdString());
#elif defined(USE_BOOSTLOG) && defined(HAS_BOOST)
#endif
}
void printGUICreationLog(const QString & strLog)
{
#ifdef USE_LOG4QT
	Log4Qt::LogManager::logger("GUICreation")->debug(strLog);
#elif defined(USE_LOG4CPP)
	log4cpp::Category::getInstance("GUICreation").debug(strLog.toStdString());
#elif defined(USE_BOOSTLOG) && defined(HAS_BOOST)
#endif
}
void printSlotsConnectionLog(const QString & strLog)
{
#ifdef USE_LOG4QT
	Log4Qt::LogManager::logger("slotsConnection")->debug(strLog);
#elif defined(USE_LOG4CPP)
	log4cpp::Category::getInstance("slotsConnection").debug(strLog.toStdString());
#elif defined(USE_BOOSTLOG) && defined(HAS_BOOST)
#endif
}
