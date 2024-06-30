#include "AudioClip.h" // ���� AudioClip �������
#include <iostream>    // ������׼���������
#include <time.h>      // ����ʱ�䴦���
// Mci ��Ĺ��캯�������� winmm.dll ����ȡ������ַ
Mci::Mci() {
    hins = LoadLibraryA("winmm.dll"); // ���� winmm.dll ��
    wmci = (w32mci) GetProcAddress(hins, "mciSendStringA"); // ��ȡ mciSendStringA �����ĵ�ַ
    wmcierror = (w32mcierror) GetProcAddress(hins, "mciGetErrorStringA"); // ��ȡ mciGetErrorStringA �����ĵ�ַ
}
// Mci ��������������ͷ� winmm.dll
Mci::~Mci() {
    FreeLibrary(hins); // �ͷ� winmm.dll ��
}
// ���� MCI �������ִ�н��
bool Mci::send(std::string command) {
    int errcode = wmci(command.c_str(), buf, 254, 0); // ���� MCI ����
    if (errcode) { // �������ִ�г���
        wmcierror(errcode, buf, 254); // ��ȡ������Ϣ
        return false; // ���� false
    }
    return true; // �������ִ�гɹ������� true
}
// AudioClip ��Ĺ��캯������ִ���κβ���
AudioClip::AudioClip() {
    // �չ��캯������ִ���κβ���
}
// AudioClip ��������������ر���Ƶ
AudioClip::~AudioClip() {
    std::string cmd;
    cmd = "close " + alias; // �����ر���Ƶ�� MCI ����
    mci.send(cmd); // ��������ر���Ƶ
}
// ������Ƶ�ļ������� MCI ����
bool AudioClip::load(const std::string& _filename) {
    filename = _filename; // ������Ƶ�ļ���
    // �滻�ļ�·���е�б��Ϊ��б��
    for (unsigned int i = 0; i < filename.length(); i++) {
        if (filename[i] == '/') {
            filename[i] = '\\';
        }
    }
    // ���ɱ���
    alias = "mp3_";
    srand(time(NULL)); // �õ�ǰʱ���ʼ�����������
    char randstr[6];
    _itoa(rand() % 65536, randstr, 10); // ����������ַ���
    alias.append(randstr); // ��������ַ�����ӵ�����
    std::string cmd;
    cmd = "open " + filename + " alias " + alias; // ��������Ƶ�ļ��� MCI ����
    if (mci.send(cmd) == false) // �����������Ƶ�ļ�
        return false;
    cmd = "set " + alias + " time format milliseconds"; // ����ʱ���ʽΪ����
    if (mci.send(cmd) == false)
        return false;
    cmd = "status " + alias + " length"; // ��ȡ��Ƶ����
    if (mci.send(cmd) == false)
        return false;
    length_ms = atoi(mci.buf); // �� MCI ���صĳ����ַ���ת��Ϊ����
    return true; // ���� true ��ʾ���سɹ�
}
// ������Ƶ��ָ������
bool AudioClip::play(int start_ms, int end_ms) {
    if (end_ms == -1) end_ms = length_ms; // ���û��ָ������ʱ�䣬�򲥷ŵ���Ƶĩβ
    std::string cmd;
    char start_str[16], end_str[16];
    _itoa(start_ms, start_str, 10); // ����ʼʱ��ת��Ϊ�ַ���
    _itoa(end_ms, end_str, 10); // ������ʱ��ת��Ϊ�ַ���
    cmd = "play " + alias + " from ";
    cmd.append(start_str);
    cmd.append(" to ");
    cmd.append(end_str); // ����������Ƶ�� MCI ����
    return mci.send(cmd); // �����������Ƶ
}
// ֹͣ������Ƶ
bool AudioClip::stop() {
    std::string cmd;
    cmd = "stop " + alias; // ����ֹͣ���ŵ� MCI ����
    if (mci.send(cmd) == false)
        return false;
    cmd = "seek " + alias + " to start"; // ����������λ�����õ���ʼ�� MCI ����
    return mci.send(cmd); // ��������
}
// ��ͣ������Ƶ
bool AudioClip::pause() {
    std::string cmd;
    cmd = "pause " + alias; // ������ͣ���ŵ� MCI ����
    return mci.send(cmd); // ��������
}
// �ָ�������Ƶ
bool AudioClip::unpause() {
    std::string cmd;
    cmd = "resume " + alias; // �����ָ����ŵ� MCI ����
    return mci.send(cmd); // ��������
}
// ������Ƶ���ܳ��ȣ����룩
int AudioClip::milliseconds() {
    return length_ms; // ������Ƶ����
}