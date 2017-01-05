﻿#pragma once
#ifndef __TINYWEB_H__
#define __TINYWEB_H__

#ifdef _MSC_VER
//去掉 warning C4996: '***': The POSIX name for this item is deprecated.Instead, use the ISO C++ conformant name...
#define _CRT_NONSTDC_NO_DEPRECATE
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <libuv\include\uv.h>
#pragma comment(lib, "libuv.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Userenv.lib")
#  if defined(WIN32) && !defined(snprintf)
#  define snprintf _snprintf
#  endif

#define strcmpi _stricmp

#else //__GNUGC__
#include <uv.h>

#define strcmpi strcasecmp

#endif

#include "tools.h"

#if TinyWeb_Function_Description //TinyWeb功能说明

0.默认编码为 utf-8
1.支持使用HTTP: GET/POST方式访问
2.支持Socket, WebSocket 连接
3.支持返回404错误页面
4.支持指定根目录（默认程序所在目录）
5.支持任意格式文件访问(带扩展名, 小文件下载)
	a.支持静态网页访问：html/htm
	b.支持其他静态文件：js, css, png, jpeg/jpg, gif, ico, txt, xml, json, log, wam, wav, mp3, apk
	c.支持其他文件格式, 默认文件类型为："application/octet-stream"
	d.支持不带扩展名文件访问
6.支持默认index页面(index.html/index.htm)，可以自定义设置
7.支持目录列表
8.不允许访问根目录上级文件或文件夹
9.支持回调
	a.404前回调（未找到页面/文件时回调, 此功能便于程序返回自定义功能）
	b.WebSocket 数据回调
	c.socket 数据回调

==============future
1.支持cookie/session
2.支持认证
3.支持大文件响应（下载）

#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	char method;//0:Socket 1:GET 2:POST
	char* host; //IP:port
	char* path; //路径
	char* query;//参数
	char* data; //数据
	size_t len; //数据长度
}reqHeads;

//服务配置
typedef struct {
	char dirlist:1; //是否允许列出目录
	char all_http_callback:1;//是否全部http访问都回调,http update to websocket 除外
	char* doc_dir;  //Web根目录，绝对路径，末尾带斜杠'\'(uninx为'/')； 默认程序文件所在目录
	char* doc_index;//默认主页文件名，逗号分隔； 默认"index.html,index.htm"
	char* ip;       //服务的IP地址 is only ipV4, can be NULL or "" or "*", which means "0.0.0.0"
	short port;     //服务监听端口

	//数据
	void* data;//可以放入数数,如对象指针

	//客户端接入
	char (*on_connect)(void* data, uv_stream_t* client);

	//404前回调(未找到请求的文件/文件夹时回调,此功能便于程序返回自定义功能)
	//返回0表示没有适合的处理请求，将自动发送404响应；否则认为已经处理
	//heads成员不需要free
	char (*on_request)(void* data, uv_stream_t* client, reqHeads* heads);

	//Socket 或 WebSocket 数据, 可以通过buf->flag判断
	//buf成员不需要free
	char (*on_data)(void* data, uv_stream_t* client, membuf_t* buf);

	//Socket 检测到错误(此时链接可能已经断开)
	//buf成员不需要free
	//错误消息格式："%d:%s,%s,%s"
	char (*on_error)(void* data, uv_stream_t* client,int errcode, char* errstr, int flag);

	//Socket 关闭(此时链接可能已经断开)
	//flag:标志字节 ([0~7]: [0]是否需要保持连接<非长连接为http> [1]是否WebSocket
	char (*on_close)(void* data, uv_stream_t* client, int flag);
} tw_config;


//start web server, start with the config
//loop: if is NULL , it will be uv_default_loop()
//conf: the server config
//返回值不为0表示错误代码,用uv_err_name(n),和uv_strerror(n)查看原因字符串
int tinyweb_start(uv_loop_t* loop, tw_config* conf);

//stop TinyWeb
//loop: if is NULL , it will be &uv_default_loop()
void tinyweb_stop(uv_loop_t* loop);

//=================================================

//返回格式华的HTTP响应内容 (需要free返回数据)
//status：http状态,如:"200 OK"
//content_type：文件类型，如："text/html" ；可以调用tw_get_content_type()得到
//content：使用utf-8编码格式的数据，特别是html文件类型的响应
//content_length：can be -1 if content is c_str (end with NULL)
//respone_size：if not NULL,可以获取发送的数据长度 the size of respone will be writen to request
//returns malloc()ed c_str, need free() by caller
char* tw_format_http_respone(const char* status, const char* content_type, const char* content, int content_length, int* respone_size);

//根据扩展名返回文件类型 content_type
//可以传入路径/文件名/扩展名
const char* tw_get_content_type(const char* fileExt);

//发送数据到客户端; 如果是短连接,则发送完后会关闭连接
//data：待发送数据
//len： 数据长度, -1 将自动计算数据长度
//need_copy_data：是否需要复制数据
//need_free_data：是否需要free数据, 如果need_copy_data非零则忽略此参数
void tw_send_data(uv_stream_t* client, const void* data, unsigned int len, int need_copy_data, int need_free_data);

//发送'200 OK' 响应; 不会释放(free)传入的数据(u8data)
//content_type：Content Type 文档类型
//u8data：utf-8编码的数据
//content_length：数据长度，为-1时自动计算(strlen)
//respone_size：获取响应最终发送的数据长度，为0表示放不需要取此长度
void tw_send_200_OK(uv_stream_t* client, const char* content_type, const void* u8data, int content_length, int* respone_size);

//关闭客户端连接
void tw_close_client(uv_stream_t* client);


inline void printx(const uchar* data, uint len) {
	uint i;
	printf("\n-----------------------------------------------\n");
	for (i = 0; i < len; i++) {
		if (i > 0 && i % 16 == 0)
			printf("\n");
		printf("%02x ", *data);
		data++;
	}
	printf("\n-----------------------------------------------\n");
}

#ifdef __cplusplus
} // extern "C"
#endif
#endif //__TINYWEB_H__