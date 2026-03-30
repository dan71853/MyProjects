import sys
from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QLineEdit, QPushButton, QVBoxLayout, QMessageBox
from PyQt5.QtCore import Qt

# https://youtu.be/POKeqTmrbyk?t=1048


text = ""

app = QApplication(sys.argv)
window = QWidget()
window.setWindowTitle("Testing QT")
window.setGeometry(0,0,480,600)
window.setWindowFlag(Qt.FramelessWindowHint)
window.setStyleSheet("background-color:black")

layout = QVBoxLayout()

label = QLabel("Enter text", window)
label.setText(text)
label.setWordWrap(True)
layout.addWidget(label)
 

quit_button = QPushButton(" ")
quit_button.clicked.connect(window.close)
layout.addWidget(quit_button)

window.setLayout(layout)

window.show()
sys.exit(app.exec())