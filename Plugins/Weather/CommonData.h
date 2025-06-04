#pragma once
#include <string>
class CommonData
{
};

enum WeahterSelected
{
    WEATHER_CURRENT,
    WEATHER_TODAY,
    WEATHER_TOMMORROW,
    WEATHER_DAY2
};

struct SettingData
{
    int m_city_index{};                     //ѡ��ĳ������б��е����
    WeahterSelected m_weather_selected{};   //Ҫ��ʾ������
    bool m_show_weather_in_tooltips{};      //�Ƿ��������ʾ����ʾ
    bool m_use_weather_icon{};
    //int m_display_width{};
    bool auto_locate{};         //�Զ���λ
};

//һ��������Ϣ
struct WeatherInfo
{
    std::wstring m_type = L"--";         //��������
    std::wstring m_high = L"-��";        //����¶�
    std::wstring m_low = L"-��";         //����¶�
    bool is_cur_weather{};                 //�Ƿ�Ϊ��ǰ�¶ȣ����Ϊtrue����m_low��Ч
    std::wstring m_wind;                //����ͷ���
    std::wstring ToString() const;
    std::wstring ToStringTemperature() const;
};
