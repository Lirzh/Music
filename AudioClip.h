// AudioClip.h - ͷ�ļ������˲�����Ƶ�ļ�����
#include<string> // ������׼�ַ�����
#include<Windows.h> // ����Windows API
// ����MCI����ͺ���������
typedef int(__stdcall * w32mci)(const char*, char*, int, int);
// ����MCI������Ϣ��ȡ����������
typedef int(__stdcall * w32mcierror)(int, char*, int);
// Mci���װ��MCI����ķ��ͺʹ�����
class Mci {
private:
    HINSTANCE hins; // MCI���ʵ�����
    w32mci wmci; // MCI����ͺ���
    w32mcierror wmcierror; // MCI������Ϣ��ȡ����
public:
    // ���캯������ʼ��MCI
    Mci();
    // ��������������MCI��Դ
    ~Mci();
    // �����������ڴ洢������Ϣ
    char buf[256];
    // ����MCI�������������󷵻�false
    bool send(std::string command);
};
// AudioClip�����ڹ�����Ƶ�ļ��Ĳ���
class AudioClip {
private:
    Mci mci; // Mci���ʵ�������ڷ���MCI����
    std::string filename; // ��Ƶ�ļ���·��
    std::string alias; // ��Ƶ�ļ��ı���
    int length_ms; // ��Ƶ�ļ����ܳ��ȣ����룩
public:
    // ���캯������ʼ��AudioClip
    AudioClip();
    // ����������������Դ
    ~AudioClip();
    // ������Ƶ�ļ�
    bool load(const std::string& _filename);
    // ������Ƶ������ָ����ʼ�ͽ���ʱ��
    bool play(int start_ms = 0, int end_ms = -1);
    // ֹͣ����
    bool stop();
    // ��ͣ����
    bool pause();
    // �������ţ�����ͣ״̬��
    bool unpause();
    // �����ܵĺ�����
    int milliseconds();
};
