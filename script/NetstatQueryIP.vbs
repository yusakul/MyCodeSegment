'yusakul
On Error Resume Next
Dim isOccupied, IPAddress
isOccupied = 0
Do
	IPAddress = InputBox("Enter the IPAddress to check:")
	If IsEmpty(IPAddress) Then 'Cancel button is clicked
		WScript.Quit
	ElseIf Len(IPAddress) = 0 Then
		MsgBox "No input!", vbExclamation + vbSystemModal
	Else
		Exit Do
	End If
Loop


Set WshShell = WScript.CreateObject("WScript.Shell")

Do
	launch1 = "cmd.exe /c netstat -ano > output_tmp.txt" 
	cmdRun1 = WshShell.Run(launch1,0,true)

	Const fileName = "output_tmp.txt"
	Const Forreading=1
	Dim  Fso1 ,tf1
	Set Fso1 = CreateObject("Scripting.FileSystemObject")
	If Fso1.fileexists(fileName)Then
			'read from file
			Set tf1 =Fso1.opentextfile(fileName,Forreading)
			Do While tf1.atendofstream<>True 
				strLine =   tf1.ReadLine
				If InStr(strLine, IPAddress) > 0 Then
					isOccupied = 1
					Exit Do
				End If
			Loop 

	Else 'file not exist
			MsgBox "No file!"
			  
	End If 
	tf1.close 
	Set Fso1 =  Nothing

	If isOccupied Then
		'get current dir
		set fileObj=createobject("Scripting.FileSystemObject")
		toFilePath1 = fileObj.GetFolder(".").Path&"/output_queryC2.txt"
		'create file and write 
		Dim fso,tf
		Set fso =  CreateObject("Scripting.FileSystemObject")
		Set tf = fso.CreateTextFile(toFilePath1, False) 'True-代表可以被下次写入覆盖
		tf.WriteLine(strLine)
		'netstat -anb
		launch = "cmd.exe /c netstat -ano > output_net.txt" 
		cmdRun = WshShell.Run(launch,0,true)
		launch2 ="cmd.exe /c tasklist > output_tasklist.txt"
		cmdRun2 = WshShell.Run(launch2,0,true)
		MsgBox "IPAddress " & IPAddress & " is occupied!", vbExclamation + vbSystemModal, "Busy"
		Exit Do
		Else
		'MsgBox "IPAddress " & IPAddress & " is idle!", vbInformation + vbSystemModal, "Idle"
	End If

	'sleep
	t = Timer
	Do While Timer - t < 60
		wscript.sleep 1000
	loop

Loop
