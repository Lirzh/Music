#include "AudioClip.cpp"
#include <conio.h>
AudioClip bulabula;
using namespace std;
// 绘制进度条的函数
void drawProgressBar(int current, int total, int barWidth = 50) {
    float progress = (float)current / total;
    int pos = (int)(progress * barWidth);
    cout << "\r[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) cout << "=";
        else if (i == pos) cout << ">";
        else cout << " ";
    }
    cout << "] " << (int)(progress * 100) << "%  ";
    cout.flush(); // 确保立即输出
}
int main() {
    if (!bulabula.load("Music/Newboy.mp3")) {
        cerr << "Failed to load audio file." << endl;
        return 1;
    }
    if (!bulabula.play()) {
        cerr << "Failed to start playback." << endl;
        return 1;
    }
    cout << "Playing... Press 'SPACE' to pause/resume, 'ESC' to stop." << endl;
    int totalLength = bulabula.milliseconds();
    int currentPos = 0;
    bool isPlaying = false;
    while (1) {
		char key=_kbhit()?' ':_getch();
		if (!isPlaying) {
            Sleep(100); // 暂停100ms来更新进度条，避免CPU过载
            currentPos = bulabula.millnow();
			drawProgressBar(currentPos, totalLength);
        }
        switch (key) {
            case ' ':
            	isPlaying=!isPlaying;
                if (isPlaying) {
                    cout << " \b\bX"; // 替换播放指示符为暂停指示符
                	bulabula.pause();
				} else {
                    cout << " \b\bO"; // 替换暂停指示符为播放指示符
                	bulabula.unpause();
				}
                break;
            case 27: // ESC key
                bulabula.stop();
                return 0;
                break;
            default:
                break;
        }
    }
    bulabula.stop(); // 停止播放
    cout << "\b" << "Playback stopped." << endl; // 清除进度条并打印停止信息
    return 0;
}