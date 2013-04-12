#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <Tools/loggers.h>
#ifdef USE_LOG4QT

#include <log4qt/logmanager.h>
#include <log4qt/consoleappender.h>
#include <log4qt/fileappender.h>
#include <log4qt/logger.h>
#include <log4qt/ttcclayout.h>
#include <log4qt/patternlayout.h>
using namespace Log4Qt;

#define DEBUG_LOGGER		"debug"
#define SLOTS_LOGGER		"slots"
#define NETWORK_LOGGER		"network"
#define LOG_DIR				"logs"
#define LOG_FILE(X)		QString("%1%2%3.log").arg(LOG_DIR).arg(QDir::separator()).arg(X)

void createLoggers(const QString &strPluginLogName /*= QString()*/)
{
	PatternLayout *		p_PatternLayout(NULL);
	p_PatternLayout = new PatternLayout;
	//need timestamp, Level, potential Network/debug/slots
	p_PatternLayout->setConversionPattern("%-8r %-5p %c %m%n");
	p_PatternLayout->activateOptions();

	// Create an appender
	if (strPluginLogName.isEmpty())
	{
		ConsoleAppender * p_ConsoleAppender = new ConsoleAppender(p_PatternLayout, ConsoleAppender::STDOUT_TARGET);
		p_ConsoleAppender->setName(QLatin1String("Console Appender"));
		p_ConsoleAppender->activateOptions();
		FileAppender * p_FileDebugAppender = new FileAppender(p_PatternLayout, LOG_FILE(DEBUG_LOGGER));
		p_FileDebugAppender->setName(QLatin1String("Debug Appender"));
		p_FileDebugAppender->activateOptions();
		//////////////////////////////////////////////////////////////////////////
		FileAppender * p_FileSlotConnAppender = new FileAppender(p_PatternLayout, LOG_FILE(SLOTS_LOGGER));
		p_FileSlotConnAppender->setName(QLatin1String("Slots Appender"));
		p_FileSlotConnAppender->activateOptions();
		//////////////////////////////////////////////////////////////////////////
		LogManager::logger(DEBUG_LOGGER)->addAppender(p_ConsoleAppender);
		LogManager::logger(DEBUG_LOGGER)->addAppender(p_FileDebugAppender);
		LogManager::logger(SLOTS_LOGGER)->addAppender(p_FileSlotConnAppender);
	}
	else
	{
		FileAppender * p_FileNetworkAppender = new FileAppender(p_PatternLayout, LOG_FILE(strPluginLogName));
		p_FileNetworkAppender->setName(QLatin1String("Debug Appender"));
		p_FileNetworkAppender->activateOptions();
		//////////////////////////////////////////////////////////////////////////
		LogManager::logger(DEBUG_LOGGER)->addAppender(p_FileNetworkAppender);
	}
}

void destroyLoggers()
{
	QList<Appender*> lApplist = LogManager::logger(DEBUG_LOGGER)->appenders();
	LogManager::logger("debug")->removeAllAppenders();

	lApplist = LogManager::logger(SLOTS_LOGGER)->appenders();
	LogManager::logger("slotsConnection")->removeAllAppenders();
}

void printLog(eDebugLevel debugLevel, eLoggerType loggerType, const QString &strMsg)
{
	Log4Qt::Logger * ptrLogger(NULL);
	Log4Qt::LogError  stLogError(strMsg);
	switch(loggerType)
	{
		case eDebug:	ptrLogger = Log4Qt::LogManager::logger("debug");			break;
		case eSlots:	ptrLogger = Log4Qt::LogManager::logger("slotsConnection");	break;
		case eNetwork:	
			stLogError.setContext("Network");
			ptrLogger = Log4Qt::LogManager::logger("debug");			break;
		default: return;
	}
	switch(debugLevel)
	{
		case eInfoLogLevel:		ptrLogger->info(stLogError); 	break;
		case eWarningLogLevel:	ptrLogger->warn(stLogError);	break;
		case eDebugLogLevel:	ptrLogger->debug(stLogError);	break;
		case eErrorLogLevel:	ptrLogger->error(stLogError);	break;
	}
}
#else
void createLoggers(const QString &strPluginLogName){};
void destroyLoggers(){};
void printLog(eDebugLevel, eLoggerType, const QString &strMsg){};
#endif

//////////////////////////////////////////////////////////////////////////
