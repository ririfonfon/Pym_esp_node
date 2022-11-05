window.addEventListener('load', setup);

var connection;

function connect() {
    console.log('connect()');
    
    connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);
    
    
    connection.onopen = function() {
        connection.send('Connect ' + new Date());
    };
    
    connection.onmessage = function(e) {
        console.log('Server: ', e.data);
        
        if (e.data.charAt(0) == 'cc') {
            var data = e.data.split(':');
            document.getElementById(data[0]).value = data[1];
        } //if

        if (e.data.charAt(0) == 'mem') {
            var val = e.data.split(':')[1];
            var elems = document.querySelectorAll('#mode li a');
            [].forEach.call(elems, function(el) {
                el.classList.remove('active');
            });
            document.getElementById(val).classList.add('active');
        } //if

        if (e.data.charAt(0) == 'b') {
            
            if (e.data.charAt(1) == 'a') {
                var data = e.data.split(':');
                document.getElementById('cc1').value = data[1];
            } //if a

            if (e.data.charAt(1) == 'b') {
                var data = e.data.split(':');
                document.getElementById('cc2').value = data[1];
            } //if b

            if (e.data.charAt(1) == 'c') {
                var data = e.data.split(':');
                document.getElementById('cc3').value = data[1];
            } //if c

            if (e.data.charAt(1) == 'd') {
                var data = e.data.split(':');
                document.getElementById('cc4').value = data[1];
            } //if d

            if (e.data.charAt(1) == 'e') {
                var data = e.data.split(':');
                document.getElementById('cc5').value = data[1];
            } //if e
            
            if (e.data.charAt(1) == 'f') {
                var data = e.data.split(':');
                document.getElementById('ssid').value = data[1];
            } //if f

            if (e.data.charAt(1) == 'g') {
                var data = e.data.split(':');
                document.getElementById('pass').value = data[1];
            } //if g

            
        } //if b

    }; //function (e)
    
    connection.onclose = function(e) {
        console.log('Socket is closed. Reconnect will be attempted in 1 second.', e.reason);
        
        // remove active class from element with id lbueno
        document.getElementById('led').classList.remove('active');
        
        connection.close();
        setTimeout(function() {
            connect();
        }, 1000);
    };

    connection.onerror = function(err) {
        console.error('Socket encountered error: ', err.message, 'Closing socket');
        connection.close();
        setTimeout(function() {
            connect();
        }, 1000);
    };
    
    // add active class to element with id lbueno
    document.getElementById('led').classList.add('active');
}

connect();

id_array = new Array('cc1');
values = new Array(id_array.length);


////////////////////////////////////////////////////////////////prepare

function prepareVar1() {
    var a = parseInt(document.getElementById('cc1').value).toString(8);
    if (a.length < 2) { a = '0' + a; }
    values = a;
    var data = "ba" + values;
    console.log('eData: ' + data);
    connection.send(data);
} //prepare 1

function prepareVar2() {
    var a = parseInt(document.getElementById('cc2').value).toString(8);
    if (a.length < 2) { a = '0' + a; }
    values = a;
    var data = "bb" + values;
    console.log('fData: ' + data);
    connection.send(data);
} //prepare 2

function prepareVar3() {
    var a = parseInt(document.getElementById('cc3').value).toString(8);
    if (a.length < 2) { a = '0' + a; }
    values = a;
    var data = "bc" + values;
    console.log('gData: ' + data);
    connection.send(data);
} //prepare 3

function prepareVar4() {
    var a = parseInt(document.getElementById('cc4').value).toString(8);
    if (a.length < 2) { a = '0' + a; }
    values = a;
    var data = "bd" + values;
    console.log('hData: ' + data);
    connection.send(data);
} //prepare 4

function prepareVar5() {
    var a = parseInt(document.getElementById('cc5').value).toString(8);
    if (a.length < 2) { a = '0' + a; }
    values = a;
    var data = "be" + values;
    console.log('iData: ' + data);
    connection.send(data);
} //prepare 5

function prepareText1() {
    var data = "bf" + document.getElementById('ssid').value;
    console.log('iData: ' + data);
    connection.send(data);
} //prepare text 1

function prepareText2() {
    var data = "bg" + document.getElementById('pass').value;
    console.log('iData: ' + data);
    connection.send(data);
} //prepare text 2

function submitVal(name, val) {
    var xhttp = new XMLHttpRequest();
    xhttp.open('GET', 'set?' + name + '=' + val, true);
    xhttp.send();
} //submitVal

function handle_M_B_S(e) {
    e.preventDefault();
    var name = e.target.className;
    var val = e.target.id;
    console.log('name: ' + name);
    console.log('val: ' + val);

    if (e.target.className.indexOf('mem') > -1) {
        elems = document.querySelectorAll('#mode li a');
        [].forEach.call(elems, function(el) {
            el.classList.remove('active');
            name = e.target.className;
        });
        e.target.classList.add('active');
    }
    submitVal(name, val);
} //handle_M_B_S

function setup() {
    console.log('setup()');
    var xhttp = new XMLHttpRequest();

    elems = document.querySelectorAll('ul li a'); // adds listener
    [].forEach.call(elems, function(el) {
        // el.addEventListener('touchstart', handle_M_B_S, false);
        el.addEventListener('click', handle_M_B_S, false);
    });
    submitVal("mem", 0);
}
