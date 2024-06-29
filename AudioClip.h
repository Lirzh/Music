// AudioClip.h - 头文件定义了操作音频文件的类
#include<string> // 包含标准字符串库
#include<Windows.h> // 包含Windows API
// 声明MCI命令发送函数的类型
typedef int(__stdcall * w32mci)(const char*, char*, int, int);
// 声明MCI错误信息获取函数的类型
typedef int(__stdcall * w32mcierror)(int, char*, int);
// Mci类封装了MCI命令的发送和错误处理
class Mci {
private:
    HINSTANCE hins; // MCI库的实例句柄
    w32mci wmci; // MCI命令发送函数
    w32mcierror wmcierror; // MCI错误信息获取函数
public:
    // 构造函数，初始化MCI
    Mci();
    // 析构函数，清理MCI资源
    ~Mci();
    // 缓冲区，用于存储错误信息
    char buf[256];
    // 发送MCI命令，如果发生错误返回false
    bool send(std::string command);
};
// AudioClip类用于管理音频文件的播放
class AudioClip {
private:
    Mci mci; // Mci类的实例，用于发送MCI命令
    std::string filename; // 音频文件的路径
    std::string alias; // 音频文件的别名
    int length_ms; // 音频文件的总长度（毫秒）
public:
    // 构造函数，初始化AudioClip
    AudioClip();
    // 析构函数，清理资源
    ~AudioClip();
    // 加载音频文件
    bool load(const std::string& _filename);
    // 播放音频，可以指定起始和结束时间
    bool play(int start_ms = 0, int end_ms = -1);
    // 停止播放
    bool stop();
    // 暂停播放
    bool pause();
    // 继续播放（从暂停状态）
    bool unpause();
    // 返回总的毫秒数
    int milliseconds();
};
