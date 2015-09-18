#ifndef QL_LOCALE
	#define QL_LOCALE

#include <QObject>
#include <QtQuick>
#include <QApplication>
#include <QTranslator>
#include <QFile>
#include <QDir>
#include <QLocale>

class QlLocale : public QObject {
	Q_OBJECT

	public:
		QlLocale(QObject* parent=0);

		// initialize, set translations dir and default locale
		static void l_init(QApplication *app, const char *dir, const char *def);

		// get list of available locales
		Q_INVOKABLE QStringList locales();

		// get list of available locales full names
		Q_INVOKABLE QStringList localeNames();

		// get index of current locale
		Q_INVOKABLE int current();

		// set index of current locale
		Q_INVOKABLE void setCurrent(int n);

		static QApplication* l_app;
		static QTranslator *l_translator;
		static QList<QLocale> l_qlocales;
		static QStringList l_locales;
		static QStringList l_localeNames;
		static QStringList l_tsMask;
		static QString     l_dir;
		static QFile       *l_fcur;
		static QString     l_def;
		static char        l_cur;
};

#endif

