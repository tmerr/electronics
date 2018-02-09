$('#feedbutton').click((e) => {
    e.preventDefault();
    $.ajax({
        url: '/feed',
        success: () => {
        },
        error: () => {
            alert('Action failed');
        },
    });
});

$('#unfeedbutton').click((e) => {
    e.preventDefault();
    $.ajax({
        url: '/unfeed',
        success: () => {
        },
        error: () => {
            alert('Action failed');
        },
    });
});

const canvas = document.getElementById('zcanvas');
const ctx = canvas.getContext('2d');
const zdown = 350;
const zup = 630;
const zmid = (zdown + zup) / 2;
const width = canvas.width;
const height = 10;
const arrowheight = 40;

window.setInterval(() => {
    $.ajax({
        type: 'GET',
        dataType: 'json',
        url: '/state',
        success: (state) => {
            document.getElementById('ipval').innerHTML = state.ip;
            document.getElementById('zval').innerHTML = state.z;
            document.getElementById('mode').innerHTML = state.mode;
            const unitz = (state.z - zmid)/((zup - zdown)/2);
            const clamped = Math.min(1, Math.max(-1, unitz));
            const radians = Math.acos(clamped);
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            ctx.save();
            ctx.translate(canvas.width/2, canvas.height/2);
            ctx.rotate(radians);
            ctx.lineWidth = 5;
            ctx.beginPath();
            ctx.moveTo(0, 0);
            ctx.lineTo(0, arrowheight);
            ctx.closePath();
            ctx.stroke();
            ctx.fillRect(-width/2, -height/2, width, height);
            ctx.restore();
        },
        error: () => {
            document.getElementById('ipval').innerHTML = 'Failed to update state';
            document.getElementById('zval').innerHTML = 'Failed to update state';
            document.getElementById('mode').innerHTML = 'Failed to update state';
        },
        timeout: 500
    });
}, 500);
