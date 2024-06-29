// AudioClip.cpp
#include "AudioClip.h"
#include <iostream>
#include <time.h>
// Mci 类的构造函数，加载 winmm.dll 并获取函数地址
Mci::Mci() {
    hins = LoadLibraryA("winmm.dll");
    wmci = (w32mci) GetProcAddress(hins, "mciSendStringA");
    wmcierror = (w32mcierror) GetProcAddress(hins, "mciGetErrorStringA");
}
// Mci 类的析构函数，释放 winmm.dll
Mci::~Mci() {
    FreeLibrary(hins);
}
// 发送 MCI 命令并返回执行结果
bool Mci::send(std::string command) {
    int errcode = wmci(command.c_str(), buf, 254, 0);
    if (errcode) {
        wmcierror(errcode, buf, 254);
        return false;
    }
    return true;
}
// AudioClip 类的构造函数，不执行任何操作
AudioClip::AudioClip() {
    // do nothing
}
// AudioClip 类的析构函数，关闭音频
AudioClip::~AudioClip() {
    std::string cmd;
    cmd = "close " + alias;
    mci.send(cmd);
}
// 加载音频文件并设置 MCI 参数
bool AudioClip::load(const std::string& _filename) {
    filename = _filename;
    // 替换文件路径中的斜杠
    for (unsigned int i = 0; i < filename.length(); i++) {
        if (filename[i] == '/') {
            filename[i] = '\\';
        }
    }
    // 生成别名
    alias = "mp3_";
    srand(time(NULL));
    char randstr[6];
    _itoa(rand() % 65536, randstr, 10);
    alias.append(randstr);
    std::string cmd;
    cmd = "open " + filename + " alias " + alias;
    if (mci.send(cmd) == false)
        return false;
    cmd = "set " + alias + " time format milliseconds";
    if (mci.send(cmd) == false)
        return false;
    cmd = "status " + alias + " length";
    if (mci.send(cmd) == false)
        return false;
    length_ms = atoi(mci.buf); // 转换长度字符串为整数
    return true;
}
// 播放音频的指定部分
bool AudioClip::play(int start_ms, int end_ms) {
    if (end_ms == -1) end_ms = length_ms;
    std::string cmd;
    char start_str[16], end_str[16];
    _itoa(start_ms, start_str, 10);
    _itoa(end_ms, end_str, 10);
    cmd = "play " + alias + " from ";
    cmd.append(start_str);
    cmd.append(" to ");
    cmd.append(end_str);
    return mci.send(cmd);
}
// 停止播放音频
bool AudioClip::stop() {
    std::string cmd;
    cmd = "stop " + alias;
    if (mci.send(cmd) == false)
        return false;
    cmd = "seek " + alias + " to start";
    return mci.send(cmd);
}
// 暂停播放音频
bool AudioClip::pause() {
    std::string cmd;
    cmd = "pause " + alias;
    return mci.send(cmd);
}
// 恢复播放音频
bool AudioClip::unpause() {
    std::string cmd;
    cmd = "resume " + alias;
    return mci.send(cmd);
}
// 返回音频的总长度（毫秒）
int AudioClip::milliseconds() {
    return length_ms;
}