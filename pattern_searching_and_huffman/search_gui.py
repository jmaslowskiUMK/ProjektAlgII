import sys
import os
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout, QLineEdit, QPushButton, QRadioButton, QLabel, QFileDialog, QGroupBox, QMessageBox, QCheckBox

from search import search 

class PatternSearch(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('Pattern search')
        self.setGeometry(500, 300, 600, 400)
        
        # main widget and layout
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        main_layout = QVBoxLayout()
        
        # pattern 
        pattern_group = QGroupBox("Search parameters")
        pattern_layout = QVBoxLayout()
        
        # pattern input
        self.pattern_input = QLineEdit()
        self.pattern_input.setPlaceholderText("Enter the word to search for...")
        pattern_layout.addWidget(QLabel("Search for a word:"))
        pattern_layout.addWidget(self.pattern_input)
        
        # search method (radio button)
        method_group = QGroupBox("Search method")
        method_layout = QVBoxLayout()
        self.rk_rb = QRadioButton("Rabin-Karp Algorithm")
        self.kmp_rb = QRadioButton("Knuth-Morris-Pratt Algorithm (KMP)")
        self.rk_rb.setChecked(True)

        self.rk_rb.toggled.connect(self.update_output_filename)
        self.kmp_rb.toggled.connect(self.update_output_filename)

        method_layout.addWidget(self.rk_rb)
        method_layout.addWidget(self.kmp_rb)
        method_group.setLayout(method_layout)
        
        # huffman coding 
        self.huffman_cb = QCheckBox("Use Huffman coding")
        self.huffman_cb.setChecked(False)
        
        pattern_layout.addWidget(method_group)
        pattern_layout.addWidget(self.huffman_cb)
        pattern_group.setLayout(pattern_layout)
        
        # file selection
        file_group = QGroupBox("Files")
        file_layout = QVBoxLayout()
        
        # choose input file 
        self.input_file_label = QLabel("No file")
        input_file_btn = QPushButton("Select CSV file to search")
        input_file_btn.clicked.connect(self.select_input_file)

        file_layout.addWidget(QLabel("Input file:"))
        file_layout.addWidget(self.input_file_label)
        file_layout.addWidget(input_file_btn)
        
        # output file name
        self.output_file_input = QLineEdit()
        self.update_output_filename()
    
        file_layout.addWidget(QLabel("Output Results File:"))
        file_layout.addWidget(self.output_file_input)
        file_group.setLayout(file_layout)
        
        # search button
        search_btn = QPushButton("Search")
        search_btn.setStyleSheet("""
        QPushButton {
            min-height: 40px;
            min-width: 150px;
            font-size: 16px;
            font-weight: bold;
        }
         """)
        search_btn.clicked.connect(self.run_search)
        
        # add to main layout
        main_layout.addWidget(pattern_group)
        main_layout.addWidget(file_group)
        main_layout.addWidget(search_btn)
        central_widget.setLayout(main_layout)
        
        # file path
        self.input_file_path = None
    
    def update_output_filename(self):
        # update the default output file name 
        if self.rk_rb.isChecked():
            default_name = "rk_search.csv"
        elif self.kmp_rb.isChecked():
            default_name = "kmp_search.csv"
        
        
        # update if user didn't modify the file name
        self.output_file_input.setPlaceholderText(default_name)
        if self.output_file_input.text() in ["rk_search_results.csv", 
                                          "kmp_search_results.csv", 
                                          "bm_search_results.csv"]:
            self.output_file_input.clear()
        

    def select_input_file(self):
        options = QFileDialog.Options()
        file_path, _ = QFileDialog.getOpenFileName(
            self, "Select CSV file", "", "CSV Files (*.csv);;All Files (*)", options=options)
        
        if file_path:
            self.input_file_path = file_path
            self.input_file_label.setText(os.path.basename(file_path))
    
    def get_search_type(self):
        if self.rk_rb.isChecked():
            return "rk"
        elif self.kmp_rb.isChecked():
            return "kmp"
        else:
            return "bm"
    
    def run_search(self):

        pattern = self.pattern_input.text()
        if not pattern:
            QMessageBox.warning(self, "Warning", "Please enter a search pattern")
            return
        
        if not self.input_file_path:
            QMessageBox.warning(self, "Warning", "Please select an input file")
            return
        
        file_name = self.input_file_path
        pattern = self.pattern_input.text()
        search_type = self.get_search_type()
        huffman_coding = self.huffman_cb.isChecked()
        save_as = self.output_file_input.text() or self.output_file_input.placeholderText()

        
        try:

            results = search(file_name, pattern, search_type, huffman_coding, save_as)
            QMessageBox.information(self, "Search Complete", f"Found {results} matches\n Saved to: {save_as}")

        except Exception as e:
            QMessageBox.critical(self, "Error", f"An error occurred:\n {str(e)}")
    

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = PatternSearch()
    window.show()
    sys.exit(app.exec_())

