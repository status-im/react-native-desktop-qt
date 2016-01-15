var net = require('net');
var repl = require('repl');
var vm = require('vm');
var util = require('util');

var DEBUG = 2;

var server = net.createServer(function(sock) {
    DEBUG && console.log("-- Connection from RN client");

    var state = 'start';
    var length = 0;
    var buffer = new Buffer(0);

    var sandbox = { console: console };
    vm.createContext(sandbox);

    var internalEval = function(code) {
        DEBUG && console.log("-- internalEval: executing script(length=" + code.length + "): " + code.slice(0, 80) + " ... " + code.slice(-80));
        DEBUG > 2 && console.log("-- before sandbox=" + util.inspect(sandbox, { colors: true, depth: null }));
        result = vm.runInContext(code, sandbox);
        DEBUG > 2 && console.log("-- internalEval: result = " + result);
        DEBUG > 2 && console.log("-- after sandbox=" + util.inspect(sandbox, { colors: true, depth: null }));
        return result;
    };

    var sendResponse = function(result) {
        stringifiedResult = JSON.stringify(result);
        DEBUG > 2 && console.log("-- sending result=" + stringifiedResult);
        if (stringifiedResult === undefined) {
            sock.write('undefined');
            return;
        }
        sock.write(JSON.stringify(result));
    }

    sock.on('data', function(chunk) {
        DEBUG > 2 && console.log("-- Data received from RN Client: state = " + state)
        DEBUG > 2 && console.log("-- chunk length: " + chunk.length)
        DEBUG > 2 && console.log("-- buffer length(original): " + buffer.length)

        if (chunk == null || state === 'eof')
            return;

        buffer = Buffer.concat([buffer, chunk]);
        DEBUG > 2 && console.log("-- buffer length(concat): " + buffer.length)

        if (state === 'start') {
            if (buffer.length < 4)
                return; 
            length = chunk.readUInt32LE(0);
            DEBUG > 2 && console.log("-- Packet length: " + length);

            if (buffer.length >= length + 4) {
                result = internalEval(buffer.toString('utf8', 4, length + 4));
                var tmpBuffer = new Buffer(buffer.length - 4 - length);
                buffer.copy(tmpBuffer, length + 4, buffer.length);
                buffer = tmpBuffer;
                sendResponse(result);
            } else {
                state = 'script';
            }
            return;
        }

        if (state == 'script') {
            DEBUG > 2 && console.log("-- Packet length: " + length);
            if (buffer.length >= length + 4) {
                result = internalEval(buffer.toString('utf8', 4, length + 4));
                var tmpBuffer = new Buffer(buffer.length - 4 - length);
                buffer.copy(tmpBuffer, length + 4, buffer.length);
                buffer = tmpBuffer;
                state = 'start';
                sendResponse(result);
            }

            return;
        }
    });

    sock.on('end', function() {
        state = 'eof';
        DEBUG && console.log("-- Socket closed");
    });
}).listen(5000, function() { console.log("-- Server starting") });

/*
TODO: create method to access available session
// Create remote repl
net.createServer(function(sock) {
    repl.start({
        input: sock,
        output: sock
    }).on('exit', function() {
        sock.end();
    });
}).listen(5001);
*/
