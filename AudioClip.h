// AudioClip.h - 头文件定义了操作音频文件的类
#include<string> // 包含标准字符串库，用于字符串操作
#include<Windows.h> // 包含Windows API，用于访问Windows系统功能
// 声明MCI命令发送函数的类型
typedef int(__stdcall * w32mci)(const char*, char*, int, int); // 定义MCI命令发送函数的指针类型
// 声明MCI错误信息获取函数的类型
typedef int(__stdcall * w32mcierror)(int, char*, int); // 定义MCI错误信息获取函数的指针类型
// Mci类封装了MCI命令的发送和错误处理
class Mci {
private:
    HINSTANCE hins; // MCI库的实例句柄，用于加载和访问winmm.dll
    w32mci wmci; // MCI命令发送函数，用于发送MCI命令到winmm.dll
    w32mcierror wmcierror; // MCI错误信息获取函数，用于获取错误信息
public:
    // 构造函数，初始化MCI
    Mci();
    // 析构函数，清理MCI资源
    ~Mci();
    // 缓冲区，用于存储错误信息
    char buf[256]; // 定义一个256字节的字符数组，用于存储MCI命令的错误信息
    // 发送MCI命令，如果发生错误返回false
    bool send(std::string command); // 发送MCI命令的成员函数声明
};
// AudioClip类用于管理音频文件的播放
class AudioClip {
private:
    Mci mci; // Mci类的实例，用于发送MCI命令控制音频播放
    std::string filename; // 音频文件的完整路径
    std::string alias; // 音频文件的别名，用于MCI命令中标识特定的音频
    int length_ms; // 音频文件的总长度，单位为毫秒
public:
    // 构造函数，初始化AudioClip
    AudioClip();
    // 析构函数，清理资源
    ~AudioClip();
    // 加载音频文件，为播放做准备
    bool load(const std::string& _filename);
    // 播放音频，可以指定起始和结束时间
    bool play(int start_ms = 0, int end_ms = -1);
    // 停止播放音频
    bool stop();
    // 暂停播放音频
    bool pause();
    // 继续播放（从暂停状态）
    bool unpause();
    // 返回音频文件的总长度，单位为毫秒
    int milliseconds();
};