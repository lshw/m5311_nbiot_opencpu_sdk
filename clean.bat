::created by xgl,2019/1/24
@set cur=%~dp0
:param
@set str=%1
@if "%str%"=="" (goto end)
@DEL  %cur%%str%
@shift /0
@goto param
:end
