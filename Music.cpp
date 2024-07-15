#include <iostream> // ��׼�����������
#include <filesystem> // �ļ�ϵͳ�⣬�����ļ���Ŀ¼����
#include <conio.h> // ����Windowsϵͳ��ʹ�ã����ڷǱ�׼��������
#include "AudioClip.cpp" // �����Զ����AudioClip��ʵ���ļ�
#define endl '\n' // ����endlΪ���з����򻯴���
namespace fs = std::filesystem; // Ϊ�ļ�ϵͳ�����ñ���fs
using namespace std; // ʹ�ñ�׼�����ռ�
// �����������������ڼ��ز���������
void loadAndPlay(string[], AudioClip &);
// ��������������������ʾ���Ž���
void showProgress(AudioClip &, bool);
// �����뻻Ϊ�ֺ��� 
string mins(int mu);
string mus[1048576]; // �洢�����ļ���·��
unsigned int num = 0; // ���������ļ�������
int now = 1; // ��ǰ���ŵ���������
int main() {
    // ���ع��
    fputs("\e[?25l", stdout);
    AudioClip music; // ����AudioClip���ʵ��
    try {
        // ����"Music"�ļ����е�����.mp3�ļ�
        for (const auto& entry : fs::directory_iterator("Music")) {
            if (entry.path().extension() == ".mp3") {
                mus[++num] = entry.path().string();
            }
        }
    } catch (const fs::filesystem_error& e) {
        // �����ļ�ϵͳ����
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
    // ���ز����ŵ�һ������
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
    bool isPlaying = true; // ����״̬
    while (true) {
        if (_kbhit()) {
            char key = _getch();
            switch (key) {
                case ' ':
                    // �л�����/��ͣ״̬
                    isPlaying = !isPlaying;
                    if (isPlaying) music.unpause();
                    else music.pause();
                    break;
                case 'A':
                case 'a':
                case 'D':
                case 'd':
                    // �ı��������������¼��ز���
                    now = (key == 'A' || key == 'a') ? (now == 1 ? num : now - 1) : (now == num ? 1 : now + 1);
                    loadAndPlay(mus, music);
                    break;
                case 27: // ESC��
                    // ֹͣ���Ų��˳�����
                    music.stop();
                    cout <<endl<< "Bye." << endl;
                    return 0;
                default:
                    break;
            }
        }
        // ��ʾ���Ž���
        showProgress(music, isPlaying);
        Sleep(500); // ����500����
    }
}
// ʵ�ָ������������ڼ��ز���������
void loadAndPlay(string mus[], AudioClip &music) {
    system("cls"); // ����
    cout << "Playing... Press 'Space' to pause/resume, 'ESC' to stop, 'A' or 'a' for the previous track, 'D' or 'd' for the next track." << endl;
    cout << "Now: " << mus[now] << endl;
    music.stop(); // ֹͣ��ǰ���ŵ�����
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
// ʵ�ָ���������������ʾ���Ž���
void showProgress(AudioClip &music, bool isPlaying) {
    cout << "   ";
    for (int i = 1; i <= 150; ++i) cout << '\b'; // �����ǰ��
    if (isPlaying) cout << "Mode: O  "; // ��ʾ����ģʽ
    else cout << "Mode: X  ";
    int progress = 50 * music.nowseconds() / music.milliseconds(); // �������
    for (int i = 0; i < progress; ++i) cout << '='; // ��ʾ������
    cout << '>'; // ��ǰ����λ��
    for (int i = progress + 1; i <= 50; ++i) cout << '-'; // ʣ�ಿ��
    cout << ' ' << mins(music.nowseconds())<<" / "<<mins(music.milliseconds()); // ��ʾ��ǰ����ʱ�����ʱ��
    if (music.nowseconds() >= music.milliseconds()) {
        // �����ǰ����ʱ����ڻ򳬹���ʱ�䣬�Զ�������һ��
        now = (now == num) ? 1 : now + 1;
        loadAndPlay(mus, music); // ���¼��ز�������һ������
    }
}
string mins(int mu){
	mu/=1000;
	if(mu<60)return to_string(mu)+'s';
	else return to_string(mu/60)+"min"+to_string(mu%60)+'s';
}
