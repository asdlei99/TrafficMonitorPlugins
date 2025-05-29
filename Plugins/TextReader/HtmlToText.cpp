#include "pch.h"
#include "HtmlToText.h"
#include "../utilities/Common.h"
#include "Common.h"
#include <regex>
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

std::string CHtmlToText::HtmlToText(const std::string& html)
{
	//��ȡp��h��ǩ
	std::regex pattern(R"(<(p|h\d*).*?>.*?</\1>)", std::regex_constants::ECMAScript);
	std::sregex_iterator it(html.begin(), html.end(), pattern);
	std::sregex_iterator end;

	std::string text;
	for (; it != end; ++it)
	{
		std::smatch match = *it;
		std::string content = match.str();
		// ȥ�������е� HTML ��ǩ
		std::regex innerTagPattern(R"(</?[^>]+>)", std::regex_constants::ECMAScript);
		std::string content_text = std::regex_replace(content, innerTagPattern, "");
		if (!content_text.empty())
		{
			text += ' ';
			text += content_text;
		}
	}

	// ���� HTML ת���ַ�
	std::regex amp_regex("&amp;");
	text = std::regex_replace(text, amp_regex, "&");

	std::regex lt_regex("&lt;");
	text = std::regex_replace(text, lt_regex, "<");

	std::regex gt_regex("&gt;");
	text = std::regex_replace(text, gt_regex, ">");

	std::regex quot_regex("&quot;");
	text = std::regex_replace(text, quot_regex, "\"");

	std::regex apostrophe_regex("&apos;");
	text = std::regex_replace(text, apostrophe_regex, "'");

	return text;
}
