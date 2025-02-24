from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QPixmap
import asyncio
import threading
import sys
import websockets


class Ui_GUI(object):      
	def setupUi(self, GUI):
		global ScreenWidth, Colour, LayoutNo
		GUI.setObjectName("Receiver GUI")
		GUI.resize(800, 400)
		
		#Actual Window
		font = QtGui.QFont()
		font.setFamily("Arial")
		GUI.setFont(font)
		icon = QtGui.QIcon()
		icon.addPixmap(QPixmap("/stuff.jpg"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
		GUI.setWindowIcon(icon)
		GUI.setAutoFillBackground(False)
		GUI.setToolButtonStyle(QtCore.Qt.ToolButtonTextBesideIcon)
		self.centralwidget = QtWidgets.QWidget(GUI)
		self.centralwidget.setObjectName("centralwidget")
		
		#Background
		self.BG = QtWidgets.QLabel(self.centralwidget)
		self.BG.setGeometry(QtCore.QRect(-1, -1, (ScreenWidth + 2), (ScreenHeight + 2)))
		self.BG.setText("")
		self.BG.setScaledContents(True)
		self.BG.setAlignment(QtCore.Qt.AlignCenter)
		self.BG.setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0.448276 rgba(0, 0, 0, 255), stop:1 rgba(255, 0, 0, 255));")
		self.BG.setObjectName("Background")
		
		#Clock Label
		self.cLabel = QtWidgets.QLabel(self.centralwidget)
		font = QtGui.QFont()
		font.setFamily("Arial")
		font.setPointSize(38)
		font.setBold(True)
		self.cLabel.setFont(font)
		self.cLabel.setAlignment(QtCore.Qt.AlignCenter)
		self.cLabel.setGeometry(QtCore.QRect(646, 9, 148, 45))
		self.cLabel.setObjectName("cbel")
		
		
		self.Lomer = QTimer()
		self.Admer = QTimer()
		self.Lomer.timeout.connect(self.PAds)
		self.Admer.timeout.connect(self.Aun)
		self.Lomer.setSingleShot(True)
		self.Admer.setSingleShot(True)
		
		self.GUI = GUI
		
		GUI.setCentralWidget(self.centralwidget)

		QtCore.QMetaObject.connectSlotsByName(GUI)
		#GUI.resizeEvent = self.onResize

	def onResize(self, event):
		global ScreenWidth, ScreenHeight, Size58,  Size45,  Size35, Size120, LayoutNo, mainTickerxpos, mainTickerypos, Gradient, Colour1, Colour2
		print(f'{event.size().width()}, {event.size().height()}')
		ScreenWidth = event.size().width()
		ScreenHeight = event.size().height()
		with open('/stuff', 'r') as file:
			lines = file.readlines()
		with open('/stuff', 'w') as file:
			lines[2] = str(ScreenWidth) + '\n'
			lines[3] = str(ScreenHeight) + '\n'
			file.writelines(lines)
		
		#Layout 1
		Height45 = round(event.size().height() * 0.13)
		Height148 = round(event.size().height() * 0.37)
		ClockX = round(event.size().width() * 0.8)
		ClockY = round(event.size().height() * 0.0225)
		QtrX = round(event.size().width() * 0.9075)
		QtrY = round(event.size().height() * 0.505)
		QtrWidth = round(event.size().width() * 0.0825)
		NameX = round(event.size().width() * 0.02875)
		NameWidth = round(event.size().width() * 0.745)
		BorderWidth = round(event.size().width() * 0.9175)
		HomeY = round(event.size().height() * 0.1475)
		AwayY = round(event.size().height() * 0.6375)
		GoalX = round(event.size().width() * 0.00375)
		BehindX = round(event.size().width() * 0.29)
		ScoreWidth = round(event.size().width() * 0.2625)
		TotalX = round(event.size().width() * 0.575)
		TotalWidth = round(event.size().width() * 0.4025)
		RadiusWidth = round(event.size().width() * 0.12875)
		RadiusX = round(event.size().width() * 0.865)
		
		#Layout 2
		Height52 = round(event.size().height() * 0.13)
		Height145 = round(event.size().height() * 0.3625)
		ClockX2 = round(event.size().width() * 0.65)
		ClockY2 = round(event.size().height() * 0.01)
		QtrX2 = round(event.size().width() * 0.8925)
		QtrWidth2 = round(event.size().width() * 0.0925)
		HNameY2 = round(event.size().height() * 0.04)
		ANameY2 = round(event.size().height() * 0.51)
		NameX2 = round(event.size().width() * 0.065)
		NameWidth2 = round(event.size().width() * 0.61375)
		HomeY2 = round(event.size().height() * 0.155)
		AwayY2 = round(event.size().height() * 0.64)
		GoalX2 = round(event.size().width() * 0.0075)
		BehindX2 = round(event.size().width() * 0.31)
		ScoreWidth2 = round(event.size().width() * 0.2625)
		TotalX2 = round(event.size().width() * 0.6075)
		TotalWidth2 = round(event.size().width() * 0.4)
		
		#Resize Fonts
		size140 = QtGui.QFont()
		size140.setFamily("Arial")
		size140.setPointSize(round(event.size().width() * 0.175))
		Size140 = round(event.size().width() * 0.175)

		size147 = QtGui.QFont()
		size147.setFamily("Arial")
		size147.setPointSize(round(event.size().width() * 0.18375))
		Size147 = round(event.size().width() * 0.18375)

		size40 = QtGui.QFont()
		size40.setFamily("Arial")
		size40.setPointSize(round(event.size().width() * 0.05))
		Size40 = round(event.size().width() * 0.05)
		
		size45 = QtGui.QFont()
		size45.setFamily("Arial")
		size45.setPointSize(round(event.size().width() * 0.05625))
		Size45 = round(event.size().width() * 0.05625)

		size38 = QtGui.QFont()
		size38.setFamily("Arial")
		size38.setPointSize(round(event.size().width() * 0.0475))
		Size38 = round(event.size().width() * 0.0475)
        
		size35 = QtGui.QFont()
		size35.setFamily("Arial")
		size35.setPointSize(round(event.size().width() * 0.04375))
		Size35 = round(event.size().width() * 0.04375)
		
		
		#Ads Window
		AdWidth = event.size().width() + 2 
		AdHeight = event.size().height() + 2
		self.Advertisement.setGeometry(-1, -1, AdWidth, AdHeight)
		tickerypos = round(event.size().height() * 0.88)
		tickerHeight = round(event.size().height() * 0.145)
		tickerWidth = round(event.size().width() * 3.875)
		self.TickerBG.setGeometry(QtCore.QRect(0, tickerypos, tickerWidth, tickerHeight))
		self.Ticker.setFont(size35)
		self.Ticker.adjustSize()
		self.Ticker.setGeometry(self.Ticker.x(), self.Ticker.y(), self.Ticker.width(), self.Ticker.height())

		self.BG.setGeometry(QtCore.QRect(-1, -1, (ScreenWidth+2), (ScreenHeight+2)))
			
		#Resize
		if LayoutNo == 1:
			self.BG.setPixmap(QPixmap("/stuff.jpg"))
			self.BG.setStyleSheet("")
			self.clockLabel.setGeometry(QtCore.QRect(ClockX2, ClockY2, Height145, Height52))
			self.qtrLabel.setGeometry(QtCore.QRect(QtrX2, ClockY2, QtrWidth2, Height52))
			self.HomeLabel.setGeometry(QtCore.QRect(NameX2, HNameY2, NameWidth2, Height52))
			self.HomeGoalLabel.setGeometry(QtCore.QRect(GoalX2, HomeY2, ScoreWidth2, Height145))
			self.HomeBehindLabel.setGeometry(QtCore.QRect(BehindX2, HomeY2, ScoreWidth2, Height145))
			self.HomeTotalLabel.setGeometry(QtCore.QRect(TotalX2, HomeY2, TotalWidth2, Height145))
			self.AwayLabel.setGeometry(QtCore.QRect(NameX2, ANameY2, NameWidth2, Height52))
			self.AwayGoalLabel.setGeometry(QtCore.QRect(GoalX2, AwayY2, ScoreWidth2, Height145))
			self.AwayBehindLabel.setGeometry(QtCore.QRect(BehindX2, AwayY2, ScoreWidth2, Height145))
			self.AwayTotalLabel.setGeometry(QtCore.QRect(TotalX2, AwayY2, TotalWidth2, Height145))
			self.HBorder.setVisible(False)
			self.HRadius.setVisible(False)
			self.ABorder.setVisible(False)
			self.ARadius.setVisible(False)
			self.clockLabel.setFont(size45)
			self.qtrLabel.setFont(size45)
			self.HomeLabel.setFont(size40)
			self.HomeBehindLabel.setFont(size147)
			self.HomeGoalLabel.setFont(size147)
			self.HomeTotalLabel.setFont(size147)
			self.AwayLabel.setFont(size40)
			self.AwayBehindLabel.setFont(size147)
			self.AwayGoalLabel.setFont(size147)
			self.AwayTotalLabel.setFont(size147)
		else:
			self.BG.setPixmap(QPixmap(""))
			if Gradient == "h":
				self.BG.setStyleSheet(f"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0.448276 {Colour1}, stop:1 {Colour2});")
			if Gradient == "v":
				self.BG.setStyleSheet(f"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 {Colour1}, stop:1 {Colour2});")
			self.BG.setGeometry(QtCore.QRect(-1, -1, (ScreenWidth+2), (ScreenHeight+2)))
			self.clockLabel.setGeometry(QtCore.QRect(ClockX, ClockY, Height148, Height45))
			self.qtrLabel.setGeometry(QtCore.QRect(QtrX, QtrY, QtrWidth, Height45))
			self.HomeLabel.setGeometry(QtCore.QRect(NameX, ClockY, NameWidth, Height45))
			self.HBorder.setGeometry(QtCore.QRect(0, HomeY, BorderWidth , Height148))
			self.HRadius.setGeometry(QtCore.QRect(RadiusX, HomeY, RadiusWidth , Height148))
			self.HomeGoalLabel.setGeometry(QtCore.QRect(GoalX, HomeY, ScoreWidth, Height148))
			self.HomeBehindLabel.setGeometry(QtCore.QRect(BehindX, HomeY, ScoreWidth, Height148))
			self.HomeTotalLabel.setGeometry(QtCore.QRect(TotalX, HomeY, TotalWidth, Height148))
			self.AwayLabel.setGeometry(QtCore.QRect(NameX, QtrY, NameWidth, Height45))
			self.ABorder.setGeometry(QtCore.QRect(0, AwayY, BorderWidth ,Height148))
			self.ARadius.setGeometry(QtCore.QRect(RadiusX, AwayY, RadiusWidth, Height148))
			self.AwayGoalLabel.setGeometry(QtCore.QRect(GoalX, AwayY, ScoreWidth, Height148))
			self.AwayBehindLabel.setGeometry(QtCore.QRect(BehindX, AwayY, ScoreWidth, Height148))
			self.AwayTotalLabel.setGeometry(QtCore.QRect(TotalX, AwayY, TotalWidth, Height148))
			self.HBorder.setVisible(True)
			self.HRadius.setVisible(True)
			self.ABorder.setVisible(True)
			self.ARadius.setVisible(True)
			self.clockLabel.setFont(size38)
			self.qtrLabel.setFont(size38)
			self.HomeLabel.setFont(size38)
			self.HomeBehindLabel.setFont(size140)
			self.HomeGoalLabel.setFont(size140)
			self.HomeTotalLabel.setFont(size140)
			self.AwayLabel.setFont(size38)
			self.AwayBehindLabel.setFont(size140)
			self.AwayGoalLabel.setFont(size140)
			self.AwayTotalLabel.setFont(size140)
        
class ReceiverWindow(QtWidgets.QMainWindow):
	sttA = QtCore.pyqtSignal()
	def __init__(self):
		super().__init__()
		
		# Initialize the UI
		self.ui = Ui_GUI()
		self.ui.setupUi(self)
		
		self.sttA.connect(self.ui.Pls)
				
		thread = threading.Thread(target=self.start_websocket_server)


    #Scores
	def GoHoe(self):
		Hal += 1
		HTol += 9
		self.ui.HoeGalLael.setText(str(HGoal))
		self.ui.HoeToalLbel.setText(str(HTotal))
		self.ui.Tiker.setText(f"{Htr}")
		self.ui.Ticer.adjustSize()
		self.ui.Ticer.setGeometry(self.ui.Ticker.x(), self.ui.Ticker.y(), self.ui.Ticker.width(), self.ui.Ticker.height())


	#Wifi
	async def echo(self):
		uri = "ws://192.168.10.156:8080"
		async with websockets.connect(uri) as websocket:
			print("Connected to WebSocket server.")
			try:
				while True:
					message = await websocket.recv()
					if message == "Hooal":
						self.Goe()
						with open('/stuff.txt', 'r') as file:
							lines = file.readlines()
						with open('/stuff.txt', 'w') as file:
							lines[1] = str(Hal) + '\n'
							lines[3] = str(Hal) + '\n'
							file.writelines(lines)
						print('Hoed!')
						
					elif message.startswith("Aame:"):
						awayname = message[len("Awame:"):]
						AName = str(awayname)
						with open('/stuff.txt', 'r') as file:
							lines = file.readlines()
						with open('/stuff.txt', 'w') as file:
							lines[4] = str(Nae) + '\n'
							file.writelines(lines)
						self.ui.Awabel.setText(Ae)
						self.ui.Tier.setText(f"{ottr}")
						self.ui.Tier.adjustSize()
						print(f"Aw se '{awame}'")
						
			except websockets.exceptions.ConnectionClosedError:
				print("WebSocket connection closed unexpectedly.")
	

	def start_websocket_server(self):
		try:
			loop = asyncio.new_event_loop()
			asyncio.set_event_loop(loop)
			start_server = websockets.serve(self.echo, "192.168.10.156", 8080)
			print("WebSocket server is starting...")
			loop.run_until_complete(start_server)
			print("WebSocket server is running.")
			loop.run_forever()
		except Exception as e:
			print(f"Error starting server: {e}")
			return
		

if __name__ == '__main__':
	global GUI
	app = QtWidgets.QApplication(sys.argv)
	GUI = ReceiverWindow()
	GUI.show()
	sys.exit(app.exec_())