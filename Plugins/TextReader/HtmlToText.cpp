#include "pch.h"
#include "HtmlToText.h"
#include "../utilities/Common.h"
#include "Common.h"
#include <vector>

CHtmlToText::CHtmlToText()
{
}

bool CHtmlToText::ParseFromUrl(const std::wstring& url)
{
    //��ȡurl����
    std::string html;
    if (!CCommon::GetURL(url, html, true))
        return false;

    //ת�����ı�
    std::string text = HtmlToText(html);
    m_text = utilities::StringHelper::StrToUnicode(text.c_str(), true);

    return true;
}

bool CHtmlToText::ParseFromFile(const std::wstring& file_path)
{
    //��ȡ�ļ�����
    std::string html;
    if (!utilities::CCommon::GetFileContent(file_path.c_str(), html))
        return false;

    //ת�����ı�
    std::string text = HtmlToText(html);
    m_text = utilities::StringHelper::StrToUnicode(text.c_str(), true);

    return false;
}

const std::wstring& CHtmlToText::GetText()
{
    return m_text;
}

static void ParseHtmlText(std::string& str)
{
    //ȥ������html��ǩ
    size_t index = 0;
    while (index < str.size() - 1)
    {
        index = str.find('<');
        if (index == std::string::npos)
            break;
        size_t label_end_index = str.find('>', index + 1);
        if (label_end_index == std::string::npos)
            break;

        str = str.erase(index, label_end_index - index + 1);
    }

    //����htmlת���ַ�
    utilities::StringHelper::StringReplace(str, "&amp;", "&");
    utilities::StringHelper::StringReplace(str, "&lt;", "<");
    utilities::StringHelper::StringReplace(str, "&gt;", ">");
    utilities::StringHelper::StringReplace(str, "&quot;", "\"");
    utilities::StringHelper::StringReplace(str, "&apos;", "'");

    index = 0;
    while (index < str.size() - 1)
    {
        index = str.find("&#");
        if (index == std::string::npos)
            break;
        size_t escape_end_index = str.find(';', index + 2);
        if (escape_end_index == std::string::npos)
            break;

        //���ַ���ת��Ϊ����
        std::string str_num = str.substr(index + 2, escape_end_index - index - 2);
        bool is_hex = (str_num.size() > 1 && (str_num[0] == 'x' || str_num[0] == 'X'));
        if (is_hex)
            str_num = str_num.substr(1);
        char* end;
        long value = strtol(str_num.c_str(), &end, is_hex ? 16 : 10);
        wchar_t escape_char = static_cast<wchar_t>(value);
        if (escape_char == 0)
            escape_char = L' ';
        std::string escape_str = utilities::StringHelper::UnicodeToStr(std::wstring(1, escape_char).c_str(), true);

        //�滻ת���ַ�
        str.replace(index, escape_end_index - index + 1, escape_str);

        index += escape_str.size();
    }
}

std::string CHtmlToText::HtmlToText(const std::string& html)
{
    std::string text;
    //��ȡp��h��ǩ
    size_t index = 0;
    while (index < html.size() - 4)
    {
        std::string cur_text;
        if (html.substr(index, 3) == "<p>")
        {
            size_t index_end = html.find("</p>", index + 3);
            if (index_end == std::string::npos)
                break;
            cur_text = html.substr(index + 3, index_end - index - 3);
            index = index_end + 4;
            if (!cur_text.empty())
            {
                ParseHtmlText(cur_text);
                text += cur_text;
                text += ' ';
            }
            continue;
        }

        if (html.substr(index, 2) == "<h" && html[index + 3] == '>')
        {
            char h_level = html[index + 2];
            std::string label_end = "</h" + h_level + '>';
            size_t index_end = html.find(label_end);
            if (index_end == std::string::npos)
                break;
            cur_text = html.substr(index + 4, index_end - index - 4);
            index = index_end + 5;
            if (!cur_text.empty())
            {
                ParseHtmlText(cur_text);
                text += cur_text;
                text += ' ';
            }
            continue;
        }
        index++;
    }

    return text;
}
