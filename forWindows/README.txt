[for Windows]
calendar.ps1を編集し、以下の2点を行ってください。
・画像ファイル(.svg, .png)を生成するディレクトリパスを指定してください。
・Inkscapeのコマンドファイル(inkscape.com)のパスを指定してください。

PowerShellの実行ファイル(powershell.exe)のショートカットを作成し、以下の2点を行ってください。
・「リンク先」に「実行ポリシーの変更」「ウィンドウの非表示設定」「対象ファイル(スクリプト(.ps1))の指定」を追記してください。例: "C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe -ExecutionPolicy RemoteSigned -WindowStyle Hidden -File C:\users\USERNAME\Desktop\Calendar\calendar.ps1"
・「作業フォルダ」に画像ファイルを生成するディレクトリパスを指定してください。例："C:\users\USERNAME\Desktop\Calendar"
