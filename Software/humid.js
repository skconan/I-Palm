//line
var ctxL = document.getElementById("lineChart").getContext('2d');
var myLineChart = new Chart(ctxL, {
    type: 'line',
    data: {
        labels: ["0", "20", "40", "60", "80", "100", "120"],
        datasets: [
            {
                label: "Humidity",
                fillColor: "rgba(220,220,220,0.2)",
                strokeColor: "rgba(220,220,220,1)",
                pointColor: "rgba(220,220,220,1)",
                pointStrokeColor: "#fff",
                pointHighlightFill: "#fff",
                pointHighlightStroke: "rgba(220,220,220,1)",
                data: [0, 350, 500, 150, 420, 380, 670]
            }
        ]
    },
    options: {
        responsive: true
    }
});
