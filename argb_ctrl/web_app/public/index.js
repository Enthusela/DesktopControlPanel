// Load socket.io-client and connect to the host that serves the page
var socket = io();

window.addEventListener('load', function () {
    var checkbox = document.getElementById('onOff');
    checkbox.addEventListener('change', function () {
        // Check if light box is checked, convert status to number, and send to server
        socket.emit('onOff', Number(this.checked));
    });
    var numberInputs = document.getElementsByClassName('numberInput');
    for (var i = 0; i < numberInputs.length; i++) {
        // Listen for both 'input' (while dragging) and 'change' (when released)
        numberInputs[i].addEventListener('input', function () {
            emitNumberInputValue(this.id);
        });
        numberInputs[i].addEventListener('change', function () {
            emitNumberInputValue(this.id);
        });
    }
});

// Get button status from client
socket.on('onOff', function (data) {
    // Change checkbox according to push button on raspi
    document.getElementById('onOff').checked = data;
    // Send push button status back to the server
    socket.emit('onOff', data);
});

// Function to emit the name and value of a number input
function emitNumberInputValue(name) {
    var input = document.getElementById(name);
    var value = constrainTo8Bit(input.value);
    socket.emit(name, value);
}

function constrainTo8Bit(value) {
    if (isNaN(value)) {
        return 0;
    }
    return Math.min(Math.max(value, 0), 255);
}