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
let fileLoaded = false;

document.querySelector("#flowButton").addEventListener("click", () => {
    if(!fileLoaded) {
        alert("Wybierz plik CSV");
    }

    let resultsArr = parserInstance.calculateFlow();

    document.getElementById("maxFlowValue").textContent = resultsArr[0]; // for 0 you have edmonds-karp, for 1 you have Dinic algorithm

});

document.addEventListener('DOMContentLoaded', async () => {
    const fileInput = document.getElementById('csvFileInput');
    const processButton = document.getElementById('processButton');
    parserInstance = await CsvParser();

    processButton.addEventListener('click', async () => {

        if (!confirm('Czy na pewno chcesz wczytać plik? Wprowadzone wcześniej dane zostaną usunięte')) {
            return;
        }

        const file = fileInput.files[0];
        if (file) {
            //  wgrywanie budynków

            const reader = new FileReader();
            reader.onload = async (event) => {
                const csvData = event.target.result;
                parserInstance.ccall('processCSVBuildings', null, ['string'], [csvData]);

                // fileLoaded BOOL
                fileLoaded = true;
            };
            reader.readAsText(file);
        } else {
            alert('Wybierz plik CSV');
        }
    });
});


// draw button listener
document.querySelector("#drawButton").addEventListener('click', () => {
    if (!fileLoaded) {
        alert('Żaden plik nie został wczytany!');
        return;
    }

    draw();
});

// MANUAL CREATION

// manual field creation
document.querySelector("#addFieldButton").addEventListener('click', async () => {
    const canvas = document.getElementById("Map");

    // zmieniamy kursor na "celownik"
    canvas.style.cursor = "crosshair";

    const handleClick = (event) => {
        if (event.target !== canvas) {
            alert("Kliknij bezpośrednio w mapę (canvas), aby dodać pole.");
            return;
        }

        const position = getWorldCoordinatesFromMouse(event, canvas, camera.x, camera.y, camera.zoom);
        parserInstance.createField(position.x, position.y);

        canvas.removeEventListener('click', handleClick);

        canvas.style.cursor = "default";
        fileLoaded = true;

        // draw
        draw();
    };

    canvas.addEventListener('click', handleClick);
});

// manual brewerie creation
document.querySelector("#addBreweryButton").addEventListener('click', async () => {
    const canvas = document.getElementById("Map");
    canvas.style.cursor = "crosshair";

    const handleClick = (event) => {
        if (event.target !== canvas) {
            alert("Kliknij bezpośrednio w mapę (canvas), aby dodać browar.");
            return;
        }

        const position = getWorldCoordinatesFromMouse(event, canvas, camera.x, camera.y, camera.zoom);
        parserInstance.createBrewery(position.x, position.y);

        canvas.removeEventListener('click', handleClick);
        canvas.style.cursor = "default";
        fileLoaded = true;
        draw();
    };

    canvas.addEventListener('click', handleClick);
});

// manual pub creation
document.querySelector("#addPubButton").addEventListener('click', async () => {
    const canvas = document.getElementById("Map");
    canvas.style.cursor = "crosshair";

    const handleClick = (event) => {
        if (event.target !== canvas) {
            alert("Kliknij bezpośrednio w mapę (canvas), aby dodać pub.");
            return;
        }

        const position = getWorldCoordinatesFromMouse(event, canvas, camera.x, camera.y, camera.zoom);
        parserInstance.createPub(position.x, position.y);

        canvas.removeEventListener('click', handleClick);
        canvas.style.cursor = "default";
        fileLoaded = true;
        draw();
    };

    canvas.addEventListener('click', handleClick);
});


// function to calculate coordinates saved in memory
function getWorldCoordinatesFromMouse(event, canvas, camX, camY, zoom) {
    const rect = canvas.getBoundingClientRect();

    const screenX = event.clientX - rect.left;
    const screenY = event.clientY - rect.top;

    const canvasWidth = canvas.width;
    const canvasHeight = canvas.height;

    const worldX = (screenX - canvasWidth * 0.5) / zoom + camX;
    const worldY = (screenY - canvasHeight * 0.5) / zoom + camY;

    return {
        x: Math.round(worldX),
        y: Math.round(worldY)
    };
}


// manual relation creation
document.querySelector("#addRelationButton").addEventListener('click', () => {
    const canvas = document.getElementById("Map");
    canvas.style.cursor = "crosshair";

    let firstID = null;

    const nodes = parserInstance.getNoRelationsCoordinates(
        Math.floor(camera.x),
        Math.floor(camera.y),
        camera.zoom,
        canvas.width,
        canvas.height
    );

    const handleClick = (event) => {
        if (event.target !== canvas) {
            alert("Kliknij bezpośrednio w mapę (canvas), aby stworzyć relację.");
            return;
        }

        const x = event.offsetX;
        const y = event.offsetY;

        const hit = nodes.find(node => {
            const dx = node.x - x;
            const dy = node.y - y;
            return Math.sqrt(dx * dx + dy * dy) <= 20;
        });

        if (!hit) {
            alert("Kliknij bliżej węzła.");
            return;
        }

        if (firstID === null) {
            firstID = hit.ID;
            console.log(`Pierwszy węzeł: ${firstID}`);
            return;
        }

        if (hit.ID === firstID) {
            alert("Nie można połączyć węzła z samym sobą.");
            return;
        }

        canvas.removeEventListener('click', handleClick);
        canvas.style.cursor = "default";

        console.log(`Drugi węzeł: ${hit.ID}`);
        parserInstance.createRelation(firstID, hit.ID);
        draw();
    };

    canvas.addEventListener('click', handleClick);
});





function draw() {
    if (!fileLoaded) {
        alert('Żaden plik nie został wczytany!');
        return;
    }


    // canvas clearing
    camera.zoom = parseFloat(document.querySelector('input[type="range"]').value);
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.fillStyle = 'rgba(10, 105, 1, 0.75)';
    ctx.fillRect(0, 0, canvas.width, canvas.height);

    //font settings
    ctx.fillStyle = "black";
    ctx.font = 'bold 10px Century Gothic';

    // HULLS

    // loading hulls
    const hulls = parserInstance.getHulls(
        Math.floor(camera.x),
        Math.floor(camera.y),
        camera.zoom,
        canvas.width,
        canvas.height
    );

    // Hull color palet
    const colors = [
        '',
        'rgba(255, 0, 0, 0.8)',
        'rgba(204, 51, 0, 0.8)',
        'rgba(153, 102, 0, 0.8)',
        'rgba(102, 153, 0, 0.8)',
        'rgba(51, 204, 0, 0.8)',
        'rgba(0, 255, 0, 0.8)'
    ];

    hulls.forEach(hull => {
        // hull colors
        ctx.fillStyle = colors[hull.groundClass];
        ctx.strokeStyle = 'rgba(0,0,0,0)';

        const points = hull.points;
        ctx.beginPath();
        ctx.moveTo(points[0].x, points[0].y);
        points.slice(1).forEach(p => ctx.lineTo(p.x, p.y));
        ctx.closePath();
        ctx.fill();
        ctx.stroke();
    });


    // RELATIONS

    const relations = parserInstance.getRelationsAndCoordinates(
        Math.floor(camera.x),
        Math.floor(camera.y),
        camera.zoom,
        canvas.width,
        canvas.height
    );

    ctx.strokeStyle = 'rgb(77, 44, 14)';
    ctx.lineWidth = 2;

    // drawing relations

    relations.forEach(rel => {

        // line drawing
        ctx.beginPath();
        ctx.moveTo(rel.startX, rel.startY);
        ctx.lineTo(rel.endX, rel.endY);
        ctx.stroke();

        // capacity
        ctx.fillStyle = "black";
        ctx.fillText(rel.capacity, (rel.startX + rel.endX) / 2 + 5, (rel.startY + rel.endY) / 2 + 5);

        // drawing verticies

        //first pair
        ctx.beginPath();
        ctx.fillStyle = "black";
        ctx.arc(rel.startX, rel.startY, rel.radius, 0, 2 * Math.PI);
        ctx.fill();

        ctx.beginPath();
        ctx.fillStyle = "white";
        ctx.arc(rel.startX, rel.startY, rel.radius - 3, 0, 2 * Math.PI);
        ctx.fill();

        // secound pair
        ctx.beginPath();
        ctx.fillStyle = "black";
        ctx.arc(rel.endX, rel.endY, rel.radius, 0, 2 * Math.PI);
        ctx.fill();

        ctx.beginPath();
        ctx.fillStyle = "white";
        ctx.arc(rel.endX, rel.endY, rel.radius - 3, 0, 2 * Math.PI);
        ctx.fill();


        // names
        ctx.fillStyle = "black";
        ctx.fillText(rel.startName, rel.startX + 10 * camera.zoom, rel.startY - 10 * camera.zoom);

        ctx.fillStyle = "black";
        ctx.fillText(rel.endName, rel.endX + 10 * camera.zoom, rel.endY - 10 * camera.zoom);
    });

    // drawing separated elements


    // IT NEEDS TO BE REBUILD, for now it's doubling elements


    const nodes = parserInstance.getNoRelationsCoordinates(
        Math.floor(camera.x),
        Math.floor(camera.y),
        camera.zoom,
        canvas.width,
        canvas.height
    );

    // drawing verticies
    nodes.forEach(node => {

        //first pair
        ctx.beginPath();
        ctx.fillStyle = "black";
        ctx.arc(node.x, node.y, node.radius, 0, 2 * Math.PI);
        ctx.fill();

        ctx.beginPath();
        ctx.fillStyle = "white";
        ctx.arc(node.x, node.y, node.radius - 3, 0, 2 * Math.PI);
        ctx.fill();

        // name
        ctx.fillStyle = "black";
        ctx.fillText(node.name, node.x + 10 * camera.zoom, node.y - 10 * camera.zoom);
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