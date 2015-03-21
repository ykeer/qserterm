
// Modbus functions
var modbus = {

	// calculate CRC16
	crc: function(a,prev){
		var crc = (prev === undefined) ? 0xFFFF : prev;
		for (var i=0; i<a.length; i++){
			crc ^= a[i] & 0xFF;
			for (var b=0; b<8; b++){
				var flag = crc & 1;
				crc >>= 1;
				if (flag) crc ^= 0xA001;
			}
		}
		return crc;
	},
	// append CRC16 to array
	crcAppend: function(a) {
		var crc = modbus.crc(a);
		a.push(crc & 0xFF, crc >> 8);
		return a;
	},
	
	// RTU
	rtu: {
		// create RTU request
		request: function(fun,slave,addr,num){
			if (fun == 3) return modbus.crcAppend( [slave, fun, addr >> 8, addr & 0xFF, num >> 8, num & 0xFF] );
			else return [];
		},
		// parse RTU response
		responseParse: function(bytes){
			var resp = { error:true, slave:0, fun:0, addr:0, num:0, regs:[] };
			if (bytes.length >= 9){
				var crc = ((bytes[bytes.length-1] << 8) | (bytes[bytes.length-2] & 0xFF)) & 0xFFFF;
				if (crc === modbus.crc( bytes.slice(0, -2) )){
					resp.slave = bytes[0] & 0xFF; // slave address
					resp.fun   = bytes[1] & 0xFF; // function code
					if (resp.fun == 3){
						resp.num  = bytes[2] & 0xFF; // number of bytes
						for (var i=0; i<resp.num/2; i++)
							resp.regs.push( (bytes[3 + i*2] << 8 | (bytes[4 + i*2] & 0xFF)) & 0xFFFF );
						resp.error = false;
					}
				}
			}
			return resp;
		},
	},
	
	// ASCII
	ascii: {
	},
};

// string operations
var s = {
	// convert to array
    toA: function(s,b){
		var a = [], m = b ? 0xFF : 0xFFFF;
		for (var i=0; i<s.length; i++) a.push( s.charCodeAt(i) & m );
		return a;
	},
};

// array operations
var a = {
	// byte array to string
	toS: function(a){
		var s = [];
		for (var i=0; i<a.length; i++) s.push( String.fromCharCode(a[i]) );
		return s.join('');
	},
	// byte array to Hex
	toHex: function(a,sep,prefix){
		var sep = (sep === undefined) ? '' : sep;
		var prefix = (prefix === undefined) ? '' : prefix;
		var h = [];
		for (var i=0; i<a.length; i++)
			h.push( prefix + ((a[i] < 16) ? '0' : '') + a[i].toString(16) );
		return h.join(sep);
	},
};

