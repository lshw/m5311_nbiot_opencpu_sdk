::created by xgl,2019/1/24
:param
@set str=%1
@if "%str%"=="" (goto end)
@if NOT exist out\%str% (md out\%str%)
@shift /0
@goto param
:end