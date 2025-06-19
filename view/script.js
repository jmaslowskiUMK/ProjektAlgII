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
let CREATION_OCCUPIED = false;


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


// listener to test reyCast with hulls
document.addEventListener('click', e => {
    const position = getWorldCoordinatesFromMouse(e, canvas, camera.x, camera.y, camera.zoom);
    //console.log(position);
    //console.log(parserInstance.isInAnyHull(position.x, position.y));
    if (parserInstance.isInAnyHull(position.x, position.y)) {
        ctx.fillStyle = 'red'; // ustaw kolor wypełnienia na czerwony
        ctx.beginPath();
        ctx.rect(e.offsetX, e.offsetY, 2, 2); // offsetX/Y = współrzędne względem canvas
        ctx.fill();
    }
});

document.addEventListener('keypress', e => {
    if (e.key === "i") {
        console.log("Rozpoczynam skanowanie...");

        const rect = canvas.getBoundingClientRect();

        for (let x = 0; x < canvas.width; x += 3) {
            for (let y = 0; y < canvas.height; y += 3) {

                // symulujemy event.clientX / clientY przez dodanie położenia canvasu w oknie
                const fakeClickEvent = {
                    clientX: rect.left + x,
                    clientY: rect.top + y
                };

                const worldPos = getWorldCoordinatesFromMouse(fakeClickEvent, canvas, camera.x, camera.y, camera.zoom);

                if (parserInstance.isInAnyHull(worldPos.x, worldPos.y)) {
                    ctx.fillStyle = 'red';
                    ctx.beginPath();
                    ctx.rect(x, y, 2, 2); // rysujemy dokładnie w canvasowych współrzędnych
                    ctx.fill();
                }
            }
        }

        console.log("Skanowanie zakończone.");
    }   else if (e.key === "o") {
        besierSwitch = !(besierSwitch);
        draw();
    }
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


// cancel manual creation
document.onkeydown = e => {
    if (e.key = "Escape") {
        canvas.removeEventListener('click', handleClick);
        canvas.style.cursor = "default";
        CREATION_OCCUPIED = false;
    }
};

// manual field creation
document.querySelector("#addFieldButton").addEventListener('click', async () => {
    if (CREATION_OCCUPIED) {
        alert("Narzędzie kreacji jest już wybrane wybrane !");
        return;
    }


    const canvas = document.getElementById("Map");

    // zmieniamy kursor na "celownik"
    canvas.style.cursor = "crosshair";

    // set creation occupied
    CREATION_OCCUPIED = true;

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

        // free manual creation
        CREATION_OCCUPIED = false;
    };

    canvas.addEventListener('click', handleClick);
});

// manual brewerie creation
document.querySelector("#addBreweryButton").addEventListener('click', async () => {
    if (CREATION_OCCUPIED) {
        alert("Narzędzie kreacji jest już wybrane wybrane !");
        return;
    }
    
    const canvas = document.getElementById("Map");
    canvas.style.cursor = "crosshair";

    // set creation occupied
    CREATION_OCCUPIED = true;

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

        // free manual creation
        CREATION_OCCUPIED = false;
    };

    canvas.addEventListener('click', handleClick);
});

// manual pub creation
document.querySelector("#addPubButton").addEventListener('click', async () => {
    if (CREATION_OCCUPIED) {
        alert("Narzędzie kreacji jest już wybrane wybrane !");
        return;
    }

    const canvas = document.getElementById("Map");
    canvas.style.cursor = "crosshair";

    // set creation occupied
    CREATION_OCCUPIED = true;

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

        // free manual creation
        CREATION_OCCUPIED = false;
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
    if (CREATION_OCCUPIED) {
        alert("Narzędzie kreacji jest już wybrane wybrane !");
        return;
    }

    const canvas = document.getElementById("Map");
    canvas.style.cursor = "crosshair";

    let firstID = null;

    const nodes = parserInstance.getNodesCoordinates(
        Math.floor(camera.x),
        Math.floor(camera.y),
        camera.zoom,
        canvas.width,
        canvas.height
    );

    // set creation occupied
    CREATION_OCCUPIED = true;

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



        // to prevent fields and pubs connections
        if (
            ((firstID % 3) == (hit.ID % 3)) ||
            (firstID % 3 == 0 && hit.ID % 3 != 1) ||
            (firstID % 3 == 2 && hit.ID % 3 != 1)
        ) {
            alert("Łączysz nieodpowiednie elementy");
            return;
        }



        // capacity popup
        let capacity = parseInt(prompt("Wprowadz przepustowosc drogi", "0"));
        if (capacity == null || capacity == "" || isNaN(capacity)) {
            alert("Wprowadzono niewłaściwą przepustowość !");
            return;
        }

        // repair_cost popup
        let repair_cost = parseInt(prompt("Wprowadz koszt naprawy drogi", "0"));
        if (repair_cost == null || repair_cost == "" || isNaN(repair_cost)) {
            alert("Wprowadzono niewłaściwy koszt !");
            return;
        }

        console.log(`Drugi węzeł: ${hit.ID}`);
        parserInstance.createRelation(firstID, hit.ID, capacity, repair_cost);

        canvas.removeEventListener('click', handleClick);
        canvas.style.cursor = "default";
        draw();

        // free manual creation
        CREATION_OCCUPIED = false;
    };

    canvas.addEventListener('click', handleClick);
});


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