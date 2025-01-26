#include "LogWrapper.h"
#include "SDL_log.h"

void LOG_INF(std::string log_text)
{
    SDL_Log(log_text.c_str());
}

void LOG_SDL_ERR(std::string err_log_text)
{
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, err_log_text.c_str());
}