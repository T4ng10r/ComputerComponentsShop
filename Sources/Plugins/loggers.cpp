#include <QtCore/QString>
#include <QtCore/QDir>

#ifdef USE_LOG4QT
#include <log4qt/logmanager.h>
#include <log4qt/fileappender.h>
#include <log4qt/logger.h>
#include <log4qt/ttcclayout.h>
using namespace Log4Qt;
#elif defined(USE_LOG4CPP)
#include	<log4cpp/Category.hh>
#include	<log4cpp/FileAppender.hh>
#include	<log4cpp/Win32DebugAppender.hh>
#include	<log4cpp/PatternLayout.hh>
#endif

#define LOG_DIR QString("logs%1").arg(QDir::separator())
#define LOGER_NAME QString("debug")

namespace ShopPluginLoggers
{
	void createLoggers(const QString & strName)
	{
	#ifdef USE_LOG4QT
		TTCCLayout *p_layout = new TTCCLayout();
		p_layout->setName(QLatin1String("My Layout"));
		p_layout->activateOptions();
		p_layout->setThreadPrinting(false);
		p_layout->setCategoryPrefixing(false);
		// Create an appender
		//////////////////////////////////////////////////////////////////////////
		FileAppender *p_FileDebugAppender = new FileAppender(p_layout, LOG_DIR+strName+"_debug.log");
		p_FileDebugAppender->setName(QLatin1String("My Debug Appender"));
		p_FileDebugAppender->activateOptions();
		p_FileDebugAppender->setImmediateFlush(true);
		//////////////////////////////////////////////////////////////////////////
		LogManager::logger(LOGER_NAME)->addAppender(p_FileDebugAppender);
		LogManager::logger(LOGER_NAME)->info("Log started");
		//////////////////////////////////////////////////////////////////////////
	#elif defined(USE_LOG4CPP)
		log4cpp::PatternLayout* layout1 = new log4cpp::PatternLayout();
		layout1->setConversionPattern("%d %p %c %x: %m%n");

		log4cpp::Appender* debugAppender = new log4cpp::FileAppender("DebugAppender", LOG_DIR.toStdString()+strName.toStdString()+"_debug.log",false);
		debugAppender->setLayout(layout1);

		log4cpp::Category &debugCategory = log4cpp::Category::getInstance(LOGER_NAME.toStdString());
		debugCategory.setAdditivity(false);
		debugCategory.setAppender(debugAppender);
		debugCategory.setPriority(log4cpp::Priority::INFO);
		//////////////////////////////////////////////////////////////////////////
	#elif defined(USE_BOOSTLOG)
	#endif
	}

	void printErrorLog(const QString &  strLog)
	{
	#ifdef USE_LOG4QT
		Log4Qt::LogManager::logger(LOGER_NAME)->error(strLog);
	#elif defined(USE_LOG4CPP)
		log4cpp::Category::getInstance(LOGER_NAME.toStdString()).error(strLog.toStdString());
	#elif defined(USE_BOOSTLOG)
	#endif
	}
	void printDebugLog(const QString &  strLog)
	{
	#ifdef USE_LOG4QT
		Log4Qt::LogManager::logger(LOGER_NAME)->debug(strLog);
	#elif defined(USE_LOG4CPP)
		log4cpp::Category::getInstance(LOGER_NAME.toStdString()).debug(strLog.toStdString());
	#elif defined(USE_BOOSTLOG)
	#endif
	}

}
