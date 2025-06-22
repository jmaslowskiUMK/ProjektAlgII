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

    document.getElementById("maxFlowValue").textContent =resultsArr[0]; //0 - fordfulkerson 1 - edmondsKarp 2 - dinic
    document.getElementById("maxFlowValue1").textContent =resultsArr[0]; //0 - fordfulkerson 1 - edmondsKarp 2 - dinic
    document.getElementById("maxFlowValueTime1").textContent =resultsArr[1]; //0 - fordfulkerson 1 - edmondsKarp 2 - dinic
    document.getElementById("maxFlowValue2").textContent =resultsArr[2];
    document.getElementById("maxFlowValueTime2").textContent =resultsArr[3];
    document.getElementById("maxFlowValue3").textContent =resultsArr[4];
    document.getElementById("maxFlowValueTime3").textContent =resultsArr[5];
//    document.getElementById("maxFlowValue4").textContent =resultsArr[6];
  //  document.getElementById("maxFlowValueTime4").textContent =resultsArr[7];
});

document.addEventListener('DOMContentLoaded', async () => {
    const fileInput = document.getElementById('csvFileInput');
    const processButton = document.getElementById('processButton');
    parserInstance = await CsvParser();

    processButton.addEventListener('click', async () => {

        if (!confirm('Are you sure you want to load the file? Any previously entered data will be lost.')) {
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
            alert('Choose CSV file');
        }
    });
});


// listener for file saveing button
document.querySelector("#saveButton").addEventListener("click", () => {
    const csv = parserInstance.saveToCSV();
    const blob = new Blob([csv], { type: "text/csv;charset=utf-8;" });
    const url = URL.createObjectURL(blob);
    const a = document.createElement("a");
    a.href = url;
    a.download = "output.csv";
    a.click();
    URL.revokeObjectURL(url);
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
        console.log("Starting to scan...");

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

        console.log("Scanning finished.");
    }   else if (e.key === "o") {
        besierSwitch = !(besierSwitch);
        draw();
    }
});




// draw button listener
document.querySelector("#drawButton").addEventListener('click', () => {
    if (!fileLoaded) {
        alert('No file was loaded!');
        return;
    }

    draw();
});



// ============================================================================
// ===                            Manual Creation                           ===
// ============================================================================

// manual field creation
document.querySelector("#addFieldButton").addEventListener('click', async () => {
    if (CREATION_OCCUPIED) {
        alert("Creation tool has already been selected !");
        return;
    }


    const canvas = document.getElementById("Map");

    // zmieniamy kursor na "celownik"
    canvas.style.cursor = "crosshair";

    // set creation occupied
    CREATION_OCCUPIED = true;

    const handleClick = (event) => {
        if (event.target !== canvas) {
            alert("Click directly on the map (canvas), to add field.");
            return;
        }

        // production
        let production = parseInt(prompt("Input barley production", "0"));
        if (production == null || production == "" || isNaN(production) || production <= 0) {
            alert("An incorrect amount of produced barley has been entered !");
            return;
        }

        const position = getWorldCoordinatesFromMouse(event, canvas, camera.x, camera.y, camera.zoom);
        parserInstance.createField(position.x, position.y, production);

        canvas.removeEventListener('click', handleClick);

        canvas.style.cursor = "default";
        fileLoaded = true;

        // draw
        draw();

        // free manual creation
        CREATION_OCCUPIED = false;
    };

    canvas.addEventListener('click', handleClick);

    //Escape to cancel manual creation
    document.addEventListener('keydown', e => {
        if (e.key === "Escape") {
            canvas.removeEventListener('click', handleClick);
            canvas.style.cursor = "default";
            CREATION_OCCUPIED = false;
            console.log("Escapeeeeee");
        }
    }, { once: true });

});

// manual brewerie creation
document.querySelector("#addBreweryButton").addEventListener('click', async () => {
    if (CREATION_OCCUPIED) {
        alert("Creation tool has already been selected !");
        return;
    }

    const canvas = document.getElementById("Map");
    canvas.style.cursor = "crosshair";

    // set creation occupied
    CREATION_OCCUPIED = true;

    const handleClick = (event) => {
        if (event.target !== canvas) {
            alert("Click directly on the map (canvas), to add field.");
            return;
        }

        // barley
        let barley = parseInt(prompt("Enter the brewery's capacity.", "0"));
        if (barley == null || barley == "" || isNaN(barley) || barley <= 0) {
            alert("Wrong efficiency has been entered !");
            return;
        }

        const position = getWorldCoordinatesFromMouse(event, canvas, camera.x, camera.y, camera.zoom);
        parserInstance.createBrewery(position.x, position.y, barley);

        canvas.removeEventListener('click', handleClick);
        canvas.style.cursor = "default";
        fileLoaded = true;
        draw();

        // free manual creation
        CREATION_OCCUPIED = false;
    };

    canvas.addEventListener('click', handleClick);

    //Escape to cancel manual creation
    document.addEventListener('keydown', e => {
        if (e.key === "Escape") {
            canvas.removeEventListener('click', handleClick);
            canvas.style.cursor = "default";
            CREATION_OCCUPIED = false;
            console.log("Escapeeeeee");
        }
    }, { once: true });

});

// manual pub creation
document.querySelector("#addPubButton").addEventListener('click', async () => {
    if (CREATION_OCCUPIED) {
        alert("Creation tool has already been selected !");
        return;
    }

    const canvas = document.getElementById("Map");
    canvas.style.cursor = "crosshair";

    // set creation occupied
    CREATION_OCCUPIED = true;

    const handleClick = (event) => {
        if (event.target !== canvas) {
            alert("Click directly on the map (canvas), to add the pub.");
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

    //Escape to cancel manual creation
    document.addEventListener('keydown', e => {
        if (e.key === "Escape") {
            canvas.removeEventListener('click', handleClick);
            canvas.style.cursor = "default";
            CREATION_OCCUPIED = false;
            console.log("Escapeeeeee");
        }
    }, { once: true });

});


// manual relation creation
document.querySelector("#addRelationButton").addEventListener('click', () => {
    if (CREATION_OCCUPIED) {
        alert("Creation tool has already been selected !");
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
            alert("Click directly on the map (canvas), to create relation.");
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
            alert("Click closer to the node.");
            return;
        }

        if (firstID === null) {
            firstID = hit.ID;
            console.log(`First node: ${firstID}`);
            return;
        }

        if (hit.ID === firstID) {
            alert("Impossible to connect node with itself.");
            return;
        }



        // to prevent fields and pubs connections
        if (
            ((firstID % 3) == (hit.ID % 3)) ||
            (firstID % 3 == 0 && hit.ID % 3 != 1) ||
            (firstID % 3 == 2 && hit.ID % 3 != 1)
        ) {
            alert("You are connecting incompatible elements.");
            return;
        }



        // capacity popup
        let capacity = parseInt(prompt("Input lane capacity (>0)", "0"));
        if (capacity == null || capacity == "" || isNaN(capacity) || capacity <= 0) {
            alert("Wrong capacity has been entered !");
            return;
        }

        // repair_cost popup
        let repair_cost = parseInt(prompt("Enter lane repair cost", "0"));
        if (repair_cost == null || repair_cost === "" || isNaN(repair_cost) || repair_cost < 0) {
            alert("Wrong lane repair cost has been entered !");
            return;
        }

        console.log(`Second node: ${hit.ID}`);
        parserInstance.createRelation(firstID, hit.ID, capacity, repair_cost);

        canvas.removeEventListener('click', handleClick);
        canvas.style.cursor = "default";
        draw();

        // free manual creation
        CREATION_OCCUPIED = false;
    };

    canvas.addEventListener('click', handleClick);

    //Escape to cancel manual creation
    document.addEventListener('keydown', e => {
        if (e.key === "Escape") {
            canvas.removeEventListener('click', handleClick);
            canvas.style.cursor = "default";
            CREATION_OCCUPIED = false;
            console.log("Escapeeeeee");
        }
    }, { once: true });
});

// ============================================================================
// ===                            Location change tools                     ===
// ============================================================================


// manual change of node position
document.querySelector("#moveNodeButton").addEventListener('click', () => {
    if (CREATION_OCCUPIED) {
        alert("Tool is activated!");
        return;
    }

    const canvas = document.getElementById("Map");
    canvas.style.cursor = "crosshair";

    let selectedNodeID = null;

    const nodes = parserInstance.getNodesCoordinates(
        Math.floor(camera.x),
        Math.floor(camera.y),
        camera.zoom,
        canvas.width,
        canvas.height
    );

    CREATION_OCCUPIED = true;

    const handleClick = (event) => {
        if (event.target !== canvas) {
            alert("Click directly on the map (canvas), to chose the node.");
            return;
        }

        const x = event.offsetX;
        const y = event.offsetY;

        if (selectedNodeID === null) {
            const hit = nodes.find(node => {
                const dx = node.x - x;
                const dy = node.y - y;
                return Math.sqrt(dx * dx + dy * dy) <= 20;
            });

            if (!hit) {
                alert("Click closer to the node.");
                return;
            }

            selectedNodeID = hit.ID;
            console.log(`Selected node for transfer.: ${selectedNodeID}`);
            alert("Now, click on the target on the map.");
            return;
        }

        // Drugi klik - ustawienie nowej pozycji
        const position = getWorldCoordinatesFromMouse(event, canvas, camera.x, camera.y, camera.zoom);

        // Przesuń węzeł przez wywołanie metody w parserInstance
        parserInstance.moveNode(selectedNodeID, position.x, position.y);

        canvas.removeEventListener('click', handleClick);
        canvas.style.cursor = "default";
        draw();

        CREATION_OCCUPIED = false;
    };

    canvas.addEventListener('click', handleClick);

    // Escape to cancel node move
    document.addEventListener('keydown', e => {
        if (e.key === "Escape") {
            canvas.removeEventListener('click', handleClick);
            canvas.style.cursor = "default";
            CREATION_OCCUPIED = false;
            console.log("Node transfer has been canceled (Escape)");
        }
    }, { once: true });
});






// ============================================================================
// ===                            Hull Creation                             ===
// ============================================================================

// manual hull creation
document.querySelector("#createHullButton").addEventListener('click', async () => {
    if (CREATION_OCCUPIED) {
        alert("Creation tool has already been activated!");
        return;
    }

    const canvas = document.getElementById("Map");
    canvas.style.cursor = "crosshair";
    CREATION_OCCUPIED = true;

    const points = [];

    // point count input
    let pointCount = parseInt(prompt("Enter number of hull points (min. 3):", "3"));
    if (pointCount == null || isNaN(pointCount) || pointCount < 3) {
        alert("Wrong number of points (min. 3)!");
        CREATION_OCCUPIED = false;
        canvas.style.cursor = "default";
        return;
    }

    // ground class input
    let groundClass = parseInt(prompt("Enter ground class (2 - 5):", "2"));
    if (groundClass == null || isNaN(groundClass) || groundClass < 2 || groundClass > 5) {
        alert("Wrong ground class (2-5)!");
        CREATION_OCCUPIED = false;
        canvas.style.cursor = "default";
        return;
    }

    alert(`Now, click ${pointCount} points on the map, to create convex hull.`);

    const handleClick = (event) => {
        if (event.target !== canvas) {
            alert("Click directly on the map (canvas), to enter hull points.");
            return;
        }

        const position = getWorldCoordinatesFromMouse(event, canvas, camera.x, camera.y, camera.zoom);
        points.push(position.x);
        points.push(position.y);

        if (points.length >= pointCount * 2) {
            canvas.removeEventListener('click', handleClick);
            canvas.style.cursor = "default";

            //pointsInt32 = new Int32Array(points);

            parserInstance.createHull(points.toString(), groundClass);

            draw();
            CREATION_OCCUPIED = false;
        }
    };

    canvas.addEventListener('click', handleClick);

    document.addEventListener('keydown', e => {
        if (e.key === "Escape") {
            canvas.removeEventListener('click', handleClick);
            canvas.style.cursor = "default";
            CREATION_OCCUPIED = false;
            console.log("Creation of hull cancelled (Escape)");
        }
    }, { once: true });
});


// ============================================================================
// ===                            Manual deletion                           ===
// ============================================================================

// hull deletion
document.querySelector("#deleteHullButton").addEventListener('click', () => {
    if (CREATION_OCCUPIED) {
        alert("Creation tool has already been activated!");
        return;
    }

    const canvas = document.getElementById("Map");
    canvas.style.cursor = "crosshair";
    CREATION_OCCUPIED = true;

    const handleClick = (event) => {
        if (event.target !== canvas) {
            alert("Click directly on the map (canvas), to select hull.");
            return;
        }

        const position = getWorldCoordinatesFromMouse(event, canvas, camera.x, camera.y, camera.zoom);
        const hullID = parserInstance.isInWhichHull(Math.round(position.x), Math.round(position.y));

        if (hullID === -1) {
            alert("No hull has been selected.");
            return;
        } else {
            const confirmDelete = confirm(`Are you sure you want to delete hull ID:${hullID}?`);
            if (confirmDelete) {
                parserInstance.deleteHull(hullID);
                draw();
            }
        }

        canvas.removeEventListener('click', handleClick);
        canvas.style.cursor = "default";
        CREATION_OCCUPIED = false;
    };

    canvas.addEventListener('click', handleClick);

    // Escape to cancel
    document.addEventListener('keydown', e => {
        if (e.key === "Escape") {
            canvas.removeEventListener('click', handleClick);
            canvas.style.cursor = "default";
            CREATION_OCCUPIED = false;
        }
    }, { once: true });
});

// relation deletion
document.querySelector("#deleteRelationButton").addEventListener('click', () => {
    if (CREATION_OCCUPIED) {
        alert("Creation tool has already been selected");
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

    CREATION_OCCUPIED = true;


    alert("Select two nodes between which the connection is to be removed.");

    const handleClick = (event) => {
        if (event.target !== canvas) {
            alert("Click directly on the map (canvas), to select nodes.");
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
            return;
        }

        const secondID = hit.ID;

        if (secondID === firstID) {
            alert("Not possible to select the same node twice.");
            return;
        }

        const confirmDelete = confirm(`Are you sure you want to delete relation between nodes: ${firstID} i ${secondID}?`);
        if (confirmDelete) {
            parserInstance.deleteRelation(firstID, secondID);
            draw();
        }

        canvas.removeEventListener('click', handleClick);
        canvas.style.cursor = "default";
        CREATION_OCCUPIED = false;
    };

    canvas.addEventListener('click', handleClick);

    // Escape to cancel
    document.addEventListener('keydown', e => {
        if (e.key === "Escape") {
            canvas.removeEventListener('click', handleClick);
            canvas.style.cursor = "default";
            CREATION_OCCUPIED = false;
        }
    }, { once: true });
});

// node deletion
document.querySelector("#deletePropertyButton").addEventListener('click', async () => {
    if (CREATION_OCCUPIED) {
        alert("Creation tool has already been activated!");
        return;
    }

    const canvas = document.getElementById("Map");
    canvas.style.cursor = "crosshair";

    // zaznacz, że narzędzie jest zajęte
    CREATION_OCCUPIED = true;

    const nodes = parserInstance.getNodesCoordinates(
        Math.floor(camera.x),
        Math.floor(camera.y),
        camera.zoom,
        canvas.width,
        canvas.height
    );

    const handleClick = (event) => {
        if (event.target !== canvas) {
            alert("Click directly on the map (canvas), to delete the node.");
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
            alert("Node has not been found near the click.");
            return;
        }

        const confirmDelete = confirm(`Are you sure you want to delete node with ID: ${hit.ID}?`);
        if (confirmDelete) {
            parserInstance.deleteNode(hit.ID);
            draw();
        }

        // posprzątaj
        canvas.removeEventListener('click', handleClick);
        canvas.style.cursor = "default";
        CREATION_OCCUPIED = false;
    };

    canvas.addEventListener('click', handleClick);

    document.addEventListener('keydown', e => {
        if (e.key === "Escape") {
            canvas.removeEventListener('click', handleClick);
            canvas.style.cursor = "default";
            CREATION_OCCUPIED = false;
        }
    }, { once: true });
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


// Arrows to move
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
