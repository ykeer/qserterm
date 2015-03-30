import QtQuick                 2.3
import QtQuick.Layouts         1.1
import QtQuick.Controls        1.2
import QtQuick.Controls.Styles 1.1
import QtQuick.Window          2.2
import QtQml                   2.2
import QtQuick.Dialogs         1.2

import 'qml'
import 'js/config.js'  as Config
import 'js/file.js'    as File
import 'js/ini.js'     as Ini
import 'js/tools.js'   as Tools
import 'js/filters.js' as Filters
import 'js/scripts.js' as Scripts


ApplicationWindow { id:app; visible:true

	property var files: Qt.createQmlObject('import com.ql.files 1.0; QlFiles {}', app, 'Main')

    property var  port:      null  // communication port
    property var  timercb:   null  // processing timer callback
    property int  timerPeriod: 100 // processing timer period, ms

    property bool rxSupress: false // supress port RX, allows full RX processing by callback function
    function rxSupressEn(en){ rxSupress = en; jsStopButton.enabled = en; }

    property var  rxin:      []    // RX log strings
    property var  txout:     []    // TX bytes/strings
    property var  logg:      []    // log strings

    Timer { interval:app.timerPeriod; running:true; repeat:true
		onTriggered: {
			if (app.port.isOpen()){
				if (timercb) timercb();
				if (!(timercb && rxSupress)){
					var dataIn = app.port.readBytes();
					if (dataIn.length > 0) app.rxin.push( Filters.rx[rxFilter.currentText].receive(dataIn) );
				}
				while (app.rxin.length > 0) rxText.insert(rxText.length, rxin.shift());
				while (app.txout.length > 0){
					var e = app.txout.shift();
					if (typeof(e) === 'string')      app.port.writeString(e);
					else if (typeof(e) === 'number') app.port.writeBytes([e & 0xFF]);
					else if (e instanceof Array)     app.port.writeBytes(e);
				}
			}
			while (logg.length > 0) logText.append(logg.shift());
		}
	}

    // enable/disable controls on port connection/disconnection
    function enableControls(en){
    	[portsCombo,portText,baudsCombo,bitsCombo,parityCombo,stopsCombo,connectButton].forEach(function(w){ w.enabled = en; });
    	[disconnectButton,txSendButton].forEach(function(w){ w.enabled = !en; });
    }

    // configure port
    function portConfig(){
    	if (port.isOpen()){
    		port.paramSet('baud',   baudsCombo.editText);
    		port.paramSet('bits',   bitsCombo.currentText);
    		port.paramSet('parity', parityCombo.currentText);
    		port.paramSet('stops',  stopsCombo.currentIndex);
    	}
    }


    // on creation load config
    Component.onCompleted: {
        console.log('Loading config ' + Config.ini)
		Ini.open(app, Config.ini);
		app.x = Ini.value('winX', 10)
		app.y = Ini.value('winY', 20)
		app.width  = Ini.value('winW', 800)
		app.height = Ini.value('winH', 600)
		
		app.port = Qt.createQmlObject('import com.ql.channels 1.0; QlChannelSerial {}', app, 'Main');

		portsCombo.model         = app.port.channels()
		portsCombo.currentIndex  = Ini.value('port', 0)
		portText.text            = Ini.value('portText', '')
		baudsCombo.editText      = Ini.value('baud', 9600)
		bitsCombo.currentIndex   = Ini.value('bits', 3)
		parityCombo.currentIndex = Ini.value('parity', 0)
		stopsCombo.currentIndex  = Ini.value('stops', 0)
 
 		rxFilter.currentIndex    = Ini.value('rxFilter', 0)
 		txFilter.currentIndex    = Ini.value('txFilter', 0)

		crCheckBox.checked       = Ini.value('CR', 'false') === 'true'
		lfCheckBox.checked       = Ini.value('LF', 'false') === 'true'
 		
 		txText.text              = Qt.atob(Ini.value('txText', ''))
 		
 		rxSaveDialog.folder      = Qt.resolvedUrl(Ini.value('rxSaveFolder', '.'))
 		txSaveDialog.folder      = Qt.resolvedUrl(Ini.value('txSaveFolder', '.'))
 		txLoadDialog.folder      = Qt.resolvedUrl(Ini.value('txLoadFolder', '.'))

		enableControls(true);

//console.log('Argv: ',app.files.argv().length, app.files.argv());
    }
    
    // on destruction update/save config
    Component.onDestruction: {
    	app.port.close();
    	
        console.log('Saving config ' + Config.ini)
		Ini.setValue('winX', app.x)
		Ini.setValue('winY', app.y)
		Ini.setValue('winW', app.width)
		Ini.setValue('winH', app.height)

		Ini.setValue('port',     portsCombo.currentIndex)
		Ini.setValue('portText', portText.text)
		Ini.setValue('baud',     baudsCombo.editText)
		Ini.setValue('bits',     bitsCombo.currentIndex)
		Ini.setValue('parity',   parityCombo.currentIndex)
		Ini.setValue('stops',    stopsCombo.currentIndex)
		
		Ini.setValue('rxFilter', rxFilter.currentIndex)
		Ini.setValue('txFilter', txFilter.currentIndex)

		Ini.setValue('CR', '' + crCheckBox.checked)
		Ini.setValue('LF', '' + lfCheckBox.checked)
		
		Ini.setValue('txText',   Qt.btoa(txText.text))
		
		Ini.setValue('rxSaveFolder', rxSaveDialog.folder.toString())
		Ini.setValue('txSaveFolder', txSaveDialog.folder.toString())
		Ini.setValue('txLoadFolder', txLoadDialog.folder.toString())

		Ini.sync()
    }
    
    

SplitView { id:mainSplit; orientation:Qt.Vertical; anchors.fill:parent
    
    RowLayout { width:parent.width; spacing:5;
    	Layout.minimumHeight:40; Layout.maximumHeight:40;

    	TText { text:qsTr(' port:') }
    	ComboBox { id:portsCombo; implicitWidth:100; anchors.verticalCenter:parent.verticalCenter; }
    	TextField { id:portText; maximumLength:20; anchors.verticalCenter:parent.verticalCenter;
			style: TextFieldStyle {
				font.family:'Consolas'
				font.pixelSize:15
				font.bold:true
			}
		}

    	TText { text:qsTr('baud:') }
    	ComboBox { id:baudsCombo; implicitWidth:100; anchors.verticalCenter:parent.verticalCenter;
    		editable:true; validator:IntValidator{bottom:300; top:3000000;}
    		model: [300,600,1200,1800,2400,4800,7200,9600,14400,19200,28800,38400,56000,57600,115200,128000,230400,460800,921600,1000000,1500000,3000000]
    	}

    	TText { text:qsTr('bits:') }
    	ComboBox { id:bitsCombo; editable:false; model: [5,6,7,8]; implicitWidth:50; anchors.verticalCenter:parent.verticalCenter; }

    	TText { text:qsTr('parity:') }
    	ComboBox { id:parityCombo; editable:false; model: ['none','even','odd','space','mark']; implicitWidth:80; anchors.verticalCenter:parent.verticalCenter; }

    	TText { text:qsTr('stops:') }
    	ComboBox { id:stopsCombo; editable:false; model: ['1','2','1.5']; implicitWidth:50; anchors.verticalCenter:parent.verticalCenter; }
    	
	TText { text:qsTr(' | ') }
    	TButton { id:connectButton; text:qsTr('connect'); onClicked:{
			console.log('Connecting to: ' + portsCombo.currentText);
    		app.port.open(portsCombo.currentText);
    		if (app.port.isOpen()){
				console.log('Connection success.');
    			app.portConfig();
    			app.enableControls(false);
			} else console.log('Connection error.');
    	}}
    	TButton { id:disconnectButton; text:qsTr('disconnect'); onClicked:{
			console.log('Disconnecting from: ' + app.port.name());
    		app.port.close();
    		app.enableControls(true);
    	}}
    }

ColumnLayout { Layout.fillWidth:true;
    Row { spacing:5; Layout.fillWidth:true; Layout.alignment:Qt.AlignTop;

    	TText { text:qsTr(' RX:') }
    	// select RX filter
    	ComboBox { id:rxFilter; editable:false; model:Filters.rx.names; implicitWidth:80; anchors.verticalCenter:parent.verticalCenter; }

	TText { text:qsTr(' | ') }

    	// clear RX text
    	TButton { text:qsTr('clear'); anchors.verticalCenter:parent.verticalCenter; onClicked:{ rxText.text = ''; } }

		// save RX text dialog/button
    	FileDialog { id:rxSaveDialog; title:qsTr('Save to...'); selectExisting:false; selectFolder:false; selectMultiple:false;
			onAccepted: app.files.writeString(app.files.urlToLocalFile(rxSaveDialog.fileUrl), rxText.text, 'UTF8');
		}
    	TButton { text:qsTr('save'); onClicked:{ rxSaveDialog.open(); } }
    }
    // RX text
    TextArea { id:rxText; readOnly:true; Layout.fillWidth:true; Layout.fillHeight:true; Layout.minimumHeight:300;
    	textFormat:TextEdit.AutoText; font.pointSize:10; font.family:'Inconsolata, Consolas, Mono, Courier New'; wrapMode:TextEdit.Wrap;
    }
}

ColumnLayout { Layout.fillWidth:true;
    Row { spacing:5; Layout.fillWidth:true; Layout.alignment:Qt.AlignTop;

    	TText { text:qsTr(' TX:') }
    	// select TX filter
    	ComboBox { id:txFilter; editable:false; model:Filters.tx.names; implicitWidth:80; anchors.verticalCenter:parent.verticalCenter; }

	TText { text:qsTr(' | ') }
		CheckBox { id:crCheckBox; text:'CR'; anchors.verticalCenter:parent.verticalCenter; }
		CheckBox { id:lfCheckBox; text:'LF'; anchors.verticalCenter:parent.verticalCenter; }
    	
	TText { text:qsTr(' | ') }
    	// clear TX text
    	TButton { text:qsTr('clear'); onClicked: txText.text = ''; }
    	// clear Log text
    	TButton { text:qsTr('...log'); onClicked: logText.text = ''; }

	TText { text:qsTr(' | ') }

		// save TX text dialog/button
    	FileDialog { id:txSaveDialog; title:qsTr('Save to...'); selectExisting:false; selectFolder:false; selectMultiple:false;
			onAccepted: app.files.writeString(app.files.urlToLocalFile(txSaveDialog.fileUrl), txText.text, 'UTF8');
		}
    	TButton { text:qsTr('save'); onClicked:{ txSaveDialog.open(); } }

		// load TX text dialog/button
    	FileDialog { id:txLoadDialog; title:qsTr('Load from...'); selectExisting:true; selectFolder:false; selectMultiple:false;
			onAccepted: txText.text = app.files.readString(app.files.urlToLocalFile(txLoadDialog.fileUrl), 'UTF8');
		}
    	TButton { text:qsTr('load'); onClicked:{ txLoadDialog.open(); } }
    	
	TText { text:qsTr(' | ') }

    	// send TX data
    	TButton { id:txSendButton; text:qsTr('send');
    		onClicked: {
    			app.txout = app.txout.concat(
    				Filters.tx[txFilter.currentText].send(
    					(txText.selectionStart != txText.selectionEnd) ? txText.selectedText : txText.text, []
					)
				);
			}
    	}

	TText { text:qsTr(' | ') }

    	TText { text:qsTr('script: ') }
    	// select predefined script
    	ComboBox { id:scriptCombo; editable:false; model:Scripts.names; implicitWidth:150; anchors.verticalCenter:parent.verticalCenter;
    		onCurrentIndexChanged:{
    			if (scriptCombo.currentIndex > 0) txFilter.currentIndex = Filters.tx.names.indexOf('script');
    			txText.text = Scripts.execs[scriptCombo.currentIndex];
    		}
    	}
    	// execute script
    	TButton { id:jsExecButton; text:qsTr('exec');
    		onClicked: {
				try { eval((txText.selectionStart != txText.selectionEnd) ? txText.selectedText : txText.text);
				} catch (e) { app.logg.push('<b><font color="red">Script error:</font></b> <font color="green">' + e.message + '</font>'); }
			}
    	}
    	// stop poll button
    	TButton { id:jsStopButton; text:qsTr('stop'); enabled:false; onClicked:{
    		if (app.timercb){
    			jsStopButton.enabled = false;
    			app.timercb   = null;  // clear callback function
    			app.rxSupress = false; // disable RX supress
    			app.port.readBytes();  // clear RX buffer
			}
    	}}
    }
    // TX text editor
    TextArea { id:txText; Layout.fillWidth:true; Layout.fillHeight:true; Layout.minimumHeight:150;
    	font.pointSize:13; font.family:'Inconsolata, Consolas, Mono, Courier New';
    }
}

// Logger text
TextArea { id:logText; readOnly:true; Layout.fillWidth:true; Layout.fillHeight:true; Layout.minimumHeight:100;
	textFormat:TextEdit.RichText; font.pointSize:9; font.family:'Inconsolata, Consolas, Mono, Courier New'; wrapMode:TextEdit.Wrap;
}

}}

