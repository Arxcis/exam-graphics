#pragma once

#include <string>
#include <vector>

#include <PMS/logger.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>


namespace GUI
    
{

    void begin() 
    {
        ImGui_ImplGlfwGL3_NewFrame();

    }
    void updateTimeOfDay(float timeofday_seconds) 
    {

        int minutes = int(timeofday_seconds / 60.0) % 60;
        int hours = int(timeofday_seconds / 60.0 / 60.0) % 24;

        std::string timeofday_name = "unknown";

        if (hours  > 16 && hours <= 22) {
            timeofday_name = "Evening";
        }
        else if (hours > 10 && hours <= 16) {
            timeofday_name = "Noon";
        }
        else if (hours > 4 && hours <= 10) {
            timeofday_name = "Morning";
        }
        else if (hours > 22 ||  hours <= 4) {
            timeofday_name = "Night";
        }

        ImGui::Text("Time Of Day: %02d:%02d", hours, minutes);  // Display some text (you can use a format string too)+
        ImGui::Text("Time Of Name: %s", timeofday_name.data());  // Display some text (you can use a format string too)+
    }   

    void updateTimeOfYear(float timeofyear_days) 
    {
        const std::vector<std::string> Months 
        {
            "JANUARY",
            "FEBRUARY",
            "MARCH",
            "APRIL",
            "MAY",
            "JUNE",
            "JULY",
            "AUGUST",
            "SEPTEMBER",
            "OCTOBER",
            "NOVEMBER",
            "DECEMBER"
        };
        const std::vector<int> NumberOfDaysInEachMonth = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        
        int monthCount = 0;
        int dayCount = 0;
        for (const auto& daysInMonths: NumberOfDaysInEachMonth) 
        {
            dayCount += daysInMonths;
            if (dayCount > timeofyear_days) {
                break;
            }
            monthCount += 1;
        }



        ImGui::Text("Days of year: %d", int(timeofyear_days));
        ImGui::Text("Days of month: %d", int(timeofyear_days) - (dayCount - NumberOfDaysInEachMonth[monthCount]));
        ImGui::Text("Month: %s", Months[monthCount].data());
    }

    void end()
    {
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
    }

}