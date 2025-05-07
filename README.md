1. Skompiluj C++ do WebAssembly uruchom w wierszu poleceń:
    emcc csv_parser.cpp ./sourceFiles/Country.cpp ./sourceFiles/Node.cpp ./sourceFiles/Lane.cpp ./sourceFiles/Brewery.cpp ./sourceFiles/Pub.cpp ./sourceFiles/Field.cpp -o csv_parser.js -s EXPORTED_FUNCTIONS="['_processCSV']" -s MODULARIZE=1 -s EXPORT_NAME="CsvParser" -s EXPORTED_RUNTIME_METHODS="['ccall']" --bind
   
3. Uruchom Serwer Node.js w wierszu poleceń:
    node server.js
   
4. Otwórz przeglądarkę, przejdź do http://localhost:8000.
   
3. Testuj - Wybierz plik CSV i kliknij „Przetwórz”. Sprawdź konsolę przeglądarki.

(można zastosować dowolny inny server, jeżeli nie macie node.js może prościej wam będzie użyć serwera w pythonie czy coś)
