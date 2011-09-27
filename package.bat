cd %SLMATH%\documentation
c:\bin\doxygen\doxygen.exe doxygen.project
copy html\index.chm slmath.chm
rd /s /q %SLMATH%\documentation\html
cd ..
php package.php
