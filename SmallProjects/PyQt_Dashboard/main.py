import sys
from PyQt6.QtWidgets import (
    QApplication,
    QWidget,
    QLabel,
    QPushButton,
    QVBoxLayout,
)
from PyQt6.QtCore import Qt, QTimer

from HermitCraft import getAllStatus

REFRESH_TIME_M = 15


class PyQtApp(QWidget):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("pyqt-dashboard")
        self.setGeometry(0, 0, 480, 600)

        self.setWindowFlag(Qt.WindowType.FramelessWindowHint)

        self.setStyleSheet("background-color:black")

        self.init_ui()

        # Setup timer
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update_status)
        self.timer.start(REFRESH_TIME_M * 60 * 1000)

        self.update_status()

    def init_ui(self):
        layout = QVBoxLayout()
        layout.setContentsMargins(0, 0, 0, 0)

        self.label = QLabel("Loading..")
        self.label.setWordWrap(True)
        self.label.setStyleSheet("color: white; font-size: 14pt;")

        layout.addWidget(
            self.label,
            alignment=Qt.AlignmentFlag.AlignTop
        )

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