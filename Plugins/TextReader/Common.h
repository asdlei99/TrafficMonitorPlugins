﻿#pragma once
#include <string>
#include <vector>
class CCommon
{
public:
    //将const char*字符串转换成宽字符字符串
    static std::wstring StrToUnicode(const char* str, bool utf8 = false);

    static std::string UnicodeToStr(const wchar_t* wstr, bool utf8 = false);

    //判断一个字符串是否UTF8编码
    static bool IsUTF8Bytes(const char* data);

    static void StringSplit(const std::wstring& str, const std::wstring& div_str, std::vector<std::wstring>& results, bool skip_empty = true);

    //获取一个文件的最后修改时间
    static bool GetFileLastModified(const std::wstring& file_path, unsigned __int64& modified_time);

    //获取URL的内容
    static bool GetURL(const std::wstring& url, std::string& result, bool utf8 = false, const std::wstring& user_agent = std::wstring());

    //判断一个字符串是否为url
    static bool IsURL(const std::wstring& str);
};
