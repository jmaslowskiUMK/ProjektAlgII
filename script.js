const canvas = document.querySelector("#Map");

canvas.width = 800;
canvas.height = 800;
let camera = {
    x: canvas.width / 2,
    y: canvas.height / 2,
    zoom: 1
}

const ctx = canvas.getContext("2d");

let parserInstance = null;

document.querySelector("#flowButton", () => {
    if(parserInstance == null) {
        alert("Nie wczytano pliku");
    }

    parserInstance.calculateFlow();
});

document.addEventListener('DOMContentLoaded', () => {
    const fileInput = document.getElementById('csvFileInput');
    const processButton = document.getElementById('processButton');

    processButton.addEventListener('click', async () => {
        const file = fileInput.files[0];
        if (file) {
            if (document.querySelector('input[type="radio"]').id == "buildingMap") {

                //  wgrywanie budynków

                const reader = new FileReader();
                reader.onload = async (event) => {
                    const csvData = event.target.result;

                    parserInstance = await CsvParser();

                    parserInstance.ccall('processCSVBuildings', null, ['string'], [csvData]);

                    // wyrysowanie mapy
                    draw();
                };
                reader.readAsText(file);
            }/*   else if (document.querySelector('input[type="radio"]').id == "borderMap") {

                //  wgrywanie otoczek

                const reader = new FileReader();
                reader.onload = async (event) => {
                    const csvData = event.target.result;

                    parserInstance = await CsvParser();

                    parserInstance.ccall('processCSVBorder', null, ['string'], [csvData]);

                    // wyrysowanie mapy
                    draw();
                };
                reader.readAsText(file);
            }*/
        } else {
            alert('Wybierz plik CSV.');
        }
    });
});

function draw() {
    if (!parserInstance) {
        alert('Żaden plik nie został wczytany!');
        return;
    }

    camera.zoom = parseFloat(document.querySelector('input[type="range"]').value);
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    const relations = parserInstance.getRelations(
        Math.floor(camera.x),
        Math.floor(camera.y),
        camera.zoom,
        canvas.width,
        canvas.height
    );

    ctx.strokeStyle = "nigger";
    ctx.lineWidth = 2;

    // drawing relations
    relations.forEach(rel => {
        ctx.beginPath();
        ctx.moveTo(rel.startX, rel.startY);
        ctx.lineTo(rel.endX, rel.endY);
        ctx.stroke();

        ctx.fillStyle = "red";
        ctx.fillText(rel.capacity, (rel.startX + rel.endX) / 2 + 5, (rel.startY + rel.endY) / 2 + 5);
        ctx.fillStyle = "nigger";
    });


    const nodes = parserInstance.getNodeCoordinates(
        Math.floor(camera.x),
        Math.floor(camera.y),
        camera.zoom,
        canvas.width,
        canvas.height
    );

    // drawing nodes
    nodes.forEach(node => {
        ctx.beginPath();
        ctx.arc(node.x, node.y, node.radius, 0, 2 * Math.PI);
        ctx.fillStyle = "black";
        ctx.fill();

        ctx.beginPath();
        ctx.arc(node.x, node.y, node.radius - 3, 0, 2 * Math.PI);
        ctx.fillStyle = "white";
        ctx.fill();

        ctx.fillStyle = "black";
        ctx.fillText(node.name, node.x + 10 * camera.zoom, node.y - 10 * camera.zoom);
    });

    // loading hulls
    const hulls = parserInstance.getHulls(
        Math.floor(camera.x),
        Math.floor(camera.y),
        camera.zoom,
        canvas.width,
        canvas.height
    );

    // drawing hulls
    ctx.strokeStyle = 'red';
    ctx.lineWidth = 2;
    ctx.fillStyle = 'rgba(255, 0, 0, 0.2)';

    hulls.forEach(hull => {
        const points = hull.points;
        ctx.beginPath();
        ctx.moveTo(points[0].x, points[0].y);
        points.slice(1).forEach(p => ctx.lineTo(p.x, p.y));
        ctx.closePath();
        ctx.fill();
        ctx.stroke();
    });

}

document.addEventListener("keydown", key => {
    if (key.keyCode == 38 || key.keyCode == 87) {
        camera.y -= 10 / camera.zoom;
    }   else if (key.keyCode == 40 || key.keyCode == 83) {
        camera.y += 10 / camera.zoom;
    }   else if (key.keyCode == 39 || key.keyCode == 68) {
        camera.x += 10 / camera.zoom;
    }   else if (key.keyCode == 37 || key.keyCode == 65) {
        camera.x -= 10 / camera.zoom;
    }   else {
        return;
    }

    draw();
});