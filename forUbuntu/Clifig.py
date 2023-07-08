from os import system, name # import only system from os

def _clear():
    if name == 'nt': # for windows
        _ = system('cls')
    else: # for mac and linux(here, os.name is 'posix')
        _ = system('clear')

class Clifig:
    def __init__(self, line:str='', row=1, column=1):
        self.lines = [] # 出力する文字列のリスト(1行毎)
        if line != '':
            self.append(line)
        self.lt = [row, column] # 図の左上の座標(第row行, 第column列)
    def append(self, line:str):
        self.lines += line.split('\n')
    def print(self, row=0, column=0):
        _clear()
        self.lt[0] = self.lt[0] if row == 0 else row
        self.lt[1] = self.lt[1] if column == 0 else column
        for line in self.lines:
            print(f"\033[{self.lt[0]};{self.lt[1]}H" + line)
            self.lt[0] += 1
        print("\033[0m")
    def len(self):
        return self.lines.__len__()

if __name__ == "__main__":
    clifig = Clifig('test', 5, 10)
    clifig.print()