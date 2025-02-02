#pragma once

#include <string>
#include <iostream>

// Log non-error information to the console.
void LOG_INF_SRC(const char* src, const std::string&);
#define LOG_INF(log) LOG_INF_SRC(__func__, log)

// Log errors to the console
void LOG_SDL_ERR_SRC(const char* src, const std::string&);
#define LOG_SDL_ERR(err_log) LOG_SDL_ERR_SRC(__func__, err_log)

#define THROW_EXCEPTION(exception_msg) throw std::runtime_error(std::format("{} : {}. Err: {}", __func__, exception_msg, SDL_GetError()))
