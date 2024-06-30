#include "AudioClip.h" // 包含 AudioClip 类的声明
#include <iostream>    // 包含标准输入输出库
#include <time.h>      // 包含时间处理库
// Mci 类的构造函数，加载 winmm.dll 并获取函数地址
Mci::Mci() {
    hins = LoadLibraryA("winmm.dll"); // 加载 winmm.dll 库
    wmci = (w32mci) GetProcAddress(hins, "mciSendStringA"); // 获取 mciSendStringA 函数的地址
    wmcierror = (w32mcierror) GetProcAddress(hins, "mciGetErrorStringA"); // 获取 mciGetErrorStringA 函数的地址
}
// Mci 类的析构函数，释放 winmm.dll
Mci::~Mci() {
    FreeLibrary(hins); // 释放 winmm.dll 库
}
// 发送 MCI 命令并返回执行结果
bool Mci::send(std::string command) {
    int errcode = wmci(command.c_str(), buf, 254, 0); // 发送 MCI 命令
    if (errcode) { // 如果命令执行出错
        wmcierror(errcode, buf, 254); // 获取错误信息
        return false; // 返回 false
    }
    return true; // 如果命令执行成功，返回 true
}
// AudioClip 类的构造函数，不执行任何操作
AudioClip::AudioClip() {
    // 空构造函数，不执行任何操作
}
// AudioClip 类的析构函数，关闭音频
AudioClip::~AudioClip() {
    std::string cmd;
    cmd = "close " + alias; // 构建关闭音频的 MCI 命令
    mci.send(cmd); // 发送命令关闭音频
}
// 加载音频文件并设置 MCI 参数
bool AudioClip::load(const std::string& _filename) {
    filename = _filename; // 保存音频文件名
    // 替换文件路径中的斜杠为反斜杠
    for (unsigned int i = 0; i < filename.length(); i++) {
        if (filename[i] == '/') {
            filename[i] = '\\';
        }
    }
    // 生成别名
    alias = "mp3_";
    srand(time(NULL)); // 用当前时间初始化随机数种子
    char randstr[6];
    _itoa(rand() % 65536, randstr, 10); // 生成随机数字符串
    alias.append(randstr); // 将随机数字符串添加到别名
    std::string cmd;
    cmd = "open " + filename + " alias " + alias; // 构建打开音频文件的 MCI 命令
    if (mci.send(cmd) == false) // 发送命令打开音频文件
        return false;
    cmd = "set " + alias + " time format milliseconds"; // 设置时间格式为毫秒
    if (mci.send(cmd) == false)
        return false;
    cmd = "status " + alias + " length"; // 获取音频长度
    if (mci.send(cmd) == false)
        return false;
    length_ms = atoi(mci.buf); // 将 MCI 返回的长度字符串转换为整数
    return true; // 返回 true 表示加载成功
}
// 播放音频的指定部分
bool AudioClip::play(int start_ms, int end_ms) {
    if (end_ms == -1) end_ms = length_ms; // 如果没有指定结束时间，则播放到音频末尾
    std::string cmd;
    char start_str[16], end_str[16];
    _itoa(start_ms, start_str, 10); // 将开始时间转换为字符串
    _itoa(end_ms, end_str, 10); // 将结束时间转换为字符串
    cmd = "play " + alias + " from ";
    cmd.append(start_str);
    cmd.append(" to ");
    cmd.append(end_str); // 构建播放音频的 MCI 命令
    return mci.send(cmd); // 发送命令播放音频
}
// 停止播放音频
bool AudioClip::stop() {
    std::string cmd;
    cmd = "stop " + alias; // 构建停止播放的 MCI 命令
    if (mci.send(cmd) == false)
        return false;
    cmd = "seek " + alias + " to start"; // 构建将播放位置设置到开始的 MCI 命令
    return mci.send(cmd); // 发送命令
}
// 暂停播放音频
bool AudioClip::pause() {
    std::string cmd;
    cmd = "pause " + alias; // 构建暂停播放的 MCI 命令
    return mci.send(cmd); // 发送命令
}
// 恢复播放音频
bool AudioClip::unpause() {
    std::string cmd;
    cmd = "resume " + alias; // 构建恢复播放的 MCI 命令
    return mci.send(cmd); // 发送命令
}
// 返回音频的总长度（毫秒）
int AudioClip::milliseconds() {
    return length_ms; // 返回音频长度
}