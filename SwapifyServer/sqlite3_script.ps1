git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
.\vcpkg install sqlite3
Set-ExecutionPolicy -Scope CurrentUser -ExecutionPolicy Undefined