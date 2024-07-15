#include <iostream> // 标准输入输出流库
#include <filesystem> // 文件系统库，用于文件和目录操作
#include <conio.h> // 仅在Windows系统下使用，用于非标准键盘输入
#include "AudioClip.cpp" // 包含自定义的AudioClip类实现文件
#define endl '\n' // 定义endl为换行符，简化代码
namespace fs = std::filesystem; // 为文件系统库设置别名fs
using namespace std; // 使用标准命名空间
// 声明辅助函数，用于加载并播放音乐
void loadAndPlay(string[], AudioClip &);
// 声明辅助函数，用于显示播放进度
void showProgress(AudioClip &, bool);
// 将毫秒换为分和秒 
string mins(int mu);
string mus[1048576]; // 存储音乐文件的路径
unsigned int num = 0; // 计数音乐文件的数量
int now = 1; // 当前播放的音乐索引
int main() {
    // 隐藏光标
    fputs("\e[?25l", stdout);
    AudioClip music; // 创建AudioClip类的实例
    try {
        // 遍历"Music"文件夹中的所有.mp3文件
        for (const auto& entry : fs::directory_iterator("Music")) {
            if (entry.path().extension() == ".mp3") {
                mus[++num] = entry.path().string();
            }
        }
    } catch (const fs::filesystem_error& e) {
        // 捕获文件系统错误
        cerr << "Error: " << e.what() << endl;
        cout << "Press any key to exit." << endl;
        _getch();
        return 1;
    }
    if (num == 0) {
        cout << "No music files found." << endl;
        cout << "Press any key to exit." << endl;
        _getch();
        return 1;
    }
    // 加载并播放第一首音乐
    if (!music.load(mus[now])) {
        cerr << "Failed to load the audio file." << endl;
        cout << "Press any key to exit." << endl;
        _getch();
        return 1;
    }
    if (!music.play()) {
        cerr << "Failed to start playback." << endl;
        cout << "Press any key to exit." << endl;
        _getch();
        return 1;
    }
    cout << "Playing... Press 'Space' to pause/resume, 'ESC' to stop, 'A' or 'a' for the previous track, 'D' or 'd' for the next track." << endl;
    cout << "Now: " << mus[now] << endl;
    bool isPlaying = true; // 播放状态
    while (true) {
        if (_kbhit()) {
            char key = _getch();
            switch (key) {
                case ' ':
                    // 切换播放/暂停状态
                    isPlaying = !isPlaying;
                    if (isPlaying) music.unpause();
                    else music.pause();
                    break;
                case 'A':
                case 'a':
                case 'D':
                case 'd':
                    // 改变音乐索引并重新加载播放
                    now = (key == 'A' || key == 'a') ? (now == 1 ? num : now - 1) : (now == num ? 1 : now + 1);
                    loadAndPlay(mus, music);
                    break;
                case 27: // ESC键
                    // 停止播放并退出程序
                    music.stop();
                    cout <<endl<< "Bye." << endl;
                    return 0;
                default:
                    break;
            }
        }
        // 显示播放进度
        showProgress(music, isPlaying);
        Sleep(500); // 休眠500毫秒
    }
}
// 实现辅助函数，用于加载并播放音乐
void loadAndPlay(string mus[], AudioClip &music) {
    system("cls"); // 清屏
    cout << "Playing... Press 'Space' to pause/resume, 'ESC' to stop, 'A' or 'a' for the previous track, 'D' or 'd' for the next track." << endl;
    cout << "Now: " << mus[now] << endl;
    music.stop(); // 停止当前播放的音乐
    if (!music.load(mus[now])) {
        cerr << "Failed to load the audio file." << endl;
        cout << "Press any key to exit." << endl;
        _getch();
        exit(1);
    }
    if (!music.play()) {
        cerr << "Failed to start playback." << endl;
        cout << "Press any key to exit." << endl;
        _getch();
        exit(1);
    }
}
// 实现辅助函数，用于显示播放进度
void showProgress(AudioClip &music, bool isPlaying) {
    cout << "   ";
    for (int i = 1; i <= 150; ++i) cout << '\b'; // 清除当前行
    if (isPlaying) cout << "Mode: O  "; // 显示播放模式
    else cout << "Mode: X  ";
    int progress = 50 * music.nowseconds() / music.milliseconds(); // 计算进度
    for (int i = 0; i < progress; ++i) cout << '='; // 显示进度条
    cout << '>'; // 当前播放位置
    for (int i = progress + 1; i <= 50; ++i) cout << '-'; // 剩余部分
    cout << ' ' << mins(music.nowseconds())<<" / "<<mins(music.milliseconds()); // 显示当前播放时间和总时间
    if (music.nowseconds() >= music.milliseconds()) {
        // 如果当前播放时间等于或超过总时间，自动播放下一首
        now = (now == num) ? 1 : now + 1;
        loadAndPlay(mus, music); // 重新加载并播放下一首音乐
    }
}
string mins(int mu){
	mu/=1000;
	if(mu<60)return to_string(mu)+'s';
	else return to_string(mu/60)+"min"+to_string(mu%60)+'s';
}
