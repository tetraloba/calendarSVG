# 導入方法 (How to Install)
## for Windows
Pythonの実行環境を構築してください。(Python3.8.10, pip23.1.2, requirements.txt)

calendar.ps1を編集し、以下を行ってください。
- 画像ファイル(.svg, .png)を生成するディレクトリパスを指定してください。

PowerShellの実行ファイル(powershell.exe)のショートカットを作成し、以下の2点を行ってください。
- 「リンク先」に「実行ポリシーの変更」「ウィンドウの非表示設定」「対象ファイル(スクリプト(.ps1))の指定」を追記してください。  
  例: "C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe -ExecutionPolicy RemoteSigned -WindowStyle Hidden -File C:\users\USERNAME\Desktop\Calendar\calendar.ps1"
- 「作業フォルダ」に画像ファイルを生成するディレクトリパスを指定してください。  
  例："C:\users\USERNAME\Desktop\Calendar"

# develop
`svglib`でSVGをPNGに変換するために`rlPyCairo(PyCairo)`が必要。以下のコマンドでインストールできる。(requirements.txtに含まれる)
```bash
pip install rlPyCairo
```
この時、`pkg-config`や`cairo`などのパッケージが存在しないというエラーが発生する場合がある。それぞれ以下のコマンドでインストールする。
```bash
sudo apt install pkg-config
sudo apt install libcairo2-dev
```
