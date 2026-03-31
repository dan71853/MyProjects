import sys
from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QLineEdit, QPushButton, QVBoxLayout, QMessageBox
from PyQt5.QtCore import Qt,QTimer
from time import gmtime,strftime, sleep
import os

from HermitCraft import getAllStatus

REFRESH_TIME_M = 15


# https://youtu.be/POKeqTmrbyk?t=1048


class PyQtApp(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Testing QT")
        self.setGeometry(0,0,480,600)
        self.setWindowFlag(Qt.FramelessWindowHint)
        self.setStyleSheet("background-color:black")
        self.init_ui()

        # Setup timer
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update_status)
        self.timer.start(REFRESH_TIME_M * 60 * 1000)  # minutes → ms

        # Run once immediately
        self.update_status()

    def init_ui(self):
        layout = QVBoxLayout()

        self.label = QLabel("Loading..")
        self.label.setWordWrap(True)
        self.label.setStyleSheet("color: white; font-size: 14pt;")
        layout.addWidget(self.label, alignment=Qt.AlignTop)


        self.quit_button = QPushButton(" ")
        self.quit_button.clicked.connect(self.close)
        layout.addWidget(self.quit_button)

        self.setLayout(layout)

    def update_status(self):
        try:
            text = getAllStatus()
            self.label.setText(str(text)) 
        except Exception as e:
            self.label.setText(f"Error: {e}")

 
if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = PyQtApp()
    window.show()
    sys.exit(app.exec())



 





# while(1):
#     os.system('clear')
#     print(getAllStatus())
#     sleep(REFRESH_TIME_M*60)