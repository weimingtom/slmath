@echo off
cls
echo This bat assumes following:
echo 1) Doxygen is installed properly with Dot Tool (graphviz)
echo 2) HTML Help Workshop is installed to C:\Program Files\HTML Help Workshop\
rem echo 3) Freeware text replace command line tool repl.exe can be found from path. If not, download from http://www.kajala.com/downloads/repl-1.1.zip
pause

del slmath.chm
doxygen doxygen.project

rem --- Change default page with text replace
cd html
rem repl "main=\"slmath\",\"index.hhc\",\"index.hhk\",\"index.html\",\"index.html\"" "main=\"slmath\",\"index.hhc\",\"index.hhk\",\"de\d0d\group__slmath.html\",\"index.html\"" index.hhp
"C:\Program Files\HTML Help Workshop\hhc.exe" index.hhp
cd ..

copy html\index.chm slmath.chm
rmdir /S /Q html
