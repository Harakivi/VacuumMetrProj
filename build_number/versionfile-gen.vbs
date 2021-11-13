'[Ќачало versionfile-gen.vbs]

'***************************************************************************
'*   ‘ункци€ создает новый файл, и форматирует его 
'*   ¬ данной версии, строка сохран€етс€ в виде:
'*   const char build [] = "00.00.00.00.0000";
'*   "04.09.2021.37.3256"
'*     |  |  |   |   |
'*     |  |  |   |   Build (номер build'а, редактируетс€ автоматически)
'*     |  |  |   Firmware version (редактируетс€ вручную)
'*     |  |  Year(редактируетс€ автоматически)
'*     |  Month  (редактируетс€ автоматически)
'*     Day       (редактируетс€ автоматически)
'*          
'***************************************************************************
'*/ 
Dim Args
Set Args = WScript.Arguments

if (Args.Count < 1) then
  MsgBox "Error generate version file." + vbCrLf + "Usage:" + vbCrLf + "version-gen.vbs <filename>"
else
  'получаем им€ выходного файла
  verfilename = Args(0)
  'получаем дату/врем€
  vYear  = CStr(Year  (Date))
  vMonth = CStr(Month (Date))
  if Len(vMonth) < 2 then
     vMonth = "0"+vMonth
  end if
  vDay    = CStr(Day    (Date))
  if Len(vDay) < 2 then
     vDay = "0"+vDay
  end if
  vHour  = CStr(Hour  (Time))
  if Len(vHour) < 2 then
     vHour = "0"+vHour
  end if
  vMinute = CStr(Minute (Time))
  if Len(vMinute) < 2 then
     vMinute = "0"+vMinute
  end if
  vSecond = CStr(Second (Time))
  if Len(vSecond) < 2 then
     vSecond = "0"+vSecond
  end if
  Set FSO = CreateObject("Scripting.FileSystemObject")
  Set FileOutStream = FSO.OpenTextFile(verfilename, 1, true, 0)
  if(Not FileOutStream.AtEndOfStream) then
  line = FileOutStream.ReadLine
  Dim Build
  Dim Version
  Dim SplittedLine
  Dim SplittedLine1
  SplittedLine = split(line, Chr(34))
  SplittedLine1 = split(SplittedLine(3), ".")
  Version = SplittedLine1(3)
  Build = CInt(SplittedLine1(4))
  Build = Build + 1
  BuildStr = CStr(Build)
  For i = 0 to 3 - Len(BuildStr)
  BuildStr = "0" + BuildStr
  Next
  build_full = " const char build[18] @ ""_version"" = """+vDay+"."+vMonth+"."+vYear+"."+Version+"."+BuildStr+""";"
  
  FileOutStream.Close
  
  Set FileOutStream = FSO.OpenTextFile(verfilename, 2, true, 0)
  
  FileOutStream.Write build_full + vbCrLf
  FileOutStream.Close
  Wscript.Echo "build incr DONE" + vbNewLine
  Wscript.Echo "Build:" + BuildStr
  else
  build_full = " const char build[18] @ ""_version"" = """+vDay+"."+vMonth+"."+vYear+".00.0000"";"
  Set FSO = CreateObject("Scripting.FileSystemObject")
  Set FileOutStream = FSO.OpenTextFile(verfilename, 2, true, 0)
  
  FileOutStream.Write build_full + vbCrLf
  FileOutStream.Close
  Wscript.Echo "build incr DONE NEW"
  end if
  
end if
'[ онец versionfile-gen.vbs]