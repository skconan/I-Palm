var button_switch_status = 1
var button_air_status = 0
var button_light_in_status = 0
var button_light_out_status = 0
var button_door_status = 0

var msg0 = ["switch","light command","light out","light in","door range","air on","door on"]
var address = ["switchStatus","lightCommand","lightOut","lightIn","doorRange","airOn","doorOn"]


$(function(){
    var check="";
    var message_switch="";
    var server_recieve =function(address,msg0){
        $.ajax({
        type: "GET",
        // url: "http://ecourse.cpe.ku.ac.th:1515/api/iPalm-"+address+"/view",
        url: "http://exceed.srakrn.me/api/iPalm" + address + "/view",
        dataType: "text",
        success: function (response) {
            console.log("get "+ msg0)
            console.log(response)
            if (response !=""){
            check = msg0
        }
            // mesage_switch = response   
        },
        fail: function(response){
            console.log(response) 
        }
    })};
    var a = function(message,keys){
        $.ajax({
            type: "POST",
            // url: "http://ecourse.cpe.ku.ac.th:1515/api/iPalm-" + keys + "/set",
            url: "http://exceed.srakrn.me/api/iPalm-" + keys + "/set",
            data: {
                value: message
            },
            dataType: "text",
            success: function (response) {
                console.log(response)
                $('#text').val('')
            }
        });
    }

    $(`#ring`).on('click',function(){
        console.log("push botton")
        check = "switch"
    })    

    setInterval(function(){ 

        let i = 0
        console.log("before loop "+ check )
        while (i < 7) {
            if (check ==""){
                server_recieve(address[i],msg0[i])         
            }
            i++;
        }
        console.log("in refresh "+ check )
        console.log("end check server")

        let keys = ""
        let message = ""

    
        // $(`#air`).on('click',function(){
        //     check = 
        // })
    
        // $(`#light-in`).on('click',function(){
        //     check = 
        // })
    
        // $(`#light-out`).on('click',function(){
        //     check = 
        // })
    
        // $(`#door`).on('click',function(){
        //     check = 
        // })     
        
        if(check == "switch"){
            keys= "switchStatus"
            if (button_switch_status=="1"){
                button_switch_status = 0
            }
            console.log("POST switch ,status : "+ button_switch_status)
            a(button_switch_status,keys)
        }
        // else if(check = "light command"){
        //     keys="lightCommand"
        // }
        else if(check == "light out")
        {
            keys="lightOut"
            if (button_light_out_status=="0"){
                button_light_out_status = 1
            }
            console.log("POST light out ,status : "+ button_light_out_status)
            a(button_light_out_status,keys)
        }
        else if(check == "light in")
        {
            keys="lightIn"
            if (button_light_in_status=="0"){
                button_light_in_status = 1
            }
            console.log("POST light in ,status : " + button_light_in_status)
            a(button_light_in_status,keys)
        }
        // else if(check = "door range"){
        //     keys="lightRange"
        // }
        else if(check == "air on")
        {
            keys="airOn"
            if (button_air_status=="0"){
                button_air_status = 1
            }
            console.log("POST air on ,status : "+button_air_status)
            a(button_air_status,keys)
        }
        // else if(check = "door on"){
        //     keys="doorOn"
        //     message =""
        //     aessage,keys)
        // }
        check =""

        }
    ,3000)

        /////////////////////////////get data from server
       
    //     $.ajax({
    //         type: "GET",
    //         url: "http://ecourse.cpe.ku.ac.th/api/iPalm-switchStatus/view",
    //         dataType: "text",
    //         success: function (response) {
    //             console.log("get switch")
    //             console.log(response)
    //             check = "switch"
    //             mesage_switch = response   
    //         },
    //         fail: function(response){
    //             console.log(response) 
    //         }
    //     });
    // }

    //     $.ajax({
    //         type: "GET",
    //         url: "http://ecourse.cpe.ku.ac.th/api/iPalm-lightCommand/view",
    //         dataType: "text",
    //         success: function (response) {
    //             console.log(response)
    //             console.log("get light command")
    //             check = "light command"   
    //         },
    //         fail: function(response){
    //             console.log(response) 
    //         }
    //     });

    //     $.ajax({
    //         type: "GET",
    //         url: "http://ecourse.cpe.ku.ac.th/api/iPalm-lightOut/view",
    //         dataType: "text",
    //         success: function (response) {
    //             console.log(response)
    //             console.log("get light out")
    //             check = "light out"   
    //         },
    //         fail: function(response){
    //             console.log(response) 
    //         }
    //     });

    //     $.ajax({
    //         type: "GET",
    //         url: "http://ecourse.cpe.ku.ac.th/api/iPalm-lightIn/view",
    //         dataType: "text",
    //         success: function (response) {
    //             console.log(response)
    //             console.log("get light in")
    //             check = "light in"   
    //         },
    //         fail: function(response){
    //             console.log(response) 
    //         }
    //     });

    //     $.ajax({
    //         type: "GET",
    //         url: "http://ecourse.cpe.ku.ac.th/api/iPalm-doorRange/view",
    //         dataType: "text",
    //         success: function (response) {
    //             console.log(response)
    //             console.log("get door range")
    //             check = "door range"   
    //         },
    //         fail: function(response){
    //             console.log(response) 
    //         }
    //     });

    //     $.ajax({
    //         type: "GET",
    //         url: "http://ecourse.cpe.ku.ac.th/api/iPalm-airOn/view",
    //         dataType: "text",
    //         success: function (response) {
    //             console.log(response)
    //             console.log("get air on")
    //             check = "air on"   
    //         },
    //         fail: function(response){
    //             console.log(response) 
    //         }
    //     });

    //     $.ajax({
    //         type: "GET",
    //         url: "http://ecourse.cpe.ku.ac.th/api/iPalm-doorOn/view",
    //         dataType: "text",
    //         success: function (response) {
    //             console.log(response)
    //             console.log("get door on")
    //             check = "door on"   
    //         },
    //         fail: function(response){
    //             console.log(response) 
    //         }
    //     });


//////////////////Function
    



        
}) 
