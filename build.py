import subprocess
import time

def compile_and_run():
    # Komenda kompilacji
    compile_cmd = [
        "emcc",
        "./view/csv_parser.cpp",
        "./sourceFiles/Country.cpp",
        "./sourceFiles/Node.cpp",
        "./sourceFiles/Lane.cpp",
        "./sourceFiles/Brewery.cpp",
        "./sourceFiles/Pub.cpp",
        "./sourceFiles/Field.cpp",
        "./sourceFiles/Hull.cpp",
        "-o", "./view/csv_parser.js",
        "-s", "EXPORTED_FUNCTIONS=[\"_processCSVBuildings\"]",
        "-s", "MODULARIZE=1",
        "-s", "EXPORT_NAME=\"CsvParser\"",
        "-s", "EXPORTED_RUNTIME_METHODS=[\"ccall\"]",
        "-g",
        "-s ASSERTIONS=1",
        "-fsanitize=address",
        "-s INITIAL_MEMORY=1024MB",
        "-s ALLOW_MEMORY_GROWTH=1",
        "-s NO_DISABLE_EXCEPTION_CATCHING",
        "--bind"
    ]

    print("Kompiluję...")
    result = subprocess.run(" ".join(compile_cmd), shell=True)  # Przekazanie komendy jako listy

    if result.returncode == 0:
        print("Kompilacja udana! Uruchamiam server.js...")

        server_process = subprocess.Popen(["node", "server.js"])

        try:
            while True:
                time.sleep(1)
        except KeyboardInterrupt:
            print("\nZatrzymuję serwer...")
            # Na Windows używamy terminate()
            server_process.terminate()
            server_process.wait()
            print("Serwer zatrzymany.")
        except Exception as e:
            print(f"Wystąpił błąd: {e}")
        finally:
            print("Kończenie programu...")
            # Upewniamy się, że serwer jest zakończony
            server_process.terminate()
            server_process.wait()

    else:
        print("Kompilacja nieudana.")

if __name__ == "__main__":
    compile_and_run()
