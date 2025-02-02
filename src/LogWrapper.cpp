#include "LogWrapper.h"
#include "SDL_log.h"
#include <format>
#include <string>

void LOG_INF_SRC(const char* src, const std::string& log_text)
{
    SDL_Log(std::format("{} : {}", src, log_text.c_str()).c_str());
}

void LOG_SDL_ERR_SRC(const char* src, const std::string& err_log_text)
{
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, std::format("{} : {}", src, err_log_text.c_str()).c_str());
}