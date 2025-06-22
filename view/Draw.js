let besierSwitch = false;

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
        'rgba(255, 0, 0, 0.7)',
        'rgba(204, 51, 0, 0.7)',
        'rgba(153, 102, 0, 0.7)',
        'rgba(91, 134, 5, 0.7)',
        'rgba(45, 160, 6, 0.7)',
        'rgba(19, 231, 19, 0.7)'
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

    // drawing relations and conected elements
    if (besierSwitch == false) {
        relations.forEach(rel => {

            if (rel.radius < 3)
                rel.radius = 3.1;

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
    }   else if (besierSwitch) {

    // pseudo random besiere curves
        console.log("Drawn using :" + seed + " seed");

        // Generates a deterministic pseudo-random function from a string seed
        function makeDeterministicRandom(seed) {
            // Simple hash function (DJB2-inspired)
            let hash = 0;
            for (let i = 0; i < seed.length; i++) {
                hash = (hash << 5) - hash + seed.charCodeAt(i);
                hash |= 0; // convert to 32bit integer
            }
            let state = Math.abs(hash);

            // Returns a pseudo-random float in [0, 1)
            return function() {
                state ^= state << 13;
                state ^= state >>> 17;
                state ^= state << 5;
                return Math.abs(state % 10000) / 10000;
            };
        }

        // Factorials for Bernstein basis up to n=11
        const factorials = [1,1,2,6,24,120,720,5040,40320,362880,3628800,39916800,479001600];

        // Bernstein polynomial basis
        function bernstein(n, i, t) {
            return factorials[n]/(factorials[i]*factorials[n-i]) * Math.pow(t, i) * Math.pow(1 - t, n - i);
        }

        // Computes a point on a Bézier curve
        function getBezierPoint(t, pts) {
            let x = 0, y = 0, n = pts.length - 1;
            for (let i = 0; i <= n; i++) {
                const b = bernstein(n, i, t);
                x += pts[i].x * b;
                y += pts[i].y * b;
            }
            return { x, y };
        }

        // Draws a Bézier curve on a canvas
        function drawBezier(ctx, pts, steps = 40) {
            ctx.beginPath();
            const p0 = getBezierPoint(0, pts);
            ctx.moveTo(p0.x, p0.y);
            for (let i = 1; i <= steps; i++) {
                const t = i / steps;
                const p = getBezierPoint(t, pts);
                ctx.lineTo(p.x, p.y);
            }
            ctx.stroke();
        }

        // Drawing each relation
        ctx.strokeStyle = 'rgb(77, 44, 14)';
        ctx.lineWidth = 2;

        relations.forEach(rel => {
            if (rel.radius < 3) {
                rel.radius = 3.1;
            } else if (rel.radius > 10) {
                rel.radius = 10;
            }

            // Random generator based on seed and product of IDs
            const rand = makeDeterministicRandom(seed + "_" + (rel.startId * rel.endId));

            // Number of points between 6 and 12
            const numPoints = 3 + Math.floor(rand() * 4);

            const pts = [{ x: rel.startX, y: rel.startY }];

            for (let i = 1; i < numPoints - 1; i++) {
                const t = i / (numPoints - 1);

                const offsetX = (rand() - 0.5) * 400 * camera.zoom;
                const offsetY = (rand() - 0.5) * 400 * camera.zoom;

                pts.push({
                    x: rel.startX + (rel.endX - rel.startX) * t + offsetX,
                    y: rel.startY + (rel.endY - rel.startY) * t + offsetY
                });
            }

            pts.push({ x: rel.endX, y: rel.endY });

            drawBezier(ctx, pts);


            const transformedRadius = rel.radius * camera.zoom;

            // capacity label
            ctx.fillStyle = "black";
            ctx.font = `${10 * camera.zoom}px Arial`;
            ctx.fillText(rel.capacity, (rel.startX + rel.endX) / 2 + 5 * camera.zoom, (rel.startY + rel.endY) / 2 + 5 * camera.zoom);


            // draw vertices
            // start
            ctx.beginPath();
            ctx.fillStyle = "black";
            ctx.arc(rel.startX, rel.startY, transformedRadius, 0, 2 * Math.PI);
            ctx.fill();
            ctx.beginPath();
            ctx.fillStyle = "white";
            ctx.arc(rel.startX, rel.startY, transformedRadius - (3 * camera.zoom), 0, 2 * Math.PI);
            ctx.fill();

            // end
            ctx.beginPath();
            ctx.fillStyle = "black";
            ctx.arc(rel.endX, rel.endY, transformedRadius, 0, 2 * Math.PI);
            ctx.fill();
            ctx.beginPath();
            ctx.fillStyle = "white";
            ctx.arc(rel.endX, rel.endY, transformedRadius - (3 * camera.zoom), 0, 2 * Math.PI);
            ctx.fill();

            // names
            ctx.fillStyle = "black";
            ctx.fillText(rel.startName, rel.startX + 10 * camera.zoom, rel.startY - 10 * camera.zoom);
            ctx.fillText(rel.endName, rel.endX + 10 * camera.zoom, rel.endY - 10 * camera.zoom);
        });
    }



    // drawing separated elements

    const nodes = parserInstance.getNoRelationsCoordinates(
        Math.floor(camera.x),
        Math.floor(camera.y),
        camera.zoom,
        canvas.width,
        canvas.height
    );

    // drawing verticies
    nodes.forEach(node => {
        if (node.radius < 3) {
            node.radius = 3.1;
        }   else if (node.radius > 10) {
            node.radius = 10;
        }

        //first pair
        ctx.beginPath();
        ctx.fillStyle = "black";
        ctx.arc(node.x, node.y, node.radius, 0, 2 * Math.PI);
        ctx.fill();

        ctx.beginPath();
        ctx.fillStyle = "red";
        ctx.arc(node.x, node.y, node.radius - 3, 0, 2 * Math.PI);
        ctx.fill();

        // name
        ctx.fillStyle = "black";
        ctx.fillText(node.name, node.x + 10 * camera.zoom, node.y - 10 * camera.zoom);
    });


    // boundries because of superSource and superSink coordinates
    for (let i = 0; i < arrOfMaxFlow.length; i += 2) {
        let worldX = arrOfMaxFlow[i];
        let worldY = arrOfMaxFlow[i + 1];

        let x = (worldX - (camera.x)) * camera.zoom + (canvas.width) * 0.5;
        let y = (worldY - (camera.y)) * camera.zoom + (canvas.height) * 0.5;

        if (screenX < 0 || screenX > canvas.width || screenY < 0 || screenY > canvas.height)
            continue;

        const glowRadius = Math.max(20 * camera.zoom, 5);
        const markerRadius = glowRadius * 0.7;

        ctx.shadowColor = 'rgba(255, 215, 0, 0.9)';
        ctx.shadowBlur = glowRadius;
        ctx.shadowOffsetX = 0;
        ctx.shadowOffsetY = 0;

        ctx.fillStyle = 'rgba(255, 215, 0, 0.9)';
        ctx.beginPath();
        ctx.arc(x, y, markerRadius, 0, Math.PI * 2, false);
        ctx.fill();
    }

    ctx.shadowBlur = 0;
    ctx.shadowColor = 'transparent';
}