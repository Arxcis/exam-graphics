#pragma once

#include <string>
#include <vector>

#include <PMS/logger.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#include <glm/glm.hpp>

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
        ImGui::Text("It's       : %s", timeofday_name.data());  // Display some text (you can use a format string too)+
    }   

    void updateTimeOfYear(float timeofyear_days) 
    {
        struct Month
        {
            std::string name;
            std::string season;
            int dayCount;
        };

        const auto Months = std::vector<Month> {

            Month{"JANUARY",   "WINTER", 31},
            Month{"FEBRUARY",  "WINTER", 28},
            Month{"MARCH",     "SPRING", 31},
            Month{"APRIL",     "SPRING", 30},
            Month{"MAY",       "SPRING", 31},
            Month{"JUNE",      "SUMMER", 30},
            Month{"JULY",      "SUMMER", 31},
            Month{"AUGUST",    "SUMMER", 31},
            Month{"SEPTEMBER", "AUTUMN", 30},
            Month{"OCTOBER",   "AUTUMN", 31},
            Month{"NOVEMBER",  "AUTUMN", 30},
            Month{"DECEMBER",   "WINTER", 31}
        };
        
        const int DaysInAYear = 365;
        
        int wrapped_timeofyear_days = int(timeofyear_days) % DaysInAYear; 
        int monthCount = 0;
        int dayCount = 0;
        for (const auto& month: Months) 
        {
            dayCount += month.dayCount;
            if (dayCount > wrapped_timeofyear_days) {
                break;
            }
            monthCount += 1;
        }

        ImGui::Text("Day of year:  %d", int(wrapped_timeofyear_days));
        ImGui::Text("Day of month: %d", int(wrapped_timeofyear_days) - (dayCount - Months[monthCount].dayCount));
        ImGui::Text("Month:        %s", Months[monthCount].name.data());
        ImGui::Text("Season:       %s", Months[monthCount].season.data());
    }


    void updateTimeOfTide(float timeoftide) 
    {
        ImGui::Text("Tide height: %f %", timeoftide);
    }

    void writeSpeed(float value) {
        ImGui::Text ("Speed: %f m/s", value);
        ImGui::Text ("       %f km/h", value/1000*3600);
    }

    void writeVector(std::string tag, glm::vec3 direction) 
    {
       ImGui::Text("%s:\n %f\n %f\n %f", tag.data(), direction.x, direction.y, direction.z);
    }

    void end()
    {
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
    }

}