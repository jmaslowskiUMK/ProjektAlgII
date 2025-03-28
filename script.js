document.addEventListener('DOMContentLoaded', () => {
    const fileInput = document.getElementById('csvFileInput');
    const processButton = document.getElementById('processButton');

    processButton.addEventListener('click', () => {
        const file = fileInput.files[0];
        if (file) {
            const reader = new FileReader();
            reader.onload = (event) => {
                const csvData = event.target.result;
                CsvParser().then((instance) => {
                    instance.ccall('processCSV', null, ['string'], [csvData]);
                });
            };
            reader.readAsText(file);
        } else {
            alert('Wybierz plik CSV.');
        }
    });
});