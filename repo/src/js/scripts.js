
var names = [ '',
	qsTr('TX bytes'),
	qsTr('TX string'),
	qsTr('RTU request'),
	qsTr('RTU poll'),
];

var execs = [ '',
	'// TX bytes \r\n' +
	'Scripts.tx(48, 0x31, 0x32);',

	'// TX string \r\n' +
	'Scripts.tx(\'12345\');',

	'// single RTU request \r\n' +
	'// parameters: slave address, function, register address, registers number \r\n' +
	'Scripts.rtu.single(5, 3, 1, 4);',

	'// continuous RTU poll \r\n' +
	'// parameters: period (ms), slave address, function, register address, registers number \r\n' +
	'Scripts.rtu.multi(1000, 5, 3, 1, 4);',
];

// TX numbers/strings/byte arrays
function tx(){
	for (var i=0; i<arguments.length; i++){
		var data = arguments[i];
		if (typeof(data) === 'number') app.txout.push(data & 0xFF);
		else if (typeof(data) === 'string') app.txout.push(data);
		else if (data instanceof Array) app.txout.push(data);
	}
}

var rtu = {
	single: function(slave,fun,addr,num){ app.txout.push(Tools.modbus.rtu.request(slave,fun,addr,num)); },
	
	multi: function(period,slave,fun,addr,num){
		app.rxSupressEn( true );
		var tcur=period, tpoll=0; // current timer, poll timer
		app.timercb = function(){
			if (tpoll === 0){
				tcur += app.timerPeriod;
				if (tcur >= period){
					tcur -= period; tpoll = 2;
					app.txout.push(Tools.modbus.rtu.request(slave,fun,addr,num));
				}
			} else { // decrease poll timer
				if (--tpoll === 0){ // read/parse response packet
					var bytes = app.port.readBytes();
					var resp = Tools.modbus.rtu.responseParse(bytes);
					if (!resp.error){
						app.logg.push(
							'<font color="green">RTU response:</font> <b>slave</b>=<font color="blue">' + resp.slave +
							'</font> <b>fun</b>=<font color="blue">'   + resp.fun +
							'</font> <b>bytes</b>=<font color="blue">' + resp.num +
							'</font> <b>regs</b>=[<font color="red">'  + resp.regs.join(',') + '</font>]'
						);
					} else {
						app.logg.push(
							'<font color="green">RTU response:</font> <font color="red">error</color>'
						);
					}
				}
			}
		};
	},
};

