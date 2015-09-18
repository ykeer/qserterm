import QtQuick                 2.0
import QtQuick.Controls        1.2
import QtQuick.Controls.Styles 1.2

// locale selection combo box
ComboBox { editable:false; anchors.verticalCenter:parent.verticalCenter;

	property var locale: Qt.createQmlObject('import QlLocale 1.0; QlLocale {}', app, 'Main')
	property bool initialized: false // initialization flag

	// populate available locales, select current locale
	Component.onCompleted: {
		model        = locale.localeNames();
		initialized  = true;
		currentIndex = locale.current(); // select current
	}

	onCurrentIndexChanged: if (initialized) locale.setCurrent(currentIndex);
}

