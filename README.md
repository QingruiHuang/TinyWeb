# TinyWeb v1.0.0
auth [lzpong]
A tiny web server based on libuv, can accept Socket,WebSocket,or http protocol,and callBack func's
```
TinyWeb����˵��

auth lzpong 2016/11/24
���ܻ��� libuv ��ƽ̨��

0.Ĭ�ϱ���Ϊ utf-8
1.֧��ʹ��GET/POST��ʽ
2.֧�ַ���404����ҳ��
3.֧��ָ����Ŀ¼��Ĭ�ϳ�������Ŀ¼��
4.֧�������ʽ�ļ�����(����չ��,С�ļ�����)
	a.֧�־�̬��ҳ���ʣ�html/htm
	b.֧��������̬�ļ���js,css,png,jpeg/jpg,gif,ico,txt,xml,json,log,wam,wav,mp3,apk
	c.֧��������ʽ�ļ�,Ĭ���ļ�����Ϊ��"application/octet-stream"
	d.֧�ֲ�����չ���ļ�����
5.֧��Ĭ��indexҳ��(index.html/index.html)�������Զ�������
6.֧��Ŀ¼�б�
7.֧��Socket, WebSocket
8.֧�ֻص�
	a.404ǰ�ص���δ�ҵ�ҳ��/�ļ�ʱ�ص�,�˹��ܱ��ڳ��򷵻��Զ��幦�ܣ�
	b.WebSocket ���ݻص�
	c.socket ���ݻص�

==============future
1.֧��cookie/session
2.֧����֤
3.֧�ִ��ļ���Ӧ�����أ�
```