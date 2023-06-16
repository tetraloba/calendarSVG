import sys # コマンドライン引数
import ctypes

if len(sys.argv) < 2:
	print(sys.argv[0],": too few arguments")
	exit()
fileName = sys.argv[1]
# fileName = "C:\users\tetraloba\Desktop\Calendar\230616_small.png"
# print(fileName)
ctypes.windll.user32.SystemParametersInfoW(20, 0, fileName , 0)
