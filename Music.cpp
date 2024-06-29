#include "AudioClip.cpp"
#include <conio.h>
AudioClip bulabula;
using namespace std;
// ���ƽ������ĺ���
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
    cout.flush(); // ȷ���������
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
            Sleep(100); // ��ͣ100ms�����½�����������CPU����
            currentPos = bulabula.millnow();
			drawProgressBar(currentPos, totalLength);
        }
        switch (key) {
            case ' ':
            	isPlaying=!isPlaying;
                if (isPlaying) {
                    cout << " \b\bX"; // �滻����ָʾ��Ϊ��ָͣʾ��
                	bulabula.pause();
				} else {
                    cout << " \b\bO"; // �滻��ָͣʾ��Ϊ����ָʾ��
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
    bulabula.stop(); // ֹͣ����
    cout << "\b" << "Playback stopped." << endl; // �������������ӡֹͣ��Ϣ
    return 0;
}