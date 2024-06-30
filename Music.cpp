#include "AudioClip.cpp" // �����Զ����AudioClip��ʵ���ļ����������ܰ�����Ƶ���ŵ���ع���
#include <conio.h>      // ����conio.h�⣬���ڼ������룬����������Windowsϵͳ��
#include <filesystem>   // �����ļ�ϵͳ�⣬�����ļ�·������
#include <bits/stdc++.h> // ������׼C++�������ͷ�ļ�
#define endl '\n'       // ����endlΪ���з����򻯴���
#define fir(a,b) for(int i=(a);i<=(b);i++) // ����һ���꣬���ڼ�forѭ����д��
namespace fs = std::filesystem; // Ϊ�ļ�ϵͳ�����ñ���fs���򻯴���
using namespace std; // ʹ�ñ�׼�����ռ�
AudioClip music; // ����AudioClip���ʵ����������Ƶ����
string mus[1048576]={}; // ����һ���ַ������飬���ڴ洢�����ļ���·��
unsigned int num=0; // ����һ���޷����������������ڼ��������ļ�������
int main(){
    int now=1; // ��ʼ����ǰ���ŵ���������Ϊ1
    try {
        // ʹ�÷�Χforѭ��������ǰĿ¼�µ�"Music"�ļ����е������ļ�
        for (const auto& entry : fs::directory_iterator(fs::current_path().string()+"\\Music"))
            mus[++num]=entry.path().string();
    } catch (const fs::filesystem_error& e) {
        // ��������ļ�ϵͳ������Ϣ
        cerr << "Wrong: " << e.what() << endl;
        return 1; // ������ִ��󣬷��ش������1
    }
    // ���Լ�����Ƶ�ļ����������ʧ�ܣ����������Ϣ�����ش������1
    if (!music.load(mus[now])) {
        cerr << "Failed to load audio file or don't have any music." << endl;
        return 1;
    }
    // ���Կ�ʼ������Ƶ���������ʧ�ܣ����������Ϣ�����ش������1
    if (!music.play()) {
        cerr << "Failed to start playback." << endl;
        return 1;
    }
    // ����
    system("cls");
    // ���û���ʾ����ָ��
    cout << "Playing... Press 'Space' to pause/resume, 'ESC' to stop, 'A' or 'a' for the previous track, 'D' or 'd' for the next track." << endl;
    cout << "Now: "<<mus[now]<<endl;
    cout << "Mode: O"; // ��ʾ����ģʽ��������ʾΪ"O"��������һ��ռλ��
    bool isPlaying = false; // ��ʼ������״̬����
    // ����ѭ�������ڴ����û�����
    while (1)
        switch (_getch()) { // �����û�����ִ����Ӧ����
            case ' ': // ����û����¿ո��
                // �л�����״̬�������¿���̨��ʾ
                isPlaying = !isPlaying;
                if (isPlaying) {
                    cout << "\bX";
                    music.pause(); // ����AudioClip���pause������ͣ����
                } else {
                    cout << "\bO";
                    music.unpause(); // ����AudioClip���unpause�����ָ�����
                }
                break;	
            case 'a': case 'A': // ����û�����'A'��'a'��
                // �л�����һ������
                now=(now==1)?num:(now-1);
                // ������������ʾ����ָ��͵�ǰ���ŵ�����
                system("cls");
                cout << "Playing... Press 'Space' to pause/resume, 'ESC' to stop, 'A' or 'a' for the previous track, 'D' or 'd' for the next track." << endl;
                cout << "Now: "<<mus[now]<<endl;
                cout << "Mode: O";
                music.stop(); // ֹͣ��ǰ���ŵ�����
                // ���¼��ز�������һ������
                if (!music.load(mus[now])) {
                    cerr << "Failed to load audio file." << endl;
                    return 1;
                }
                if (!music.play()) {
                    cerr << "Failed to start playback." << endl;
                    return 1;
                }
                break;
            case 'D': case 'd': // ����û�����'D'��'d'��
                // �л�����һ������
                now=(now==num)?1:(now+1);
                // ������������ʾ����ָ��͵�ǰ���ŵ�����
                system("cls");
                cout << "Playing... Press 'Space' to pause/resume, 'ESC' to stop, 'A' or 'a' for the previous track, 'D' or 'd' for the next track." << endl;
                cout << "Now: "<<mus[now]<<endl;
                cout << "Mode: O";
                music.stop(); // ֹͣ��ǰ���ŵ�����
                // ���¼��ز�������һ������
                if (!music.load(mus[now])) {
                    cerr << "Failed to load audio file." << endl;
                    return 1;
                }
                if (!music.play()) {
                    cerr << "Failed to start playback." << endl;
                    return 1;
                }
                break;
            case 27: // ����û�����ESC����ASCII��Ϊ27��
                // ֹͣ������Ƶ���˳�����
                music.stop();
                return 0;
            default:
                break; // �����������������κβ���
        }
    // ֹͣ���ţ���Ȼ����������в���ִ�е����
    music.stop();
    // �����˳�������Ȼ����������в���ִ�е����
    return 0;
}