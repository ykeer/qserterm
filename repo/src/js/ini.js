// load config

var ini = null;

var open = function(parent,file){
    ini = Qt.createQmlObject('import com.ql.settings 1.0; QlSettings {}', parent, 'MainIni');
    ini.open(file);
}

var value = function(key,def){ return ini.value(key,def); }
var setValue = function(key,val){ ini.setValue(key,val); }
var sync = function(){ ini.sync(); }
