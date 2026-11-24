@echo off
setlocal enabledelayedexpansion
:: 切换当前目录为脚本所在的工程根目录
cd /d "%~dp0"

echo =======================================
echo [1/2] 开始编译工程 (make)...
echo =======================================
make
:: 检查上一条命令（make）是否执行成功
if %errorlevel% neq 0 (
    echo.
    echo [错误] 编译失败！终止烧录。
    pause
    exit /b %errorlevel%
)

echo.
echo =======================================
echo [2/2] 正在自动查找最新的 .elf 文件...
echo =======================================

:: 自动在当前目录及子目录下查找最新的一个 .elf 文件
set "ELF_FILE="
for /f "delims=" %%i in ('dir /b /s /a-d /o-d "*.elf" 2^>nul') do (
    set "ELF_FILE=%%i"
    goto :found_elf
)

:found_elf
if "!ELF_FILE!"=="" (
    echo [错误] 编译虽然成功，但没有找到任何 .elf 文件！
    echo 请检查你的 Makefile 输出路径。
    pause
    exit /b 1
)

echo 成功找到目标文件: !ELF_FILE!
echo 开始烧录 (OpenOCD)...
echo =======================================

:: 注意这里用 {} 包裹文件路径，这是 OpenOCD 官方推荐处理带空格路径的语法
openocd -f interface/stlink.cfg -f target/stm32h7x.cfg -c "adapter speed 1000" -c "program {!ELF_FILE!} verify reset exit"

:: 检查烧录结果
if %errorlevel% equ 0 (
    echo.
    echo =======================================
    echo [成功] 烧录完成，芯片已复位运行！
    echo =======================================
) else (
    echo.
    echo [错误] 烧录失败，请检查接线或 ST-Link 是否被占用。
)

pause
