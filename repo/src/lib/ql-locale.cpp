#include "ql-locale.hpp"
#include <QDebug>

QlLocale::QlLocale(QObject* parent) : QObject(parent) { }

void QlLocale::l_init(QApplication *app, const char *dir, const char *def){
	l_app        = app;
	l_translator = new QTranslator;
	l_dir        = QString(dir);
	l_def        = QString(def);

	// list available locales
	QStringList fl = QDir(dir).entryList(l_tsMask);
	for (int i=0; i<fl.length(); i++)
		l_locales.append( fl[i].midRef(5, fl[i].length()-8).toString() );
	l_locales.sort(Qt::CaseInsensitive);
	l_locales.prepend( l_def );
	
	// list of full locale names
	for (int i=0; i<l_locales.size(); i++){
		l_qlocales.append( QLocale(l_locales[i]) );
		l_localeNames.append( l_qlocales[i].nativeLanguageName() + " [" + l_qlocales[i].nativeCountryName() + "]" );
	}

	// get current locale
	l_fcur = new QFile(l_dir + "/current");
	if (l_fcur->open( QIODevice::ReadOnly ) && l_fcur->getChar(&l_cur) ) l_fcur->close();
	else l_cur = 0;
	if ((l_cur < 0)||(l_cur >= l_locales.size())) l_cur = 0;

	// load selected translation
	if (l_cur > 0) l_translator->load(QString("lang_") + l_locales[l_cur], l_dir);

	// install application translator
	app->installTranslator(l_translator);
}

QStringList QlLocale::locales(){ return l_locales; }
QStringList QlLocale::localeNames(){ return l_localeNames; }
int         QlLocale::current(){ return l_cur; }
void        QlLocale::setCurrent(int n){
	l_cur = n;
	if (l_fcur->open( QIODevice::WriteOnly ) && l_fcur->putChar(n) ) l_fcur->close();
}

QApplication*  QlLocale::l_app         = 0;
QTranslator*   QlLocale::l_translator  = 0;
QList<QLocale> QlLocale::l_qlocales    = QList<QLocale>();
QStringList    QlLocale::l_locales     = QStringList();
QStringList    QlLocale::l_localeNames = QStringList();
QStringList    QlLocale::l_tsMask      = QStringList() << "lang_*.qm";
QString        QlLocale::l_dir         = QString();
QFile*         QlLocale::l_fcur        = 0;
QString        QlLocale::l_def         = QString();
char           QlLocale::l_cur         = 0;

Q_DECLARE_METATYPE(QlLocale*)

