xcopy /s /y  /exclude:exclude_list.txt ".\1. Project\Engine\*.h" ".\3. External\Include\Engine"
xcopy /s /y  /exclude:exclude_list.txt ".\1. Project\Engine\*.fx" ".\2. OutputFile\bin\content\shader"