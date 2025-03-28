1. Skompiluj C++ do WebAssembly:
    Uruchom w wierszu poleceń:
    emcc csv_parser.cpp -o csv_parser.js -s EXPORTED_FUNCTIONS="['_processCSV']" -s MODULARIZE=1 -s EXPORT_NAME="CsvParser" -s EXPORTED_RUNTIME_METHODS="['ccall']"
   
3. Uruchom Serwer Node.js:
    Uruchom w wierszu poleceń:
    node server.js
   
4. Otwórz Przeglądarkę:
    Przejdź do http://localhost:8000.
   
3. Testuj:
    Wybierz plik CSV i kliknij „Przetwórz”.
    Sprawdź konsolę przeglądarki.

(można zastosować dowolny inny server, jeżeli nie macie node.js może prościej wam będzie użyć serwera w pythonie czy coś)
