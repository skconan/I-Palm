
$(function () {
    $("#ring").on("click", function () {

        $("#ringArea").html(`
        <button type="button" class="btn btn-success" id="ring">Ring</button>
        `)

        setTimeout(function () {
        $("#ringArea").html(`
        <button type="button" class="btn btn-danger" id="ring">Ring</button>
        `)  
        }, 5000);


        })
    })
    
function doorSwitch() {
    var checkbox = document.getElementById('door');
    if (checkbox.checked == true) {
        $("#door-switch").html(`
                    <div class="alert alert-warning" role="alert">
                    open
                    </div>`)
    } else {
        $("#door-switch").html(`
                    <div class="alert alert-warning" role="alert">
                    close
                    </div>`)
    }
}


function airSwitch() {
    var checkbox = document.getElementById('air');
    if (checkbox.checked == true) {
        $("#air-switch").html(`
                    <div class="alert alert-warning" role="alert">
                    off
                    </div>`)
    } else {
        $("#air-switch").html(`
                    <div class="alert alert-warning" role="alert">
                    on
                    </div>`)
    }
}