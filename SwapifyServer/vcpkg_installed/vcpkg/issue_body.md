Package: sqlite3[core,json1]:x86-windows -> 3.45.0

**Host Environment**

- Host: x64-windows
- Compiler: MSVC 19.38.33135.0
-    vcpkg-tool version: 2023-06-22-f19f3d9939100085fe2f7e41502d60b08e2083b6
    vcpkg-readonly: true
    vcpkg-scripts version: 638b1588be3a265a9c7ad5b212cef72a1cad336a

**To Reproduce**

`vcpkg install `

**Failure logs**

```
-- Using cached sqlite-autoconf-3450000.zip.
-- Cleaning sources at C:/Users/sudha/OneDrive/Desktop/College/Semester 4/Project 4/Swapify_Integration/SwapifyServer/vcpkg_installed/vcpkg/blds/sqlite3/src/nf-3450000-a492cb06d8.clean. Use --editable to skip cleaning for the packages you specify.
-- Extracting source C:/Users/sudha/AppData/Local/vcpkg/downloads/sqlite-autoconf-3450000.zip
-- Applying patch fix-arm-uwp.patch
-- Applying patch add-config-include.patch
-- Using source at C:/Users/sudha/OneDrive/Desktop/College/Semester 4/Project 4/Swapify_Integration/SwapifyServer/vcpkg_installed/vcpkg/blds/sqlite3/src/nf-3450000-a492cb06d8.clean
-- Found external ninja('1.11.0').
-- Configuring x86-windows
-- Building x86-windows-dbg
-- Building x86-windows-rel
-- Fixing pkgconfig file: C:/Users/sudha/OneDrive/Desktop/College/Semester 4/Project 4/Swapify_Integration/SwapifyServer/vcpkg_installed/vcpkg/pkgs/sqlite3_x86-windows/lib/pkgconfig/sqlite3.pc
-- Using cached mingw-w64-i686-pkg-config-0.29.2-3-any.pkg.tar.zst.
-- Downloading https://mirror.msys2.org/mingw/i686/mingw-w64-i686-libwinpthread-git-9.0.0.6373.5be8fcd83-1-any.pkg.tar.zst;https://repo.msys2.org/mingw/i686/mingw-w64-i686-libwinpthread-git-9.0.0.6373.5be8fcd83-1-any.pkg.tar.zst;https://repo.msys2.org/mingw/i686/mingw-w64-i686-libwinpthread-git-9.0.0.6373.5be8fcd83-1-any.pkg.tar.zst;https://repo.msys2.org/mingw/i686/mingw-w64-i686-libwinpthread-git-9.0.0.6373.5be8fcd83-1-any.pkg.tar.zst;https://repo.msys2.org/mingw/i686/mingw-w64-i686-libwinpthread-git-9.0.0.6373.5be8fcd83-1-any.pkg.tar.zst;https://repo.msys2.org/mingw/i686/mingw-w64-i686-libwinpthread-git-9.0.0.6373.5be8fcd83-1-any.pkg.tar.zst -> mingw-w64-i686-libwinpthread-git-9.0.0.6373.5be8fcd83-1-any.pkg.tar.zst...
[DEBUG] To include the environment variables in debug output, pass --debug-env
[DEBUG] Trying to load bundleconfig from C:\Program Files\Microsoft Visual Studio\2022\Community\VC\vcpkg\vcpkg-bundle.json
[DEBUG] Bundle config: readonly=true, usegitregistry=true, embeddedsha=638b1588be3a265a9c7ad5b212cef72a1cad336a, deployment=VisualStudio, vsversion=17.0
[DEBUG] VS telemetry opted in at SOFTWARE\WOW6432Node\Microsoft\VSCommon\17.0\SQM\\OptIn
[DEBUG] Metrics enabled.
[DEBUG] Feature flag 'binarycaching' unset
[DEBUG] Feature flag 'compilertracking' unset
[DEBUG] Feature flag 'registries' unset
[DEBUG] Feature flag 'versions' unset
[DEBUG] Feature flag 'dependencygraph' unset
Downloading https://mirror.msys2.org/mingw/i686/mingw-w64-i686-libwinpthread-git-9.0.0.6373.5be8fcd83-1-any.pkg.tar.zst
Downloading https://repo.msys2.org/mingw/i686/mingw-w64-i686-libwinpthread-git-9.0.0.6373.5be8fcd83-1-any.pkg.tar.zst
Downloading https://repo.msys2.org/mingw/i686/mingw-w64-i686-libwinpthread-git-9.0.0.6373.5be8fcd83-1-any.pkg.tar.zst
Downloading https://repo.msys2.org/mingw/i686/mingw-w64-i686-libwinpthread-git-9.0.0.6373.5be8fcd83-1-any.pkg.tar.zst
Downloading https://repo.msys2.org/mingw/i686/mingw-w64-i686-libwinpthread-git-9.0.0.6373.5be8fcd83-1-any.pkg.tar.zst
Downloading https://repo.msys2.org/mingw/i686/mingw-w64-i686-libwinpthread-git-9.0.0.6373.5be8fcd83-1-any.pkg.tar.zst
error: Failed to download from mirror set
error: https://mirror.msys2.org/mingw/i686/mingw-w64-i686-libwinpthread-git-9.0.0.6373.5be8fcd83-1-any.pkg.tar.zst: failed: status code 404
error: https://repo.msys2.org/mingw/i686/mingw-w64-i686-libwinpthread-git-9.0.0.6373.5be8fcd83-1-any.pkg.tar.zst: failed: status code 404
error: https://repo.msys2.org/mingw/i686/mingw-w64-i686-libwinpthread-git-9.0.0.6373.5be8fcd83-1-any.pkg.tar.zst: failed: status code 404
error: https://repo.msys2.org/mingw/i686/mingw-w64-i686-libwinpthread-git-9.0.0.6373.5be8fcd83-1-any.pkg.tar.zst: failed: status code 404
error: https://repo.msys2.org/mingw/i686/mingw-w64-i686-libwinpthread-git-9.0.0.6373.5be8fcd83-1-any.pkg.tar.zst: failed: status code 404
error: https://repo.msys2.org/mingw/i686/mingw-w64-i686-libwinpthread-git-9.0.0.6373.5be8fcd83-1-any.pkg.tar.zst: failed: status code 404
[DEBUG] D:\a\_work\1\s\src\vcpkg\base\downloads.cpp(1003): 
[DEBUG] Time in subprocesses: 0us
[DEBUG] Time in parsing JSON: 14us
[DEBUG] Time in JSON reader: 0us
[DEBUG] Time in filesystem: 6072us
[DEBUG] Time in loading ports: 0us
[DEBUG] Exiting after 2.4 s (2388126us)

CMake Error at scripts/cmake/vcpkg_download_distfile.cmake:32 (message):
      
      Failed to download file with error: 1
      If you are using a proxy, please check your proxy setting. Possible causes are:
      
      1. You are actually using an HTTP proxy, but setting HTTPS_PROXY variable
         to `https://address:port`. This is not correct, because `https://` prefix
         claims the proxy is an HTTPS proxy, while your proxy (v2ray, shadowsocksr
         , etc..) is an HTTP proxy. Try setting `http://address:port` to both
         HTTP_PROXY and HTTPS_PROXY instead.
      
      2. If you are using Windows, vcpkg will automatically use your Windows IE Proxy Settings
         set by your proxy software. See https://github.com/microsoft/vcpkg-tool/pull/77
         The value set by your proxy might be wrong, or have same `https://` prefix issue.
      
      3. Your proxy's remote server is out of service.
      
      If you've tried directly download the link, and believe this is not a temporary
      download server failure, please submit an issue at https://github.com/Microsoft/vcpkg/issues
      to report this upstream download server failure.
      

Call Stack (most recent call first):
  scripts/cmake/vcpkg_download_distfile.cmake:270 (z_vcpkg_download_distfile_show_proxy_and_fail)
  scripts/cmake/vcpkg_acquire_msys.cmake:25 (vcpkg_download_distfile)
  scripts/cmake/vcpkg_acquire_msys.cmake:124 (z_vcpkg_acquire_msys_download_package)
  scripts/cmake/vcpkg_acquire_msys.cmake:209 (z_vcpkg_acquire_msys_download_packages)
  scripts/cmake/vcpkg_find_acquire_program(PKGCONFIG).cmake:22 (vcpkg_acquire_msys)
  scripts/cmake/vcpkg_find_acquire_program.cmake:118 (include)
  scripts/cmake/vcpkg_fixup_pkgconfig.cmake:203 (vcpkg_find_acquire_program)
  C:/Users/sudha/AppData/Local/vcpkg/registries/git-trees/537fe5086cb7cad8b2c24f4182b0777b81d7c175/portfile.cmake:94 (vcpkg_fixup_pkgconfig)
  scripts/ports.cmake:147 (include)



```


**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "dependencies": [
    "sqlite3"
  ],
  "builtin-baseline": "a58271747fd00c2f53342f20c2d9fd944fbaa386"
}

```
</details>
