There is a tool for inspecting Qt apps - [GammaRay from KDAB](https://github.com/KDAB/GammaRay)
It can be very convenient to investigate `QQuickItem`s hierarchy in the app.

## Building

Steps to build it (inside cloned src folder):
```
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=PATH_TO_YOUR_QT_FOLDER/5.11.2/gcc_64/lib/cmake ..
make
make install
```

After build `gammaray` binary can be found in `GAMMARAY_PATH/build/bin`


## Usage
1. Start `gammaray`
2. Open `Attach` tab in `gammaray` UI
3. Set `Access mode` combo box to `Out-of-process, local debugging only`
4. Start your `react-native-desktop` application
5. Wait until it is listed in processes on gammaray `Attach` screen
6. Press `Attach` button
7. Wait until `gammaray` attached
8. Select `Quick scenes`tab and inspect QQuickItems hierarchy in your app

## Troubleshooting
### For Ubuntu
If you get a error on attaching `gammaray` to app, here is what can be done:

1) edit `/etc/sysctl.d/10-ptrace.conf` and change the line:
`kernel.yama.ptrace_scope = 1`
To
`kernel.yama.ptrace_scope = 0`

2) To apply changes call `sudo service procps restart`
