#include "AudioClip.cpp" // 包含自定义的AudioClip类实现文件，这个类可能包含音频播放的相关功能
#include <conio.h>      // 包含conio.h库，用于键盘输入，这个库是针对Windows系统的
#include <filesystem>   // 包含文件系统库，用于文件路径操作
#include <bits/stdc++.h> // 包含标准C++库的所有头文件
#define endl '\n'       // 定义endl为换行符，简化代码
#define fir(a,b) for(int i=(a);i<=(b);i++) // 定义一个宏，用于简化for循环的写法
namespace fs = std::filesystem; // 为文件系统库设置别名fs，简化代码
using namespace std; // 使用标准命名空间
AudioClip music; // 创建AudioClip类的实例，用于音频播放
string mus[1048576]={}; // 定义一个字符串数组，用于存储音乐文件的路径
unsigned int num=0; // 定义一个无符号整数变量，用于计数音乐文件的数量
int main(){
    int now=1; // 初始化当前播放的音乐索引为1
    try {
        // 使用范围for循环遍历当前目录下的"Music"文件夹中的所有文件
        for (const auto& entry : fs::directory_iterator(fs::current_path().string()+"\\Music"))
            mus[++num]=entry.path().string();
    } catch (const fs::filesystem_error& e) {
        // 捕获并输出文件系统错误信息
        cerr << "Wrong: " << e.what() << endl;
        return 1; // 如果出现错误，返回错误代码1
    }
    // 尝试加载音频文件，如果加载失败，输出错误信息并返回错误代码1
    if (!music.load(mus[now])) {
        cerr << "Failed to load audio file or don't have any music." << endl;
        return 1;
    }
    // 尝试开始播放音频，如果播放失败，输出错误信息并返回错误代码1
    if (!music.play()) {
        cerr << "Failed to start playback." << endl;
        return 1;
    }
    // 清屏
    system("cls");
    // 向用户显示控制指令
    cout << "Playing... Press 'Space' to pause/resume, 'ESC' to stop, 'A' or 'a' for the previous track, 'D' or 'd' for the next track." << endl;
    cout << "Now: "<<mus[now]<<endl;
    cout << "Mode: O"; // 显示播放模式，这里显示为"O"，可能是一个占位符
    bool isPlaying = false; // 初始化播放状态变量
    // 无限循环，用于处理用户输入
    while (1)
        switch (_getch()) { // 根据用户按键执行相应操作
            case ' ': // 如果用户按下空格键
                // 切换播放状态，并更新控制台显示
                isPlaying = !isPlaying;
                if (isPlaying) {
                    cout << "\bX";
                    music.pause(); // 调用AudioClip类的pause方法暂停播放
                } else {
                    cout << "\bO";
                    music.unpause(); // 调用AudioClip类的unpause方法恢复播放
                }
                break;	
            case 'a': case 'A': // 如果用户按下'A'或'a'键
                // 切换到上一首音乐
                now=(now==1)?num:(now-1);
                // 清屏并重新显示控制指令和当前播放的音乐
                system("cls");
                cout << "Playing... Press 'Space' to pause/resume, 'ESC' to stop, 'A' or 'a' for the previous track, 'D' or 'd' for the next track." << endl;
                cout << "Now: "<<mus[now]<<endl;
                cout << "Mode: O";
                music.stop(); // 停止当前播放的音乐
                // 重新加载并播放上一首音乐
                if (!music.load(mus[now])) {
                    cerr << "Failed to load audio file." << endl;
                    return 1;
                }
                if (!music.play()) {
                    cerr << "Failed to start playback." << endl;
                    return 1;
                }
                break;
            case 'D': case 'd': // 如果用户按下'D'或'd'键
                // 切换到下一首音乐
                now=(now==num)?1:(now+1);
                // 清屏并重新显示控制指令和当前播放的音乐
                system("cls");
                cout << "Playing... Press 'Space' to pause/resume, 'ESC' to stop, 'A' or 'a' for the previous track, 'D' or 'd' for the next track." << endl;
                cout << "Now: "<<mus[now]<<endl;
                cout << "Mode: O";
                music.stop(); // 停止当前播放的音乐
                // 重新加载并播放下一首音乐
                if (!music.load(mus[now])) {
                    cerr << "Failed to load audio file." << endl;
                    return 1;
                }
                if (!music.play()) {
                    cerr << "Failed to start playback." << endl;
                    return 1;
                }
                break;
            case 27: // 如果用户按下ESC键（ASCII码为27）
                // 停止播放音频并退出程序
                music.stop();
                return 0;
            default:
                break; // 对于其他按键不做任何操作
        }
    // 停止播放（虽然在这个程序中不会执行到这里）
    music.stop();
    // 正常退出程序（虽然在这个程序中不会执行到这里）
    return 0;
}