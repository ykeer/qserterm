
var names = [ '',
	qsTr('TX bytes'),
	qsTr('TX string'),
	qsTr('RTU request'),
	qsTr('RTU poll'),
];

var execs = [ '',
	'// TX array of bytes \n' +
	'Scripts.tx([48, 0x31, 0x32]);',

	'// TX string \n' +
	'Scripts.tx(\'12345\');',

	'// single RTU request \n' +
	'// parameters: function, slave address, register address, registers number \n' +
	'Scripts.rtu.single(3, 5, 1, 4);',

	'// continuous RTU poll \n' +
	'// parameters: period (ms), function, slave address, register address, registers number \n' +
	'Scripts.rtu.multi(1000, 3, 5, 1, 4);',
];

function tx(data){
	if (typeof(data) === 'number') app.txout.push(data);
	else if (typeof(data) === 'string') app.txout.push(data);
	else if (data instanceof Array)
		for (var i=0; i<data.length; i++)
			app.txout.push(data[i]);
}

var rtu = {
	single: function(fun,slave,addr,num){ app.txout.push(Tools.modbus.rtu.request(fun,slave,addr,num)); },
	
	multi: function(period,fun,slave,addr,num){
		app.rxSupress = true;
		var tcur=period, tpoll=0; // current timer, poll timer
		app.timercb = function(){
			if (tpoll === 0){
				tcur += app.timerPeriod;
				if (tcur >= period){
					tcur -= period; tpoll = 2;
					app.txout.push(Tools.modbus.rtu.request(fun,slave,addr,num));
				}
			} else { // decrease poll timer
				if (--tpoll === 0){ // read/parse response packet
					var bytes = app.port.readBytes();
					var resp = Tools.modbus.rtu.responseParse(bytes);
					if (!resp.error){
						app.logg.push(
							'<font color="green">RTU response:</font> <b>slave</b>=<font color="blue">' + resp.slave +
							'</font>, <b>fun</b>=<font color="blue">' + resp.fun +
							'</font>, <b>bytes</b>=<font color="blue">' + resp.num +
							'</font>, <b>regs</b>=[<font color="red">' + resp.regs.join(',') + '</font>]'
						);
					}
				}
			}
		};
	},
};

